#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QObject>
#include <QImage>
#include <QString>

class Thumbnail : public QObject
{
	Q_OBJECT

public:
	QImage resizedImage;
	bool isResized;

	Thumbnail(QObject *parent = 0);
	void setImageFromFile(QString filePath, int w, int h, bool fast);

signals:
	
public slots:
	void resizeImage();

private:
	QString imageFilePath;
	int resizedWidth;
	int resizedHeight;
	bool fastTransform;
};

#endif // THUMBNAIL_H
