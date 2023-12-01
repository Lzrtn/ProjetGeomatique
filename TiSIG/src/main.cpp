#include "interface/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	setlocale(LC_NUMERIC,"C");		// change default parameters to resolve atof function
	MainWindow w;
	w.setWindowIcon(QIcon("icons/logo.png"));
	w.show();
	return a.exec();
}
