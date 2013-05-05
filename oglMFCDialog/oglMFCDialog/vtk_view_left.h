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
	vtkContourFilter_Sptr		m_SkinExtractor;
	vtkPolyDataNormals_Sptr		m_SkinNormals;
	vtkPolyDataMapper_Sptr		m_PolyMapper;
	vtkActor_Sptr				m_skinActor;
	vtkImagePlaneWidget_Sptr	m_planeWidget;
	HWND	m_hwnd;
	vtk_view_left(void);
	~vtk_view_left(void);
	void InitVTK(HWND hwnd, int w, int h);
	void Render()
	{
		m_RenderWindow->Render();
		double pos[3];
		if (m_planeWidget->GetCursorDataStatus())
		{
			m_planeWidget->GetCurrentCursorPosition(pos);
			pos[2] *= 10;
			printf("x:%f y:%f z:%f\n", pos[0], pos[1], pos[2]);
			
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
			actor->SetMapper(mapper);
			m_Renderer->AddActor(actor);
		}

	}
};

