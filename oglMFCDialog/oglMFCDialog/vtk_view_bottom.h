#pragma once
#include <windows.h>
#include "vtk_define.h"
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include "vtkBounds.h"
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkImagePlaneWidget.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLineSource.h>
#include "KeyPressInteractorStyle.h"

class vtk_view_bottom
{
	friend void KeyPressInteractorStyle::OnKeyPress();
public:
	vtkRenderWindow_Sptr	m_RenderWindow;
	vtkRenderer_Sptr	m_Renderer;
	vtkVolume_Sptr		m_Volume;
	vtkCamera_Sptr		m_Camera;
	vtkAxesActor_Sptr	m_Axes;
	vtkRenderWindowInteractor_Sptr	m_WindowInteractor;
	vtkOrientationMarkerWidget_Sptr	m_Axes_widget;
	vtkInteractorStyleTrackballCamera_Sptr	m_style;
	vtkDICOMImageReader_Sptr	m_DICOM;
	vtkContourFilter_Sptr		m_SkinExtractor;
	vtkPolyDataNormals_Sptr		m_SkinNormals;
	vtkPolyDataMapper_Sptr		m_PolyMapper;
	vtkActor_Sptr				m_skinActor;
	vtkImagePlaneWidget_Sptr	m_planeWidgetX;
	vtkImagePlaneWidget_Sptr	m_planeWidgetY;
	vtkImagePlaneWidget_Sptr	m_planeWidgetZ;
	vtkPiecewiseFunction_Sptr	compositeOpacity;
	vtkColorTransferFunction_Sptr	colorTransferFunction;
	vtkSmartVolumeMapper_Sptr	volumeMapper;
	vtkVolumeProperty_Sptr		volumeProperty;
	vtkVolume_Sptr				m_volume;
	
	
	int		m_clipX, m_clipY, m_clipZ;
	HWND	m_hwnd;
	double  m_alpha;
	vtk_view_bottom(void);
	~vtk_view_bottom(void);
	void InitVTK(HWND hwnd, int w, int h, vtkDICOMImageReader_Sptr dicom);
	void SetAlpha(double a);
	void Render();
	double Cylinder10_lenth;
	double niddlePos[3];
private:
	void Draw_robotic_arm();
	double Cube1_thita,Cylinder5_thita,Cylinder6_thita,Cylinder9_thita,Cylinder10_displace;
};

