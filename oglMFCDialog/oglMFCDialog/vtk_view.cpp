
#include "vtk_view.h"
#include <auto_link_vtk.hpp>

vtk_view::vtk_view(void)
{
}


vtk_view::~vtk_view(void)
{
}

void vtk_view::InitVTK( HWND hwnd, int w, int h )
{
	m_hwnd = CreateWindowA("edit", "", WS_CHILD | WS_DISABLED | WS_VISIBLE
		, 0, 0, w, h, hwnd, 
		(HMENU)"", GetModuleHandle(NULL), NULL);
	ShowWindow(m_hwnd, true);
	UpdateWindow(m_hwnd);
	//m_hwnd = hwnd;
	m_DICOM	= vtkSmartNew;
	m_DICOM->SetDataByteOrderToLittleEndian();  
	m_DICOM->SetDirectoryName("3b_mpr_pr_hf_vfh");  
	m_DICOM->SetDataSpacing (3.2, 3.2, 1.5);  
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
	// Create a mapper and actor.
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());
	vtkBounds bounding;
	bounding.SetBounds(m_PolyMapper->GetBounds());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	m_Camera->SetPosition(0, 0, (bounding.Xmid() + bounding.Ymid() + bounding.Zmid()) / 2);
	m_Camera->SetFocalPoint(bounding.Xmid(), bounding.Ymid(), bounding.Zmid());
	// Add the actors to the scene
	//m_Renderer->AddActor(actor);
	m_Renderer->AddActor(m_skinActor);
}
