#include "WorkbenchView.h"
#include "ui_WorkbenchView.h"
class vtkImageInteractionCallback : public vtkCommand
{
public:
	static vtkImageInteractionCallback* New() { return new vtkImageInteractionCallback; }

	vtkImageInteractionCallback()
	{
		this->Slicing = 0;
		this->ImageReslice = nullptr;
		this->Interactor = nullptr;
	}

	void SetImageReslice(vtkImageReslice* reslice) { this->ImageReslice = reslice; }

	vtkImageReslice* GetImageReslice() { return this->ImageReslice; }

	void SetInteractor(vtkRenderWindowInteractor* interactor) { this->Interactor = interactor; }

	vtkRenderWindowInteractor* GetInteractor() { return this->Interactor; }

	void Execute(vtkObject* caller, unsigned long event, void* callData) override
	{
		vtkRenderWindowInteractor* interactor = this->GetInteractor();

		int lastPos[2];
		interactor->GetLastEventPosition(lastPos);
		int currPos[2];
		interactor->GetEventPosition(currPos);

		if (event == vtkCommand::LeftButtonPressEvent)
		{
			this->Slicing = 1;
		}
		else if (event == vtkCommand::LeftButtonReleaseEvent)
		{
			this->Slicing = 0;
		}
		else if (event == vtkCommand::MouseMoveEvent)
		{
			if (this->Slicing)
			{
				vtkImageReslice* reslice = this->ImageReslice;

				// Increment slice position by deltaY of mouse
				int deltaY = lastPos[1] - currPos[1];

				reslice->Update();
				double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
				vtkMatrix4x4* matrix = reslice->GetResliceAxes();
				// move the center point that we are slicing through
				double point[4];
				double center[4];
				point[0] = 0.0;
				point[1] = 0.0;
				point[2] = sliceSpacing * deltaY;
				point[3] = 1.0;
				matrix->MultiplyPoint(point, center);
				matrix->SetElement(0, 3, center[0]);
				matrix->SetElement(1, 3, center[1]);
				matrix->SetElement(2, 3, center[2]);
				interactor->Render();
			}
			else
			{
				vtkInteractorStyle* style =
					vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
				if (style)
				{
					style->OnMouseMove();
				}
			}
		}
		else if (event == vtkCommand::WindowLevelEvent) {

		}
	}

private:
	// Actions (slicing only, for now)
	int Slicing;

	// Pointer to vtkImageReslice
	vtkImageReslice* ImageReslice;

	// Pointer to the interactor
	vtkRenderWindowInteractor* Interactor;
};



WorkbenchView::WorkbenchView() {
	this->ui = new Ui_WorkbenchView;
	this->ui->setupUi(this);
	
	this->ui->ThreeDimensionsWidget->setRenderWindow(renWin[3]);



	LoadModel(ModelType::Head);
	GetResliceImage();
	SetResliceWindow();
}

WorkbenchView::~WorkbenchView() {
	delete this->renWin;
	delete this->ui;
}

void WorkbenchView::SetResliceWindow() {
	//Init window
	for (int i = 0; i < 3; i++)
	{
		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		riw[i]->SetRenderWindow(renderWindow);
	}
	this->ui->AxialWidget->setRenderWindow(riw[0]->GetRenderWindow());
	this->ui->CoronalWidget->setRenderWindow(riw[1]->GetRenderWindow());
	this->ui->SagittalWidget->setRenderWindow(riw[2]->GetRenderWindow());
	riw[0]->SetupInteractor(this->ui->AxialWidget->renderWindow()->GetInteractor());
	riw[1]->SetupInteractor(this->ui->CoronalWidget->renderWindow()->GetInteractor());
	riw[2]->SetupInteractor(this->ui->SagittalWidget->renderWindow()->GetInteractor());
	//Create a greyscale lookup table
	vtkNew<vtkLookupTable> table;
	table->SetRange(0, 2000);            // image intensity range
	table->SetValueRange(0.0, 1.0);      // from black to white
	table->SetSaturationRange(0.0, 0.0); // no color saturation
	table->SetRampToLinear();
	table->Build();

	for (int i = 0; i < 3; i++) {
		// Map the image through the lookup table
		vtkNew<vtkImageMapToColors> color;
		color->SetLookupTable(table);
		color->SetInputConnection(reslice[i]->GetOutputPort());
		// Display the image
		vtkNew<vtkImageActor> actor;
		actor->GetMapper()->SetInputConnection(color->GetOutputPort());

		vtkNew<vtkRenderer> renderer;
		renderer->AddActor(actor);
		riw[i]->GetRenderWindow()->AddRenderer(renderer);
		riw[i]->GetRenderWindow()->Render();
		
		// Set up the interaction
		vtkNew<vtkInteractorStyleImage> imageStyle;
		riw[i]->GetInteractor()->SetInteractorStyle(imageStyle);
		riw[i]->Render();
		vtkNew<vtkImageInteractionCallback> callback;
		callback->SetImageReslice(reslice[i]);
		callback->SetInteractor(riw[i]->GetInteractor());

		imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
		imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
		imageStyle->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);
		imageStyle->AddObserver(vtkCommand::WindowLevelEvent, callback);
	}
	
	

}

