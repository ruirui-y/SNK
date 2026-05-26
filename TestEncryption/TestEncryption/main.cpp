#include "Encryption.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    char* pUserPassword = new char[strlen("K7#pL2!m") + 1];
    strcpy(pUserPassword, "K7#pL2!m");

    char* pAdminPassword = new char[strlen("K7#pL2!m") + 1];
    strcpy(pAdminPassword, "K7#pL2!m");

    // ModifyPassword(pUserPassword, pAdminPassword);
    LoginVerify(pUserPassword, pAdminPassword);
    return a.exec();
}