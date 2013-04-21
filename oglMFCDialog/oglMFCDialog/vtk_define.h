#pragma once

#include <vtkSmartPointer.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkProgrammableSource.h>
#include <vtkContourFilter.h>
#include <vtkReverseSense.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkMath.h>
#include <vtkImageData.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include <vtkCubeAxesActor.h>
#include <vtkDICOMImageReader.h>

#include <vector>
#include <vtkVolume.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageMapToColors.h>

#define VTK_SMART_POINTER(x) \
	typedef vtkSmartPointer< x >	x##_Sptr; \
	typedef std::vector< x##_Sptr >	x##_Sptrs;
VTK_SMART_POINTER(vtkPoints);
VTK_SMART_POINTER(vtkDICOMImageReader);
VTK_SMART_POINTER(vtkPolyData);
VTK_SMART_POINTER(vtkDoubleArray);
VTK_SMART_POINTER(vtkImageData);
VTK_SMART_POINTER(vtkRenderWindow);
VTK_SMART_POINTER(vtkRenderer);
VTK_SMART_POINTER(vtkAxesActor);
VTK_SMART_POINTER(vtkCamera);
VTK_SMART_POINTER(vtkRenderWindowInteractor);
VTK_SMART_POINTER(vtkOrientationMarkerWidget);
VTK_SMART_POINTER(vtkInteractorStyleTrackballCamera);
VTK_SMART_POINTER(vtkActor);
VTK_SMART_POINTER(vtkPolyDataMapper);
VTK_SMART_POINTER(vtkVertexGlyphFilter);
VTK_SMART_POINTER(vtkUnsignedCharArray);
VTK_SMART_POINTER(vtkLookupTable);
VTK_SMART_POINTER(vtkVolume);
VTK_SMART_POINTER(vtkPiecewiseFunction);
VTK_SMART_POINTER(vtkColorTransferFunction);
VTK_SMART_POINTER(vtkSmartVolumeMapper);
VTK_SMART_POINTER(vtkVolumeProperty);
VTK_SMART_POINTER(vtkContourFilter);
VTK_SMART_POINTER(vtkPolyDataNormals);
VTK_SMART_POINTER(vtkImagePlaneWidget);
VTK_SMART_POINTER(vtkImageMapToColors);
VTK_SMART_POINTER(vtkScalarBarActor);
VTK_SMART_POINTER(vtkCubeAxesActor);


template<typename T, typename U> class has_member_Initialize_tester
{
private:
	template<U> struct helper;
	template<typename T> static char check(helper < &T::Initialize >*);
	template<typename T> static char(&check(...))[2];
public:
	enum { value = (sizeof(check<T> (0)) == sizeof(char)) };
};
template<char Doit, class T> struct static_Check_To_Initialize
{
	static void	Do(T& ic)	{ ic; }
};
template<class T> struct static_Check_To_Initialize<1, T>
{
	static void	Do(T& ic)	{ ic->Initialize(); }
};
static struct
{
	template<class T> operator vtkSmartPointer<T> ()
	{
		vtkSmartPointer<T> ptr = vtkSmartPointer<T>::New();
		static_Check_To_Initialize<has_member_Initialize_tester<T, void(T::*)()>::value, vtkSmartPointer<T> >::Do(ptr);
		return ptr;
	}
}
vtkSmartNew;
static struct
{
	template<class T> operator vtkSmartPointer<T> ()
	{
		return vtkSmartPointer<T>::New();
	}
}
vtkOnlyNew;

