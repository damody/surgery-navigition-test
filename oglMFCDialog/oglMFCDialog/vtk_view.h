#pragma once
#include <windows.h>
#include "vtk_define.h"
#include <vtkCubeSource.h>
#include "vtkBounds.h"

class vtk_view
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
	vtkContourFilter_Sptr		m_SkinExtractor;
	vtkPolyDataNormals_Sptr		m_SkinNormals;
	vtkPolyDataMapper_Sptr		m_PolyMapper;
	vtkActor_Sptr				m_skinActor;
	HWND	m_hwnd;
	vtk_view(void);
	~vtk_view(void);
	void InitVTK(HWND hwnd, int w, int h);
	void Render()
	{
	//	m_RenderWindow->Render();
	}
};

