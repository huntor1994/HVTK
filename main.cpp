#include <QtWidgets/QApplication>
#include "MainView.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	MainView* myView = new MainView();
	myView->show();

	return app.exec();
}