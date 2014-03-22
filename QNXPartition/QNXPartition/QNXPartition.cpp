// QNXPartition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "WinBase.h"
#include "windows.h"	// WinBase.h судя 
#include "FileAPI.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//Searching for a volumes
	//TCHAR tcDevName[256];
	//TCHAR tcVolumePathName[256];
	//DWORD dwResult = 0;
	//unsigned long size;
	//int counter = 1;
	//HANDLE hTempDev;
	//HANDLE hDev = FindFirstVolume(tcDevName, 256);
	//PARTITION_INFORMATION piDiskInfo;
	//do{
	//	wcout << _T("Volume GUID (") << counter << _T("): ") << tcDevName << endl;
	//	GetVolumePathNamesForVolumeName(tcDevName, tcVolumePathName, 256, &size);
	//	//GetVolumeNameForVolumeMountPoint(tcDevName, tcVolumePathName, 256);
	//	if (!wcslen(tcVolumePathName))
	//	{
	//		cout << "\tNot mounted" << endl;
	//	}
	//	else
	//	{
	//		wcout << _T("\tMounted on: ") << tcVolumePathName << endl;
	//	}
	//	
	//	//hTempDev = CreateFile(tcDevName, GENERIC_READ | GENERIC_WRITE,
	//	//						FILE_SHARE_READ | FILE_SHARE_WRITE,
	//	//						0, OPEN_EXISTING, 0, 0);
	//	//if (hTempDev == INVALID_HANDLE_VALUE)
	//	//{
	//	//	cout << "\tERROR: CreateFile:" << GetLastError() << endl;
	//	//	continue;
	//	//}

	//	if (DeviceIoControl(hDev, IOCTL_DISK_GET_PARTITION_INFO_EX, NULL, 0,
	//		&piDiskInfo, sizeof(piDiskInfo), &dwResult, NULL))
	//	{
	//		cout << "\t" << piDiskInfo.PartitionType << endl;
	//	}
	//	else
	//	{
	//		cout << "\tERROR: DeviceIoConrtol: " << GetLastError() << endl;
	//	}
	//} while (++counter, FindNextVolume(hDev, tcDevName, 256));

	//FindVolumeClose(hDev);


	__int64 pos = 1000000;
	TCHAR drivename[255];
	TCHAR GUIDPath[255];
	memset(drivename, 0, sizeof(drivename));
	//	swprintf_s(drivename, _T("\\\\.\\PhysicalDrive%d"), 0);
	swprintf_s(drivename, _T("\\\\.\\I:"), 0);

	//Drive umounting

	// Drive opening
	HANDLE hDrive = CreateFile(drivename,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		0,
		NULL);
	if (hDrive == INVALID_HANDLE_VALUE){
		cout << "Fail to open disk;" << endl;
		cin.get();
		return 0;
	}
	cout << "Drive is opened;" << endl;

	GetVolumeNameForVolumeMountPoint(_T("I:\\"), GUIDPath, 255);
	wcout << _T("Volume GUID :") << GUIDPath << endl;

	DWORD dwReturn;
	DeviceIoControl(hDrive, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &dwReturn, NULL);
	DefineDosDevice(DDD_RAW_TARGET_PATH | DDD_REMOVE_DEFINITION | DDD_EXACT_MATCH_ON_REMOVE,
					drivename,
					GUIDPath);

	CloseHandle(hDrive);

	hDrive = CreateFile(drivename,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		0,
		NULL);
	if (hDrive == INVALID_HANDLE_VALUE){
		cout << "Fail to open disk;" << endl;
		cin.get();
		return 0;
	}
	cout << "Drive is opened;" << endl;

	const int bufSize = 6;
	char buf[bufSize];
	memset(buf, 0, bufSize);

	// Setting file pointer position
	LARGE_INTEGER n64Pos;
	n64Pos.QuadPart = 4;
	DWORD nRet = SetFilePointer(hDrive, n64Pos.LowPart, &n64Pos.HighPart, FILE_BEGIN);
	cout << "nRet: " << nRet << endl;

	// Reading data from drive
	DWORD dwBytesRead = 0;
	if (ReadFile(drivename, buf, 5, &dwBytesRead, NULL)){
		buf[dwBytesRead] = '\0';
		cout << "Bytes read: " << dwBytesRead << endl
			<< "Bytes: " << buf << endl;
	}
	else{
		DWORD err = GetLastError();
		cout << "Bytes read: " << dwBytesRead << endl;
		cout << "Error code: " << err << endl;
	}


	//PARTITION_INFORMATION piDiskInfo;
	//DWORD dwResult = 0;
	//if (DeviceIoControl(hDrive, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0,
	//	&piDiskInfo, sizeof(piDiskInfo), &dwResult, NULL))
	//{
	//	cout << (int)piDiskInfo.PartitionType << endl;
	//}
	//else
	//{
	//	cout << "\tERROR: DeviceIoConrtol: " << GetLastError() << endl;
	//}
	

	cin.get();
	return 0;
}

