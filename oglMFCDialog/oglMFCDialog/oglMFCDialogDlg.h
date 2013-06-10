
// oglMFCDialogDlg.h : 標頭檔
//

#pragma once
#include "OpenGLControl.h"
#include "vtk_view.h"
#include "vtk_view_left.h"
#include "vtk_view_right.h"
#include "vtk_view_bottom.h"
#include "afxcmn.h"

// CoglMFCDialogDlg 對話方塊
class CoglMFCDialogDlg : public CDialogEx
{
private:
	COpenGLControl	m_oglWindow;
	vtk_view_left	m_left_vtk;
	vtk_view		m_center_vtk;
	vtk_view_right	m_right_vtk;
	vtk_view_bottom m_bottom_vtk;
// 建構
public:
	CoglMFCDialogDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_OGLMFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMThemeChangedSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	int m_SliderCenter;
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_ControlSliderCenter;
	CSliderCtrl m_ControlSliderLeft;
	int m_SliderLeft;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_ControlSliderRight;
	int m_SliderRight;
	CString m_ShowValue_Right;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_ShowValue_Center;
	CString m_ShowValue_Left;
};
