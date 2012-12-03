#include <cstdlib>
#include "Timing.h"
#include <QDebug>

Timing *Timing::instance = NULL;

Timing *Timing::getInstance()
{
	if (instance == NULL) {
		qDebug() << "Creating new Timing object.";
		instance = new Timing;
	}

	return instance;
}

Timing::Timing() {
	timingFile.setFileName(QString::fromUtf8(TIMING_FILE_PATH));
	if (!timingFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Could not open timing file.";
		exit(-1);
	}

	QTextStream timingTextStream(&timingFile);
	timingTextStream << "Timing Measurements\n";
	timingTextStream.flush();
}

Timing::~Timing() {
    if(timingFile.isOpen()) timingFile.close();
}

void Timing::outputToConsoleAndFile(const char* prefix, int elapsedTime) {
	qDebug() << prefix << elapsedTime << "ms";

	QTextStream timingTextStream(&timingFile);
	timingTextStream << prefix << elapsedTime << "ms\n";
	timingTextStream.flush();
}
