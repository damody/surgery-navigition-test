
// oglMFCDialogDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "oglMFCDialog.h"
#include "oglMFCDialogDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <cstdio>


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	
	// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };
	
protected:
	virtual void DoDataExchange ( CDataExchange* pDX ); // DDX/DDV 支援
	
	// 程式碼實作
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


// CoglMFCDialogDlg 對話方塊



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
	, m_ShowRPos1(_T(""))
	, m_ShowRPos2(_T(""))
	, m_ShowRPos3(_T(""))
	, m_ShowRPos4(_T(""))
	, m_ShowRPos5(_T(""))
	, m_ShowRPos6(_T(""))
	, m_Modify_Z(_T(""))
	, m_ValueSlider_Z(0)
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
	DDX_Text(pDX, IDC_STATIC_RP1, m_ShowRPos1);
	DDX_Text(pDX, IDC_STATIC_RP2, m_ShowRPos2);
	DDX_Text(pDX, IDC_STATIC_RP3, m_ShowRPos3);
	DDX_Text(pDX, IDC_STATIC_RP4, m_ShowRPos4);
	DDX_Text(pDX, IDC_STATIC_RP5, m_ShowRPos5);
	DDX_Text(pDX, IDC_STATIC_RP6, m_ShowRPos6);
	DDX_Text(pDX, IDC_Modify_Z, m_Modify_Z);
	DDX_Control(pDX, IDC_SLIDER_4, m_controlslider_Z);
	DDX_Slider(pDX, IDC_SLIDER_4, m_ValueSlider_Z);
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
	ON_BN_CLICKED(IDC_BUTTON7, &CoglMFCDialogDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CoglMFCDialogDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CoglMFCDialogDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CoglMFCDialogDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CoglMFCDialogDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CoglMFCDialogDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON_Registration, &CoglMFCDialogDlg::OnBnClickedButtonRegistration)
	ON_BN_CLICKED(IDC_PlaneWidget, &CoglMFCDialogDlg::OnBnClickedPlanewidget)
	ON_BN_CLICKED(IDC_PlaneWidgetOff, &CoglMFCDialogDlg::OnBnClickedPlanewidgetoff)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_4, &CoglMFCDialogDlg::OnNMCustomdrawSlider4)
END_MESSAGE_MAP()


// CoglMFCDialogDlg 訊息處理常式


