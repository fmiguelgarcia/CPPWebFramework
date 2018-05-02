/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#ifndef RESPONSE_H
#define RESPONSE_H

#include "httpcookie.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QFile>
#include <QThread>
#include <QJsonDocument>
#include "cppwebframework_global.h"

CWF_BEGIN_NAMESPACE

class Configuration;
/**
 * @brief The Response class is responsable to response a Http request.
 */
class CPPWEBFRAMEWORKSHARED_EXPORT Response
{
    QTcpSocket     *socket;
    const Configuration &configuration;
    int            statusCode;
    QByteArray     content;
    QByteArray     statusText;
    QString        httpStatus;
    QMap<QByteArray,QByteArray> headers;
    QVector<HttpCookie> cookies;
public:
	enum SC {
		SC_CONTINUE = 100,
		SC_SWITCHING_PROTOCOLS = 101,
		SC_OK = 200,
		SC_CREATED = 201,
		SC_ACCEPTED = 202,
		SC_NON_AUTHORITATIVE_INFORMATION = 203,
		SC_NO_CONTENT = 204,
		SC_RESET_CONTENT = 205,
		SC_PARTIAL_CONTENT = 206,
		SC_MULTIPLE_CHOICES = 300,
		SC_MOVED_PERMANENTLY = 301,
		SC_MOVED_TEMPORARILY = 302,
		SC_FOUND = 302,
		SC_SEE_OTHER = 303,
		SC_NOT_MODIFIED = 304,
		SC_USE_PROXY = 305,
		SC_TEMPORARY_REDIRECT = 307,
		SC_BAD_REQUEST = 400,
		SC_UNAUTHORIZED = 401,
		SC_PAYMENT_REQUIRED = 402,
		SC_FORBIDDEN = 403,
		SC_NOT_FOUND = 404,
		SC_METHOD_NOT_ALLOWED = 405,
		SC_NOT_ACCEPTABLE = 406,
		SC_PROXY_AUTHENTICATION_REQUIRED = 407,
		SC_REQUEST_TIMEOUT = 408,
		SC_CONFLICT = 409,
		SC_GONE = 410,
		SC_LENGTH_REQUIRED = 411,
		SC_PRECONDITION_FAILED = 412,
		SC_REQUEST_ENTITY_TOO_LARGE = 413,
		SC_REQUEST_URI_TOO_LONG = 414,
		SC_UNSUPPORTED_MEDIA_TYPE = 415,
		SC_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
		SC_EXPECTATION_FAILED = 417,
		SC_INTERNAL_SERVER_ERROR = 500,
		SC_NOT_IMPLEMENTED = 501,
		SC_BAD_GATEWAY = 502,
		SC_SERVICE_UNAVAILABLE = 503,
		SC_GATEWAY_TIMEOUT = 504,
		SC_HTTP_VERSION_NOT_SUPPORTED = 505
	};


    Response(QTcpSocket &socket, const Configuration &configuration);

    ~Response() {}

    void write(const QJsonObject &json, bool writeContentType = true);

    void write(const QJsonArray &array, bool writeContentType = true);

    void write(QByteArray &&data);

    void write(const QByteArray &data, bool flush = true);

    void writeHeaders();

    void writeToSocket(const QByteArray &data);

    void sendError(int sc, const QByteArray &msg);

    void flushBuffer();

    inline int getBufferSize() const noexcept { return content.size(); }

    inline void addHeader(const QByteArray &name, const QByteArray &value) noexcept { headers.insert(name, value); }

    inline void addCookie(const HttpCookie &cookie) noexcept { cookies.push_back(cookie); }

    inline QTcpSocket &getSocket() const noexcept { return *socket; }

    void setStatus(const int &statusCode, const QByteArray &description);

    void sendRedirect(const QByteArray &url);
};

CWF_END_NAMESPACE

#endif // RESPONSE_H
