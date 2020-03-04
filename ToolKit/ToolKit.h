
// ToolKit.h: ToolKit 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号
#include "ZClassLib.h"

// CToolKitApp:
// 有关此类的实现，请参阅 ToolKit.cpp
//

class CToolKitApp : public CWinAppEx
{
public:
	CToolKitApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CToolKitApp theApp;

#define WM_MSGRECVPRO WM_USER+1

enum MSGUSER
{
	MSGUSER_UPDATEPROGRESS = 0,
	MSGUSER_IMPORTRESULT,
	MSGUSER_EXPORTRESULT,
	MSGUSER_CALCULATERESULT
};