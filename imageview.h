#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QMainWindow>
#include <QtCore>


class QTimer;
class QPushButton;

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
	void slideShow();
	void setSlideShowDelay(int delay);

protected:
	void resizeEvent(QResizeEvent *event);
	
private:
	Ui::ImageView *ui;
	QString folderName;
	QFileInfoList fileList;
	QPixmap currentPixmap;
	int imageNumber;
	int totalImages;
	float slideShowDelay;
	QTimer* slideshowTimer;
	void updateGUI();
	void setImage(QString filePath);

	void setButtonIcon(QPushButton* button, QString fileName, int width, int size);
};

#endif // IMAGEVIEW_H
