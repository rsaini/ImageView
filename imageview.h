#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QMainWindow>
#include <QtCore>

class QTimer;
class QPushButton;
class QLabel;

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
	void thumb1();
	void thumb2();
	void thumb3();
	void thumb4();
	void thumb5();
	void thumb6();
	void thumb7();
	void thumb8();
	void thumb9();
	void thumb10();

protected:
	void resizeEvent(QResizeEvent *event);
	
private:
	Ui::ImageView *ui;
	QString folderName;
	QFileInfoList fileList;
	QPixmap currentPixmap;
	int imageNumber;
	int pageNumber;
	int totalImages;
	float slideShowDelay;
	QTimer* slideshowTimer;

	QList<QPushButton*> thumbList;

	void updateGUI();
	void setImage(QString filePath); // Sets the image of the central label from the file in filePath
	void setButtonIcon(QPushButton* button, QString fileName, int width, int size, bool fast);
	void setThumbnailImage(QPushButton *button, QString fileName, int width, int height, bool fast);
	void generateThumbnails();
	void updateThumbnails();

};

#endif // IMAGEVIEW_H
