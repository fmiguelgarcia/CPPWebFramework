/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "response.h"
#include "configuration.h"
#include "constants.h"

#include <QString>
#include <QDateTime>
#include <QDataStream>
#include <QStringBuilder>

namespace 
{
	QByteArray formatedCurrentDateTime()
	{
		const QString format = QStringLiteral("ddd, dd MMM yyyy hh:mm:ss t");
		
		return QDateTime::currentDateTime()
			.toString( format)
			.toUtf8();
	}
}

CWF_BEGIN_NAMESPACE

Response::Response(QTcpSocket &socket, const Configuration &configuration) 
	: socket( &socket),
	configuration( configuration),
	statusCode( Response::SC_OK),
	statusText( HTTP::OK())
{}

void Response::flushBuffer()
{
    const int max = 32768;
    if(!content.isEmpty())
    {
        bool biggerThanLimit = content.size() > max;
        headers.insert( HTTP::CONTENT_LENGTH(), QByteArray::number(content.size()));
        headers.insert( HTTP::SERVER(), HTTP::SERVER_VERSION());
        headers.insert( HTTP::DATA(), formatedCurrentDateTime());

        if(!biggerThanLimit)
        {
            writeHeaders();
            writeToSocket(content);
        }
        else
        {
            headers.insert( HTTP::TRANSFER_ENCODING(), HTTP::CHUNKED());
            writeHeaders();
            int total = (content.size() / max) + 1, last = 0;

            QVector<QByteArray> vetor;
            for(int i = 0; i < total; ++i)
            {
                vetor.push_back(std::move(content.mid(last, max)));
                last += max;
            }

            for(int i = 0; i < vetor.size(); ++i)
            {
                QByteArray data(std::move(vetor[i]));
                if(!data.isEmpty())
                {
                    QByteArray buffer(std::move(QByteArray::number(data.size(), 16)));
                    buffer.append( HTTP::END_LINE());
                    writeToSocket(buffer);
                    writeToSocket(data);
                    writeToSocket( HTTP::END_LINE());
                }
            }
            writeToSocket( HTTP::END_OF_MENSAGE_WITH_ZERO());
        }
        socket->disconnectFromHost();
        content.clear();
    }
}

void Response::sendError(int sc, const QByteArray &msg)
{
    writeHeaders();
	
	 const QByteArray html = "<html><body><h1>" % QByteArray::number(sc) % ' ' % msg % "</h1></body></html>";
    writeToSocket( html);
}

void Response::write(const QJsonObject &json, bool writeContentType)
{
    if(writeContentType)
        addHeader( CWF::HTTP::CONTENT_TYPE(), CWF::HTTP::APPLICATION_JSON());
    content = std::move(QJsonDocument(json).toJson());
    flushBuffer();
}

void Response::write(const QJsonArray &array, bool writeContentType)
{
    if(writeContentType)
        addHeader( CWF::HTTP::CONTENT_TYPE(), CWF::HTTP::APPLICATION_JSON());
    content = std::move(QJsonDocument(array).toJson());
    flushBuffer();
}

void Response::write(QByteArray &&data)
{
    content = std::move(data);
    flushBuffer();
}

void Response::write(const QByteArray &data, bool flush)
{
    content += data;
    if(flush)
        flushBuffer();
}

void Response::setStatus(const int &statusCode, const QByteArray &description)
{
    this->statusCode = statusCode;
    statusText       = description;
}

void Response::writeToSocket(const QByteArray &data)
{
    socket->moveToThread(QThread::currentThread());
    if(socket->ConnectingState > 0 && data.size() > 0)
    {
        socket->write( data);
        socket->flush();
        //qDebug() << data;
        if(socket->ConnectingState > 0)
        {            
            int timeOut = configuration.getTimeOut();
            socket->waitForBytesWritten(timeOut);
        }
    }
}

void Response::writeHeaders()
{
	const QByteArray httpEndLine = HTTP::END_LINE();
	
	QByteArray buffer;
	QDataStream stream( &buffer, QIODevice::WriteOnly);

	stream << "HTTP/1.1 " << statusCode << ' ' << statusText.constData() << httpEndLine.constData();

    if(!headers.contains( HTTP::CONTENT_TYPE()))
        headers.insert( HTTP::CONTENT_TYPE(), HTTP::TEXT_HTML_UTF8());

    QList<QByteArray> headersList(std::move(headers.keys()));

    for(QByteArray &name : headersList)
		 stream << name << ": " << headers.value(name) << httpEndLine.constData();
    
    for(HttpCookie &cookie : cookies)
		 stream << "Set-Cookie: " << cookie.toByteArray().constData() << httpEndLine.constData();
   
	stream << httpEndLine.constData();

	writeToSocket(buffer);
}

void Response::sendRedirect(const QByteArray &url)
{
    setStatus( Response::SC_SEE_OTHER, HTTP::SEE_OTHER());
    addHeader( HTTP::LOCATION(), url);
    write( HTTP::REDIRECT(), true);
}

CWF_END_NAMESPACE
