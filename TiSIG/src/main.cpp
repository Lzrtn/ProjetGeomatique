#include "interface/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.setWindowIcon(QIcon("src/icons/logo.png"));
	w.show();
	return a.exec();
}
