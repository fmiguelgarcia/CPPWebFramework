/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#ifndef HTTPREADREQUEST_H
#define HTTPREADREQUEST_H

#include <QMap>
#include <QFile>
#include <chrono>
#include <QRunnable>
#include <QTcpSocket>
#include <QStringList>
#include <QSslConfiguration>
#include <memory>
#include "controller.h"
#include "request.h"
#include "response.h"
#include "session.h"
#include "filter.h"
#include "httpparser.h"
#include "constants.h"
#include "filterchain.h"
#include "httpcookie.h"
#include "qmapthreadsafety.h"
#include "cppwebframework_global.h"

CWF_BEGIN_NAMESPACE
class Configuration;
class CppWebServer;
class HttpReadRequestPrivate;

/**
 * @brief The HttpReadRequest class is created automatically by the CppWebServer and inserted <br>
 * in a QThreadPool, always when the CppWebServer has a call by a client(Browser).
 */
class CPPWEBFRAMEWORKSHARED_EXPORT HttpReadRequest : public QRunnable
{
protected:
    HttpReadRequestPrivate *d_ptr;
    Q_DECLARE_PRIVATE(HttpReadRequest);

private:
    const CppWebServer &mWebServer;

    QScopedPointer<QTcpSocket> socket;
    qintptr socketDescriptor;
    qint64 maxUploadFile;

    bool readBody(HttpParser &parser, Request &request, Response &response);
    void createSocket();

public:
    /**
     * @brief This constructor provides the necessary information to create a HttpReadRequest
     * @param qintptr socketDescriptor                             : Used to create a socket.
     * @param QMapThreadSafety<QString, Controller *> &urlController : All mapped controllers
     * @param QMapThreadSafety<QString, Session *> &sessions   : Sessions.
     * @param QSslConfiguration *sslConfiguration                  : SSL configuration.
     * @param Filter *filter                                       : Filter
     */
    HttpReadRequest(qintptr socketDescriptor,
                    const CppWebServer& webServer);

    /**
     * @brief Destroys dynamically allocated resources.
     */
    virtual ~HttpReadRequest();
    /**
     * @brief Starts to read the requisition.
     */
    void run() override;
};

CWF_END_NAMESPACE

#endif // HTTPREADREQUEST_H
