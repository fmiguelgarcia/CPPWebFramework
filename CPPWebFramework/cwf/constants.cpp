#include "constants.h"

CWF_BEGIN_NAMESPACE

namespace CONFIGURATION
{
    QByteArray CPP_CONFIG() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("/config/");}

    QByteArray CPP_WEB_INI() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("CPPWeb.ini");}

    QByteArray CPP_LOG_FILE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("CPPWebServer.log");}

    QByteArray CPP_LOG_VAR() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("cwf_log_file");}

    QByteArray CPP_LOG_MAX_VAR() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("cwf_log_max");}
}

//XML CONSTANTS
namespace XML
{
    QString HEADER() Q_DECL_NOEXCEPT
    { return QStringLiteral("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n");}
}

//GETTER AND SETTER CONSTANTS
namespace GET_SET
{
    QString GET_LOWER() Q_DECL_NOEXCEPT
    { return QStringLiteral("get");}

    QString SET_LOWER() Q_DECL_NOEXCEPT
    { return QStringLiteral("set");}

    QString GET_UPPER() Q_DECL_NOEXCEPT
    { return QStringLiteral("GET");}

    QString SET_UPPER() Q_DECL_NOEXCEPT
    { return QStringLiteral("SET");}
}

//STATUS CONSTANTS
namespace STATUS
{
    QByteArray STATUS_401() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("/config/cppwebserverpages/401.view");}

    QByteArray STATUS_403() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("/config/cppwebserverpages/403.view");}

    QString STATUS_404() Q_DECL_NOEXCEPT
    { return QStringLiteral("/config/cppwebserverpages/404.view");}

    QByteArray NOT_FOUND() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Not Found");}
}


//CSTL CONSTANTS
namespace CSTL
{
    //CSTL TAGS CONSTANTS
    namespace TAG
    {
        QString OUT() Q_DECL_NOEXCEPT
        { return QStringLiteral("out");}

        QString FOR() Q_DECL_NOEXCEPT
        { return QStringLiteral("for");}

        QString IF() Q_DECL_NOEXCEPT
        { return QStringLiteral("if");}

        QString IMPORT() Q_DECL_NOEXCEPT
        { return QStringLiteral("import");}

        //PROPERTIES CONSTANTS
        namespace PROPERTY
        {
            QString VAR() Q_DECL_NOEXCEPT
            { return QStringLiteral("var");}

            QString ERROR() Q_DECL_NOEXCEPT
            { return QStringLiteral("error");}

            QString CONDITION() Q_DECL_NOEXCEPT
            { return QStringLiteral("condition");}

            namespace IMPORT
            {
                QString URL() Q_DECL_NOEXCEPT
                { return QStringLiteral("url");}
            }

            namespace FOR
            {
                QString ITEMS() Q_DECL_NOEXCEPT
                { return QStringLiteral("items");}

                QString FROM() Q_DECL_NOEXCEPT
                { return QStringLiteral("from");}

                QString TO() Q_DECL_NOEXCEPT
                { return QStringLiteral("to");}

                QString INCREMENT() Q_DECL_NOEXCEPT
                { return QStringLiteral("increment");}
            }

            namespace IF
            {
                QString EQUAL() Q_DECL_NOEXCEPT
                { return QStringLiteral("equal");}

                QString DIFFERENT() Q_DECL_NOEXCEPT
                { return QStringLiteral("different");}

                QString GREATER() Q_DECL_NOEXCEPT
                { return QStringLiteral("greater");}

                QString GREATER_EQUAL() Q_DECL_NOEXCEPT
                { return QStringLiteral("greater_equal");}

                QString LESS() Q_DECL_NOEXCEPT
                { return QStringLiteral("less");}

                QString LESS_EQUAL() Q_DECL_NOEXCEPT
                { return QStringLiteral("less_equal");}
            }
        }
    }

    //SUPORTED TYPES CONSTANTS
    namespace SUPPORTED_TYPES
    {
        QString QSTRING() Q_DECL_NOEXCEPT
        { return QStringLiteral("QString");}

        QString STD_STRING() Q_DECL_NOEXCEPT
        { return QStringLiteral("std::string");}

        QString BOOL() Q_DECL_NOEXCEPT
        { return QStringLiteral("bool");}

        QString CHAR() Q_DECL_NOEXCEPT
        { return QStringLiteral("char");}

