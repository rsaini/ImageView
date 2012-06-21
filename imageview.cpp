#include <QtGui>
#include "imageview.h"
#include "ui_imageview.h"

ImageView::ImageView(QWidget *parent) : QMainWindow(parent), ui(new Ui::ImageView)
{
	ui->setupUi(this);

	connect(ui->openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui->exitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionAbout_ImageView, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(nextImage()));
	connect(ui->previousButton, SIGNAL(clicked()), this, SLOT(previousImage()));
	connect(ui->beginButton, SIGNAL(clicked()), this, SLOT(setBegin()));
	connect(ui->endButton, SIGNAL(clicked()), this, SLOT(setEnd()));
	connect(ui->slideShowButton, SIGNAL(clicked()), this, SLOT(slideShow()));
	slideshowTimer = new QTimer(this);
	connect(slideshowTimer, SIGNAL(timeout()), this, SLOT(nextImage()));

	ui->imageNameLabel->setText(tr("Select a folder from the <b>file -> open</b> menu to view images."));
	ui->openAction->setShortcut(tr("Ctrl+O"));

	ui->previousButton->setEnabled(false);
	ui->nextButton->setEnabled(false);
	ui->beginButton->setEnabled(false);
	ui->endButton->setEnabled(false);
	ui->slideShowButton->setEnabled(false);

	thumbList << ui->thumb1 << ui->thumb2 << ui->thumb3 << ui->thumb4 <<
				 ui->thumb5 << ui->thumb6 << ui->thumb7 << ui->thumb8 <<
				 ui->thumb9 << ui->thumb10;

	connect(thumbList.at(0), SIGNAL(clicked()), this, SLOT(thumb1()));
	connect(thumbList.at(1), SIGNAL(clicked()), this, SLOT(thumb2()));
	connect(thumbList.at(2), SIGNAL(clicked()), this, SLOT(thumb3()));
	connect(thumbList.at(3), SIGNAL(clicked()), this, SLOT(thumb4()));
	connect(thumbList.at(4), SIGNAL(clicked()), this, SLOT(thumb5()));
	connect(thumbList.at(5), SIGNAL(clicked()), this, SLOT(thumb6()));
	connect(thumbList.at(6), SIGNAL(clicked()), this, SLOT(thumb7()));
	connect(thumbList.at(7), SIGNAL(clicked()), this, SLOT(thumb8()));
	connect(thumbList.at(8), SIGNAL(clicked()), this, SLOT(thumb9()));
	connect(thumbList.at(9), SIGNAL(clicked()), this, SLOT(thumb10()));

	for (int i = 0; i < 10; i++) {
		thumbList.at(i)->setEnabled(false);
	}

	setButtonIcon(ui->slideShowButton, ":/images/buttons/play.png", 48, 48, false);
	setButtonIcon(ui->nextButton, ":/images/buttons/next.png", 48, 48, false);
	setButtonIcon(ui->previousButton, ":/images/buttons/back.png", 48, 48, false);
	setButtonIcon(ui->beginButton, ":/images/buttons/rewind.png", 48, 48, false);
	setButtonIcon(ui->endButton, ":/images/buttons/fastfwd.png", 48, 48, false);

	setWindowTitle(tr("ImageView"));

	folderName = "";
	imageNumber = 0;
	totalImages = 0;
	slideShowDelay = 2.0;
}

ImageView::~ImageView()
{
	delete ui;
}


void ImageView::setButtonIcon(QPushButton *button, QString fileName, int width, int height, bool fast)
{
	QPixmap iconPixmap(fileName);
	QIcon buttonIcon(iconPixmap.scaled(width, height, Qt::KeepAspectRatio, (fast ? Qt::FastTransformation : Qt::SmoothTransformation) ));
	button->setIcon(buttonIcon);
	button->setIconSize(iconPixmap.scaled(width, height).rect().size());
}


void ImageView::open()
{
	folderName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "./", QFileDialog::ShowDirsOnly);
	if (folderName == "") {
		return;
	}

	QDir imageDir(folderName);
	QStringList imageFilters;
	imageFilters << "*.bmp" << "*.png" << "*.jpg" << "*.jpeg" << "*.gif";

	imageDir.setNameFilters(imageFilters);
	imageDir.setFilter(QDir::Files);
	imageDir.setSorting(QDir::Name);

	fileList.clear();
	fileList = imageDir.entryInfoList();

	imageNumber = 0;
	pageNumber = 0;
	totalImages = fileList.size();

	for (int i = 0; i < 10; i++) {
		QIcon ico;
		thumbList.at(i)->setIcon(ico);
		thumbList.at(i)->setEnabled(false);
	}
	generateThumbnails();
	updateGUI();
}


