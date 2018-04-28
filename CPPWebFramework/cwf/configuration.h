/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>
#include <QSettings>
#include <QHostAddress>
#include "filemanager.h"
#include "cppwebframework_global.h"

class QFileInfo;

/**
 * @brief All classes of C++ Web Framework are contained within the namespace CWF.
 */
CWF_BEGIN_NAMESPACE

class ConfigurationPrivate;
/**
 * @brief This class is responsable to read a ini file and extract its information.
 */
class CPPWEBFRAMEWORKSHARED_EXPORT Configuration
{
    bool valid                = false;
    bool accessCPPWebIni      = false;
    bool accessServerPages    = false;
    int timeOut               = 30000;
    int sessionExpirationTime = 1800000;
    int cleanupInterval       = 86400000;
    int port                  = 8080;
    int maxThread             = 100;
    QString sslKeyFile;
    QString sslCertFile;
    QString path;
    QString logFilePath;
    QString indexPage;
    QHostAddress host;
    qint64 maxUploadFile = 2097152;
    qint64 maxLogFile    = 20000000;

	 ConfigurationPrivate *d;	///< d-Pointer for PImpl.

    /// @brief It loads configuration from @p confInfo.
    void configure( const QFileInfo& confInfo);

public:
    /**
     * @brief Will make reading the CPPWeb.ini file and extract all of its properties.
     * The argument @p serverFilesPath can contains absolute or relative paths
     * to a folder or to a file. The order to find configuration file is the
     * following:
     *      - Use @p serverFilesPath as absolute path to INI file. This one can
     *      be use to support @c QResource address.
     *      - Use @p serverFilesPath + 'config/CPPWeb.ini' as absolute path to
     *      INI file.
     *      - Use standard app config dir (see QStrandardPaths) dir + @p
     *      serverFilesPath.
     *      - Use standard app config dir + '/config/CPPWeb.ini'
     *      - Use current app folder + @p serverFilesPath
     *      - Use current app folder + '/config/CPPWeb.ini'
     *      .
     * @param QString serverFilesPath : You should always points to the directory server.
     * @par Example
     * @code
     * #include <QCoreApplication>
     * #include <cwf/cppwebapplication.h>
     *
     * int main(int argc, char *argv[])
     * {     
     *     CWF::CppWebApplication a(argc, argv, "PATH_TO_SERVER_FOLDER");
     *     return a.start();
     * }
     * @endcode
     */
    explicit Configuration(const QString &serverFilesPath = QString());
    /**
     * @brief Returns the timeOut property that will be used by the server to expire threads that are not in use.
     * Such threads will be restarted as needed. The default timeOut is 30000 milliseconds (30 seconds).
     * If timeOut is negative, newly created threads will not expire, e.g., they will not exit until the thread pool is destroyed.
     * @param int : Time in milliseconds.
     */
    inline int getTimeOut() const noexcept { return timeOut; }
    /**
     * @brief Returns the Session Expiration Time.
     * @param int : Time in milliseconds.
     */
    inline int getSessionExpirationTime() const noexcept { return sessionExpirationTime; }
    /**
     * @brief Returns the clean up interval Time.
     * @param int : Time in milliseconds.
     */
    inline int getCleanupInterval() const noexcept { return cleanupInterval; }
    /**
     * @brief Returns the port number.
     * @param int : port.
     */
    inline int getPort() const noexcept { return port; }
    /**
     * @brief Returns the address.
     * @param QHostAddress : host.
     */
    inline QHostAddress getHost() const noexcept { return host; }
    /**
     * @brief Returns the max thread number.
     * @param int : max.
     */
    inline int getMaxThread() const noexcept { return maxThread; }
    /**
     * @brief Returns the ssl key file.
     * @param QString : sslKeyFile name.
     */
    inline QString getSslKeyFile() const noexcept { return sslKeyFile; }
    /**
     * @brief Returns the ssl cert file.
     * @param QString : sslCertFile name.
     */
    inline QString getSslCertFile() const noexcept { return sslCertFile; }
    /**
     * @brief Returns the path to the server's folder.
     * @param QString : path.
     */
    inline QString getPath() const noexcept { return path; }
    /**
     * @brief Returns the log file path.
     * @param QString : log file.
     */
    inline QString getLogFilePath() const noexcept { return logFilePath; }
    /**
     * @brief Returns max upload file size supported by the server.
     * @param qint64: max file size.
     */
    inline qint64 getMaxUploadFile() const noexcept { return maxUploadFile; }
    /**
     * @brief getMaxLogFile the max file log
     * @return qint64 : Max file log in bytes.
     */
    inline qint64 getMaxLogFile() const noexcept { return maxLogFile; }
    /**
     * @brief Returns the index page.
     * @return QString : index page name.
     */
    inline QString getIndexPage() const noexcept { return indexPage; }
    /**
     * @brief Returns the access server page.
     * @return bool : Access server pages.
     */
    inline bool getAccessServerPages() const noexcept { return accessServerPages; }
    /**
     * @brief Returns true if the Configuration is ok. Otherwise returns false.
     * @param bool : is valid.
     */
    inline bool isValid() const noexcept { return valid; }
};

CWF_END_NAMESPACE

#endif // CONFIGURATION_H
