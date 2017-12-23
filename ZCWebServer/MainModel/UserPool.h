#pragma once
#include "UserEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TUserPool
{
public:
	TUserPool();
	virtual ~TUserPool();

public: //����ӿ�
	/**
	 * @brief �����û������ж������Ƿ���ȷ
	 */
	bool CheckUserPassword(string& strUserName,string& strUserPw);
	/**
	 * @brief �����û�
	 */
	int AddUser(string& strUserName, string& strUserPw, int nType);
	/**
	 * @brief ɾ���û�
	 */
	int DeleteUser(string& strUserName);
	/**
	 * @brief �޸��û�����
	 */
	int ChangeUserPassword(string& strUserName, string& strUserOldPw, string& strUserNewPw);
	/**
	 * @brief ��ȡ�û�Ȩ��
	 */
	int GetUserTypeByName(string& strUserName);
	/**
	 * @breif ���������û�
	 */
	void EachUsers(function<void(TUserEntity*)> pCallBack);

	int GetUserCount();

private:
	bool LoadUserFromConfig(); 
	int SaveUserToConfig(); 

private:
	std::vector<TUserEntity*> m_vecUserList;
	CMyCriticalSection m_csList;				//�ٽ���
};