void ImageView::previousImage()
{
	imageNumber--;
	updateGUI();
}


void ImageView::nextImage()
{
	imageNumber++;
	if (imageNumber > totalImages-1) {
		imageNumber = 0;
	}
	updateGUI();
	if(slideshowTimer->isActive()) {
		slideshowTimer->start((int)(slideShowDelay * 1000));
	}
	updateThumbnails();
}


void ImageView::setBegin()
{
	if (totalImages == 0) {
		return;
	}
	//imageNumber = 0;

	imageNumber = (pageNumber -1) * 10;
	updateGUI();
}


void ImageView::setEnd()
{
	if (totalImages == 0) {
		return;
	}
	imageNumber = (pageNumber + 1) * 10;
	//imageNumber = totalImages-1;
	updateGUI();
}


void ImageView::updateGUI()
{
	if (totalImages == 0 ) {
		ui->nextButton->setEnabled(false);
		ui->previousButton->setEnabled(false);
		ui->beginButton->setEnabled(false);
		ui->endButton->setEnabled(false);
		ui->slideShowButton->setEnabled(false);
		ui->imageLabel->clear();
		ui->imageNameLabel->setText(tr("No image files found.\nSelect a folder with image files (*.png, *.jpg, *.gif, *.bmp)."));
		ui->imageNumberLabel->setText("0 / 0");
		updateThumbnails();
		return;
	}

	if (totalImages == 1) {
		ui->slideShowButton->setEnabled(false);

	} else {
		ui->slideShowButton->setEnabled(true);
	}

	QFileInfo currentFile = fileList.at(imageNumber);
	setImage(currentFile.absoluteFilePath());

	QString number = QString::number(imageNumber+1) + " / " + QString::number(totalImages);
	ui->imageNumberLabel->setText(number);
	ui->imageNameLabel->setText(currentFile.fileName());

	if (pageNumber == 0) {
		ui->beginButton->setEnabled(false);
	} else {
		ui->beginButton->setEnabled(true);
	}

	if (pageNumber == ((totalImages-1) / 10)) {
		ui->endButton->setEnabled(false);
	} else {
		ui->endButton->setEnabled(true);
	}

	ui->previousButton->setEnabled(true);
	ui->nextButton->setEnabled(true);

	if (imageNumber == 0) {
		ui->previousButton->setEnabled(false);
	}
	if (imageNumber == totalImages-1) {
		ui->nextButton->setEnabled(false);
	}

	updateThumbnails();
}


