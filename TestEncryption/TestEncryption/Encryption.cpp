#include "Encryption.h"

#include "Vikey.h"
#include <QDebug>
#include <QMessageBox>

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>

bool CheckViKey(DWORD* dwRetCode, DWORD* dwCount, DWORD* dwHID, char* pUserPassword, char* pAdminPassword)
{
    char szUserSN[64] = { "718A5C0B8E591B5A3B7B0DB260DBB27B" };
    *dwRetCode = VikeySetUserSN(szUserSN);
    if (*dwRetCode)
    {
        qDebug() << __FUNCTION__ << QStringLiteral("VikeySetUserSN 失败!\n");
        QMessageBox::warning(0, QStringLiteral("提示"), QStringLiteral("VikeySetUserSN 失败!\n"), QMessageBox::Ok);
        return false;
    }

    /* 查找设备 */
    *dwRetCode = VikeyFind(dwCount);

    if (*dwRetCode)
    {
        qDebug() << __FUNCTION__ << QStringLiteral("没有发现Vikey!\n");
        QMessageBox::warning(0, QStringLiteral("提示"), QStringLiteral("没有发现Vikey!\n"), QMessageBox::Ok);
        return false;
    }

    WORD Index = 0;

    //设置加密狗ApiKey 硬件版本为7.4.5(含)以后的加密狗必须要调用此接口才能登录
    //ApiKey 是由ViKey加密管理工具的->加密狗管理->密钥设置页面  通过Api密钥种子生成的
    char szApiKey[256] = { "2F4DA4D72B5DF341A20CD9D617BD0976B366335662CB53011B8889B892247AF9F7EE5E0C9D3405309B775229806C79C491B1AE2318549169186884938C8993D8" };
    *dwRetCode = VikeySetApiKey(Index, szApiKey);
    if (*dwRetCode)
    {
        qDebug() << __FUNCTION__ << QStringLiteral("设置Api密钥 失败!\n");
        QMessageBox::warning(0, QStringLiteral("提示"), QStringLiteral("设置Api密钥 失败!\n"), QMessageBox::Ok);
        return false;
    }

    /* 获取设备ID */
    *dwRetCode = VikeyGetHID(Index, dwHID);
    if (*dwRetCode)
    {
        qDebug() << __FUNCTION__ << QStringLiteral("VikeyGetHID 失败!\n");
        QMessageBox::warning(0, QStringLiteral("提示"), QStringLiteral("VikeyGetHID 失败!\n"), QMessageBox::Ok);
        return false;
    }
    qDebug() << __FUNCTION__ << QString("VikeyGetHID:%u\n").arg(*dwHID);

    /* 用户权限登录 */
    *dwRetCode = VikeyUserLogin(Index, pUserPassword);
    if (*dwRetCode)
    {
        qDebug() << __FUNCTION__ << QStringLiteral("用户权限登录失败!\n");
        QMessageBox::warning(0, QStringLiteral("提示"), QStringLiteral("用户权限登录失败!\n"), QMessageBox::Ok);
        return false;
    }

    /* 管理员权限登录 */
    *dwRetCode = VikeyAdminLogin(Index, pAdminPassword);
    if (*dwRetCode)
    {
        qDebug() << __FUNCTION__ << QStringLiteral("管理员权限登录失败!\n");
        QMessageBox::warning(0, QStringLiteral("提示"), QStringLiteral("管理员权限登录失败!\n"), QMessageBox::Ok);
        return false;
    }
    return true;
}

bool LoginVerify(char* pUserPassword, char* pAdminPassword)
{
    DWORD* dwRetCode = new DWORD;                                                                                       // 存储函数调用的返回码
    DWORD* dwCount = new DWORD;                                                                                         // 存储找到的Vikey数量
    DWORD* dwHID = new DWORD;                                                                                           // 存储Vikey的HID

    bool ret = CheckViKey(dwRetCode, dwCount, dwHID, pUserPassword, pAdminPassword);
    if (!ret)
        return false;

    qDebug() << "LoginVerify success";
    return true;
}

void ModifyPassword(char* pNewUserPassword, char* pNewAdminPassword, WORD Index)
{
    DWORD* dwRetCode = new DWORD;                                                                                       // 存储函数调用的返回码
    DWORD* dwCount = new DWORD;                                                                                         // 存储找到的Vikey数量
    DWORD* dwHID = new DWORD;                                                                                           // 存储Vikey的HID

    char* pOldUserPassword = new char[strlen("11111111") + 1];
    strcpy(pOldUserPassword, "11111111");
    char* pOldAdminPassword = new char[strlen("00000000") + 1];
    strcpy(pOldAdminPassword, "00000000");
    qDebug() << "pOldUserPassword:" << pOldUserPassword << "pOldAdminPassword:" << pOldAdminPassword;

    bool ret = CheckViKey(dwRetCode, dwCount, dwHID, pOldUserPassword, pOldAdminPassword);
    if (!ret)
        return;
    else
        qDebug() << "CheckViKey success";

    *dwRetCode = VikeyResetPassword(Index, pNewUserPassword, pNewAdminPassword);
    if (*dwRetCode)
    {
        qDebug() << __FUNCTION__ << QStringLiteral("修改密码失败!\n");
        QMessageBox::warning(0, QStringLiteral("提示"), QStringLiteral("修改密码失败!\n"), QMessageBox::Ok);
        return;
    }

    qDebug() << "ModifyPassword success";
}