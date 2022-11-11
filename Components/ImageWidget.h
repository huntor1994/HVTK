#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
class ImageWidget :public QWidget
{
	Q_OBJECT
public:
	ImageWidget(QString imgPath);
	~ImageWidget() override;
protected:
	virtual void paintEvent(QPaintEvent* event) override;
private:
	QPixmap* image;
	QLabel* imageLabel;
};