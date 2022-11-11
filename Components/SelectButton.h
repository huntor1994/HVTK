#pragma once
#include <QWidget>
#include <QPushButton>

class SelectButton :public QWidget
{
	Q_OBJECT

public:
	SelectButton(QString imgPath,QString text,int imageRatio=0.8);
	~SelectButton() override;
signals:
	void buttonPressed();
protected:
	void resizeEvent(QResizeEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
	void initQssFiles();
	QPixmap* buttonImage;
	QString text;
	int imageRatio;
};