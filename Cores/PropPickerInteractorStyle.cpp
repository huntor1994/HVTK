#include "../Cores/PropPickerInteractorStyle.h"
#include<vtkPointPicker.h>
#include<iostream>
vtkStandardNewMacro(PropPickerInteractorStyle);

PropPickerInteractorStyle::PropPickerInteractorStyle()
{
	LastPickedActor = NULL;
	LastPickedProperty = vtkProperty::New();
}

PropPickerInteractorStyle::~PropPickerInteractorStyle()
{
	LastPickedProperty->Delete();
	LastPickedActor->Delete();
	delete LastPos;
}

void PropPickerInteractorStyle::OnLeftButtonDown()
{
	//std::cout << "Mouse Down" << std::endl;
	int* clickPos = this->GetInteractor()->GetEventPosition();

	// Pick from this location.
	vtkNew<vtkPropPicker>  picker;
	picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());


	if (picker->GetActor() != nullptr) {
		this->LastPos[0] = picker->GetPickPosition()[0];
		this->LastPos[1] = picker->GetPickPosition()[1];
		this->LastPos[2] = picker->GetPickPosition()[2];
		//std::cout << "LastPos:" << this->LastPos[0] << std::endl;
		// If we picked something before, reset its property
		if (this->LastPickedActor)
		{
			this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
		}

		this->LastPickedActor = picker->GetActor();
		propStatus = PropStatus::Moving;
		if (this->LastPickedActor)
		{
			// Save the property of the picked actor so that we can restore it next time
			this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
			// Highlight the picked actor by changing its properties
			this->LastPickedActor->GetProperty()->SetColor(47. / 255, 215. / 255, 69. / 255);
			this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
			this->LastPickedActor->GetProperty()->SetSpecular(0.0);
		}
	}


	// Forward events
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void PropPickerInteractorStyle::OnLeftButtonUp()
{
	propStatus = PropStatus::Static;
	//std::cout << "Mouse Up" << std::endl;
	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void PropPickerInteractorStyle::OnMouseMove() {
	if (propStatus == PropStatus::Moving) {
		int* clickPos = this->GetInteractor()->GetEventPosition();
		// Pick from this location.
		vtkNew<vtkPointPicker>  picker;
		picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
		double* currentPos = picker->GetPickPosition();
		double x = currentPos[0] - this->LastPos[0],
			y = 0,//currentPos[1] - this->LastPos[1],
			z = 0;// currentPos[2] - this->LastPos[2];
		if (abs(x) >= 10)
		{
			//std::cout << "LastPos:" << this->LastPos[0] << " CurrentPos:" << currentPos[0] << std::endl;
			this->LastPos[0] = currentPos[0];
			this->LastPos[1] = currentPos[1];
			this->LastPos[2] = currentPos[2];
			{
				vtkTransform* transform = vtkTransform::SafeDownCast(this->LastPickedActor->GetUserTransform());
				if (transform == nullptr) {
					vtkNew<vtkTransform> transform2;
					transform2->PostMultiply();
					transform = transform2;
					this->LastPickedActor->SetUserTransform(transform);
				}
				transform->Translate(x, y, z);
				this->GetInteractor()->Render();
			}
		}

	}

	else if (propStatus != PropStatus::Moving)// Forward events
		vtkInteractorStyleTrackballCamera::OnMouseMove();
}