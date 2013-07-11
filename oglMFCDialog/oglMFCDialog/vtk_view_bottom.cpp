#include "stdafx.h"
#include "vtk_view_bottom.h"
#include "KeyPressInteractorStyle.h"
#include <vtkTransformFilter.h>
#include <auto_link_vtk.hpp>
#include <cstdio>
#include <windows.h>
#include <cstdio>
#include <string>

class FileMappingGetter
{
public:
	int	m_BufferSize;
	HANDLE m_hMapFile;
	char* m_pBuf;
	FileMappingGetter():m_BufferSize(1024){}
	~FileMappingGetter()
	{
		UnmapViewOfFile(m_pBuf);
		CloseHandle(m_hMapFile);
	}
	bool OpenShareMemory(std::wstring name)
	{
		m_hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			name.c_str());         // name of mapping object
		if (m_hMapFile == NULL)
		{
			return false;
		}
		m_pBuf = (char*) MapViewOfFile(m_hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			m_BufferSize);

		if (m_pBuf == NULL)
		{
			CloseHandle(m_hMapFile);

			return false;
		}
		return true;
	}
	void GetData(int* data)
	{
		for (int i=0;i<6;i++)
		{
			data[i] = ((int*)m_pBuf)[i];
		}
	}
};


vtkStandardNewMacro(KeyPressInteractorStyle);
vtk_view_bottom::vtk_view_bottom(void)
{
	Cube1_thita=0,Cylinder5_thita=0,Cylinder6_thita=0,Cylinder9_thita=0,Cylinder10_displace=0,Cylinder10_lenth=0;
	m_alpha = 2;
}


vtk_view_bottom::~vtk_view_bottom(void)
{
}


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
	imgdata->SetDimensions(512,655,512);
	imgdata->GetPointData()->SetScalars(input_scalars);
	return imgdata;
}

