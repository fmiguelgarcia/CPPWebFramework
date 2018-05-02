/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "cppwebapplication.h"
#include "cppwebcontroller.h"

CWF_BEGIN_NAMESPACE

QPair<QString, qint64> getFileAndMaxSize()
{
    QPair<QString, qlonglong> info {
        QString::fromLocal8Bit( 
            qgetenv( CONFIGURATION::CPP_LOG_VAR().constData())),
        qgetenv( CONFIGURATION::CPP_LOG_MAX_VAR().constData()).toLongLong()
    };

    if(info.second <= 0)
        info.second = 20000000;

    return info;
}

void writeLog(QtMsgType type, const QMessageLogContext &logContext, const QString &msg)
{
    const QPair<QString, qint64> info(getFileAndMaxSize());
    QFile file(info.first);

    if(file.size() > info.second)
    {
        file.resize(0);
    }

    if(file.open(QIODevice::Append))
    {
        QTextStream out(&file);
        const QString date = QDateTime::currentDateTime().toString( Qt::ISODate);
        out << "Date: "       << date;
        out << "\nCategory: " << logContext.category;
        out << "\nFile: "     << logContext.file;
        out << "\nFunction: " << logContext.function;
        out << "\nVersion: "  << logContext.version;
        out << "\nMessage: "  << msg << "\n\n";

        if(type == QtFatalMsg)
        {
            out << "\nFATAL: SERVER OFFLINE\n\n";
            file.close();
            abort();
        }
        file.close();
    }
}

CppWebApplication::CppWebApplication(int argc, char *argv[],
                                     const QString &serverPath,
                                     Filter *filter) : application(argc, argv), configuration(serverPath)
{
    if(configuration.isValid())
    {               
        qunsetenv(CONFIGURATION::CPP_LOG_VAR().constData());
        qunsetenv(CONFIGURATION::CPP_LOG_MAX_VAR().constData());

        qputenv(CONFIGURATION::CPP_LOG_VAR().constData(), configuration.getLogFilePath().toLatin1());
        qputenv(CONFIGURATION::CPP_LOG_MAX_VAR().constData(), QByteArray::number(configuration.getMaxLogFile()));

        QPair<QString, qint64> info(getFileAndMaxSize());
        if(!QFile(info.first).exists())
        {            
            qDebug() << "Path not found to log file: " << configuration.getLogFilePath();
            qDebug() << "Note: Use only US-ASCII characters for the serverPath.";
        }
        else if(configuration.isValid())
        {
            mOptions.setFlag( Option::Valid);
            qInstallMessageHandler(writeLog);
            server = new CppWebServer(configuration, filter);

            if(configuration.getAccessServerPages())
            {
                server->addController<CppWebController>( QLatin1String("/example"));
                server->addController<CppWebController>( QLatin1String("/authors"));
                server->addController<CppWebController>( QLatin1String("/documentation"));
                server->addController<CppWebController>( QLatin1String("/ssl"));
                server->addController<CppWebController>( QLatin1String("/index"));
            }
        }        
    }
    else
    {
        qDebug() << "CPPWeb.ini not found. Please copy the CWF server folder to your project.";
    }
}

CppWebApplication::~CppWebApplication()
{
    if(!server)
        delete server;
}

int CppWebApplication::start()
{
    if(!configuration.isValid() || !mOptions.testFlag(Option::Valid))
    {
        qDebug() << "Invalid configuration.\nServer offline\n";
        return -1;
    }
    else if(!server->listen(configuration.getHost(), configuration.getPort()))
    {
        qDebug() << "Error: " << server->errorString() << "\n";
        qDebug() << "Server offline\n";
        return -1;
    }

    qDebug() << "Server online\n";
    return application.exec();
}

CWF_END_NAMESPACE
