#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QMainWindow>
#include <QtGui>

class QTimer;
class QPushButton;
class QLabel;
class Thumbnail;

namespace Ui {
class ImageView;
}

class ImageView : public QMainWindow
{
	Q_OBJECT

public:
	explicit ImageView(QWidget *parent = 0);
	~ImageView();

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void open();
	void on_loadFolderButton_clicked();
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
	void on_folderTreeView_clicked(const QModelIndex &index);
	void on_folderTreeView_doubleClicked(const QModelIndex &index);
	void on_fileListView_doubleClicked(const QModelIndex &index);
	void upa();

private:
	Ui::ImageView *ui;
	QList<QPushButton*> thumbList; // List of the thumbnail QPushButtons in the ui.
	QString folderName;  // Current folder which is displayed
	QFileInfoList fileList; // List of FileInfos of all files in folderName
	QPixmap currentPixmap; // The current picture displayed. Stored, so that resizing does not cause loss of data.
	int imageNumber; // Current number in the fileList of the picture displayed.
	int pageNumber; // Current page of the file displayed. = totalImages-1 / 10;
	int totalImages; // The total images in folderName
	float slideShowDelay; // Delay for the slideshow in secs.
	QTimer* slideshowTimer; // Timer for the slideshow. triggers the next() slot after slideShowDelay * 1000
	QFileSystemModel* folderTreeModel; // Model for the folderTreeView.
	QFileSystemModel* fileListModel; // Model for the fileListView.

	QList<Thumbnail*> thumbnailImageList;
	QList<QThread*> threadList;
	int numberOfThreadsDone;
	QMutex thumbnailThreadsMutex;

	void openFolder(QString f_name);
	void updateGUI();
	void setImage(QString filePath); // Sets the image of the central label from the file in filePath
	void setButtonIcon(QPushButton* button, QString fileName, int width, int size, bool fast);
	void generateThumbnails();
	void updateThumbnails();
};

#endif // IMAGEVIEW_H
