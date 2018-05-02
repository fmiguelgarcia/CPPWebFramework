/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QFile>
#include "cppwebframework_global.h"

CWF_BEGIN_NAMESPACE
/**
 * @brief The FileManager class can manage file's name.
 */
class CPPWEBFRAMEWORKSHARED_EXPORT FileManager
{
public:
     static QStringRef extract( const QString& name, const QChar ch) noexcept;

     static QStringRef fileName( const QString &name) noexcept;

     static QStringRef fileExtention( const QString& name) noexcept;

     static void removeLastBar(QString &path);
     static void removeFirstBar(QString &path);
     static void putFirstBar(QString &path);
     static QByteArray readAll(const QString &fileName, QFileDevice::FileError &fileErro);
     static bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
};

CWF_END_NAMESPACE

#endif // FILEMANAGER_H
