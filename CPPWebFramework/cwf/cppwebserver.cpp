/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "cppwebserver.h"
#include "httpreadrequest.h"
#include "filter.h"

#include <QTimer>
#include <QSslKey>

CWF_BEGIN_NAMESPACE

CppWebServer::CppWebServer(const Configuration &configuration, Filter *filter) 
    : mConfiguration(configuration),
    timer( new QTimer),
    filter(filter)
{
    loadSslConfiguration();
    this->thread()->setPriority(QThread::TimeCriticalPriority);
    pool.setMaxThreadCount(configuration.getMaxThread());
    pool.setExpiryTimeout(configuration.getTimeOut());
    if(!filter)
        this->filter.reset( new Filter);

    connect( timer.data(), &QTimer::timeout, this, &CppWebServer::doClean);
    timer->start(configuration.getCleanupInterval());
}

CppWebServer::~CppWebServer()
{
    while(!pool.waitForDone());
}

void CppWebServer::incomingConnection(qintptr socketfd)
{
    QMutexLocker _(&sessionsMtx);
    pool.start( 
        new HttpReadRequest( socketfd, *this),
        QThread::LowPriority);
}

void CppWebServer::doClean()
{    
    QMutexLocker _(&sessionsMtx);

#if 0
    while(!pool.waitForDone(sleepTime));

    Session *session = nullptr;
    QStringList idSessionsToDelete;
    for(auto it = sessions.begin(); it != sessions.end(); ++it)
    {
        session = it.value();
        if(session->isExpired())
        {
            idSessionsToDelete.push_back(session->getId());
            delete session;
        }
    }
    for(int i = 0; i < idSessionsToDelete.size(); ++i)
        sessions.remove(idSessionsToDelete[i]);

    block = 0;
#endif
}

void CppWebServer::loadSslConfiguration()
{
    QString sslKey  = mConfiguration.getSslKeyFile();
    QString sslCert = mConfiguration.getSslCertFile();
    if (!sslKey.isEmpty() && !sslCert.isEmpty())
    {
#ifdef QT_NO_OPENSSL
            qDebug() << "SSL is not supported";
            return;
#else
            QFile certFile(sslCert);
            QFile keyFile(sslKey);

            if(!certFile.open(QIODevice::ReadOnly))
            {
                qDebug() << "Can't open SSL Cert File";
                return;
            }
            if(!keyFile.open(QIODevice::ReadOnly))
            {
                qDebug() << "Can't open SSL Key File";
                return;
            }

            QSslCertificate certificate(&certFile, QSsl::Pem);
            QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem);
            keyFile.close();
            certFile.close();

            if(key.isNull())
            {
                qDebug() << "Invalid SLL Key File";
                return;
            }
            else if(certificate.isNull())
            {
                qDebug() << "Invalid SLL Cert File";
                return;
            }

            mSslConfiguration.reset( new QSslConfiguration());

            mSslConfiguration->setLocalCertificate(certificate);
            mSslConfiguration->setPrivateKey(key);
            mSslConfiguration->setPeerVerifyMode(QSslSocket::VerifyNone);
            mSslConfiguration->setProtocol(QSsl::TlsV1SslV3);
#endif
    }
}

CWF_END_NAMESPACE
