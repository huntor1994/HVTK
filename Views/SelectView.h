#pragma once
#include <iostream>
#include <QWidget>
#include <QScrollArea>
#include <QMainWindow>
#include <QStackedLayout>
#include "../Components/AutoGridLayout.h"
#include "../Components/SelectButton.h"
#include "../Views/WorkbenchView.h"

class Ui_SelectView;
class SelectView :public QWidget
{
	Q_OBJECT

public:
	SelectView(QStackedLayout* workbenchView);
	~SelectView() override;
public slots:
	void ChangeView();
private:
	Ui_SelectView* ui;
	AutoGridLayout* gridLayout{ nullptr };
	const static int buttonCount = 11;
	SelectButton* button[buttonCount];
	QString buttonImageName[buttonCount] = {
	"Head",
	"Shoulder",
	"Hand",
	"Spine",
	"Chest",
	"Abdomen",
	"Pelvic",
	"Hip",
	"Knee",
	"Man",
	"Woman"
	};
	
	QStackedLayout* stackedLayout{nullptr};
};