#include "UserEntity.h"

TUserEntity::TUserEntity(string& strName, string& strPw, int nType)
{
	m_nType = nType;                 
	m_strName = strName;          
	m_strPassWord = strPw;        
}

TUserEntity::~TUserEntity()
{

}

bool TUserEntity::CheckPassword(string& strPw)
{
	if (m_strPassWord == strPw)
	{
		return true;
	}

	return false;
}

bool TUserEntity::ChangePassword(string& strOldPw, string& strNewPw)
{
	if (m_strPassWord == strOldPw)
	{
		m_strPassWord = strNewPw;
		return true;
	}

	return false;
}