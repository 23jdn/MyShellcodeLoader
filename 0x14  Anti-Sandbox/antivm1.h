#pragma once

#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <psapi.h>

#pragma comment(lib, "Shlwapi.lib")

class VenvChecker {
public:
    static BOOL IsVenvByHardwareCheck();

private:
    VenvChecker() = default;
};

BOOL VenvChecker::IsVenvByHardwareCheck() {
    SYSTEM_INFO SysInfo = { 0 };
    MEMORYSTATUSEX MemStatus = { sizeof(MEMORYSTATUSEX) };
    HKEY hKey = NULL;
    DWORD dwUsbNumber = NULL;
    DWORD dwRegErr = NULL;

    // CPU ���
    GetSystemInfo(&SysInfo);

    // ����������2��
    if (SysInfo.dwNumberOfProcessors < 2) {
        return TRUE;
    }

    // �ڴ���
    if (!GlobalMemoryStatusEx(&MemStatus)) {
        printf("\n\t[!] GlobalMemoryStatusEx ʧ�ܣ�������: %d \n", GetLastError());
        return FALSE;
    }

    // �ڴ�����2GB
    if ((DWORD)MemStatus.ullTotalPhys < (DWORD)(2 * 1073741824)) {
        return TRUE;
    }

    // ����������ӹ���USB����
    if ((dwRegErr = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Enum\\USBSTOR", NULL, KEY_READ, &hKey)) != ERROR_SUCCESS) {
        printf("\n\t[!] RegOpenKeyExA ʧ�ܣ�������: %d | 0x%0.8X \n", dwRegErr, dwRegErr);
        return FALSE;
    }

    if ((dwRegErr = RegQueryInfoKeyA(hKey, NULL, NULL, NULL, &dwUsbNumber, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) != ERROR_SUCCESS) {
        printf("\n\t[!] RegQueryInfoKeyA ʧ�ܣ�������: %d | 0x%0.8X \n", dwRegErr, dwRegErr);
        return FALSE;
    }

    // �������ӹ���USB����2��
    if (dwUsbNumber < 2) {
        return TRUE;
    }

    RegCloseKey(hKey);

    return FALSE;
}
