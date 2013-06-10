

#ifndef KEY_PRESS_INTERACTOR_STYLE_H
#define KEY_PRESS_INTERACTOR_STYLE_H

#include "stdafx.h"
#include <vtkPolyDataMapper.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "vtk_view_bottom.h"

class vtk_view_bottom;

class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static KeyPressInteractorStyle* New();
	vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

	virtual void OnKeyPress() ;
	
	void Set(vtk_view_bottom *vtk);
private:
	vtk_view_bottom *vtk;
};

#endif