        QString UNSIGNED_CHAR() Q_DECL_NOEXCEPT
        { return QStringLiteral("uchar");}

        QString SHORT() Q_DECL_NOEXCEPT
        { return QStringLiteral("short");}

        QString UNSIGNED_SHORT() Q_DECL_NOEXCEPT
        { return QStringLiteral("ushort");}

        QString INT() Q_DECL_NOEXCEPT
        { return QStringLiteral("int");}

        QString UNSIGNED_INT() Q_DECL_NOEXCEPT
        { return QStringLiteral("uint");}

        QString LON() Q_DECL_NOEXCEPT
        { return QStringLiteral("long");}

        QString UNSIGNED_LONG() Q_DECL_NOEXCEPT
        { return QStringLiteral("ulong");}

        QString LONG_LONG() Q_DECL_NOEXCEPT
        { return QStringLiteral("qlonglong");}

        QString UNSIGNED_LONG_LONG() Q_DECL_NOEXCEPT
        { return QStringLiteral("qulonglong");}

        QString FLOAT() Q_DECL_NOEXCEPT
        { return QStringLiteral("float");}

        QString DOUBLE() Q_DECL_NOEXCEPT
        { return QStringLiteral("double");}

        QString CWF_QLISTOBJECT() Q_DECL_NOEXCEPT
        { return QStringLiteral("CWF::QListObject");}
    }
}

//CONTENT TYPE CONSTANTS
namespace HTTP
{
    QByteArray SERVER() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Server");}

    QByteArray SERVER_VERSION() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("C++-Web-Server/3.1");}

    QByteArray DATA() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Data");}

    QByteArray TRANSFER_ENCODING() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Transfer-Encoding");}

    QByteArray CHUNKED() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("chunked");}

    QByteArray END_LINE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("\r\n");}

    QByteArray END_OF_MENSAGE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("\r\n\r\n");}

    QByteArray END_OF_MENSAGE_WITH_ZERO() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("0\r\n\r\n");}

    QByteArray CONTENT_TYPE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Content-Type");}

    QByteArray CONTENT_LENGTH() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Content-Length");}

    QByteArray CONTENT_DISPOSITION() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Content-Disposition");}

    QByteArray CONTENT_DISPOSITION_COLON() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Content-Disposition:");}

    QByteArray CONTENT_DISPOSITION_COLON_SPACE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Content-Disposition: ");}

    QByteArray MULTIPART() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("multipart");}

    QByteArray URLENCODED() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("urlencoded");}

    QByteArray COOKIE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Cookie");}

    QByteArray SESSION_ID() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("sessionId");}

    QByteArray FILENAME() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("filename");}

    QByteArray FORM_DATA_COLON_SPACE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("form-data; ");}

    QByteArray WEBKIT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("WebKit");}

    QByteArray LOCATION() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Location");}

    QByteArray REDIRECT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Redirect");}

    QByteArray SEE_OTHER() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("See Other");}

    QByteArray ACCEPT_ENCODING() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Accept-Encoding");}

    QByteArray ACCEPT_LANGUAGE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Accept-Language");}

    QByteArray HOST() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Host");}

    QByteArray USER_AGENT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("User-Agent");}

    QByteArray OK() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("OK");}

    /*
    QByteArray            = "";
    QByteArray            = "";
    */


    //TEXT
    QByteArray TEXT_HTML_UTF8() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("text/html; charset=UTF-8");}


    //IMAGE
    QByteArray IMAGE_MICROSOFT_ICO() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("image/vnd.microsoft.icon");}

    QByteArray IMAGE_JPEG() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("image/jpeg");}

    QByteArray IMAGE_TIFF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("image/tiff");}

    QByteArray IMAGE_SVG_XML() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("image/svg+xml");}


    //AUDIO
    QByteArray AUDIO_MP3() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("image/mpeg");}

    QByteArray AUDIO_MP4() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("image/mp4");}

    QByteArray APPLICATION_PHOTOSHOP() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("image/vnd.adobe.photoshop");}


    //VIDEO
    QByteArray VIDEO_FLV() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("video/x-flv");}


    //APPLICATION
    QByteArray APPLICATION_MSWORD() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/msword");}

    QByteArray APPLICATION_RTF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/rtf");}

    QByteArray APPLICATION_EXCEL() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/vnd.ms-excel");}

    QByteArray APPLICATION_JSON() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/json");}

    QByteArray APPLICATION_POWER_POINT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/vnd.ms-powerpoint");}

    QByteArray APPLICATION_JAVASCRIPT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/javascript");}

    QByteArray APPLICATION_OPEN_DOCUMENT_TEXT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/vnd.oasis.opendocument.text");}

    QByteArray APPLICATION_OPEN_DOCUMENT_SPREADSHEET() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/vnd.oasis.opendocument.spreadsheet");}

    QByteArray APPLICATION_SHOCKWAVE_FLASH() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/x-shockwave-flash");}

    QByteArray APPLICATION_RAR_COMPRESSED() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/x-rar-compressed");}

    QByteArray APPLICATION_MS_DOWNLOAD() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/x-msdownload");}

    QByteArray APPLICATION_CAB_COMPRESSED() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/vnd.ms-cab-compressed");}

    QByteArray APPLICATION_POSTSCRIPT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/postscript");}

    QByteArray APPLICATION_WWW_FORM_URLENCODED() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("application/x-www-form-urlencoded");}


    //HTTP METHODS CONSTANTS
    namespace METHOD
    {
        QByteArray GET() Q_DECL_NOEXCEPT
        { return QByteArrayLiteral("GET");}

        QByteArray PUT() Q_DECL_NOEXCEPT
        { return QByteArrayLiteral("PUT");}

        QByteArray POST() Q_DECL_NOEXCEPT
        { return QByteArrayLiteral("POST");}

        QByteArray DELETE() Q_DECL_NOEXCEPT
        { return QByteArrayLiteral("DELETE");}

        QByteArray OPTIONS() Q_DECL_NOEXCEPT
        { return QByteArrayLiteral("OPTIONS");}

        QByteArray HEAD() Q_DECL_NOEXCEPT
        { return QByteArrayLiteral("HEAD");}

        QByteArray TRACE() Q_DECL_NOEXCEPT
        { return QByteArrayLiteral("TRACE");}
    }
}