vtkImageData_Sptr InverseZ(vtkImageData* imgdata)
{
	vtkPolyData_Sptr polydata = vtkSmartNew;
	vtkPoints_Sptr input_points = vtkSmartNew;
	vtkDoubleArray_Sptr input_scalars_tmp = vtkSmartNew;
	vtkDoubleArray_Sptr input_scalars = vtkSmartNew;

	int nx, ny, nz, dim[3];
	vtkIdType count = imgdata->GetPointData()->GetScalars()->GetNumberOfTuples();
	imgdata->GetDimensions(dim);
	nx = dim[0];
	ny = dim[1];
	nz = dim[2];
	printf("%d %d %d\n", nx, ny, nz);
	std::vector<double> data;
	for (int k=0;k<nz;++k)
	{
		for (int j=0;j<ny;++j)
		{
			for (int i=0;i<nx;++i)
			{
				input_scalars_tmp->InsertNextTuple1(imgdata->GetPointData()->GetScalars()->GetTuple1(i+nx*j+nx*ny*(nz-k-1)));
			}
		}
	}
	for (int j=0;j<ny;++j)
	{
		for (int k=0;k<nz;++k)
		{
			for (int i=0;i<nx;++i)
			{
				input_points->InsertNextPoint(i, k, j);
				input_scalars->InsertNextTuple1(input_scalars_tmp->GetTuple1(i+nx*j+nx*ny*k));
			}
		}
	}
	// 	polydata->SetPoints(input_points);
	// 	polydata->GetPointData()->SetScalars(input_scalars);
	imgdata->SetDimensions(nx,nz,ny);
	imgdata->GetPointData()->SetScalars(input_scalars);
	return imgdata;
}


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
	m_controlslider_Z.SetRange( 0,100,TRUE );
	m_controlslider_Z.SetPos ( 0 );
	m_Modify_Z.Format ( _T ( "%d" ),0 );

	// 將 [關於...] 功能表加入系統功能表。
	// IDM_ABOUTBOX 必須在系統命令範圍之中。
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
	
	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon ( m_hIcon, TRUE );			// 設定大圖示
	SetIcon ( m_hIcon, FALSE );		// 設定小圖示
	// TODO: 在此加入額外的初始設定
	CRect rect;
	vtkDICOMImageReader_Sptr dicom	= vtkSmartNew;
	dicom->SetDataByteOrderToLittleEndian();
	dicom->SetDirectoryName ( "SE4" ); // 68B0A
	dicom->Update();
	dicom = dicom;
	vtkImageData_Sptr img = vtkSmartNew;
	img->DeepCopy(dicom->GetOutput(0));
	double Bounds[6];

	img->GetBounds(Bounds);
	printf("%f %f %f %f %f %f \n", Bounds[0], Bounds[1], Bounds[2], Bounds[3], Bounds[4], Bounds[5]);
	
	img->Update();
	double m_Offset[3];
	m_Offset[0] = 420;
	m_Offset[1] = -350;
	m_Offset[2] = -500;
	img->SetOrigin(m_Offset);
	img->Update();
	img->SetSpacing(0.65, 0.65, 0.65);
	InverseZ(img);

	GetDlgItem ( IDC_OPENGL )->GetWindowRect ( rect );
	m_bottom_vtk.InitVTK ( GetDlgItem ( IDC_OPENGL )->GetSafeHwnd(), rect.Width(), rect.Height(), img );	
	GetDlgItem ( IDC_LEFT_VTK )->GetWindowRect ( rect );
	m_left_vtk.InitVTK ( GetDlgItem ( IDC_LEFT_VTK )->GetSafeHwnd(), rect.Width(), rect.Height(), img );	
	m_center_vtk.InitVTK ( GetDlgItem ( IDC_CENTER_VTK )->GetSafeHwnd(), rect.Width(), rect.Height(), img );	
	m_right_vtk.InitVTK ( GetDlgItem ( IDC_RIGHT_VTK )->GetSafeHwnd(), rect.Width(), rect.Height(), img );
	img->SetSpacing(0.65, 0.65, 0.65);
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
	m_ControlSliderCenter.SetRange(-60,60);
	m_ControlSliderRight.SetRange(-60,60);
	m_controlslider_Z.SetRange(-60,80);
	this->SetTimer ( IDC_OPENGL, 10, 0 );
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。


void CoglMFCDialogDlg::OnPaint()
{
	if ( IsIconic() )
	{
		CPaintDC dc ( this ); // 繪製的裝置內容
		SendMessage ( WM_ICONERASEBKGND, reinterpret_cast<WPARAM> ( dc.GetSafeHdc() ), 0 );
		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics ( SM_CXICON );
		int cyIcon = GetSystemMetrics ( SM_CYICON );
		CRect rect;
		GetClientRect ( &rect );
		int x = ( rect.Width() - cxIcon + 1 ) / 2;
		int y = ( rect.Height() - cyIcon + 1 ) / 2;
		// 描繪圖示
		dc.DrawIcon ( x, y, m_hIcon );
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CoglMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR> ( m_hIcon );
}

void CoglMFCDialogDlg::getcoordinate ( double* a )
{
	m_showvalue_X = a[0];
	m_showvalue_Y = a[1];
	m_showvalue_Z = a[2];
}

void CoglMFCDialogDlg::OnSize ( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize ( nType, cx, cy );
	// TODO: 在此加入您的訊息處理常式程式碼
}


void CoglMFCDialogDlg::OnTimer ( UINT_PTR nIDEvent )
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	CDialogEx::OnTimer ( nIDEvent );
	
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
	m_bottom_vtk.SetCylinder(tmp1, tmp2);
	m_left_vtk.SetCylinder(tmp1, tmp2);
	m_right_vtk.SetCylinder(tmp1, tmp2);
	m_center_vtk.SetCylinder(tmp1, tmp2);
	this->UpdateData ( 0 );
}


