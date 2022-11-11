#pragma once
#include <QWidget>
#include <QMainWindow>
#include "Views/WorkbenchView.h"
#include "Views/SelectView.h"

class Ui_MainView;
class MainView :public QMainWindow
{
	Q_OBJECT

public:
	MainView();
	~MainView() override;

private:
	Ui_MainView* ui;
	QStackedLayout* stackedLayout{nullptr};
	QScrollArea* scrollArea{ nullptr };
	WorkbenchView* workBenchView{ nullptr };
	SelectView* selectView{ nullptr };
};
