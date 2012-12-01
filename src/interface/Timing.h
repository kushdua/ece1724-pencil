#ifndef TIMING_H
	#define TIMING_H

	#include <QTime>
	#include <QFile>
	#include <QTextStream>

	#define TIMING_FILE_PATH "timing.txt"

	class Timing {
		public:
			static Timing *getInstance();

			QTime snapshotTimer, removeSnapshotDirTimer, PencilLoggingTimer;
			int snapshotTime, removeSnapshotDirTime, PencilLoggingTime;

			Timing();
			~Timing();

			void captureSnapshotTime();

		private:
			static Timing *instance;

			QFile timingFile;
	};
#endif
