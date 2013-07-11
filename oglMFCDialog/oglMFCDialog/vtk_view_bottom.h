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
#include <vtkVersion.h>
#include "vtkSmartPointer.h"

#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkSphereSource.h"
#include "vtkGlyph3D.h"
#include "vtkPointWidget.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProbeFilter.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkXMLPolyDataReader.h"


class vtkmyPWCallback : public vtkCommand
{
public:
	static vtkmyPWCallback *New() 
	{
		return new vtkmyPWCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkPointWidget *pointWidget = reinterpret_cast<vtkPointWidget*>(caller);
		pointWidget->GetPosition(position);
		this->CursorActor->VisibilityOn();
		this->CursorActor->SetPosition(position);
	}
	vtkmyPWCallback():CursorActor(0) {}
	double position[3];
	vtkActor     *CursorActor;
};


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
	vtkSmartPointer<vtkPointWidget> m_pointWidget;
	vtkSmartPointer<vtkmyPWCallback> m_vtkmyPWCallback;
	vtkImageData_Sptr m_imgdata;


	int		m_clipX, m_clipY, m_clipZ;
	HWND	m_hwnd;
	double  m_alpha;
	vtk_view_bottom(void);
	~vtk_view_bottom(void);
	void InitVTK(HWND hwnd, int w, int h, vtkDICOMImageReader_Sptr dicom);
	void SetAlpha(double a);
	void Render();
	void Get3DCursor(double* position)
	{
		memcpy(position, m_vtkmyPWCallback->position, sizeof (double)*3);
	}
	void Get3DCursorR(double* position)
	{
		memcpy(position, niddlePos1, sizeof (double)*3);
	}
	double Cylinder10_lenth;
	void GetNiddlePos1(double* v)
	{
		v[0] = niddlePos1[0];
		v[1] = niddlePos1[1];
		v[2] = niddlePos1[2];
	}
	void GetNiddlePos2(double* v)
	{
		v[0] = niddlePos2[0];
		v[1] = niddlePos2[1];
		v[2] = niddlePos2[2];
	}
   void MovePatientPosition(double x, double y, double z);
   
private:
	void Draw_robotic_arm();
	double Cube1_thita,Cylinder5_thita,Cylinder6_thita,Cylinder9_thita,Cylinder10_displace;
	double niddlePos_tmp1[6];
	double niddlePos1[3];
	double niddlePos_tmp2[3];
	double niddlePos2[3];
	double m_Offset[3];	
};

