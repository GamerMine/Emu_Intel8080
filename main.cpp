#include <QApplication>

#include "hardware/Bus.h"
#include "hardware/Screen.h"

/*
This code redirects the output of qDebug() to a log file.
It also rotates the file base on size.
Uses QMutex for thread safety.
*/

#include <QtTypes>
#include <QString>
#include <QMutex>
#include <QDateTime>
#include <QFile>

//some constants to parameterize.
const qint64 LOG_FILE_LIMIT = 64000000;
const QString LOG_PATH = "";
const QString LOG_FILENAME = "../log.log";

//thread safety
QMutex mutex;

void redirectDebugMessages(QtMsgType type, const QMessageLogContext & context, const QString & str)
{
    //thread safety
    mutex.lock();

    QString filePath = LOG_PATH + LOG_FILENAME;
    QFile outFile(filePath);

    //if file reached the limit, rotate to filename.1
    if(outFile.size() > LOG_FILE_LIMIT){
        //roll the log file.
        QFile::remove(filePath + ".1");
        QFile::rename(filePath, filePath + ".1");
        QFile::resize(filePath, 0);
    }

    //write message
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << str << Qt::endl;

    //close fd
    outFile.close();
    mutex.unlock();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    //qInstallMessageHandler(redirectDebugMessages);

    Bus bus;

    return app.exec();
}
