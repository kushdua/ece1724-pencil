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
	snapshotTime = 0;
	removeSnapshotDirTime = 0;
	PencilLoggingTime = 0;

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
}

void Timing::captureSnapshotTime() {
	int elapsedTime = snapshotTimer.elapsed();

	qDebug() << "Removal time: " << elapsedTime << "ms";

	QTextStream timingTextStream(&timingFile);
	timingTextStream << "Removal time: " << elapsedTime << "ms\n";
	timingTextStream.flush();
}
