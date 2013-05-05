#include "stdafx.h"
#include "vtk_view_left.h"


vtk_view_left::vtk_view_left(void)
{
}


vtk_view_left::~vtk_view_left(void)
{
}

void vtk_view_left::InitVTK( HWND hwnd, int w, int h )
{
	m_hwnd = CreateWindowA("edit", "", WS_CHILD | WS_DISABLED | WS_VISIBLE
		, 0, 0, w, h, hwnd, 
		(HMENU)"", GetModuleHandle(NULL), NULL);
	ShowWindow(m_hwnd, true);
	UpdateWindow(m_hwnd);
	m_hwnd = hwnd;
	m_DICOM	= vtkSmartNew;
	m_DICOM->SetDataByteOrderToLittleEndian();  
	m_DICOM->SetDirectoryName("3b_mpr_pr_hf_vfh");  
	m_DICOM->SetDataSpacing (3, 3, 3);  
	m_SkinExtractor = vtkSmartNew;
	m_SkinExtractor->SetInputConnection(m_DICOM->GetOutputPort());  
	m_SkinExtractor->SetValue(0, 500);  

	m_SkinNormals = vtkSmartNew;
	m_SkinNormals->SetInputConnection(m_SkinExtractor->GetOutputPort());  
	m_SkinNormals->SetFeatureAngle(60.0);  

	m_PolyMapper = vtkSmartNew;
	m_PolyMapper->SetInputConnection(m_SkinNormals->GetOutputPort());  
	m_PolyMapper->ScalarVisibilityOff();

	m_skinActor = vtkSmartNew;
	m_skinActor->SetMapper(m_PolyMapper);  

	m_RenderWindow = vtkSmartNew;
	m_Renderer = vtkSmartNew;
	m_WindowInteractor = vtkOnlyNew;
	m_Camera = vtkSmartNew;
	m_Axes_widget = vtkSmartNew;
	m_Axes = vtkSmartNew;
	m_style = vtkSmartNew;
	m_RenderWindow->AddRenderer(m_Renderer);
	m_WindowInteractor->SetRenderWindow(m_RenderWindow);
	m_WindowInteractor->SetInteractorStyle(m_style);
	m_WindowInteractor->EnableRenderOn();
	m_Renderer->SetActiveCamera(m_Camera);
	m_Renderer->SetBackground(.0, .0, .0);
	m_Axes_widget->SetOutlineColor(0.8300, 0.6700, 0.5300);
	m_Axes_widget->SetOrientationMarker(m_Axes);
	m_Axes_widget->SetInteractor(m_WindowInteractor);
	m_Axes_widget->On();
	m_RenderWindow->SetParentId(hwnd);
	m_RenderWindow->Render();
	m_RenderWindow->SetSize(w, h);



	vtkSmartPointer<vtkCubeSource> cubeSource =
		vtkSmartPointer<vtkCubeSource>::New();

	vtkSmartPointer<vtkSphereSource> sphereSource = 
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->Update();




	// Create a mapper and actor.
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());
	vtkBounds bounding;
	bounding.SetBounds(m_PolyMapper->GetBounds());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	m_Camera->SetPosition((bounding.Xmid() + bounding.Ymid() + bounding.Zmid()) / 2, 0, 0);
	m_Camera->SetFocalPoint(bounding.Xmid(), bounding.Ymid(), bounding.Zmid());
	// Add the actors to the scene
	//m_Renderer->AddActor(actor);
	m_Renderer->AddActor(m_skinActor);





	

	vtkSmartPointer<vtkImagePlaneWidget> planeWidget = 
		vtkSmartPointer<vtkImagePlaneWidget>::New();
	planeWidget->SetInteractor(m_WindowInteractor);
// 
	double origin[3] = {0, 1,0};
	planeWidget->SetOrigin(origin);
	planeWidget->UpdatePlacement();
// 
// 	// Render
	m_RenderWindow->Render();
// 
	m_WindowInteractor->Initialize();
	m_RenderWindow->Render();
	//planeWidget->On();
	

// 
// 	// Begin mouse interaction
// 	
	
}
