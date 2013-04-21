
// oglMFCDialogDlg.h : ���Y��
//

#pragma once
#include "OpenGLControl.h"
#include "vtk_view.h"
#include "vtk_view_left.h"
#include "vtk_view_right.h"

// CoglMFCDialogDlg ��ܤ��
class CoglMFCDialogDlg : public CDialogEx
{
private:
	COpenGLControl	m_oglWindow;
	vtk_view_left	m_left_vtk;
	vtk_view	m_center_vtk;
	vtk_view_right	m_right_vtk;
// �غc
public:
	CoglMFCDialogDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_OGLMFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};