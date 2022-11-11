#include "MainView.h"
#include "ui_MainWindow.h"
#include<QScrollArea>
#include<QStackedLayout>
MainView::MainView() {
	this->ui = new Ui_MainView;
	this->ui->setupUi(this);
	
	this->stackedLayout = new QStackedLayout();
	workBenchView = new WorkbenchView();
	workBenchView->setParent(this->ui->centralwidget);
	//workBenchView->hide();

	this->scrollArea=new QScrollArea(this->ui->centralwidget);
	selectView = new SelectView(stackedLayout);
	scrollArea->setWidget(selectView);
	scrollArea->setWidgetResizable(true);

	stackedLayout->addWidget(workBenchView);
	stackedLayout->addWidget(scrollArea);
	stackedLayout->setCurrentIndex(1);

	this->ui->centralwidget->layout()->addItem(stackedLayout);
}

MainView::~MainView() {
	delete this->workBenchView;
	delete this->selectView;
	delete this->stackedLayout;
	delete this->scrollArea;
	delete this->ui;
}