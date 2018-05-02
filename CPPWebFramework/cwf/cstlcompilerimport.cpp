/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "cstlcompilerimport.h"
#include "configuration.h"
#include "filemanager.h"
#include "constants.h"

namespace
{
	QString errorMissingUrlAttribute() Q_DECL_NOEXCEPT
	{ return QStringLiteral("***ERROR IMPORT TAG - IMPORT TAG NEEDS THE URL ATTRIBUTE***");}
}

CWF_BEGIN_NAMESPACE

CSTLCompilerImport::CSTLCompilerImport(const QXmlStreamAttributes &attr, QString path)
{
    int size = attr.size();
    if(size == 0)
    {
        attributes.insert(
            CSTL::TAG::PROPERTY::ERROR(),
            errorMissingUrlAttribute());
    }
    else if(size == 1)
    {
         QString name(std::move(attr[0].name().toString().toLower()));
         QString value(std::move(attr[0].value().toString()));

         if(name != CSTL::TAG::PROPERTY::IMPORT::URL())
         {
             attributes.insert(
                CSTL::TAG::PROPERTY::ERROR(),
                errorMissingUrlAttribute());
         }
         else
         {
             FileManager fileManager;
             fileManager.removeLastBar(path);

             path += value;

             QFile::FileError fileError;
             QByteArray data = fileManager.readAll(path, fileError);
             attributes.insert(
                CSTL::TAG::PROPERTY::IMPORT::URL(),
                QString::fromUtf8( data));
         }
    }
    else
    {
        attributes.insert(
            CSTL::TAG::PROPERTY::ERROR(),
            errorMissingUrlAttribute());
    }
}

CWF_END_NAMESPACE
