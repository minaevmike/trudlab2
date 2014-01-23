#include "window.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    #if QT_VERSION < 0x050000
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); //изменения
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); //изменения
    #endif
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
	QApplication a(argc, argv);
	Window w;
	w.show();

	return a.exec();
}
