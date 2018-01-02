#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "CommonFunction.h"

#include "InterfaceCalculatePrice.h"
#include "../MainModel/PriceCounter.h"
using namespace rapidjson;

CInterfaceCalculatePrice::CInterfaceCalculatePrice()
{
}

CInterfaceCalculatePrice::~CInterfaceCalculatePrice()
{
}

string CInterfaceCalculatePrice::GetUrl()
{
	return I_CALCULATE_PRICE;
}

void CInterfaceCalculatePrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	CPriceCounter newCounter;     //材料价格计算类
	vector<LogInfo> vecLogList;   //业务日志
	CounterParam tParam;
	float fTotalMinPrice = 0.0;

	//解析参数
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	if (tReqDoc.FindMember("count") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("source") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("parts") != tReqDoc.MemberEnd())
	{
		StringBuffer pcConditionBuffer;
		Writer<StringBuffer> writer(pcConditionBuffer);
		tReqDoc.Accept(writer);
		string pcConditionTemp = pcConditionBuffer.GetString();

		CDBHelper::Instance()->LogUTF8("管理员", "计算费用", pcConditionTemp.c_str());

		tParam.nNumber = tReqDoc["count"].GetInt();

		bool bHavingDiecutter = false;    //是否需要刀模费
		int nProductType = 0;             //产品类型
		//计算材料费：原材料+印刷+覆膜
		string strReqSource;
		Value &cSourceArray = tReqDoc["source"];
		if (cSourceArray.IsArray())
		{
			for (int i = 0; i < cSourceArray.Size(); i++)
			{
				const Value& object = cSourceArray[i];

				tParam.nProductType = object["producttype"].GetInt();
				tParam.nLen = object["l"].GetInt();
				tParam.nWidth = object["w"].GetInt();
				tParam.nHeight = object["h"].GetInt();
				tParam.bTangjin = object["btangjin"].GetBool();
				tParam.bFilm = object["bfilm"].GetBool();
				tParam.bPress = object["bpress"].GetBool();

				bHavingDiecutter = CMainModel::Instance()->IsHavingDieCutter(tParam.nLen, tParam.nWidth, tParam.nHeight);
				nProductType = tParam.nProductType;

				char pcTemp[256] = { 0 };
				CEncodingTools::ConvertUTF8ToGB(object["sourcetype"].GetString(), pcTemp, 256);
				tParam.strSourceTypeName = string(pcTemp);

				memset(pcTemp, 0, 256);
				CEncodingTools::ConvertUTF8ToGB(object["presstype"].GetString(), pcTemp, 256);
				tParam.strPressType = string(pcTemp);

				if (tParam.nProductType == 0) //手提袋，分单片制作和整片制作
				{
					float fPrice1 = newCounter.GetPrice(0, tParam, vecLogList);
					float fPrice2 = newCounter.GetPrice(1, tParam, vecLogList);
					fTotalMinPrice += fPrice1 < fPrice2 ? fPrice1 : fPrice2;
				}
				else
				{
					int nSourceUseType = object["sourceusetype"].GetInt();
					fTotalMinPrice += newCounter.GetPrice(nSourceUseType, tParam, vecLogList);
				}
			}
		}

		//计算刀模费
		if (bHavingDiecutter)
		{
			LogInfo tInfo;
			tInfo.nType = 0;
			tInfo.strLog = "刀模成本明细>>规格刀模存在，不需要刀模费用";
			vecLogList.push_back(tInfo);
		}
		else
		{
			float fDieCutterPrice = (nProductType == 0) ? CMainModel::Instance()->GetDieCutterPrice() : CMainModel::Instance()->GetBoxDieCutterPrice();
			char chLog[512] = { 0 };
			sprintf_s(chLog, 512, "刀模成本明细>>规格刀模不存在，刀模费用：%.2f；",
				fDieCutterPrice);
			LogInfo tInfo;
			tInfo.nType = 0;
			tInfo.strLog = chLog;
			vecLogList.push_back(tInfo);
			fTotalMinPrice += fDieCutterPrice;
		}

		//计算压痕费
		//{
		//	//int nCheckPrice = CMainModel::Instance()->GetImpressCheckPrice();
		//	//int nImpressPrice = CMainModel::Instance()->GetImpressPrice();
		//	//int nTotalImpressPrice = nCheckPrice + (nImpressPrice * tParam.nNumber);

		//	float nCheckPrice = CMainModel::Instance()->GetImpressCheckPrice();
		//	float nImpressPrice = CMainModel::Instance()->GetImpressPrice();
		//	float nTotalImpressPrice = nCheckPrice + (nImpressPrice * tParam.nNumber);
		//	fTotalMinPrice += nTotalImpressPrice;

		//	char chLog[512] = { 0 };
		//	sprintf_s(chLog, 512, "压痕成本明细>>压痕费用：%.2f；", nTotalImpressPrice);

		//	LogInfo tInfo;
		//	tInfo.nType = 0;
		//	tInfo.strLog = chLog;
		//	vecLogList.push_back(tInfo);
		//}

		//计算配件费
		Value &cPartsArray = tReqDoc["parts"];
		if (cPartsArray.IsArray())
		{
			for (int i = 0; i < cPartsArray.Size(); i++)
			{
				const Value& object = cPartsArray[i];
				char pcTemp[256] = { 0 };
				CEncodingTools::ConvertUTF8ToGB(object.GetString(), pcTemp, 256);
				string strPartName = string(pcTemp);
				//计算各个配件的价格
				OutputDebugStringA(strPartName.c_str());

				float fPrice = 0;
				bool bRet = CMainModel::Instance()->GetPartPriceByName(strPartName, fPrice);
				if (bRet)
				{
					char chLog[512] = { 0 };
					//sprintf_s(chLog, 512, "配件成本明细>>名称：%s；单价：%.2f；总价：%.2f；",
					//	strPartName.c_str(), fPrice, fPrice * tParam.nNumber);
					sprintf_s(chLog, 512, "配件成本明细>>名称：%s；总价：%.2f；",
						strPartName.c_str(), fPrice * tParam.nNumber);
					LogInfo tInfo;
					tInfo.nType = 0;
					tInfo.strLog = chLog;
					vecLogList.push_back(tInfo);

					fTotalMinPrice += fPrice * tParam.nNumber;
				}
				else
				{
					char chLog[512] = { 0 };
					sprintf_s(chLog, 512, "配件成本明细>>名称：%s；匹配失败，成本价未配置",
						strPartName.c_str());
					LogInfo tInfo;
					tInfo.nType = 0;
					tInfo.strLog = chLog;
					vecLogList.push_back(tInfo);
				}
			}
		}
	}

	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();

	Value array(kArrayType);
	for (auto& item : vecLogList)
	{
		Value arraySub(kArrayType);

		string strLogType;
		switch (item.nType)
		{
		case 1:
			strLogType = "错误";
			break;
		case 3:
			strLogType = "需求";
			break;
		case 4:
			strLogType = "材料";
			break;
		case 5:
			strLogType = "印刷";
			break;
		case 6:
			strLogType = "覆膜";
			break;
		case 7:
			strLogType = "压痕";
			break;
		default:
			strLogType = "跟踪";
			break;

		}
		//strLogType = (item.nType == 1) ? "错误" : "跟踪";
		Value vLogType(kStringType);
		vLogType.SetString(strLogType.c_str(), strLogType.size(), allocator);
		arraySub.PushBack(vLogType, allocator);

		Value vLogContent(kStringType);
		vLogContent.SetString(item.strLog.c_str(), item.strLog.size(), allocator);
		arraySub.PushBack(vLogContent, allocator);

		array.PushBack(arraySub, allocator);
	}
	tDoc.AddMember("log", array, allocator);

	Value vTotalPrice(kStringType);
	char chTotalPrice[64] = { 0 };
	sprintf_s(chTotalPrice, "%.2f", fTotalMinPrice);
	vTotalPrice.SetString(chTotalPrice, strlen(chTotalPrice), allocator);
	tDoc.AddMember("totalprice", vTotalPrice, allocator);

	Value vPrice(kStringType);
	char chPrice[64] = { 0 };
	sprintf_s(chPrice, "%.2f", fTotalMinPrice / tParam.nNumber);
	vPrice.SetString(chPrice, strlen(chPrice), allocator);
	tDoc.AddMember("price", vPrice, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;

	return;
}
