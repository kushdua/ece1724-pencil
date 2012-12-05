#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QtGui/QImage>

bool compareImages(char* imagePath1, char* imagePath2)
{
	QImage image1(imagePath1);
	QImage image2(imagePath2);

	if (image1.isNull()) {
		qDebug() << "Image 1 couldn't be loaded.";
		return false;
	}

	if (image2.isNull()) {
		qDebug() << "Image 2 couldn't be loaded.";
		return false;
	}

	return (image1 == image2);
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	if (argc != 3) {
		qDebug() << "Invalid params. Need two image paths to comapre.";
		return (1);
	}

	if (compareImages(argv[1], argv[2])) {
		qDebug() << "The images are the same. =]";
	}
	else qDebug() << "The images are not the same. ='[";

	return app.exec();
}
