#pragma once
#include <string>
//using namespace std;

using std::string;

class TUserEntity
{
public:
	TUserEntity(string& strName,string& strPw,int nType);   
	~TUserEntity();

public:
	string GetName(){ return m_strName; }
	string GetPassword(){ return m_strPassWord; }
	int GetType() { return m_nType; }
	bool CheckPassword(string& strPw);
	bool ChangePassword(string& strOldPw, string& strNewPw);

private:
	int m_nType;                 ///< �û�����
	string m_strName;            ///< �û��� 
	string m_strPassWord;        ///< ��½����
};


