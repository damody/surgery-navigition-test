
// oglMFCDialogDlg.h : 標頭檔
//

#pragma once
#include "OpenGLControl.h"
#include "vtk_view.h"
#include "vtk_view_left.h"
#include "vtk_view_right.h"
#include "vtk_view_bottom.h"
#include "afxcmn.h"
#include "afxwin.h"



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
	virtual ~CoglMFCDialogDlg();
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
	afx_msg void OnStnClickedCenterValue();
	void getcoordinate(double* a);
	double m_showvalue_X;
	double m_editcontrol_showX;
	afx_msg void OnEnUpdateEdit1();
	double m_showvalue_Y;
	double m_showvalue_Z;
	double m_start[3];
	double m_end[3];
	double m_P1[3];
	double m_P2[3];
	double m_P3[3];
	double m_P4[3];
	double m_P5[3];
	double m_P6[3];
	double m_RP1[3];
	double m_RP2[3];
	double m_RP3[3];
	double m_RP4[3];
	double m_RP5[3];
	double m_RP6[3];
	double registrationvector[3];
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	CString m_ShowPos1;
	CString m_ShowPos2;
	CString m_ShowPos3;
	CString m_ShowPos4;
	CString m_ShowPos5;
	CString m_ShowPos6;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	CString m_ShowRPos1;
	CString m_ShowRPos2;
	CString m_ShowRPos3;
	CString m_ShowRPos4;
	CString m_ShowRPos5;
	CString m_ShowRPos6;
	afx_msg void OnBnClickedButtonRegistration();
	afx_msg void OnBnClickedPlanewidget();
	afx_msg void OnBnClickedPlanewidgetoff();
	double coordinatevalue[3];
	CString m_Modify_Z;
	CSliderCtrl m_controlslider_Z;
	int m_ValueSlider_Z;
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
};