void WorkbenchView::LoadModel(ModelType type) {
	QString folderPath = "../Resources/STLLib/" + folder[type];
	QDir dir(folderPath);
	if (!dir.exists())return;
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	QFileInfoList list = dir.entryInfoList();
	int file_count = list.count();
	if (file_count <= 0)return;
	vtkNew<vtkRenderer> renderer;
	renderer->SetBackground(0.24, 0.24, 0.24);
	renderer->SetBackground2(0.1, 0.1, 0.1);
	renderer->SetGradientBackground(true);
	//renderer->SetViewport(0.25, 0.0, 0.5, 0.5);
	renWin[3]->AddRenderer(renderer);

	vtkNew<PropPickerInteractorStyle> style;
	style->SetDefaultRenderer(renderer);
	renWin[3]->GetInteractor()->SetInteractorStyle(style);
	for (int i = 0; i < file_count; i++)
	{
		QFileInfo file_info = list.at(i);
		QString suffix = file_info.suffix();

		if (QString::compare(suffix, QString("stl"), Qt::CaseInsensitive) == 0)
		{
			QString absolute_file_path = file_info.absoluteFilePath();
			vtkNew<vtkSTLReader> stlReader;
			stlReader->SetFileName(absolute_file_path.toStdString().c_str());
			stlAppend->AddInputConnection(stlReader->GetOutputPort());

			vtkNew<vtkPolyDataMapper> stlMapper;
			vtkNew<vtkActor> stlActor;
			stlMapper->AddInputConnection(stlReader->GetOutputPort());
			stlActor->SetMapper(stlMapper);
			stlActor->GetProperty()->SetColor(220. / 255, 220. / 255, 170. / 255);
			renderer->AddActor(stlActor);
		}
	}
}

vtkAlgorithmOutput* WorkbenchView::Polydata2ResliceData() {
	auto pd = stlAppend->GetOutput();
	stlAppend->Update();

	vtkNew<vtkImageData> whiteImage;
	double bounds[6];

	pd->GetBounds(bounds);
	double spacing[3]; // desired volume spacing
	spacing[0] = 0.5;
	spacing[1] = 0.5;
	spacing[2] = 0.5;
	whiteImage->SetSpacing(spacing);

	// compute dimensions
	int dim[3];
	for (int i = 0; i < 3; i++)
	{
		dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i]));
	}
	whiteImage->SetDimensions(dim);
	whiteImage->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);

	double origin[3];
	origin[0] = bounds[0] + spacing[0] / 2;
	origin[1] = bounds[2] + spacing[1] / 2;
	origin[2] = bounds[4] + spacing[2] / 2;
	whiteImage->SetOrigin(origin);
	whiteImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	// fill the image with foreground voxels:
	unsigned char inval = 255;
	unsigned char outval = 0;
	vtkIdType count = whiteImage->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
	}

	// polygonal data --> image stencil:
	vtkNew<vtkPolyDataToImageStencil> pol2stenc;
	pol2stenc->SetInputData(pd);
	pol2stenc->SetOutputOrigin(origin);
	pol2stenc->SetOutputSpacing(spacing);
	pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
	pol2stenc->Update();

	// cut the corresponding white image and set the background:

	imgstenc->SetInputData(whiteImage);
	imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
	imgstenc->ReverseStencilOff();
	imgstenc->SetBackgroundValue(outval);
	imgstenc->Update();

	vtkImageData* data = vtkImageData::SafeDownCast(imgstenc->GetInputDataObject(0, 0));
	int extent2[6];
	double spacing2[3];
	double origin2[3];
	data->GetExtent(extent2);
	data->GetSpacing(spacing2);
	data->GetOrigin(origin2);

	return imgstenc->GetOutputPort();
	/*vtkNew<vtkMetaImageWriter> writer;
	writer->SetFileName("SphereVolume.mhd");
	writer->SetInputData(imgstenc->GetOutput());
	writer->Write();*/
}

void WorkbenchView::GetResliceImage() {
	//ÇÐÆ¬Í¼Ïñ	
	vtkNew<vtkMetaImageReader> reader;
	reader->SetFileName("../data/SphereVolume.mhd");
	reader->SetDataExtent(0, 63, 0, 63, 1, 93);
	reader->SetDataSpacing(3.2, 3.2, 1.5);
	reader->SetDataOrigin(0, 0, 0);
	reader->UpdateWholeExtent();

	for (int i = 0; i < 3; i++) {
		reslice[i]->SetInputConnection(reader->GetOutputPort());
		/*vtkAlgorithmOutput* alg = Polydata2ResliceData();
		reslice[i]->SetInputConnection(Polydata2ResliceData());*/

		int extent[6];
		double spacing[3];
		double origin[3];

		reader->GetOutputInformation(0)->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
		reader->GetOutput()->GetSpacing(spacing);
		reader->GetOutput()->GetOrigin(origin);

		/*alg->GetOutputInformation(0)->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
		alg->GetOutput()->GetSpacing(spacing);
		alg->GetOutput()->GetOrigin(origin);*/

		double center[3];
		center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
		center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
		center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);
		static double ResliceAxes[3][9] = {
			{1,0,0,0,1,0,0,0,1},	//axialElements
			{1,0,0,0,0,-1,0,1,0},	//coronalElements
			{0,1,0,0,0,-1,-1,0,0} };//sagittalElements

		reslice[i]->SetOutputDimensionality(2);
		reslice[i]->SetResliceAxesDirectionCosines(ResliceAxes[i]);
		reslice[i]->SetResliceAxesOrigin(center);
		reslice[i]->SetInterpolationModeToLinear();
	}
	
}
