#include <vtkMath.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkAbstractPicker.h>
class PropPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	enum PropStatus {
		Moving,
		Static,
	};
	static PropPickerInteractorStyle* New();
	vtkTypeMacro(PropPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

	PropPickerInteractorStyle();
	virtual ~PropPickerInteractorStyle();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseMove();

private:
	vtkActor* LastPickedActor;
	vtkProperty* LastPickedProperty;
	double LastPos[3];
	PropStatus propStatus = PropStatus::Static;
	
};