namespace COOKIE
{
    QByteArray COMMENT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Comment");}

    QByteArray DOMAIN_COOKIE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Domain");}

    QByteArray MAX_AGE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Max-Age");}

    QByteArray PATH() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Path");}

    QByteArray SECURE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Secure");}

    QByteArray VERSION() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("Version");}
}

//FILE EXTENTIONS CONSTANTS
namespace FILE_EXTENTION
{
    QByteArray BAR() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("/");}

    QByteArray HTML() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("html");}

    QByteArray HTM() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("htm");}

    QByteArray CSS() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("css");}

    QByteArray TXT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("txt");}

    QByteArray PHP() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("php");}

    QByteArray ICO() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("ico");}

    QByteArray PNG() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("png");}

    QByteArray GIF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("gif");}

    QByteArray BMP() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("bmp");}

    QByteArray JPE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("jpe");}

    QByteArray JPG() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("jpg");}

    QByteArray TIF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("TIF");}

    QByteArray TIFF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("TIFF");}

    QByteArray SVG() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("svg");}

    QByteArray SVGZ() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("svgz");}

    QByteArray PDF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("pdf");}

    QByteArray XML() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("xml");}

    QByteArray JSON() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("json");}

    QByteArray ZIP() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("zip");}

    QByteArray MP3() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("mp3");}

    QByteArray MP4() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("mp4");}

    QByteArray FLV() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("flv");}

    QByteArray DOC() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("doc");}

    QByteArray RTF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("rtf");}

    QByteArray XLS() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("xls");}

    QByteArray PPT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("ppt");}

    QByteArray JS() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("js");}

    QByteArray ODT() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("odt");}

    QByteArray ODS() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("ods");}

    QByteArray SWF() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("swf");}

    QByteArray RAR() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("rar");}

    QByteArray EXE() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("exe");}

    QByteArray MSI() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("msi");}

    QByteArray CAB() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("cab");}

    QByteArray PSD() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("psd");}

    QByteArray AI() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("ai");}

    QByteArray EPS() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("eps");}

    QByteArray PS() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("ps");}

    QByteArray INI() Q_DECL_NOEXCEPT
    { return QByteArrayLiteral("ini");}
}
CWF_END_NAMESPACE
