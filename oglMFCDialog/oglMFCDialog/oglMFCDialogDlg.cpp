
// oglMFCDialogDlg.cpp : ��@��
//

#include "stdafx.h"
#include "oglMFCDialog.h"
#include "oglMFCDialogDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ܤ�����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CoglMFCDialogDlg ��ܤ��



CoglMFCDialogDlg::CoglMFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CoglMFCDialogDlg::IDD, pParent)
	, m_SliderCenter(0)
	, m_SliderLeft(0)
	, m_SliderRight(0)
	, m_ShowValue_Right(_T(""))
	, m_ShowValue_Center(_T(""))
	, m_ShowValue_Left(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CoglMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER2, m_SliderCenter);
	DDX_Control(pDX, IDC_SLIDER2, m_ControlSliderCenter);
	DDX_Control(pDX, IDC_SLIDER1, m_ControlSliderLeft);
	DDX_Slider(pDX, IDC_SLIDER1, m_SliderLeft);
	DDX_Control(pDX, IDC_SLIDER3, m_ControlSliderRight);
	DDX_Slider(pDX, IDC_SLIDER3, m_SliderRight);
	DDX_Text(pDX, IDC_RIGHT_VALUE, m_ShowValue_Right);
	DDX_Text(pDX, IDC_CENTER_VALUE, m_ShowValue_Center);
	DDX_Text(pDX, IDC_LEFT_VALUE, m_ShowValue_Left);
}

BEGIN_MESSAGE_MAP(CoglMFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDER2, &CoglMFCDialogDlg::OnNMThemeChangedSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CoglMFCDialogDlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CoglMFCDialogDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CoglMFCDialogDlg::OnNMCustomdrawSlider3)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CoglMFCDialogDlg �T���B�z�`��

BOOL CoglMFCDialogDlg::OnInitDialog()
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	CDialogEx::OnInitDialog();

	m_ControlSliderRight.SetRange(0,100,TRUE);
	m_ControlSliderRight.SetPos(0);
	m_ShowValue_Right.Format(_T("%d"),0);
	m_ControlSliderCenter.SetRange(0,100,TRUE);
	m_ControlSliderCenter.SetPos(0);
	m_ShowValue_Center.Format(_T("%d"),0);
	m_ControlSliderLeft.SetRange(0,100,TRUE);
	m_ControlSliderLeft.SetPos(0);
	m_ShowValue_Left.Format(_T("%d"),0);
	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w

	CRect rect;

	GetDlgItem(IDC_LEFT_VTK)->GetWindowRect(rect);
	
	m_left_vtk.InitVTK(GetDlgItem(IDC_LEFT_VTK)->GetSafeHwnd(), rect.Width(), rect.Height());
	this->SetTimer(IDC_LEFT_VTK, 1, 0);
	m_center_vtk.InitVTK(GetDlgItem(IDC_CENTER_VTK)->GetSafeHwnd(), rect.Width(), rect.Height());
	this->SetTimer(IDC_CENTER_VTK, 1, 0);
	m_right_vtk.InitVTK(GetDlgItem(IDC_RIGHT_VTK)->GetSafeHwnd(), rect.Width(), rect.Height());
	this->SetTimer(IDC_RIGHT_VTK, 1, 0);

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	//ScreenToClient(rect);
	rect.MoveToX(0);
	rect.MoveToY(0);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, GetDlgItem(IDC_OPENGL));
	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);

	//m_ControlSliderCenter.SetRangeMax(200);
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

void CoglMFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CoglMFCDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CoglMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CoglMFCDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �b���[�J�z���T���B�z�`���{���X
	
}


void CoglMFCDialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �b���[�J�z���T���B�z�`���{���X�M (��) �I�s�w�]��

	CDialogEx::OnTimer(nIDEvent);
	if (nIDEvent == IDC_LEFT_VTK)
	{
		m_left_vtk.Render();
		m_center_vtk.Render();
		m_right_vtk.Render();
	}
}


void CoglMFCDialogDlg::OnNMThemeChangedSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// ���\��ݭn Windows XP �Χ�s�������C
	// �Ÿ� _WIN32_WINNT �����O >= 0x0501�C
	// TODO: �b���[�J����i���B�z�`���{���X
	printf("slider: %d\n", m_SliderCenter);
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	this->UpdateData();
	printf("slider: %d\n", m_SliderCenter);
	m_center_vtk.m_SkinExtractor->SetValue(0, m_SliderCenter);
	// TODO: �b���[�J����i���B�z�`���{���X
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	this->UpdateData();
	printf("slider: %d\n", m_SliderLeft);
	m_left_vtk.m_SkinExtractor->SetValue(0, m_SliderLeft);
	// TODO: �b���[�J����i���B�z�`���{���X
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	this->UpdateData();
	printf("slider: %d\n", m_SliderRight);
	m_right_vtk.m_SkinExtractor->SetValue(0, m_SliderRight);
	// TODO: �b���[�J����i���B�z�`���{���X
	*pResult = 0;
}


void CoglMFCDialogDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar ==(CScrollBar*)& m_ControlSliderRight)
	{
		int value = m_ControlSliderRight.GetPos();
		m_ShowValue_Right.Format(_T("%d"),value);
		UpdateData(FALSE);
	} 
		if (pScrollBar ==(CScrollBar*)& m_ControlSliderCenter)
			{
						int value = m_ControlSliderCenter.GetPos();
						m_ShowValue_Center.Format(_T("%d"),value);
						UpdateData(FALSE);
						 	} 
		if (pScrollBar ==(CScrollBar*)& m_ControlSliderLeft)
			{
			 		int value = m_ControlSliderLeft.GetPos();
					m_ShowValue_Left.Format(_T("%d"),value);
			 		UpdateData(FALSE);
					} 

	else 
	{
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}

	// TODO: �b���[�J�z���T���B�z�`���{���X�M (��) �I�s�w�]��

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
