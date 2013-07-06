
// oglMFCDialogDlg.cpp : ��@��
//

#include "stdafx.h"
#include "oglMFCDialog.h"
#include "oglMFCDialogDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <cstdio>


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	
	// ��ܤ�����
	enum { IDD = IDD_ABOUTBOX };
	
protected:
	virtual void DoDataExchange ( CDataExchange* pDX ); // DDX/DDV �䴩
	
	// �{���X��@
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx ( CAboutDlg::IDD )
{
}

void CAboutDlg::DoDataExchange ( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange ( pDX );
}

BEGIN_MESSAGE_MAP ( CAboutDlg, CDialogEx )
END_MESSAGE_MAP()


// CoglMFCDialogDlg ��ܤ��



CoglMFCDialogDlg::CoglMFCDialogDlg ( CWnd* pParent /*=NULL*/ )
	: CDialogEx ( CoglMFCDialogDlg::IDD, pParent )
	, m_SliderCenter ( 0 )
	, m_SliderLeft ( 0 )
	, m_SliderRight ( 0 )
	, m_ShowValue_Right ( _T ( "" ) )
	, m_ShowValue_Center ( _T ( "" ) )
	, m_ShowValue_Left ( _T ( "" ) )
	, m_showvalue_X ( 0 )
	, m_editcontrol_showX ( 0 )
	, m_showvalue_Y ( 0 )
	, m_showvalue_Z ( 0 )
	, m_ShowPos2(_T(""))
	, m_ShowPos1(_T(""))
	, m_ShowPos3(_T(""))
	, m_ShowPos4(_T(""))
	, m_ShowPos5(_T(""))
	, m_ShowPos6(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
}

void CoglMFCDialogDlg::DoDataExchange ( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Slider ( pDX, IDC_SLIDER2, m_SliderCenter );
	DDX_Control ( pDX, IDC_SLIDER2, m_ControlSliderCenter );
	DDX_Control ( pDX, IDC_SLIDER1, m_ControlSliderLeft );
	DDX_Slider ( pDX, IDC_SLIDER1, m_SliderLeft );
	DDX_Control ( pDX, IDC_SLIDER3, m_ControlSliderRight );
	DDX_Slider ( pDX, IDC_SLIDER3, m_SliderRight );
	DDX_Text ( pDX, IDC_RIGHT_VALUE, m_ShowValue_Right );
	DDX_Text ( pDX, IDC_CENTER_VALUE, m_ShowValue_Center );
	DDX_Text ( pDX, IDC_LEFT_VALUE, m_ShowValue_Left );
	DDX_Text ( pDX, IDC_STATIC_X, m_showvalue_X );
	DDX_Text ( pDX, IDC_STATIC_Y, m_showvalue_Y );
	DDX_Text ( pDX, IDC_STATIC_Z, m_showvalue_Z );
	DDX_Text(pDX, IDC_STATIC_P1, m_ShowPos1);
	DDX_Text(pDX, IDC_STATIC_P2, m_ShowPos2);
	DDX_Text(pDX, IDC_STATIC_P3, m_ShowPos3);
	DDX_Text(pDX, IDC_STATIC_P4, m_ShowPos4);
	DDX_Text(pDX, IDC_STATIC_P5, m_ShowPos5);
	DDX_Text(pDX, IDC_STATIC_P6, m_ShowPos6);
}

