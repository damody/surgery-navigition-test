
// oglMFCDialog.h : PROJECT_NAME 應用程式的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"		// 主要符號


// CoglMFCDialogApp:
// 請參閱實作此類別的 oglMFCDialog.cpp
//

class CoglMFCDialogApp : public CWinApp
{
public:
	CoglMFCDialogApp();

// 覆寫
public:
	virtual BOOL InitInstance();

// 程式碼實作

	DECLARE_MESSAGE_MAP()
};

extern CoglMFCDialogApp theApp;