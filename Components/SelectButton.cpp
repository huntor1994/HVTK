#include "SelectButton.h"
#include <QVBoxLayout>
#include <QLabel>
#include "ImageWidget.h"
#include <iostream>
#include <QResizeEvent>
SelectButton::SelectButton(QString imgPath, QString text, int imageRatio):text(text), imageRatio(imageRatio){
	this->initQssFiles();
	this->setMinimumSize(10, 10);
	this->setMaximumSize(600, 600);
	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setObjectName(QString::fromUtf8("selectBtn"));

	ImageWidget* imageWidget = new ImageWidget(imgPath);
	QLabel* textLabel = new QLabel(text);
	textLabel->setAlignment(Qt::AlignCenter);
	QFont serifFont("Times", 10, QFont::Bold);
	textLabel->setFont(serifFont);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(imageWidget, imageRatio);
	layout->addWidget(textLabel,1- imageRatio);
}

SelectButton::~SelectButton() {

}

void SelectButton::initQssFiles()
{
	QFile file("../Components/SelectButton.qss");
	file.open(QFile::ReadOnly);
	this->setStyleSheet(file.readAll());
	file.close();
}


void SelectButton::resizeEvent(QResizeEvent* event) {

}

void SelectButton::mousePressEvent(QMouseEvent* event) {
	std::cout << "Button Pressed!" << std::endl;
	emit buttonPressed();
}