#include "thumbnail.h"
#include <QtCore>
#include <QtGui>
#include <QThread>


Thumbnail::Thumbnail(QObject *parent) :
    QObject(parent)
{
	imageFilePath = "";
	resizedWidth = 64;
	resizedHeight = 64;
	fastTransform = true;
	isResized = false;
}

void Thumbnail::setImageFromFile(QString filePath, int w, int h, bool fast)
{
	imageFilePath = filePath;
	resizedWidth = w;
	resizedHeight = h;
	fastTransform = fast;

}


void Thumbnail::resizeImage()
{
	if (!isResized) {
		QImage image(imageFilePath);
		resizedImage = image.scaled(resizedWidth, resizedHeight, Qt::KeepAspectRatio, (fastTransform ? Qt::FastTransformation : Qt::SmoothTransformation) );
		isResized = true;
	}
	this->moveToThread(QApplication::instance()->thread());
	QThread::currentThread()->exit(0);
}
