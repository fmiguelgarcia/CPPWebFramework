/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QByteArray>
#include "cppwebframework_global.h"

CWF_BEGIN_NAMESPACE
//CONFIGURATION CONSTANTS
namespace CONFIGURATION
{
    QByteArray CPP_CONFIG() Q_DECL_NOEXCEPT;
    QByteArray CPP_WEB_INI() Q_DECL_NOEXCEPT;
    QByteArray CPP_LOG_FILE() Q_DECL_NOEXCEPT;
    QByteArray CPP_LOG_VAR() Q_DECL_NOEXCEPT;
    QByteArray CPP_LOG_MAX_VAR() Q_DECL_NOEXCEPT;
}

//XML CONSTANTS
namespace XML
{
    QString HEADER() Q_DECL_NOEXCEPT;
}

//GETTER AND SETTER CONSTANTS
namespace GET_SET
{
    QString GET_LOWER() Q_DECL_NOEXCEPT;
    QString SET_LOWER() Q_DECL_NOEXCEPT;
    QString GET_UPPER() Q_DECL_NOEXCEPT;
    QString SET_UPPER() Q_DECL_NOEXCEPT;
}

//STATUS CONSTANTS
namespace STATUS
{
    QByteArray  STATUS_401() Q_DECL_NOEXCEPT;
    QByteArray  STATUS_403() Q_DECL_NOEXCEPT;
    QString STATUS_404() Q_DECL_NOEXCEPT;
    QByteArray  NOT_FOUND() Q_DECL_NOEXCEPT;
}

//CSTL CONSTANTS
namespace CSTL
{
    //CSTL TAGS CONSTANTS
    namespace TAG
    {
        QString OUT() Q_DECL_NOEXCEPT;
        QString FOR() Q_DECL_NOEXCEPT;
        QString IF() Q_DECL_NOEXCEPT;
        QString IMPORT() Q_DECL_NOEXCEPT;

        //PROPERTIES CONSTANTS
        namespace PROPERTY
        {
            QString VAR() Q_DECL_NOEXCEPT;
            QString ERROR() Q_DECL_NOEXCEPT;
            QString CONDITION() Q_DECL_NOEXCEPT;

            namespace IMPORT
            {
                QString URL() Q_DECL_NOEXCEPT;
            }

            namespace FOR
            {
                QString ITEMS() Q_DECL_NOEXCEPT;
                QString FROM() Q_DECL_NOEXCEPT;
                QString TO() Q_DECL_NOEXCEPT;
                QString INCREMENT() Q_DECL_NOEXCEPT;
            }

            namespace IF
            {
                QString EQUAL() Q_DECL_NOEXCEPT;
                QString DIFFERENT() Q_DECL_NOEXCEPT;
                QString GREATER() Q_DECL_NOEXCEPT;
                QString GREATER_EQUAL() Q_DECL_NOEXCEPT;
                QString LESS() Q_DECL_NOEXCEPT;
                QString LESS_EQUAL() Q_DECL_NOEXCEPT;
            }
        }
    }

    //SUPORTED TYPES CONSTANTS
    namespace SUPPORTED_TYPES
    {
        QString QSTRING() Q_DECL_NOEXCEPT;
        QString STD_STRING() Q_DECL_NOEXCEPT;
        QString BOOL() Q_DECL_NOEXCEPT;
        QString CHAR() Q_DECL_NOEXCEPT;
        QString UNSIGNED_CHAR() Q_DECL_NOEXCEPT;
        QString SHORT() Q_DECL_NOEXCEPT;
        QString UNSIGNED_SHORT() Q_DECL_NOEXCEPT;
        QString INT() Q_DECL_NOEXCEPT;
        QString UNSIGNED_INT() Q_DECL_NOEXCEPT;
        QString LONG() Q_DECL_NOEXCEPT;
        QString UNSIGNED_LONG() Q_DECL_NOEXCEPT;
        QString LONG_LONG() Q_DECL_NOEXCEPT;
        QString UNSIGNED_LONG_LONG() Q_DECL_NOEXCEPT;
        QString FLOAT() Q_DECL_NOEXCEPT;
        QString DOUBLE() Q_DECL_NOEXCEPT;
        QString CWF_QLISTOBJECT() Q_DECL_NOEXCEPT;
    }
}