void CoglMFCDialogDlg::OnNMThemeChangedSlider2 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	// 此功能需要 Windows XP 或更新的版本。
	// 符號 _WIN32_WINNT 必須是 >= 0x0501。
	// TODO: 在此加入控制項告知處理常式程式碼
	printf ( "slider: %d\n", m_SliderCenter );
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider2 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> ( pNMHDR );
	this->UpdateData();
	printf ( "slider: %d\n", m_SliderCenter );
	//m_center_vtk.m_SkinExtractor->SetValue(0, m_SliderCenter);
	m_bottom_vtk.MovePatientPosition(coordinatevalue[0]+m_SliderRight,coordinatevalue[1]+m_SliderCenter,coordinatevalue[2]+m_ValueSlider_Z);
	this->UpdateData(TRUE);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider1 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> ( pNMHDR );
	this->UpdateData();
	printf ( "slider: %d\n", m_SliderLeft );
	//m_left_vtk.m_SkinExtractor->SetValue(0, m_SliderLeft);
	m_bottom_vtk.SetAlpha ( m_SliderLeft*0.01 );
	//m_bottom_vtk.MovePatientPosition(m_SliderLeft,0,0);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}


void CoglMFCDialogDlg::OnNMCustomdrawSlider3 ( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> ( pNMHDR );
	this->UpdateData();
	printf ( "slider: %d\n", m_SliderRight );
	//m_bottom_vtk.Cylinder10_lenth=m_SliderRight;
	//m_right_vtk.m_SkinExtractor->SetValue(0, m_SliderRight);
	//m_bottom_vtk.MovePatientPosition(m_SliderRight,0,0);
	m_bottom_vtk.MovePatientPosition(coordinatevalue[0]+m_SliderRight,coordinatevalue[1]+m_SliderCenter,coordinatevalue[2]+m_ValueSlider_Z);
	this->UpdateData(TRUE);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}
void CoglMFCDialogDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> ( pNMHDR );
	this->UpdateData();
	printf ( "sliderZ: %d\n", m_ValueSlider_Z );
	//m_bottom_vtk.Cylinder10_lenth=m_SliderRight;
	//m_right_vtk.m_SkinExtractor->SetValue(0, m_SliderRight);
	//m_bottom_vtk.MovePatientPosition(m_SliderRight,0,0);
	m_bottom_vtk.MovePatientPosition(coordinatevalue[0]+m_SliderRight,coordinatevalue[1]+m_SliderCenter,coordinatevalue[2]+m_ValueSlider_Z);
	this->UpdateData(TRUE);
	// TODO: 在此加入控制項告知處理常式程式碼
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

	if ( pScrollBar == ( CScrollBar* ) &  m_controlslider_Z )
	{
		int value = m_controlslider_Z.GetPos();
		m_Modify_Z.Format ( _T ( "%d" ),value );
		UpdateData ( FALSE );
	}
	else
	{
		CDialogEx::OnHScroll ( nSBCode, nPos, pScrollBar );
	}
	
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	CDialogEx::OnHScroll ( nSBCode, nPos, pScrollBar );
}


void CoglMFCDialogDlg::OnStnClickedCenterValue()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}


void CoglMFCDialogDlg::OnEnUpdateEdit1()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 要傳送 EM_SETEVENTMASK 訊息到控制項的函式
	// 將具有 ENM_UPDATE 旗標 ORed 加入 lParam 遮罩。
	// TODO:  在此加入控制項告知處理常式程式碼
}

CoglMFCDialogDlg::~CoglMFCDialogDlg()
{
}



void CoglMFCDialogDlg::OnBnClickedButton1()
{
	m_bottom_vtk.Get3DCursor(m_P1);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P1[0], m_P1[1], m_P1[2]);
	//swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", 650.95,-79.48,-275.62);
	wprintf(buffer);
	m_ShowPos1.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton2()
{
	m_bottom_vtk.Get3DCursor(m_P2);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P2[0], m_P2[1], m_P2[2]);
	//swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f",602.63 ,-29.20 ,-259.48);
	wprintf(buffer);
	m_ShowPos2.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton3()
{
	m_bottom_vtk.Get3DCursor(m_P3);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P3[0], m_P3[1], m_P3[2]);
	//swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f",556.86 ,-52.77 ,-263.66 );
	wprintf(buffer);
	m_ShowPos3.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton4()
{
	m_bottom_vtk.Get3DCursor(m_P4);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_P4[0], m_P4[1], m_P4[2]);
	//swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f",593.56 ,-102.55,-240.46);
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


