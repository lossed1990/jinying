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
	int m_nType;                 ///< 用户类型
	string m_strName;            ///< 用户名 
	string m_strPassWord;        ///< 登陆密码
};


