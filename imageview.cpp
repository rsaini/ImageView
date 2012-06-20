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
	connect(ui->slideShowDelaySlider, SIGNAL(valueChanged(int)), this, SLOT(setSlideShowDelay(int)));
	slideshowTimer = new QTimer(this);
	connect(slideshowTimer, SIGNAL(timeout()), this, SLOT(nextImage()));

	ui->imageNameLabel->setText(tr("Select a folder from the <b>file -> open</b> menu to view images."));
	ui->openAction->setShortcut(tr("Ctrl+O"));

	ui->previousButton->setEnabled(false);
	ui->nextButton->setEnabled(false);
	ui->beginButton->setEnabled(false);
	ui->endButton->setEnabled(false);
	ui->slideShowButton->setEnabled(false);
	ui->slideShowDelaySlider->setEnabled(false);
	ui->slideShowDelayLabel->setEnabled(false);
	ui->slideShowDelaySlider->setValue(15);

	setButtonIcon(ui->slideShowButton, ":/images/buttons/play.png", 48, 48);
	setButtonIcon(ui->nextButton, ":/images/buttons/next.png", 48, 48);
	setButtonIcon(ui->previousButton, ":/images/buttons/back.png", 48, 48);
	setButtonIcon(ui->beginButton, ":/images/buttons/rewind.png", 48, 48);
	setButtonIcon(ui->endButton, ":/images/buttons/fastfwd.png", 48, 48);

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


void ImageView::setButtonIcon(QPushButton *button, QString fileName, int width, int height)
{
	QPixmap iconPixmap(fileName);
	QIcon buttonIcon(iconPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
	totalImages = fileList.size();

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
}


void ImageView::setBegin()
{
	if (totalImages == 0) {
		return;
	}
	imageNumber = 0;
	updateGUI();
}


void ImageView::setEnd()
{
	if (totalImages == 0) {
		return;
	}
	imageNumber = totalImages-1;
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
		ui->slideShowDelayLabel->setEnabled(false);
		ui->slideShowDelaySlider->setEnabled(false);
		ui->imageLabel->clear();
		ui->imageNameLabel->setText(tr("No image files found.\nSelect a folder with image files (*.png, *.jpg, *.gif, *.bmp)."));
		ui->imageNumberLabel->setText("0 / 0");

		return;
	}

	if (totalImages == 1) {
		ui->slideShowButton->setEnabled(false);
		ui->slideShowDelayLabel->setEnabled(false);
		ui->slideShowDelaySlider->setEnabled(false);

	} else {
		ui->slideShowButton->setEnabled(true);
		ui->slideShowDelayLabel->setEnabled(true);
		ui->slideShowDelaySlider->setEnabled(true);
	}

	QFileInfo currentFile = fileList.at(imageNumber);
	setImage(currentFile.absoluteFilePath());

	QString number = QString::number(imageNumber+1) + " / " + QString::number(totalImages);
	ui->imageNumberLabel->setText(number);

	ui->imageNameLabel->setText(currentFile.fileName());

	ui->previousButton->setEnabled(true);
	ui->nextButton->setEnabled(true);
	ui->beginButton->setEnabled(true);
	ui->endButton->setEnabled(true);

	if (imageNumber == 0) {
		ui->previousButton->setEnabled(false);
	}
	if (imageNumber == totalImages-1) {
		ui->nextButton->setEnabled(false);
	}
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
		ui->imageLabel->setPixmap(currentPixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}

}


void ImageView::about()
{
	QMessageBox::about(this, tr("About ImageView"), tr("<b>Made for Eyenuk LLC.</b><br />My first GUI app :) - 2012/06/18"));
}

void ImageView::slideShow()
{
	if (!slideshowTimer->isActive()) {
		slideshowTimer->start((int)(slideShowDelay * 1000));
		setButtonIcon(ui->slideShowButton, ":/images/buttons/pause.png", 48, 48);
	} else {
		slideshowTimer->stop();
		setButtonIcon(ui->slideShowButton, ":/images/buttons/play.png", 48, 48);
	}
}


void ImageView::setSlideShowDelay(int delay)
{
	slideShowDelay = 0.5 + delay * 0.1;
	ui->slideShowDelayLabel->setText(QString::number(slideShowDelay) + " s.");
	if (slideshowTimer->isActive()) {
		slideshowTimer->start((int)(slideShowDelay * 1000));
	}
}
