#pragma once
#include <QWidget>
#include <QMainWindow>
#include<vtkSmartPointer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderer.h>
#include<vtkGenericOpenGLRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkCylinderSource.h>
#include<vtkPolyDataMapper.h>
#include<vtkActor.h>
#include<vtkProperty.h>
#include<vtkLight.h>
#include<vtkCamera.h>
#include<vtkStructuredPointsReader.h>
#include<vtkMarchingCubes.h>
#include<vtkSTLReader.h>
#include<vtkPolyDataWriter.h>
#include<vtkLine.h>
#include<vtkBMPReader.h>
#include<vtkMetaImageReader.h>
#include<vtkImageViewer2.h>
#include<vtkImageData.h>
#include<vtkMatrix4x4.h>
#include<vtkImageReslice.h>
#include<vtkLookupTable.h>
#include<vtkImageMapToColors.h>
#include<vtkImageActor.h>
#include<vtkInteractorStyleImage.h>
#include<vtkDistanceWidget.h>
#include<vtkGenericOpenGLRenderWindow.h>
#include<vtkResliceImageViewer.h>
#include <vtkImageData.h>
#include <vtkImageStencil.h>
#include <vtkMetaImageWriter.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataToImageStencil.h>
#include<QDir>
#include<vtkImagePlaneWidget.h>
#include<vtkResliceCursorLineRepresentation.h>
#include<vtkResliceCursorWidget.h>
#include<vtkResliceCursorPolyDataAlgorithm.h>
#include<vtkResliceCursorActor.h>
#include<vtkPlaneSource.h>
#include<vtkCellPicker.h>
#include<vtkAppendPolyData.h>
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformation.h"
#include<vtkImageActor.h>
#include<vtkInformation.h>
#include<vtkAlgorithmOutput.h>
#include "vtkImageMapper3D.h"
#include "../Cores/PropPickerInteractorStyle.h"
class Ui_WorkbenchView;
enum ModelType {
	Head=0,
	Shoulder=1,
	Hand=2,
	Spine=3,
	Chest=4,
	Abdomen=5,
	Pelvic=6,
	Hip=7,
	Knee=8,
	Man=9,
	Woman=10
};

class WorkbenchView :public QWidget
{
	Q_OBJECT

public:
	WorkbenchView();
	~WorkbenchView() override;
	void LoadModel(ModelType type);
	

private:
	
	vtkAlgorithmOutput* Polydata2ResliceData();
	void GetResliceImage();
	void SetResliceWindow();
	Ui_WorkbenchView* ui;
	vtkNew<vtkGenericOpenGLRenderWindow> renWin[4];
	vtkNew<vtkResliceImageViewer> riw[3];
	vtkNew<vtkImagePlaneWidget> planeWidget[3];
	vtkNew<vtkDistanceWidget> DistanceWidget[3];
	vtkNew<vtkImageStencil> imgstenc;
	vtkNew<vtkImageReslice> reslice[3];
	vtkNew<vtkAppendPolyData> stlAppend;
	QString folder[11] = {"Head","Shoulder","Hand","Spine","Chest","Abdomen","Pelvic","Hip","Knee","Man","Woman"};
};