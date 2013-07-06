#include "stdafx.h"
#include "vtk_view_left.h"
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>

vtk_view_left::vtk_view_left(void)
{
	m_cubePos[0] = 0;
	m_cubePos[1] = 0;
	m_cubePos[2] = 0;
	m_init = false;
}


vtk_view_left::~vtk_view_left(void)
{
}

void vtk_view_left::InitVTK( HWND hwnd, int w, int h, vtkDICOMImageReader_Sptr dicom )
{
	m_init = true;
	m_tubeActor = vtkSmartNew;
	m_hwnd = CreateWindowA("edit", "", WS_CHILD | WS_DISABLED | WS_VISIBLE
		, 0, 0, w, h, hwnd, 
		(HMENU)"", GetModuleHandle(NULL), NULL);
	ShowWindow(m_hwnd, true);
	UpdateWindow(m_hwnd);
	m_hwnd = hwnd;
	m_DICOM = dicom;
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
	imgdata->SetSpacing(1, 1, 1);
	imgdata->GetSpacing(Spacing);
	imgdata->Update();
	printf("%f %f %f\n", Spacing[0], Spacing[1], Spacing[2]);
	
	imgdata->GetBounds(Bounds);
	printf("%f %f %f %f %f %f \n", Bounds[0], Bounds[1], Bounds[2], Bounds[3], Bounds[4], Bounds[5]);

	vtkColorTransferFunction_Sptr	colorTransferFunction = vtkSmartNew;
	double step = (1000.0)/6;
	double gray = 0;
// 	colorTransferFunction->AddRGBPoint(step*6, 1.0 / 2, 0.0, 0.0);
// 	colorTransferFunction->AddRGBPoint(step*5, 1.0 / 2, 165 / 255 / 2.0, 0.0);
// 	colorTransferFunction->AddRGBPoint(step*4, 1.0 / 2, 1.0 / 2, 0.0);
// 	colorTransferFunction->AddRGBPoint(step*3, 0.0, 1.0 / 2, 0.0);
// 	colorTransferFunction->AddRGBPoint(step*2, 0.0, 0.5 / 2, 1.0 / 2);
// 	colorTransferFunction->AddRGBPoint(step*1, 0.0, 0.0, 1.0 / 2);
// 	colorTransferFunction->AddRGBPoint(step*0, 139 / 255.0 / 2, 0.0, 1.0 / 2);
	gray = 1/6.0*6;
	colorTransferFunction->AddRGBPoint(step*6, gray, gray, gray);
	gray = 1/6.0*5;
	colorTransferFunction->AddRGBPoint(step*5, gray, gray, gray);
	gray = 1/6.0*4;
	colorTransferFunction->AddRGBPoint(step*4, gray, gray, gray);
	gray = 1/6.0*3;
	colorTransferFunction->AddRGBPoint(step*3, gray, gray, gray);
	gray = 1/6.0*2;
	colorTransferFunction->AddRGBPoint(step*2, gray, gray, gray);
	gray = 1/6.0*1;
	colorTransferFunction->AddRGBPoint(step*1, gray, gray, gray);
	gray = 1/6.0*0;
	colorTransferFunction->AddRGBPoint(step*0, gray, gray, gray);


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
	m_RenderWindow->SetSize(w, h);

	vtkBounds bounding;
	bounding.SetBounds(imgdata->GetBounds());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	m_Camera->SetViewUp(0,1,0);
	m_Camera->SetPosition(2*(bounding.Xmid() + bounding.Ymid() + bounding.Zmid()), bounding.Ymid(), bounding.Zmid());
	m_Camera->SetFocalPoint(bounding.Xmid(), bounding.Ymid(), bounding.Zmid());
	// Add the actors to the scene
	//m_Renderer->AddActor(actor);
	//m_Renderer->AddActor(m_skinActor);
	vtkSmartPointer<vtkCylinderSource> CylinderSource11 ;
	CylinderSource11 =vtkCylinderSource::New();
	CylinderSource11->SetCenter(centerpos);
	CylinderSource11->SetRadius(1.0);
	CylinderSource11->SetHeight(200);
	CylinderSource11->SetResolution(100);
	CylinderSource11->Update();

//  	vtkSmartPointer<vtkTransform> transform11 = vtkSmartPointer<vtkTransform>::New();
//  	//transform->RotateWXYZ(double angle, double x, double y, double z);
//  	
//  
//  	transform11->Translate(102+Cylinder10_displace,0,-10);
//  	transform11->RotateWXYZ(90, 0, 0, 1);
//  	//transform10->Translate(Cylinder10_displace,0,0)
//  	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter10 = 
//  		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//  	transformFilter10->SetTransform(transform10);
//  	transformFilter10->SetInputConnection(CylinderSource11->GetOutputPort());
//  	transformFilter10->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper11 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper11->SetInput(CylinderSource11->GetOutput());
	Cylindermapper11->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor11 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor11->SetMapper(Cylindermapper11);
	
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

void vtk_view_left::Render()
{
	m_RenderWindow->Render();
	{
		static vtkSmartPointer<vtkCubeSource> cubeSource =
			vtkSmartPointer<vtkCubeSource>::New();
		cubeSource->SetXLength(10);
		cubeSource->SetYLength(10);
		cubeSource->SetZLength(10);
		cubeSource->SetCenter(m_cubePos);
		// Create a mapper and actor.
		static vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(cubeSource->GetOutputPort());
		static vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		mapper->Update();
		actor->SetMapper(mapper);
		m_Renderer->AddActor(actor);
	}
}

void vtk_view_left::SetCylinder( double* start, double* end )
{
	if (!m_init) return;
	vtkSmartPointer<vtkLineSource> lineSource = 
		vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(start[0], start[1], start[2]);
	lineSource->SetPoint2(end[0], end[1], end[2]);
	vtkSmartPointer<vtkTubeFilter> tubeFilter = 
		vtkSmartPointer<vtkTubeFilter>::New();
	tubeFilter->SetInputConnection(lineSource->GetOutputPort());
	tubeFilter->SetRadius(1); //default is .5
	tubeFilter->SetNumberOfSides(50);
	tubeFilter->Update();
	vtkSmartPointer<vtkPolyDataMapper> tubeMapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();
	tubeMapper->SetInputConnection(tubeFilter->GetOutputPort());
	
	m_tubeActor->GetProperty()->SetOpacity(0.5); //Make the tube have some transparency.
	m_tubeActor->SetMapper(tubeMapper);
	m_Renderer->AddActor(m_tubeActor);
}