BEGIN_MESSAGE_MAP ( CoglMFCDialogDlg, CDialogEx )
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY ( NM_THEMECHANGED, IDC_SLIDER2, &CoglMFCDialogDlg::OnNMThemeChangedSlider2 )
	ON_NOTIFY ( NM_CUSTOMDRAW, IDC_SLIDER2, &CoglMFCDialogDlg::OnNMCustomdrawSlider2 )
	ON_NOTIFY ( NM_CUSTOMDRAW, IDC_SLIDER1, &CoglMFCDialogDlg::OnNMCustomdrawSlider1 )
	ON_NOTIFY ( NM_CUSTOMDRAW, IDC_SLIDER3, &CoglMFCDialogDlg::OnNMCustomdrawSlider3 )
	ON_WM_HSCROLL()
	ON_STN_CLICKED ( IDC_CENTER_VALUE, &CoglMFCDialogDlg::OnStnClickedCenterValue )
	ON_EN_UPDATE ( IDC_EDIT1, &CoglMFCDialogDlg::OnEnUpdateEdit1 )
	ON_BN_CLICKED(IDC_BUTTON1, &CoglMFCDialogDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CoglMFCDialogDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CoglMFCDialogDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CoglMFCDialogDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CoglMFCDialogDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CoglMFCDialogDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CoglMFCDialogDlg �T���B�z�`��

BOOL CoglMFCDialogDlg::OnInitDialog()
{
	AllocConsole();
	HANDLE handle_out = GetStdHandle ( STD_OUTPUT_HANDLE );
	int hCrt = _open_osfhandle ( ( long ) handle_out, _O_TEXT );
	FILE* hf_out = _fdopen ( hCrt, "w" );
	setvbuf ( hf_out, NULL, _IONBF, 1 );
	*stdout = *hf_out;
	double m_showvalue_X=0;
	CDialogEx::OnInitDialog();
	m_ControlSliderRight.SetRange ( 0,100,TRUE );
	m_ControlSliderRight.SetPos ( 0 );
	m_ShowValue_Right.Format ( _T ( "%d" ),0 );
	m_ControlSliderCenter.SetRange ( 0,100,TRUE );
	m_ControlSliderCenter.SetPos ( 0 );
	m_ShowValue_Center.Format ( _T ( "%d" ),0 );
	m_ControlSliderLeft.SetRange ( 0,100,TRUE );
	m_ControlSliderLeft.SetPos ( 0 );
	m_ShowValue_Left.Format ( _T ( "%d" ),0 );
	// �N [����...] �\���[�J�t�Υ\���C
	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
	ASSERT ( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
	ASSERT ( IDM_ABOUTBOX < 0xF000 );
	CMenu* pSysMenu = GetSystemMenu ( FALSE );
	
	if ( pSysMenu != NULL )
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString ( IDS_ABOUTBOX );
		ASSERT ( bNameValid );
		
		if ( !strAboutMenu.IsEmpty() )
		{
			pSysMenu->AppendMenu ( MF_SEPARATOR );
			pSysMenu->AppendMenu ( MF_STRING, IDM_ABOUTBOX, strAboutMenu );
		}
	}
	
	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon ( m_hIcon, TRUE );			// �]�w�j�ϥ�
	SetIcon ( m_hIcon, FALSE );		// �]�w�p�ϥ�
	// TODO: �b���[�J�B�~����l�]�w
	CRect rect;
	vtkDICOMImageReader_Sptr dicom	= vtkSmartNew;
	dicom->SetDataByteOrderToLittleEndian();
	dicom->SetDirectoryName ( "kevin_DICOM" );
	dicom->Update();
	GetDlgItem ( IDC_OPENGL )->GetWindowRect ( rect );
	m_bottom_vtk.InitVTK ( GetDlgItem ( IDC_OPENGL )->GetSafeHwnd(), rect.Width(), rect.Height(), dicom );	
	GetDlgItem ( IDC_LEFT_VTK )->GetWindowRect ( rect );
	m_left_vtk.InitVTK ( GetDlgItem ( IDC_LEFT_VTK )->GetSafeHwnd(), rect.Width(), rect.Height(), dicom );	
	m_center_vtk.InitVTK ( GetDlgItem ( IDC_CENTER_VTK )->GetSafeHwnd(), rect.Width(), rect.Height(), dicom );	
	m_right_vtk.InitVTK ( GetDlgItem ( IDC_RIGHT_VTK )->GetSafeHwnd(), rect.Width(), rect.Height(), dicom );

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem ( IDC_OPENGL )->GetWindowRect ( rect );
	// Convert screen coordinates to client coordinates
	//ScreenToClient(rect);
	rect.MoveToX ( 0 );
	rect.MoveToY ( 0 );
	// Create OpenGL Control window
	//m_oglWindow.oglCreate(rect, GetDlgItem(IDC_OPENGL));
	// Setup the OpenGL Window's timer to render
	//m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);
	m_ControlSliderLeft.SetRangeMax ( 1000 );

	this->SetTimer ( IDC_OPENGL, 10, 0 );
	this->SetTimer ( IDC_LEFT_VTK, 10, 0 );
	this->SetTimer ( IDC_CENTER_VTK, 10, 0 );
	this->SetTimer ( IDC_RIGHT_VTK, 10, 0 );
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

void CoglMFCDialogDlg::OnSysCommand ( UINT nID, LPARAM lParam )
{
	if ( ( nID & 0xFFF0 ) == IDM_ABOUTBOX )
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand ( nID, lParam );
	}
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C


void CoglMFCDialogDlg::OnPaint()
{
	if ( IsIconic() )
	{
		CPaintDC dc ( this ); // ø�s���˸m���e
		SendMessage ( WM_ICONERASEBKGND, reinterpret_cast<WPARAM> ( dc.GetSafeHdc() ), 0 );
		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics ( SM_CXICON );
		int cyIcon = GetSystemMetrics ( SM_CYICON );
		CRect rect;
		GetClientRect ( &rect );
		int x = ( rect.Width() - cxIcon + 1 ) / 2;
		int y = ( rect.Height() - cyIcon + 1 ) / 2;
		// �yø�ϥ�
		dc.DrawIcon ( x, y, m_hIcon );
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
	return static_cast<HCURSOR> ( m_hIcon );
}

void CoglMFCDialogDlg::getcoordinate ( double* a )
{
	m_showvalue_X = ( a[1]+a[0] ) /2;
	m_showvalue_Y = ( a[3]+a[2] ) /2;
	m_showvalue_Z = ( a[5]+a[4] ) /2;
}

void CoglMFCDialogDlg::OnSize ( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize ( nType, cx, cy );
	// TODO: �b���[�J�z���T���B�z�`���{���X
}


void CoglMFCDialogDlg::OnTimer ( UINT_PTR nIDEvent )
{
	// TODO: �b���[�J�z���T���B�z�`���{���X�M (��) �I�s�w�]��
	CDialogEx::OnTimer ( nIDEvent );
	
	if ( nIDEvent == IDC_LEFT_VTK )
	{
		m_bottom_vtk.Render();
		m_left_vtk.SetClip ( m_bottom_vtk.m_clipX );
		m_center_vtk.SetClip ( m_bottom_vtk.m_clipY );
		m_right_vtk.SetClip ( m_bottom_vtk.m_clipZ );
		m_left_vtk.SetCubePos ( m_bottom_vtk.m_clipX, m_bottom_vtk.m_clipY, m_bottom_vtk.m_clipZ*10 );
		m_center_vtk.SetCubePos ( m_bottom_vtk.m_clipX, m_bottom_vtk.m_clipY, m_bottom_vtk.m_clipZ*10 );
		m_right_vtk.SetCubePos ( m_bottom_vtk.m_clipX, m_bottom_vtk.m_clipY, m_bottom_vtk.m_clipZ*10 );
		m_left_vtk.Render();
		m_center_vtk.Render();
		m_right_vtk.Render();
		double tmp1[3], tmp2[3];
		m_bottom_vtk.GetNiddlePos1(tmp1);
		m_bottom_vtk.GetNiddlePos2(tmp2);
		getcoordinate ( tmp1 );
		m_left_vtk.SetCylinder(tmp1, tmp2);
 		m_right_vtk.SetCylinder(tmp1, tmp2);
 		m_center_vtk.SetCylinder(tmp1, tmp2);
		this->UpdateData ( 0 );
	}
}


void CoglMFCDialogDlg::OnNMThemeChangedSlider2 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	// ���\��ݭn Windows XP �Χ�s�������C
	// �Ÿ� _WIN32_WINNT �����O >= 0x0501�C
	// TODO: �b���[�J����i���B�z�`���{���X
	printf ( "slider: %d\n", m_SliderCenter );
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider2 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> ( pNMHDR );
	this->UpdateData();
	printf ( "slider: %d\n", m_SliderCenter );
	//m_center_vtk.m_SkinExtractor->SetValue(0, m_SliderCenter);
	// TODO: �b���[�J����i���B�z�`���{���X
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider1 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> ( pNMHDR );
	this->UpdateData();
	printf ( "slider: %d\n", m_SliderLeft );
	//m_left_vtk.m_SkinExtractor->SetValue(0, m_SliderLeft);
	m_bottom_vtk.SetAlpha ( m_SliderLeft*0.01 );
	// TODO: �b���[�J����i���B�z�`���{���X
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider3 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> ( pNMHDR );
	this->UpdateData();
	printf ( "slider: %d\n", m_SliderRight );
	m_bottom_vtk.Cylinder10_lenth=m_SliderRight;
	//m_right_vtk.m_SkinExtractor->SetValue(0, m_SliderRight);
	// TODO: �b���[�J����i���B�z�`���{���X
	*pResult = 0;
}