void ImageView::resizeEvent(QResizeEvent* /* event */)
{
	if (!currentPixmap.isNull()) {
		QSize imageSize = currentPixmap.size();
		if (imageSize.width() < ui->imageLabel->width() && imageSize.height() < ui->imageLabel->height()) {
			ui->imageLabel->setPixmap(currentPixmap);
		} else {
			ui->imageLabel->setPixmap(currentPixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}
}

void ImageView::setImage(QString filePath)
{
	currentPixmap = QPixmap(filePath);

	if (currentPixmap.isNull()) {
		QMessageBox::information(this, tr("File open error..."), tr("Cannot open file \"%1\"").arg(filePath));
		return;
	}

	QSize imageSize = currentPixmap.size();
	if (imageSize.width() < ui->imageLabel->width() && imageSize.height() < ui->imageLabel->height()) {
		ui->imageLabel->setPixmap(currentPixmap);
	} else {
		ui->imageLabel->setPixmap(currentPixmap.scaled(ui->imageLabel->size(),
													   Qt::KeepAspectRatio,
													   Qt::SmoothTransformation));
	}

}


void ImageView::about()
{
	QMessageBox::about(this, tr("About ImageView"),
					   tr("<b>Made for Eyenuk LLC.</b><br />My first GUI app :) - 2012/06/18"));
}

void ImageView::slideShow()
{
	if (!slideshowTimer->isActive()) {
		slideshowTimer->start((int)(slideShowDelay * 1000));
		setButtonIcon(ui->slideShowButton, ":/images/buttons/pause.png", 48, 48, false);
	} else {
		slideshowTimer->stop();
		setButtonIcon(ui->slideShowButton, ":/images/buttons/play.png", 48, 48, false);
	}
}


void ImageView::setSlideShowDelay(int delay)
{
	slideShowDelay = 0.5 + delay * 0.1;
	if (slideshowTimer->isActive()) {
		slideshowTimer->start((int)(slideShowDelay * 1000));
	}
}


void ImageView::generateThumbnails()
{
	if (totalImages == 0) {
		return;
	}

	for (int i = 0; i < 10 && i < totalImages ; i++) {
		thumbList.at(i)->setEnabled(true);
		QString filePath = fileList.at(i).absoluteFilePath();
		setThumbnailImage(thumbList.at(i), filePath, thumbList.at(i)->width()-8, thumbList.at(i)->height()-8, true);
	}

	thumbList.at(0)->setStyleSheet("QPushButton { background-color : #404040; border-top: 4px;}");
}


void ImageView::updateThumbnails()
{
	if (totalImages == 0) {
		for (int i = 0; i < 10; i++) {
			thumbList.at(i)->setEnabled(false);
			thumbList.at(i)->setStyleSheet("");
			QIcon ico;
			thumbList.at(i)->setIcon(ico);
			pageNumber = 0;
		}
		return;
	}

	for (int i = 0; i < 10; i++) {
		thumbList.at(i)->setStyleSheet("");
	}
	thumbList.at(imageNumber % 10)->setStyleSheet("QPushButton { background-color : #404040; border-top: 4px;}");

	int currentPage = imageNumber / 10;
	if (currentPage == pageNumber) {
		return;
	}

	pageNumber = currentPage;

	if (pageNumber == 0) {
		ui->beginButton->setEnabled(false);
	} else {
		ui->beginButton->setEnabled(true);
	}

	if (pageNumber == ((totalImages-1) / 10)) {
		ui->endButton->setEnabled(false);
	} else {
		ui->endButton->setEnabled(true);
	}

	for (int i = 0; i < 10; i++) {
		QIcon ico;
		thumbList.at(i)->setIcon(ico);
		thumbList.at(i)->setEnabled(false);
	}

	for (int i = 0; i < 10 && i < (totalImages- currentPage*10) ; i++) {
		thumbList.at(i)->setEnabled(true);
		QString filePath = fileList.at(currentPage*10 + i).absoluteFilePath();
		setThumbnailImage(thumbList.at(i), filePath, thumbList.at(i)->width()-8, thumbList.at(i)->height()-8, true);
	}
}


void ImageView::setThumbnailImage(QPushButton *button, QString fileName, int width, int height, bool fast)
{
	QPixmap iconPixmap(fileName);
	QIcon buttonIcon(iconPixmap.scaled(width, height, Qt::KeepAspectRatio, (fast ? Qt::FastTransformation : Qt::SmoothTransformation) ));
	button->setIcon(buttonIcon);
	button->setIconSize(iconPixmap.scaled(width+8, height+8).rect().size());
}


void ImageView::thumb1()
{
	imageNumber = pageNumber*10;
	updateGUI();
}

void ImageView::thumb2()
{
	imageNumber = pageNumber*10 + 1;
	updateGUI();
}

void ImageView::thumb3()
{
	imageNumber = pageNumber*10 + 2;
	updateGUI();
}

void ImageView::thumb4()
{
	imageNumber = pageNumber*10  +3;
	updateGUI();
}

void ImageView::thumb5()
{
	imageNumber = pageNumber*10 + 4;
	updateGUI();
}

void ImageView::thumb6()
{
	imageNumber = pageNumber*10 + 5;
	updateGUI();
}

void ImageView::thumb7()
{
	imageNumber = pageNumber*10 + 6;
	updateGUI();
}

void ImageView::thumb8()
{
	imageNumber = pageNumber*10 + 7;
	updateGUI();
}

void ImageView::thumb9()
{
	imageNumber = pageNumber*10 + 8;
	updateGUI();
}

void ImageView::thumb10()
{
	imageNumber = pageNumber*10 + 9;
	updateGUI();
}