void vtk_view_bottom::InitVTK( HWND hwnd, int w, int h, vtkDICOMImageReader_Sptr dicom )
{
// 	m_hwnd = CreateWindowA("edit", "", WS_CHILD | WS_DISABLED | WS_VISIBLE
// 		, 0, 0, w, h, hwnd, 
// 		(HMENU)"", GetModuleHandle(NULL), NULL);
// 	ShowWindow(m_hwnd, true);
// 	UpdateWindow(m_hwnd);
	m_hwnd = hwnd;
	m_DICOM = dicom;
	m_imgdata = vtkSmartNew;
	m_imgdata->DeepCopy(m_DICOM->GetOutput(0));
	

	double Bounds[6];
	int ext[6];
	m_imgdata->GetExtent(ext);
	vtkIdType count = m_imgdata->GetPointData()->GetScalars()->GetNumberOfTuples();
	printf("%d\n", m_imgdata->GetDataDimension());
	printf("%d %d %d %d %d %d \n", ext[0], ext[1], ext[2], ext[3], ext[4], ext[5]);
// 	ext[2] -= 1500;
// 	ext[3] -= 1500;
 	m_imgdata->SetExtent(ext);

	m_imgdata->GetBounds(Bounds);
	printf("%f %f %f %f %f %f \n", Bounds[0], Bounds[1], Bounds[2], Bounds[3], Bounds[4], Bounds[5]);
	double Spacing[3];
	printf("x=%f y=%f z=%f",m_imgdata->GetOrigin()[0], m_imgdata->GetOrigin()[1], m_imgdata->GetOrigin()[2]);
	m_Offset[0] = 420;
	m_Offset[1] = -350;
	m_Offset[2] = -500;
	m_imgdata->SetOrigin(m_Offset);
	m_imgdata->GetSpacing(Spacing);
	m_imgdata->SetSpacing(0.65, 0.65, 0.65);
	m_imgdata->GetSpacing(Spacing);
	m_imgdata->Update();
	printf("%f %f %f\n", Spacing[0], Spacing[1], Spacing[2]);

	m_imgdata->GetBounds(Bounds);
	printf("%f %f %f %f %f %f \n", Bounds[0], Bounds[1], Bounds[2], Bounds[3], Bounds[4], Bounds[5]);

	InverseZ(m_imgdata);
	vtkSmartPointer<vtkPolyData> inputPolyData;

	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetPhiResolution(15);
	sphereSource->SetThetaResolution(15);
	sphereSource->Update();
	inputPolyData = sphereSource->GetOutput();


	m_SkinExtractor = vtkSmartNew;
	m_SkinExtractor->SetInput(m_imgdata);
	//m_SkinExtractor->SetInputConnection(transformFilter->GetOutputPort());  
	m_SkinExtractor->SetValue(0, 500);  

	m_SkinNormals = vtkSmartNew;
	m_SkinNormals->SetInputConnection(m_SkinExtractor->GetOutputPort());  
	//m_SkinNormals->SetFeatureAngle(60.0);  

	m_PolyMapper = vtkSmartNew;
	m_PolyMapper->SetInputConnection(m_SkinNormals->GetOutputPort());  
	//m_PolyMapper->ScalarVisibilityOff();
	m_PolyMapper->SetLookupTable(colorTransferFunction);

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
	//m_WindowInteractor->EnableRenderOn();
	m_Renderer->SetActiveCamera(m_Camera);
	m_Renderer->SetBackground(.0, .0, .0);
	// reference coordinate frame color
	m_Axes_widget->SetOutlineColor(0.8300, 0.6700, 0.5300);
	m_Axes_widget->SetOrientationMarker(m_Axes);
	m_Axes_widget->SetInteractor(m_WindowInteractor);
	m_Axes_widget->On();
	
	m_RenderWindow->SetParentId(m_hwnd);
	m_RenderWindow->SetSize(w, h);
	
	//volume rendering
	compositeOpacity = vtkSmartNew;
	colorTransferFunction = vtkSmartNew;
	volumeMapper = vtkOnlyNew;
	volumeProperty = vtkSmartNew;

	volumeMapper->SetBlendModeToComposite();		// composite first
	volumeMapper->SetRequestedRenderMode(vtkSmartVolumeMapper::GPURenderMode);
	volumeMapper->SetInputConnection(m_imgdata->GetProducerPort());
	double g_max = 1000, g_min = 0;
	double step = (g_max-g_min)/6;
 	compositeOpacity->AddPoint(g_min+step*0, 1);
 	compositeOpacity->AddPoint(g_min+step*1, 1);
 	compositeOpacity->AddPoint(g_min+step*2, 1);
 	compositeOpacity->AddPoint(g_min+step*3, 1);
 	compositeOpacity->AddPoint(g_min+step*4, 1);
 	compositeOpacity->AddPoint(g_min+step*5, 1);
 	compositeOpacity->AddPoint(g_min+step*6, 1);

	volumeProperty->SetScalarOpacity(compositeOpacity);	// composite first.
// 	volumeProperty->SetDiffuse(0.2);
// 	volumeProperty->ShadeOff();
// 	volumeProperty->SetDisableGradientOpacity(1);
 	volumeProperty->SetInterpolationType(VTK_CUBIC_INTERPOLATION);
// 	colorTransferFunction->AddRGBPoint(g_min+step*6, 1.0 / 2, 0.0, 0.0);
// 	colorTransferFunction->AddRGBPoint(g_min+step*5, 1.0 / 2, 165 / 255 / 2.0, 0.0);
// 	colorTransferFunction->AddRGBPoint(g_min+step*4, 1.0 / 2, 1.0 / 2, 0.0);
// 	colorTransferFunction->AddRGBPoint(g_min+step*3, 0.0, 1.0 / 2, 0.0);
// 	colorTransferFunction->AddRGBPoint(g_min+step*2, 0.0, 0.5 / 2, 1.0 / 2);
// 	colorTransferFunction->AddRGBPoint(g_min+step*1, 0.0, 0.0, 1.0 / 2);
// 	colorTransferFunction->AddRGBPoint(g_min+step*0, 139 / 255.0 / 2, 0.0, 1.0 / 2);
	colorTransferFunction->AddRGBPoint(0.0, 0.5, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(1000.0, 0.0, 1.0, 0.0);//皮肤
	colorTransferFunction->AddRGBPoint(1150, 1, 1, 1);//骨骼
	colorTransferFunction->AddRGBPoint(2550.0, 1, 1, 1);
	volumeProperty->SetColor(colorTransferFunction);
	m_volume = vtkSmartNew;
	m_volume->SetMapper(volumeMapper);
	m_volume->SetProperty(volumeProperty);
	m_Renderer->AddViewProp(m_volume);
	//volume rendering end

	vtkBounds bounding;
	bounding.SetBounds(m_PolyMapper->GetBounds());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	m_Camera->SetViewUp(1,0,0);
	m_Camera->SetPosition(bounding.Xmid(), bounding.Ymid(), (bounding.Xmid() + bounding.Ymid() + bounding.Zmid()));
	m_Camera->SetFocalPoint(bounding.Xmid(), bounding.Ymid(), bounding.Zmid());
	// Add the actors to the scene
	//m_Renderer->AddActor(actor);
	//m_Renderer->AddActor(m_skinActor);
	
	m_planeWidgetX = vtkSmartNew;
	m_planeWidgetX->SetInteractor(m_WindowInteractor);
	m_planeWidgetX->RestrictPlaneToVolumeOn();
	m_planeWidgetX->SetInput(m_imgdata);
	m_planeWidgetX->SetPlaneOrientationToXAxes();
	m_planeWidgetX->GetColorMap()->SetLookupTable(colorTransferFunction);
	m_planeWidgetX->UpdatePlacement();
	//m_planeWidgetX->On();

	m_planeWidgetY = vtkSmartNew;
	m_planeWidgetY->SetInteractor(m_WindowInteractor);
	m_planeWidgetY->RestrictPlaneToVolumeOn();
	m_planeWidgetY->SetInput(m_imgdata);
	m_planeWidgetY->SetPlaneOrientationToYAxes();
	m_planeWidgetY->GetColorMap()->SetLookupTable(colorTransferFunction);
	m_planeWidgetY->UpdatePlacement();
	//m_planeWidgetY->On();

	m_planeWidgetZ = vtkSmartNew;
	m_planeWidgetZ->SetInteractor(m_WindowInteractor);
	m_planeWidgetZ->RestrictPlaneToVolumeOn();
	m_planeWidgetZ->SetInput(m_imgdata);
	m_planeWidgetZ->SetPlaneOrientationToZAxes();
	m_planeWidgetZ->GetColorMap()->SetLookupTable(colorTransferFunction);
	m_planeWidgetZ->UpdatePlacement();
	//m_planeWidgetZ->On();


	vtkSmartPointer<vtkConeSource> cone =
		vtkSmartPointer<vtkConeSource>::New();
	cone->SetResolution(16);
	cone->SetHeight(5);
	cone->SetRadius(2.5);
	vtkSmartPointer<vtkPolyDataMapper> glyphMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	glyphMapper->SetInputConnection(cone->GetOutputPort());
	vtkSmartPointer<vtkActor> glyphActor =
		vtkSmartPointer<vtkActor>::New();
	glyphActor->SetMapper(glyphMapper);
	//glyphActor->VisibilityOn();
	m_Renderer->AddActor(glyphActor);
	m_vtkmyPWCallback =
		vtkSmartPointer<vtkmyPWCallback>::New();
	m_vtkmyPWCallback->CursorActor = glyphActor;
	m_pointWidget = vtkSmartPointer<vtkPointWidget>::New();
	m_pointWidget->SetInteractor(m_WindowInteractor);
	m_pointWidget->SetInput(m_imgdata);
	m_pointWidget->AllOff();
	m_pointWidget->PlaceWidget();
	m_pointWidget->AddObserver(vtkCommand::InteractionEvent, m_vtkmyPWCallback);
	m_pointWidget->On();
	
	
	
	// 
	// 	// Begin mouse interaction
	// 	
// 	vtkSmartPointer<KeyPressInteractorStyle> style = 
// 		vtkSmartPointer<KeyPressInteractorStyle>::New();
// 	style->Set(this);
// 	m_WindowInteractor->SetInteractorStyle(style);
//	style->SetCurrentRenderer(m_Renderer);
}  

void vtk_view_bottom::Render()
{
	m_RenderWindow->Render();
	double pos[3];
	//if (m_planeWidgetX->GetCursorDataStatus())
	{
		//m_planeWidgetX->GetCurrentCursorPosition(pos);
		pos[0] = m_clipX+400;;
		pos[1] = m_clipY+400;
		pos[2] = m_clipZ-50;
		pos[2] *= 10;
		//printf("x:%f y:%f z:%f\n", pos[0], pos[1], pos[2]);

		static vtkSmartPointer<vtkCubeSource> cubeSource =
			vtkSmartPointer<vtkCubeSource>::New();
		cubeSource->SetXLength(10);
		cubeSource->SetYLength(10);
		cubeSource->SetZLength(10);
		cubeSource->SetCenter(pos);
		// Create a mapper and actor.
		static vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(cubeSource->GetOutputPort());
		static vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		mapper->Update();

// 		vtkSmartPointer<vtkLineSource> line ;
// 
// 		line = vtkLineSource::New();
// 
// 		line->SetPoint1(0.0,0.0,0.0);
// 		line->SetPoint2(0.0,100.0,100.0);
// 		line->Update();
// 		mapper->SetInput(line->GetOutput());
// 		//mapper->Update();
		mapper->Update();
		actor->SetMapper(mapper);

		m_Renderer->AddActor(actor);
			
		Draw_robotic_arm();
		niddlePos1[0] =(niddlePos_tmp1[1]+niddlePos_tmp1[0])/2;
		niddlePos1[1] =(niddlePos_tmp1[3]+niddlePos_tmp1[2])/2;
		niddlePos1[2] =(niddlePos_tmp1[5]+niddlePos_tmp1[4])/2;
		niddlePos2[0] =(niddlePos_tmp2[1]+niddlePos_tmp2[0])/2;
		niddlePos2[1] =(niddlePos_tmp2[3]+niddlePos_tmp2[2])/2;
		niddlePos2[2] =(niddlePos_tmp2[5]+niddlePos_tmp2[4])/2;

		//printf("x=%f,y=%f,z=%f",niddlePos[1]-((niddlePos[1]-niddlePos[0])/2),niddlePos[3]-((niddlePos[3]-niddlePos[2])/2),niddlePos[5]-((niddlePos[5]-niddlePos[4])/2));  		
	}
	m_clipX = m_planeWidgetX->GetSliceIndex();
	m_clipY = m_planeWidgetY->GetSliceIndex();
	m_clipZ = m_planeWidgetZ->GetSliceIndex();

	FileMappingGetter fmg;
	fmg.OpenShareMemory(L"Global\\Lsi5123a");
	int data[6];
	fmg.GetData(data);
	printf("x: %d, ", data[0]);
	printf("y: %d, ", data[1]);
	printf("z: %d, ", data[2]);
	printf("\n");
	Cylinder9_thita = 90-data[2] / 11.4;
	Cylinder5_thita = data[3] / 227.556;
	Cylinder6_thita = -data[1] / 227.556;
	Cylinder10_displace = 97-data[0] / 78.3;
	Cube1_thita = -data[5] / 45.5;
}

void vtk_view_bottom::SetAlpha( double a )
{
	m_alpha = a;
	double g_max = 4095, g_min = 0;
	double step = (g_max-g_min)/6;
  	compositeOpacity->AddPoint(g_min+step*0, 0.001*m_alpha);
  	compositeOpacity->AddPoint(g_min+step*1, 0.002*m_alpha);
  	compositeOpacity->AddPoint(g_min+step*2, 0.003*m_alpha);
  	compositeOpacity->AddPoint(g_min+step*3, 0.004*m_alpha);
  	compositeOpacity->AddPoint(g_min+step*4, 0.005*m_alpha);
  	compositeOpacity->AddPoint(g_min+step*5, 0.006*m_alpha);
    compositeOpacity->AddPoint(g_min+step*6, 0.007*m_alpha);
// 	compositeOpacity->AddPoint(70.0, 0.0);
// 	compositeOpacity->AddPoint(599.0, 0);
// 	compositeOpacity->AddPoint(600.0, 0);
// 	compositeOpacity->AddPoint(1195.0, 0);
// 	compositeOpacity->AddPoint(1200, 0.2);
// 	compositeOpacity->AddPoint(1300, 0.3);
// 	compositeOpacity->AddPoint(2000, 0.3);
// 	compositeOpacity->AddPoint(4095.0, 1.0);
	volumeProperty->SetScalarOpacity(compositeOpacity);

	vtkPiecewiseFunction_Sptr	gradientTransferFunction = vtkSmartNew;
	gradientTransferFunction->AddPoint(0, 2.0);
	gradientTransferFunction->AddPoint(50, 2.0);
	gradientTransferFunction->AddPoint(200, 1.7);
	gradientTransferFunction->AddPoint(300, 1.5);
	gradientTransferFunction->AddPoint(500, 1.3);
	gradientTransferFunction->AddSegment (600, 0.73, 900, 0.9);
	gradientTransferFunction->AddPoint(1150, 0.6);
	gradientTransferFunction->AddPoint(1300, 0.1);  
	volumeProperty->SetGradientOpacity(gradientTransferFunction);
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->SetAmbient(0.2);
	volumeProperty->SetDiffuse(0.9);
	volumeProperty->SetSpecular(0.2);
	volumeProperty->SetSpecularPower(10);    
	//定义光线投射方法为MIP体绘制方法,MIP为体绘制经典算法，
	m_volume->SetProperty(volumeProperty);
	m_volume->Update();
}


void vtk_view_bottom::Draw_robotic_arm()
{
	static vtkSmartPointer<vtkCubeSource> cube = vtkCubeSource::New();
	/*cube ->SetCenter(29,36,29);*/
	/*cube ->SetBounds(0.0,58,0.0,72.0,0.0,58.0);*/
 	cube ->SetXLength(58);
 	cube->SetYLength(72);
 	cube->SetZLength(58);
	cube ->Update();
	
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform->Translate(29,36,29);
	transform->RotateWXYZ(Cube1_thita, 0, 0, 1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetTransform(transform);
	transformFilter->SetInputConnection(cube->GetOutputPort());
	transformFilter->Update();
	static vtkSmartPointer<vtkPolyDataMapper> mapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInput(transformFilter->GetOutput());
	mapper1->Update();
	static vtkSmartPointer<vtkActor> actor1 =
		vtkSmartPointer<vtkActor>::New();
	actor1->SetMapper(mapper1);

	vtkSmartPointer<vtkCylinderSource> CylinderSource1 ;
	CylinderSource1 =vtkCylinderSource::New();
	//CylinderSource1->SetCenter(4.0,-202.0,49.0);
	CylinderSource1->SetRadius(5.0);
	CylinderSource1->SetHeight(288.0);
	CylinderSource1->SetResolution(100);

	//transform
	vtkSmartPointer<vtkTransform> transform1 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform1->SetMatrix(transform->GetMatrix());
	transform1->Translate(173,-32,20);
	transform1->RotateWXYZ(90, 0, 0, 1);
	
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter1 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter1->SetTransform(transform1);
	transformFilter1->SetInputConnection(CylinderSource1->GetOutputPort());
	transformFilter1->Update();
	//vtkSmartPointer<vtkTransform> transform_B = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	//將現在的矩陣與前一個旋轉矩陣做轉換
	// create mapper
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	// 這裡不是直接將CylinderSource->GetOutput()放進mapper3->SetInput()裡
	// 而是要從transformFilter去得到Output放進mapper3->SetInput()中
	Cylindermapper1->SetInput(transformFilter1->GetOutput());
	Cylindermapper1->Update();



	static vtkSmartPointer<vtkActor> Cylinderactor1 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor1->SetMapper(Cylindermapper1);
	//transform
	// 		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 		//transform->RotateWXYZ(double angle, double x, double y, double z);
	// 		transform->RotateWXYZ(45, 1, 1, 1);
	// 
	// 		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = 
	// 			vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	// 
	// 		transformFilter->SetTransform(transform);
	// 		transformFilter->SetInputConnection(CylinderSource->GetOutputPort());
	// 		transformFilter->Update();

	vtkSmartPointer<vtkCylinderSource> CylinderSource2;
	CylinderSource2 =vtkCylinderSource::New();
	//CylinderSource2->SetCenter(4.0, -202.0, 9.0);
	CylinderSource2->SetRadius(5.0);
	CylinderSource2->SetHeight(288.0);
	CylinderSource2->SetResolution(100);

	vtkSmartPointer<vtkTransform> transform2 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform2->SetMatrix(transform->GetMatrix());
	transform2->Translate(173,-32,-20);
	transform2->RotateWXYZ(90, 0, 0, 1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter2 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter2->SetTransform(transform2);
	transformFilter2->SetInputConnection(CylinderSource2->GetOutputPort());
	transformFilter2->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper2->SetInput(transformFilter2->GetOutput());
	Cylindermapper2->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor2 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor2->SetMapper(Cylindermapper2);




	vtkSmartPointer<vtkCylinderSource> CylinderSource3 ;
	CylinderSource3 =vtkCylinderSource::New();
	//CylinderSource3->SetCenter(63.0,-202.0, 49.0);
	CylinderSource3->SetRadius(5.0);
	CylinderSource3->SetHeight(288.0);
	CylinderSource3->SetResolution(100);
	CylinderSource3 ->Update();

	vtkSmartPointer<vtkTransform> transform3 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform3->SetMatrix(transform->GetMatrix());
	transform3->Translate(173,27,20);
	transform3->RotateWXYZ(90, 0, 0, 1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter3 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter3->SetTransform(transform3);
	transformFilter3->SetInputConnection(CylinderSource3->GetOutputPort());
	transformFilter3->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper3 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper3->SetInput(transformFilter3->GetOutput());
	Cylindermapper3->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor3 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor3->SetMapper(Cylindermapper3);



	vtkSmartPointer<vtkCylinderSource> CylinderSource4 ;
	CylinderSource4 =vtkCylinderSource::New();
	//CylinderSource4->SetCenter(63.0,-202.0, 9.0);
	CylinderSource4->SetRadius(5.0);
	CylinderSource4->SetHeight(288.0);
	CylinderSource4->SetResolution(100);
	CylinderSource4 ->Update();

	vtkSmartPointer<vtkTransform> transform4 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform4->SetMatrix(transform->GetMatrix());
	transform4->Translate(173,27,-20);
	transform4->RotateWXYZ(90, 0, 0, 1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter4 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter4->SetTransform(transform4);
	transformFilter4->SetInputConnection(CylinderSource4->GetOutputPort());
	transformFilter4->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper4 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper4->SetInput(transformFilter4->GetOutput());
	Cylindermapper4->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor4 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor4->SetMapper(Cylindermapper4);


	vtkSmartPointer<vtkCubeSource> cube2 = vtkCubeSource::New();
	//cube2 ->SetBounds(346.0,404,0.0,72.0,0.0,58.0)	;
	cube2->SetXLength(58);
	cube2->SetYLength(72);
	cube2->SetZLength(58);
	cube2->Update();
	vtkSmartPointer<vtkTransform> transform_A = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_A->SetMatrix(transform->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_A->Translate(346,0,0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_A = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_A->SetTransform(transform_A);
	transformFilter_A->SetInputConnection(cube2->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_A->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper2->SetInput(transformFilter_A->GetOutput());
	cubemapper2->Update();

	static vtkSmartPointer<vtkActor> cubeactor2 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor2->SetMapper(cubemapper2);


	vtkSmartPointer<vtkCylinderSource> CylinderSource5 ;
	CylinderSource5 =vtkCylinderSource::New();
	//CylinderSource5->SetCenter(375,-20,-36);
	CylinderSource5->SetRadius(27.5);
	CylinderSource5->SetHeight(40.0);
	CylinderSource5->SetResolution(100);
	CylinderSource5 ->Update();

	vtkSmartPointer<vtkTransform> transform5 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform5->SetMatrix(transform->GetMatrix());
	transform5->Translate(346,0,-49);
	transform5->RotateWXYZ(90, 1, 0, 0);
	transform5->RotateWXYZ(Cylinder5_thita, 0,0,1);

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter5 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter5->SetTransform(transform5);
	transformFilter5->SetInputConnection(CylinderSource5->GetOutputPort());
	transformFilter5->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper5 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper5->SetInput(transformFilter5->GetOutput());
	Cylindermapper5->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor5 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor5->SetMapper(Cylindermapper5);


	vtkSmartPointer<vtkCubeSource> cube3 ;
	cube3 = vtkCubeSource::New();
	
	//cube3 ->SetBounds(354.0,396,15,57.0,-64,-40);
	cube3->SetXLength(42);
	cube3->SetYLength(42);
	cube3->SetZLength(24);
	cube3->Update();

	vtkSmartPointer<vtkTransform> transform_B = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_B->SetMatrix(transform->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_B->Translate(346,0,-81);
	transform_B->RotateWXYZ(Cylinder5_thita,0,0,1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_B = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_B->SetTransform(transform_B);
	transformFilter_B->SetInputConnection(cube3->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_B->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper3 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper3->SetInput(transformFilter_B->GetOutput());
	cubemapper3->Update();

	static vtkSmartPointer<vtkActor> cubeactor3 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor3->SetMapper(cubemapper3);

	vtkSmartPointer<vtkCylinderSource> CylinderSource6 ;
	CylinderSource6 =vtkCylinderSource::New();
	//CylinderSource6->SetCenter(25,-451,-52);
	CylinderSource6->SetRadius(5.0);
	CylinderSource6->SetHeight(110.0);
	CylinderSource6->SetResolution(100);
	CylinderSource6->Update();

	vtkSmartPointer<vtkTransform> transform6 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform6->SetMatrix(transform_B->GetMatrix());
	transform6->Translate(76,-11,0);
	transform6->RotateWXYZ(90, 0, 0, 1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter6 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter6->SetTransform(transform6);
	transformFilter6->SetInputConnection(CylinderSource6->GetOutputPort());
	transformFilter6->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper6 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper6->SetInput(transformFilter6->GetOutput());
	Cylindermapper6->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor6 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor6->SetMapper(Cylindermapper6);



	vtkSmartPointer<vtkCylinderSource> CylinderSource7 ;
	CylinderSource7 =vtkCylinderSource::New();
	//CylinderSource7->SetCenter(47,-451,-52);
	CylinderSource7->SetRadius(5.0);
	CylinderSource7->SetHeight(110.0);
	CylinderSource7->SetResolution(100);
	CylinderSource7 ->Update();

	vtkSmartPointer<vtkTransform> transform7 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform7->SetMatrix(transform_B->GetMatrix());
	transform7->Translate(76,11,0);
	transform7->RotateWXYZ(90, 0, 0, 1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter7 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter7->SetTransform(transform7);
	transformFilter7->SetInputConnection(CylinderSource7->GetOutputPort());
	transformFilter7->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper7 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper7->SetInput(transformFilter7->GetOutput());
	Cylindermapper7->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor7 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor7->SetMapper(Cylindermapper7);

	vtkSmartPointer<vtkCubeSource> cube4 ;
	cube4 = vtkCubeSource::New();
	
	cube4->SetXLength(36);
	cube4->SetYLength(42);
	cube4->SetZLength(54);
	//cube4->SetBounds(506,542,15,57,-64,-10);
	cube4->Update();
	vtkSmartPointer<vtkTransform> transform_C = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_C->SetMatrix(transform_B->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_C->Translate(149,0,15);
	
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_C = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_C->SetTransform(transform_C);
	transformFilter_C->SetInputConnection(cube4->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_C->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper4 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper4->SetInput(transformFilter_C->GetOutput());
	cubemapper4->Update();
	static vtkSmartPointer<vtkActor> cubeactor4 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor4->SetMapper(cubemapper4);

	vtkSmartPointer<vtkCylinderSource> CylinderSource8 ;
	CylinderSource8 =vtkCylinderSource::New();
	//CylinderSource8->SetCenter(36,-557,-37);
	CylinderSource8->SetRadius(20.0);
	CylinderSource8->SetHeight(30.0);
	CylinderSource8->SetResolution(100);
	CylinderSource8 ->Update();

	vtkSmartPointer<vtkTransform> transform8 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform8->SetMatrix(transform_B->GetMatrix());
	transform8->Translate(182,0,15);
	transform8->RotateWXYZ(90, 0, 0, 1);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter8 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter8->SetTransform(transform8);
	transformFilter8->SetInputConnection(CylinderSource8->GetOutputPort());
	transformFilter8->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper8 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper8->SetInput(transformFilter8->GetOutput());
	Cylindermapper8->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor8 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor8->SetMapper(Cylindermapper8);


	vtkSmartPointer<vtkCubeSource> cube5 ;
	cube5 = vtkCubeSource::New();
	//cube5->SetBounds(572,606,13,59,-50,-24);
	cube5->SetXLength(34);
	cube5->SetYLength(46);
	cube5->SetZLength(26);
	cube5->Update();
	vtkSmartPointer<vtkTransform> transform_D = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_D->SetMatrix(transform_B->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_D->Translate(214,0,15);
	transform_D->RotateWXYZ(Cylinder6_thita,1,0,0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_D = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_D->SetTransform(transform_D);
	transformFilter_D->SetInputConnection(cube5->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_D->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper5 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper5->SetInput(transformFilter_D->GetOutput());
	cubemapper5->Update();
	static vtkSmartPointer<vtkActor> cubeactor5 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor5->SetMapper(cubemapper5);

	vtkSmartPointer<vtkCubeSource> cube6 ;
	cube6 = vtkCubeSource::New();
	cube6->SetXLength(205);
	cube6->SetYLength(9);
	cube6->SetZLength(26);
	//cube6->SetBounds(572,777,4,13,-50,-24);
	cube6->Update();
	vtkSmartPointer<vtkTransform> transform_E = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_E->SetMatrix(transform_D->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
    transform_E->Translate(85.5,-27.5,0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_E = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_E->SetTransform(transform_E);
	transformFilter_E->SetInputConnection(cube6->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_E->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper6 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper6->SetInput(transformFilter_E->GetOutput());
	cubemapper6->Update();
	static vtkSmartPointer<vtkActor> cubeactor6 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor6->SetMapper(cubemapper6);

	vtkSmartPointer<vtkCubeSource> cube7 ;
	cube7 = vtkCubeSource::New();
	cube7->SetXLength(205);
	cube7->SetYLength(9);
	cube7->SetZLength(26);
	cube7->Update();
	vtkSmartPointer<vtkTransform> transform_F = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_F->SetMatrix(transform_D->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_F->Translate(85.5,27.5,0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_F = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_F->SetTransform(transform_F);
	transformFilter_F->SetInputConnection(cube7->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_F->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper7 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper7->SetInput(transformFilter_F->GetOutput());
	cubemapper7->Update();
	static vtkSmartPointer<vtkActor> cubeactor7 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor7->SetMapper(cubemapper7);

	vtkSmartPointer<vtkCylinderSource> CylinderSource9 ;
	CylinderSource9 =vtkCylinderSource::New();
	//CylinderSource8->SetCenter(36,-557,-37);
	CylinderSource9->SetRadius(4.0);
	CylinderSource9->SetHeight(64.0);
	CylinderSource9->SetResolution(100);
	CylinderSource9 ->Update();

	vtkSmartPointer<vtkTransform> transform9 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform9->SetMatrix(transform_D->GetMatrix());
	transform9->Translate(162,0,0);
	transform9->RotateWXYZ(Cylinder9_thita,0,1,0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter9 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter9->SetTransform(transform9);
	transformFilter9->SetInputConnection(CylinderSource9->GetOutputPort());
	transformFilter9->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper9 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper9->SetInput(transformFilter9->GetOutput());
	Cylindermapper9->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor9 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor9->SetMapper(Cylindermapper9);

	

	vtkSmartPointer<vtkCubeSource> cube8 ;
	cube8 = vtkCubeSource::New();
	//cube8->SetBounds(751,955,14,20,-59,-15);
	cube8 ->SetXLength(204);
	cube8->SetYLength(6);
	cube8->SetZLength(44);
	cube8->Update();
	vtkSmartPointer<vtkTransform> transform_G = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_G->SetMatrix(transform9->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_G->Translate(102,-18,0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_G = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_G->SetTransform(transform_G);
	transformFilter_G->SetInputConnection(cube8->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_G->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper8 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper8->SetInput(transformFilter_G->GetOutput());
	cubemapper8->Update();
	static vtkSmartPointer<vtkActor> cubeactor8 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor8->SetMapper(cubemapper8);

	vtkSmartPointer<vtkCubeSource> cube9 ;
	cube9 = vtkCubeSource::New();
	//cube9->SetBounds(751,955,15,57,-59,-54);
	cube9 ->SetXLength(204);
	cube9->SetYLength(42);
	cube9->SetZLength(6);
	cube9->Update();
	vtkSmartPointer<vtkTransform> transform_H = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_H->SetMatrix(transform9->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_H->Translate(102,0,-19);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_H = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_H->SetTransform(transform_H);
	transformFilter_H->SetInputConnection(cube9->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_H->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper9 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper9->SetInput(transformFilter_H->GetOutput());
	cubemapper9->Update();
	static vtkSmartPointer<vtkActor> cubeactor9 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor9->SetMapper(cubemapper9);



	vtkSmartPointer<vtkCubeSource> cube10 ;
	cube10 = vtkCubeSource::New();
	//cube10->SetBounds(751,955,50,55,-59,-15);
	cube10->SetXLength(204);
	cube10->SetYLength(6);
	cube10->SetZLength(44);
	cube10->Update();
	vtkSmartPointer<vtkTransform> transform_I = vtkSmartPointer<vtkTransform>::New();
	//transform_A->RotateWXYZ(double angle, double x, double y, double z);
	transform_I->SetMatrix(transform9->GetMatrix()); //將現在的矩陣與前一個旋轉矩陣做轉換
	transform_I->Translate(102,18,0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_I = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_I->SetTransform(transform_I);
	transformFilter_I->SetInputConnection(cube10->GetOutputPort());//與轉完後的旋轉矩陣做結合
	transformFilter_I->Update();
	static vtkSmartPointer<vtkPolyDataMapper> cubemapper10 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper10->SetInput(transformFilter_I->GetOutput());
	cubemapper10->Update();
	static vtkSmartPointer<vtkActor> cubeactor10 =
		vtkSmartPointer<vtkActor>::New();
	cubeactor10->SetMapper(cubemapper10);


	vtkSmartPointer<vtkCylinderSource> CylinderSource10 ;
	CylinderSource10 =vtkCylinderSource::New();
	//CylinderSource8->SetCenter(36,-557,-37);
	CylinderSource10->SetRadius(1.0);
	CylinderSource10->SetHeight(200);
	CylinderSource10->SetResolution(100);
	CylinderSource10 ->Update();

	vtkSmartPointer<vtkTransform> transform10 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform10->SetMatrix(transform9->GetMatrix());
	
	transform10->Translate(142+Cylinder10_displace,0,-10);
	transform10->RotateWXYZ(90, 0, 0, 1);
	//transform10->Translate(Cylinder10_displace,0,0)
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter10 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter10->SetTransform(transform10);
	transformFilter10->SetInputConnection(CylinderSource10->GetOutputPort());
	transformFilter10->Update();
	static vtkSmartPointer<vtkPolyDataMapper> Cylindermapper10 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	Cylindermapper10->SetInput(transformFilter10->GetOutput());
	Cylindermapper10->Update();
	static vtkSmartPointer<vtkActor> Cylinderactor10 =
		vtkSmartPointer<vtkActor>::New();
	Cylinderactor10->SetMapper(Cylindermapper10);
	
	


	vtkSmartPointer<vtkSphereSource> sphereSource =		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(1);
	sphereSource->Update();
	
	
	vtkSmartPointer<vtkTransform> transform_spheresource = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform_spheresource->SetMatrix(transform9->GetMatrix());

	transform_spheresource->Translate(142+Cylinder10_displace,0,-10);

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_spheresource = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_spheresource->SetTransform(transform_spheresource);
	transformFilter_spheresource->SetInputConnection(sphereSource->GetOutputPort());
	transformFilter_spheresource->Update();
	static vtkSmartPointer<vtkPolyDataMapper> spheresourcemapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	spheresourcemapper->SetInput(transformFilter_spheresource->GetOutput());
	spheresourcemapper->Update();
	static vtkSmartPointer<vtkActor> spheresourceactor =
		vtkSmartPointer<vtkActor>::New();
	spheresourceactor->SetMapper(spheresourcemapper);
	
	//取得針端坐標點
	spheresourcemapper ->GetBounds(niddlePos_tmp1);
	
	vtkSmartPointer<vtkSphereSource> sphereSource1 =		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource1->SetRadius(1);
	sphereSource1->Update();


	vtkSmartPointer<vtkTransform> transform_spheresource1 = vtkSmartPointer<vtkTransform>::New();
	//transform->RotateWXYZ(double angle, double x, double y, double z);
	transform_spheresource1->SetMatrix(transform9->GetMatrix());

	transform_spheresource1->Translate(Cylinder10_displace,0,-10);

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_spheresource1 = 
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_spheresource1->SetTransform(transform_spheresource1);
	transformFilter_spheresource1->SetInputConnection(sphereSource1->GetOutputPort());
	transformFilter_spheresource1->Update();
	static vtkSmartPointer<vtkPolyDataMapper> spheresourcemapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	spheresourcemapper1->SetInput(transformFilter_spheresource1->GetOutput());
	spheresourcemapper1->Update();
	static vtkSmartPointer<vtkActor> spheresourceactor1 =
		vtkSmartPointer<vtkActor>::New();
	spheresourceactor1->SetMapper(spheresourcemapper1);
	spheresourcemapper1->GetBounds(niddlePos_tmp2);

	m_Renderer->AddActor(actor1);
	m_Renderer->AddActor(cubeactor2);
	m_Renderer->AddActor(cubeactor3);
	m_Renderer->AddActor(cubeactor4);
	m_Renderer->AddActor(cubeactor5);
	m_Renderer->AddActor(cubeactor6);
	m_Renderer->AddActor(cubeactor7);
	m_Renderer->AddActor(cubeactor8);
	m_Renderer->AddActor(cubeactor9);
	m_Renderer->AddActor(cubeactor10);
	m_Renderer->AddActor(Cylinderactor1);
	m_Renderer->AddActor(Cylinderactor2);
	m_Renderer->AddActor(Cylinderactor3);
	m_Renderer->AddActor(Cylinderactor4);
	m_Renderer->AddActor(Cylinderactor5);
	m_Renderer->AddActor(Cylinderactor6);
	m_Renderer->AddActor(Cylinderactor7);
	m_Renderer->AddActor(Cylinderactor8);
	m_Renderer->AddActor(Cylinderactor9);
	m_Renderer->AddActor(Cylinderactor10);
	m_Renderer->AddActor(spheresourceactor);
	m_Renderer->AddActor(spheresourceactor1);
}
void KeyPressInteractorStyle::OnKeyPress()
{
	
		// Get the keypress
		vtkRenderWindowInteractor *rwi = this->Interactor;
		std::string key = rwi->GetKeySym();

		// Output the key that was pressed
		std::cout << "Pressed " << key << std::endl;
		//控制第一個關節的旋轉
			if(key == "a")
		{
			vtk->Cube1_thita += 1;

		}
		if(key == "A")
		{
			vtk->Cube1_thita -= 1;

		}
		//控制第二個關節旋轉
		if(key == "b")
		{
			vtk->Cylinder5_thita +=1;

		}	
		if(key == "B")
		{
			vtk->Cylinder5_thita -=1;

		}
		//控制第三個關節旋轉
		if(key == "c")
		{
			vtk->Cylinder6_thita +=1;

		}
		if(key == "C")
		{
			vtk->Cylinder6_thita -=1;

		}
		//控制第四個關節旋轉
		if(key == "e")
		{
			vtk->Cylinder10_displace +=1;

		}
		if(key == "E")
		{
			vtk->Cylinder10_displace -=1;

		}
		// 控制末端針的前進後退

		if(key == "D")
		{
			vtk->Cylinder9_thita +=1;

		}
		// Handle a "normal" key
		
		if(key == "d")
		{
			vtk->Cylinder9_thita -=1;
		}
	
		// Forward events
		vtkInteractorStyleTrackballCamera::OnKeyPress();
		
}

void vtk_view_bottom::MovePatientPosition( double x, double y, double z )
{
	m_Offset[0] =  420+x;
	m_Offset[1] = -350+y;
	m_Offset[2] = -500+z;
	m_imgdata->SetOrigin(m_Offset);
}

void KeyPressInteractorStyle::Set(vtk_view_bottom *v)
{
	vtk = v;
}