void CoglMFCDialogDlg::OnHScroll ( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	if ( pScrollBar == ( CScrollBar* ) & m_ControlSliderRight )
	{
		int value = m_ControlSliderRight.GetPos();
		m_ShowValue_Right.Format ( _T ( "%d" ),value );
		UpdateData ( FALSE );
	}
	
	if ( pScrollBar == ( CScrollBar* ) & m_ControlSliderCenter )
	{
		int value = m_ControlSliderCenter.GetPos();
		m_ShowValue_Center.Format ( _T ( "%d" ),value );
		UpdateData ( FALSE );
	}
	
	if ( pScrollBar == ( CScrollBar* ) & m_ControlSliderLeft )
	{
		int value = m_ControlSliderLeft.GetPos();
		m_ShowValue_Left.Format ( _T ( "%d" ),value );
		UpdateData ( FALSE );
	}
	else
	{
		CDialogEx::OnHScroll ( nSBCode, nPos, pScrollBar );
	}
	
	// TODO: �b���[�J�z���T���B�z�`���{���X�M (��) �I�s�w�]��
	CDialogEx::OnHScroll ( nSBCode, nPos, pScrollBar );
}


void CoglMFCDialogDlg::OnStnClickedCenterValue()
{
	// TODO: �b���[�J����i���B�z�`���{���X
}


