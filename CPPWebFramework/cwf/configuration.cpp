/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "configuration.h"
#include "constants.h"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStringBuilder>
#include <QCoreApplication>

CWF_BEGIN_NAMESPACE

Configuration::Configuration(const QString &serverFilesPath)
    : path( serverFilesPath),
    logFilePath(
       QStandardPaths::writableLocation( QStandardPaths::AppLocalDataLocation)
       % QLatin1String("/log/cwf.log"))
{
    const QString iniFile = QStringLiteral("/config/CPPWeb.ini");
    const QDir appConfigDir( QStandardPaths::writableLocation( 
        QStandardPaths::StandardLocation::AppConfigLocation));
    const QDir appDir( QCoreApplication::applicationDirPath());

    // Check those possible locations for configuration file:
    const std::vector<QFileInfo> configs = {
        QFileInfo( path),
        QFileInfo( QDir(path), iniFile),
        QFileInfo( appConfigDir, path),
        QFileInfo( appConfigDir, iniFile),
        QFileInfo( appDir, path),
        QFileInfo( appDir, iniFile)
    };

    for( const QFileInfo& confPath: configs)
    {
        if( confPath.isFile() && confPath.isReadable())
        {
            mOptions.setFlag( ConfigurationOption::Valid);
            configure( confPath);
            break;
        }
    }
}


void Configuration::configure( const QFileInfo& confInfo)
{
    QSettings settings(
        confInfo.absoluteFilePath(),
        QSettings::Format::IniFormat);

    settings.beginGroup("config");
    host.setAddress(settings.value("host", "0.0.0.0").toString());
    port                  = settings.value("port", port).toInt();
    maxThread             = settings.value("maxThread", maxThread).toInt();
    cleanupInterval       = settings.value("cleanupInterval", cleanupInterval).toInt();
    timeOut               = settings.value("timeOut", timeOut).toInt();
    sslKeyFile            = settings.value("sslKeyFile").toString();
    sslCertFile           = settings.value("sslCertFile").toString();
    sessionExpirationTime = settings.value("sessionExpirationTime", sessionExpirationTime).toInt();
    maxUploadFile         = settings.value("maxUploadFile", maxUploadFile).toULongLong();
    indexPage             = settings.value("indexPage").toString();
    maxLogFile            = settings.value("maxLogFile", maxLogFile).toLongLong();
    mOptions.setFlag( 
        ConfigurationOption::AccessCPPWebIni,
        settings.value(
            "accessCPPWebIni", 
            mOptions.testFlag(ConfigurationOption::AccessCPPWebIni)).toBool());
    mOptions.setFlag( 
        ConfigurationOption::AccessServerPages,
        settings.value(
            "accessServerPages", 
            mOptions.testFlag(ConfigurationOption::AccessServerPages)).toBool());
    
    // Use standard paths for logs and SSL files by default.
    const QDir appConfDir( QStandardPaths::standardLocations( 
        QStandardPaths::AppConfigLocation).first());
    const std::vector<QString*> sslFiles = { &sslKeyFile, &sslCertFile };

    for( QString* sslFile: sslFiles)
    {
        QFileInfo sslFileInfo( *sslFile);
        if( !sslFileInfo.isAbsolute())
        {
            sslFileInfo = QFileInfo( appConfDir, *sslFile);
            if( sslFileInfo.isFile() && sslFileInfo.isReadable())
                *sslFile = sslFileInfo.absoluteFilePath();
        }
    }
}

CWF_END_NAMESPACE
