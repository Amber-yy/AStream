#include "astream.h"
#include <QApplication>
#include <QSharedMemory>
#include "trayIcon.h"

#include <QProcess>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QSharedMemory sharedMemory;
	sharedMemory.setKey(QString("AStream"));

	if (sharedMemory.attach())
	{
		return 0;
	}

	if (!sharedMemory.create(1))
	{
		return 0;
	}

	AStream w;
	w.show();
	return a.exec();
}