//CONTENT TYPE CONSTANTS
namespace HTTP
{
    QByteArray SERVER() Q_DECL_NOEXCEPT;
    QByteArray SERVER_VERSION() Q_DECL_NOEXCEPT;
    QByteArray DATA() Q_DECL_NOEXCEPT;
    QByteArray TRANSFER_ENCODING() Q_DECL_NOEXCEPT;
    QByteArray CHUNKED() Q_DECL_NOEXCEPT;
    QByteArray END_LINE() Q_DECL_NOEXCEPT;
    QByteArray END_OF_MENSAGE() Q_DECL_NOEXCEPT;
    QByteArray END_OF_MENSAGE_WITH_ZERO() Q_DECL_NOEXCEPT;
    QByteArray CONTENT_TYPE() Q_DECL_NOEXCEPT;
    QByteArray CONTENT_LENGTH() Q_DECL_NOEXCEPT;
    QByteArray CONTENT_DISPOSITION() Q_DECL_NOEXCEPT;
    QByteArray CONTENT_DISPOSITION_COLON() Q_DECL_NOEXCEPT;
    QByteArray CONTENT_DISPOSITION_COLON_SPACE() Q_DECL_NOEXCEPT;
    QByteArray MULTIPART() Q_DECL_NOEXCEPT;
    QByteArray URLENCODED() Q_DECL_NOEXCEPT;
    QByteArray COOKIE() Q_DECL_NOEXCEPT;
    QByteArray SESSION_ID() Q_DECL_NOEXCEPT;
    QByteArray FILENAME() Q_DECL_NOEXCEPT;
    QByteArray FORM_DATA_COLON_SPACE() Q_DECL_NOEXCEPT;
    QByteArray WEBKIT() Q_DECL_NOEXCEPT;
    QByteArray LOCATION() Q_DECL_NOEXCEPT;
    QByteArray REDIRECT() Q_DECL_NOEXCEPT;
    QByteArray SEE_OTHER() Q_DECL_NOEXCEPT;
    QByteArray ACCEPT_ENCODING() Q_DECL_NOEXCEPT;
    QByteArray ACCEPT_LANGUAGE() Q_DECL_NOEXCEPT;
    QByteArray HOST() Q_DECL_NOEXCEPT;
    QByteArray USER_AGENT() Q_DECL_NOEXCEPT;
    QByteArray OK() Q_DECL_NOEXCEPT;
    /*
    QByteArray            = "";
    QByteArray            = "";
    */


    //TEXT
    QByteArray TEXT_HTML_UTF8() Q_DECL_NOEXCEPT;

    //IMAGE
    QByteArray IMAGE_MICROSOFT_ICO() Q_DECL_NOEXCEPT;
    QByteArray IMAGE_JPEG() Q_DECL_NOEXCEPT;
    QByteArray IMAGE_TIFF() Q_DECL_NOEXCEPT;
    QByteArray IMAGE_SVG_XML() Q_DECL_NOEXCEPT;

    //AUDIO
    QByteArray AUDIO_MP3() Q_DECL_NOEXCEPT;
    QByteArray AUDIO_MP4() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_PHOTOSHOP() Q_DECL_NOEXCEPT;

    //VIDEO
    QByteArray VIDEO_FLV() Q_DECL_NOEXCEPT;

    //APPLICATION
    QByteArray APPLICATION_MSWORD() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_RTF() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_EXCEL() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_JSON() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_POWER_POINT() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_JAVASCRIPT() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_OPEN_DOCUMENT_TEXT() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_OPEN_DOCUMENT_SPREADSHEET() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_SHOCKWAVE_FLASH() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_RAR_COMPRESSED() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_MS_DOWNLOAD() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_CAB_COMPRESSED() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_POSTSCRIPT() Q_DECL_NOEXCEPT;
    QByteArray APPLICATION_WWW_FORM_URLENCODED() Q_DECL_NOEXCEPT;

