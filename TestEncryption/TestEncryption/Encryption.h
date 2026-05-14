#ifndef __TESTENCRYPTION_H__
#define __TESTENCRYPTION_H__

#include <windows.h>

bool CheckViKey(DWORD* dwRetCode, DWORD* dwCount, DWORD* dwHID, char* pUserPassword, char* pAdminPassword);
bool LoginVerify(char* pUserPassword, char* pAdminPassword);
void ModifyPassword(char* pNewUserPassword, char* pNewAdminPassword, WORD Index = 0);

#endif // __TESTENCRYPTION_H__