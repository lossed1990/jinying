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

public: //对外接口
	/**
	 * @brief 根据用户名，判断密码是否正确
	 */
	bool CheckUserPassword(string& strUserName,string& strUserPw);
	/**
	 * @brief 新增用户
	 */
	int AddUser(string& strUserName, string& strUserPw, int nType);
	/**
	 * @brief 删除用户
	 */
	int DeleteUser(string& strUserName);
	/**
	 * @brief 修改用户密码
	 */
	int ChangeUserPassword(string& strUserName, string& strUserOldPw, string& strUserNewPw);
	/**
	 * @brief 获取用户权限
	 */
	int GetUserTypeByName(string& strUserName);
	/**
	 * @breif 遍历所有用户
	 */
	void EachUsers(function<void(TUserEntity*)> pCallBack);

	int GetUserCount();

private:
	bool LoadUserFromConfig(); 
	int SaveUserToConfig(); 

private:
	std::vector<TUserEntity*> m_vecUserList;
	CMyCriticalSection m_csList;				//临界区
};


