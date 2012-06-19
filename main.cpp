#include <QtGui/QApplication>
#include "imageview.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageView w;
	w.show();
	
	return a.exec();
}