void CoglMFCDialogDlg::OnEnUpdateEdit1()
{
	// TODO:  �p�G�o�O RICHEDIT ����A����N���|
	// �ǰe���i���A���D�z�мg CDialogEx::OnInitDialog()
	// �n�ǰe EM_SETEVENTMASK �T���챱����禡
	// �N�㦳 ENM_UPDATE �X�� ORed �[�J lParam �B�n�C
	// TODO:  �b���[�J����i���B�z�`���{���X
}

CoglMFCDialogDlg::~CoglMFCDialogDlg()
{
}



void CoglMFCDialogDlg::OnBnClickedButton1()
{
	m_bottom_vtk.Get3DCursor(m_P1);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P1[0], m_P1[1], m_P1[2]);
	wprintf(buffer);
	m_ShowPos1.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton2()
{
	m_bottom_vtk.Get3DCursor(m_P2);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P2[0], m_P2[1], m_P2[2]);
	wprintf(buffer);
	m_ShowPos2.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton3()
{
	m_bottom_vtk.Get3DCursor(m_P3);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P3[0], m_P3[1], m_P3[2]);
	wprintf(buffer);
	m_ShowPos3.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton4()
{
	m_bottom_vtk.Get3DCursor(m_P4);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P4[0], m_P4[1], m_P4[2]);
	wprintf(buffer);
	m_ShowPos4.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton5()
{
	m_bottom_vtk.Get3DCursor(m_P5);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P5[0], m_P5[1], m_P5[2]);
	wprintf(buffer);
	m_ShowPos5.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton6()
{
	m_bottom_vtk.Get3DCursor(m_P6);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P6[0], m_P6[1], m_P6[2]);
	wprintf(buffer);
	m_ShowPos6.SetString(buffer);
	this->UpdateData(FALSE);
}
