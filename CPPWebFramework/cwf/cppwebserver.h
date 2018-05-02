/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#ifndef CPPWEBSERVER_H
#define CPPWEBSERVER_H

#include "configuration.h"
#include "cppwebframework_global.h"

#include <QHash>
#include <QMutex>
#include <QTcpServer>
#include <QThreadPool>
#include <QMutexLocker>
#include <QScopedPointer>
#include <QWeakPointer>

#include <memory>

class QTimer;
class QSslConfiguration;

CWF_BEGIN_NAMESPACE
class Filter;
class Session;
class Controller;
class CppWebServerPrivate;

/**
 * @brief The CppWebServer class is a HTTP server, responsable to receive and dispatch the requisitions.
 */
class CPPWEBFRAMEWORKSHARED_EXPORT CppWebServer : public QTcpServer
{
    Q_OBJECT
protected:
    CppWebServerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(CppWebServer);

private:
    QThreadPool pool;
    Configuration mConfiguration;

    QHash<QString, std::shared_ptr<Controller>> urlController;
    QMutex urlControllerMtx;
    QHash<QString, QWeakPointer<Session> > sessions;
    QMutex sessionsMtx;

    QScopedPointer<QTimer> timer;
    QScopedPointer<Filter> filter;
    QScopedPointer<QSslConfiguration> mSslConfiguration;

    /**
     * @brief Load the SSL Configuration to the server.
     */
    void loadSslConfiguration();
public:
    /**
     * @brief Load SSL configuration, configure the thread pool and the filter.
     * @param Filter *filter : Install a filter for requests on the server. Optional.
     */
    explicit CppWebServer(const Configuration &configuration, Filter *filter = nullptr);
    /**
     * @brief Destroys all controllers and sessions.
    */
    virtual ~CppWebServer();

    /**
     * @brief Hitches a url to a Controller.
     * @param const QString &url   : Url name.
     * it will not be possible to map the controllers through the addController function, instead everything should be handled inside the Filter.
     */
    template<typename CONTROLLER>
    void addController(const QString &url) noexcept
    {
        static_assert(std::is_base_of<Controller, CONTROLLER>::value, "CONTROLLER must be a descendant of Controller");

        std::shared_ptr<CONTROLLER> ctrl = std::make_shared<CONTROLLER>();

        QMutexLocker _(&urlControllerMtx);
        urlController.insert(url, ctrl);
    }

    inline QSslConfiguration* sslConfiguration() const noexcept
    { return mSslConfiguration.data();}

    inline const Configuration& configuration() const noexcept
    { return mConfiguration;}

protected:
    /**
     * @brief incomingConnection
     * @param qintptr socketfd : Socket descriptor.
     */
    void incomingConnection(qintptr socketfd) override;
    /**
     * @brief Clean expired sessions on Server.
     */
    void doClean();
};

CWF_END_NAMESPACE

#endif // CPPWEBSERVER_H
