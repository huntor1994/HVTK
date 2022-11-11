#include "SelectView.h"
#include "ui_SelectView.h"

SelectView::SelectView(QStackedLayout* stackedLayout):stackedLayout(stackedLayout){
	this->ui = new Ui_SelectView;
	this->gridLayout = new AutoGridLayout(this);

	this->ui->setupUi(this);
	this->setObjectName(QString::fromUtf8("selectView"));
	this->setStyleSheet("#selectView{background-color: #202020;}");
	
	gridLayout->setSizeConstraint(gridLayout->SetMinimumSize);
	//this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	for(int i=0;i< buttonCount;i++)
	{
		button[i] = new SelectButton("../Resources/ButtonImage/"+buttonImageName[i] + ".png", buttonImageName[i]);
		connect(button[i], &SelectButton::buttonPressed, this, &SelectView::ChangeView);
		gridLayout->addWidget(button[i]);
	}
}

SelectView::~SelectView() {
	delete this->button;
	delete this->gridLayout;
	delete this->ui;
}

void SelectView::ChangeView() {
	stackedLayout->setCurrentIndex(0);
}