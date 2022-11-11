#include "ImageWidget.h"
#include <QLabel>
#include <QVboxLayout>

ImageWidget::ImageWidget(QString imgPath){

	if (imgPath != "")
	{
		this->setAttribute(Qt::WA_StyledBackground);
		QGridLayout* layout = new QGridLayout(this);
		image = new QPixmap(imgPath);

		int s = this->width() < this->height() ? this->width() : this->height();
		imageLabel = new QLabel();
		imageLabel->setPixmap(image->scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		//imageLabel->setScaledContents(true);
		layout->addWidget(imageLabel);
	}
}

ImageWidget::~ImageWidget() {
	delete image;
	delete imageLabel;
}

void ImageWidget::paintEvent(QPaintEvent* event)
{
	/*this->imageLabel->resize(this->size());
	int s = this->width() < this->height() ? this->width() : this->height();
	imageLabel->setPixmap(image->scaled(s, s, Qt::KeepAspectRatio));*/
}