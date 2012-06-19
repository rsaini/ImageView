#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QMainWindow>
#include <QtCore>

namespace Ui {
class ImageView;
}

class ImageView : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit ImageView(QWidget *parent = 0);
	~ImageView();

public slots:
	void open();
	void nextImage();
	void previousImage();
	void setBegin();
	void setEnd();
	void about();
	
private:
	Ui::ImageView *ui;
	QString folderName;
	QFileInfoList fileList;
	int imageNumber;
	int totalImages;

	void updateGUI();
	void setImage(QString filePath);
};

#endif // IMAGEVIEW_H
