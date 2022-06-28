#include <Windows.h>
#include "resource.h"
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{

	WNDCLASSEX winclass;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;
	//winclass.lpfnWndProc = WndProc; //WndProc��û�ж��塣
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = (LPCWSTR)"WINCLASS1";
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	winclass.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(DarkAlbertCorpIcon));	//create an icon.
	winclass.hIconSm = LoadIcon(hinstance, MAKEINTRESOURCE(DarkAlbertCorpIcon)); //this is the icon you can see on the window of this programme.
	return 0;
}

WNDCLASSEX winclass;


#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#pragma comment(lib, "Winmm.lib")    // ���� Windows Multimedia API
#include "resource.h"     // ע�������Դ ID Ϊ�궨�壬�뱣������


//////////////////////////////////////////////////////////////////
// ��ȡָ��ģ���е���Դ�ļ�
// ������
//     strDstFile:     Ŀ���ļ�������ȡ����Դ�����������
//     strResType:     ��Դ���ͣ�
//     strResName:     ��Դ���ƣ�
// ����ֵ��
//     true: ִ�гɹ���
//     false: ִ��ʧ�ܡ�
bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName)
{
	// �����ļ�
	HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
	HRSRC    hRes = ::FindResource(NULL, strResName, strResType);
	HGLOBAL    hMem = ::LoadResource(NULL, hRes);
	DWORD    dwSize = ::SizeofResource(NULL, hRes);

	// д���ļ�
	DWORD dwWrite = 0; // ����д���ֽ�
	::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
	::CloseHandle(hFile);

	return true;
}