    //HTTP METHODS CONSTANTS
    namespace METHOD
    {
        QByteArray GET() Q_DECL_NOEXCEPT;
        QByteArray PUT() Q_DECL_NOEXCEPT;
        QByteArray POST() Q_DECL_NOEXCEPT;
        QByteArray DELETE() Q_DECL_NOEXCEPT;
        QByteArray OPTIONS() Q_DECL_NOEXCEPT;
        QByteArray HEAD() Q_DECL_NOEXCEPT;
        QByteArray TRACE() Q_DECL_NOEXCEPT;
    }
}

namespace COOKIE
{
    QByteArray COMMENT() Q_DECL_NOEXCEPT;
    QByteArray DOMAIN_COOKIE() Q_DECL_NOEXCEPT;
    QByteArray MAX_AGE() Q_DECL_NOEXCEPT;
    QByteArray PATH() Q_DECL_NOEXCEPT;
    QByteArray SECURE() Q_DECL_NOEXCEPT;
    QByteArray VERSION() Q_DECL_NOEXCEPT;
}

//FILE EXTENTIONS CONSTANTS
namespace FILE_EXTENTION
{
    QByteArray BAR() Q_DECL_NOEXCEPT;
    QByteArray HTML() Q_DECL_NOEXCEPT;
    QByteArray HTM() Q_DECL_NOEXCEPT;
    QByteArray CSS() Q_DECL_NOEXCEPT;
    QByteArray TXT() Q_DECL_NOEXCEPT;
    QByteArray PHP() Q_DECL_NOEXCEPT;
    QByteArray ICO() Q_DECL_NOEXCEPT;
    QByteArray PNG() Q_DECL_NOEXCEPT;
    QByteArray GIF() Q_DECL_NOEXCEPT;
    QByteArray BMP() Q_DECL_NOEXCEPT;
    QByteArray JPE() Q_DECL_NOEXCEPT;
    QByteArray JPG() Q_DECL_NOEXCEPT;
    QByteArray TIF() Q_DECL_NOEXCEPT;
    QByteArray TIFF() Q_DECL_NOEXCEPT;
    QByteArray SVG() Q_DECL_NOEXCEPT;
    QByteArray SVGZ() Q_DECL_NOEXCEPT;
    QByteArray PDF() Q_DECL_NOEXCEPT;
    QByteArray XML() Q_DECL_NOEXCEPT;
    QByteArray JSON() Q_DECL_NOEXCEPT;
    QByteArray ZIP() Q_DECL_NOEXCEPT;
    QByteArray MP3() Q_DECL_NOEXCEPT;
    QByteArray MP4() Q_DECL_NOEXCEPT;
    QByteArray FLV() Q_DECL_NOEXCEPT;
    QByteArray DOC() Q_DECL_NOEXCEPT;
    QByteArray RTF() Q_DECL_NOEXCEPT;
    QByteArray XLS() Q_DECL_NOEXCEPT;
    QByteArray PPT() Q_DECL_NOEXCEPT;
    QByteArray JS() Q_DECL_NOEXCEPT;
    QByteArray ODT() Q_DECL_NOEXCEPT;
    QByteArray ODS() Q_DECL_NOEXCEPT;
    QByteArray SWF() Q_DECL_NOEXCEPT;
    QByteArray RAR() Q_DECL_NOEXCEPT;
    QByteArray EXE() Q_DECL_NOEXCEPT;
    QByteArray MSI() Q_DECL_NOEXCEPT;
    QByteArray CAB() Q_DECL_NOEXCEPT;
    QByteArray PSD() Q_DECL_NOEXCEPT;
    QByteArray AI() Q_DECL_NOEXCEPT;
    QByteArray EPS() Q_DECL_NOEXCEPT;
    QByteArray PS() Q_DECL_NOEXCEPT;
    QByteArray INI() Q_DECL_NOEXCEPT;
}

CWF_END_NAMESPACE

#endif // CONSTANTS_H
