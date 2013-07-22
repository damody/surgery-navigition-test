#pragma once
#include <windows.h>
#include "vtk_define.h"
#include <vtkCubeSource.h>
#include "vtkBounds.h"
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include "vtkBounds.h"
#include <vtkPolyDataMapper.h>
#include <vtkImagePlaneWidget.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>

class vtk_view_left
{
	
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
	vtkPolyDataMapper_Sptr		m_PolyMapper;
	vtkActor_Sptr				m_skinActor;
	vtkImagePlaneWidget_Sptr	m_planeWidget;
	vtkActor_Sptr m_tubeActor;
	bool m_init;
	HWND	m_hwnd;
	double	m_cubePos[3];
	vtk_view_left(void);
	~vtk_view_left(void);
	
	void InitVTK(HWND hwnd, int w, int h, vtkImageData_Sptr imgdata);
	void Render();
	void SetClip(int c)
	{
		m_planeWidget->SetSliceIndex(c);
	}
	void SetCubePos(double x, double y, double z)
	{
		m_cubePos[0] = x;
		m_cubePos[1] = y;
		m_cubePos[2] = z;
	}
	double centerpos[3];
	void SetCylinder(double* start, double* end);

};