void CoglMFCDialogDlg::OnBnClickedButton7()
{
	m_bottom_vtk.Get3DCursorR(m_RP1);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_RP1[0], m_RP1[1], m_RP1[2]);
	wprintf(buffer);
	m_ShowRPos1.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton8()
{
	m_bottom_vtk.Get3DCursorR(m_RP2);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_RP2[0], m_RP2[1], m_RP2[2]);
	wprintf(buffer);
	m_ShowRPos2.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton9()
{

	m_bottom_vtk.Get3DCursorR(m_RP3);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_RP3[0], m_RP3[1], m_RP3[2]);
	wprintf(buffer);
	m_ShowRPos3.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton10()
{
	m_bottom_vtk.Get3DCursorR(m_RP4);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_RP4[0], m_RP4[1], m_RP4[2]);
	wprintf(buffer);
	m_ShowRPos4.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton11()
{
	m_bottom_vtk.Get3DCursorR(m_RP5);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_RP5[0], m_RP5[1], m_RP5[2]);
	wprintf(buffer);
	m_ShowRPos5.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButton12()
{
	m_bottom_vtk.Get3DCursorR(m_RP6);
	wchar_t buffer[100];
	swprintf_s(buffer, L"x:%.2f y:%.2f z:%.2f", m_RP6[0], m_RP6[1], m_RP6[2]);
	wprintf(buffer);
	m_ShowRPos6.SetString(buffer);
	this->UpdateData(FALSE);
}


void CoglMFCDialogDlg::OnBnClickedButtonRegistration()
{
	double tmp[3],tmpr[3];
	// TODO: 在此加入控制項告知處理常式程式碼
//   	tmp[0]=(m_P1[0]+m_P2[0]+m_P3[0]+m_P4[0])/4;
//   	tmp[1]=(m_P1[1]+m_P2[1]+m_P3[1]+m_P4[1])/4;
//   	tmp[2]=(m_P1[2]+m_P2[2]+m_P3[2]+m_P4[2])/4;
  	tmp[0]=(650.69+597.05+553.25+532.64)/4;
  	tmp[1]=(-74.2-29.97-55.70-97.84)/4;
  	tmp[2]=(-282.76-267.28-272-285.31)/4;
   	tmpr[0]=(m_RP1[0]+m_RP2[0]+m_RP3[0]+m_RP4[0])/4;
   	tmpr[1]=(m_RP1[1]+m_RP2[1]+m_RP3[1]+m_RP4[1])/4;
   	tmpr[2]=(m_RP1[2]+m_RP2[2]+m_RP3[2]+m_RP4[2])/4;
//   	tmpr[0]=(617.03+568.71+552.94+559.64)/4;
//  	tmpr[1]=(92.63+142.91+119.34+69.56)/4;
//  	tmpr[2]=(-241.54-225.4-229.58-206.38)/4;
	//m_bottom_vtk.MovePatientPosition(m_RP1[0]-(m_P1[0]-42),m_RP1[1]-(m_P1[1]+32),m_RP1[2]-(m_P1[2]+87));
	//m_bottom_vtk.MovePatientPosition(tmpr[0]-tmp[0]-7-5-5-5,tmpr[1]-tmp[1]-27-18.6,tmpr[2]-tmp[2]-87);
	m_bottom_vtk.MovePatientPosition(tmpr[0]-tmp[0],tmpr[1]-tmp[1],tmpr[2]-tmp[2]);
	//registrationvector[3]=(tmpr[0]-tmp[0],tmpr[1]-tmp[1],tmpr[2]-tmp[2]);
	coordinatevalue[0]=tmpr[0]-tmp[0];
	coordinatevalue[1]=tmpr[1]-tmp[1];
	coordinatevalue[2]=tmpr[2]-tmp[2];
	
	this->UpdateData(FALSE);

	
}


void CoglMFCDialogDlg::OnBnClickedPlanewidget()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	m_bottom_vtk.ImagePlaneWidgetOn();
	this->UpdateData(FALSE);

}


void CoglMFCDialogDlg::OnBnClickedPlanewidgetoff()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	m_bottom_vtk.ImagePlaneWidgetOff();
	this->UpdateData(FALSE);

}


