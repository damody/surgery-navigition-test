
// oglMFCDialog.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CoglMFCDialogApp:
// �аѾ\��@�����O�� oglMFCDialog.cpp
//

class CoglMFCDialogApp : public CWinApp
{
public:
	CoglMFCDialogApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CoglMFCDialogApp theApp;