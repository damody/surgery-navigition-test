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
	m_DICOM->Update();
	vtkImageData_Sptr imgdata = vtkSmartNew;
	imgdata->DeepCopy(m_DICOM->GetOutput(0));
	
	
	double Bounds[6];
	int ext[6];
	imgdata->GetExtent(ext);
	vtkIdType count = imgdata->GetPointData()->GetScalars()->GetNumberOfTuples();
	
	printf("%d\n", imgdata->GetDataDimension());
	printf("%d %d %d %d %d %d \n", ext[0], ext[1], ext[2], ext[3], ext[4], ext[5]);
	
	imgdata->GetBounds(Bounds);
	printf("%f %f %f %f %f %f \n", Bounds[0], Bounds[1], Bounds[2], Bounds[3], Bounds[4], Bounds[5]);
	double Spacing[3];
	imgdata->GetSpacing(Spacing);
	imgdata->SetSpacing(1, 1, 10);
	imgdata->GetSpacing(Spacing);
	imgdata->Update();
	printf("%f %f %f\n", Spacing[0], Spacing[1], Spacing[2]);
	
	imgdata->GetBounds(Bounds);
	printf("%f %f %f %f %f %f \n", Bounds[0], Bounds[1], Bounds[2], Bounds[3], Bounds[4], Bounds[5]);

	m_SkinExtractor = vtkSmartNew;
	m_SkinExtractor->SetInput(imgdata);
	//m_SkinExtractor->SetInputConnection(m_DICOM->GetOutputPort());  
	m_SkinExtractor->SetValue(0, 500);  

	m_SkinNormals = vtkSmartNew;
	m_SkinNormals->SetInputConnection(m_SkinExtractor->GetOutputPort());  
	//m_SkinNormals->SetFeatureAngle(60.0);  

	vtkColorTransferFunction_Sptr	colorTransferFunction = vtkSmartNew;
	double step = (1000.0)/6;
	colorTransferFunction->AddRGBPoint(step*6, 1.0 / 2, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(step*5, 1.0 / 2, 165 / 255 / 2.0, 0.0);
	colorTransferFunction->AddRGBPoint(step*4, 1.0 / 2, 1.0 / 2, 0.0);
	colorTransferFunction->AddRGBPoint(step*3, 0.0, 1.0 / 2, 0.0);
	colorTransferFunction->AddRGBPoint(step*2, 0.0, 0.5 / 2, 1.0 / 2);
	colorTransferFunction->AddRGBPoint(step*1, 0.0, 0.0, 1.0 / 2);
	colorTransferFunction->AddRGBPoint(step*0, 139 / 255.0 / 2, 0.0, 1.0 / 2);

	m_PolyMapper = vtkSmartNew;
	m_PolyMapper->SetInputConnection(m_SkinNormals->GetOutputPort());  
	//m_PolyMapper->ScalarVisibilityOff();
	m_PolyMapper->SetLookupTable(colorTransferFunction);
	m_PolyMapper->Update();

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

	

	vtkBounds bounding;
	bounding.SetBounds(m_PolyMapper->GetBounds());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	m_Camera->SetPosition((bounding.Xmid() + bounding.Ymid() + bounding.Zmid()) / 2, 0, 0);
	m_Camera->SetFocalPoint(bounding.Xmid(), bounding.Ymid(), bounding.Zmid());
	// Add the actors to the scene
	//m_Renderer->AddActor(actor);
	
	m_Renderer->AddActor(m_skinActor);
	
	
	m_planeWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
	m_planeWidget->SetInteractor(m_WindowInteractor);
	m_planeWidget->RestrictPlaneToVolumeOn();
	m_planeWidget->SetInput(imgdata);

	m_planeWidget->SetPlaneOrientationToXAxes();
	m_planeWidget->GetColorMap()->SetLookupTable(colorTransferFunction);
	m_planeWidget->UpdatePlacement();
	m_planeWidget->On();
	
// 
// 	// Begin mouse interaction
// 	
	
}
