/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "request.h"
#include "metaclassparser.h"
#include "constants.h"
#include "response.h"
#include "urlencoder.h"

#include <QUuid>
#include <QStringBuilder>

#include <type_traits>

namespace 
{
	template< class T>
	void invokeMethodFirstCharacter( QObject* object, const char* method, const QString& value)
	{
		const T valueChar = (!value.isEmpty())
			?(value.leftRef(1).toUtf8()[0])
			:(' ');
		QMetaObject::invokeMethod(object, method, Q_ARG(T, valueChar));
	}
	
	template< 
		class T
		// ,typename std::enable_if< std::is_unsigned<T>::value >::type
	>
	void invokeMethodUnsignedInteger( QObject* object, const char* method, const QString& value)
	{
		const T intValue = static_cast<T>( value.toULongLong());
		QMetaObject::invokeMethod( object, method, Q_ARG(T, intValue));
	}
	
	template< class T>
	void invokeMethodSignedInteger( QObject* object, const char* method, const QString& value)
	{
		const T intValue = static_cast<T>( value.toLongLong());
		QMetaObject::invokeMethod( object, method, Q_ARG(T, intValue));
	}

	template< class T>
	void invokeMethodReal( QObject* object, const char* method, const QString& value)
	{
		const T realValue = static_cast<T>( value.toDouble());
		QMetaObject::invokeMethod( object, method, Q_ARG(T, realValue));
	}
}

CWF_BEGIN_NAMESPACE

Request::Request(QTcpSocket &socket, QMapThreadSafety<QString, Session *> &sessions,
	const Configuration &configuration) 
		: socket(&socket),
		sessions(sessions),
		configuration(configuration)
{
}

Request::~Request()
{
    delete requestDispatcher;
}

void Request::fillQObject(QObject *object)
{
    fillQObject(object, httpParser->getParameters());
}

void Request::fillQObject(QObject *object, const QMap<QByteArray, QByteArray> &parameters)
{
	using InvokeHandler = std::function< void( QObject*, const char*, const QString&)>;
	static const QHash<QString, InvokeHandler> typeHandlerMapper = {
		{
			CSTL::SUPPORTED_TYPES::QSTRING(),
			[]( QObject* object, const char* method, const QString& value){
				QMetaObject::invokeMethod(object, method, Q_ARG(QString, value));
			}
		},
		{
			CSTL::SUPPORTED_TYPES::STD_STRING(),
			[]( QObject* object, const char* method, const QString& value){
				QMetaObject::invokeMethod(object, method, Q_ARG(std::string, value.toStdString()));
			}
		},
		{
			CSTL::SUPPORTED_TYPES::BOOL(),
			[]( QObject* object, const char* method, const QString& value){
				QMetaObject::invokeMethod(object, method, Q_ARG(bool, value.toInt() == 0));
			}
		},
		{ CSTL::SUPPORTED_TYPES::CHAR(), & invokeMethodFirstCharacter<char> },
		{ CSTL::SUPPORTED_TYPES::UNSIGNED_CHAR(), & invokeMethodFirstCharacter<unsigned char> },
		{ CSTL::SUPPORTED_TYPES::SHORT(), & invokeMethodSignedInteger<short> },
		{ CSTL::SUPPORTED_TYPES::UNSIGNED_SHORT(), & invokeMethodUnsignedInteger<unsigned short> },
		{ CSTL::SUPPORTED_TYPES::INT(), & invokeMethodUnsignedInteger<int> },
		{ CSTL::SUPPORTED_TYPES::UNSIGNED_INT(), & invokeMethodUnsignedInteger<unsigned int> },
		{ CSTL::SUPPORTED_TYPES::LONG(), & invokeMethodSignedInteger<long> },
		{ CSTL::SUPPORTED_TYPES::UNSIGNED_LONG(), & invokeMethodUnsignedInteger<unsigned long> },
		{ CSTL::SUPPORTED_TYPES::LONG_LONG(), & invokeMethodSignedInteger<long long> },
		{ CSTL::SUPPORTED_TYPES::UNSIGNED_LONG_LONG(), & invokeMethodUnsignedInteger<unsigned long long> },
		{ CSTL::SUPPORTED_TYPES::FLOAT(), & invokeMethodReal<float> },
		{ CSTL::SUPPORTED_TYPES::DOUBLE(), & invokeMethodReal<double> }
	};
	
    MetaClassParser meta(object);

    for(QMap<QByteArray, QByteArray>::const_iterator it = parameters.constBegin(); it != parameters.constEnd(); ++it)
    {
        QString method = it.key();
        if(method.size() > 0)
        {
            QString value = it.value();

            method[0] = method[0].toUpper();
            method = GET_SET::SET_LOWER() % method;

            QString parameterType(std::move(meta.getParameterType(method)));

				auto paramTypeHandlerItr = typeHandlerMapper.find( parameterType);
				if (paramTypeHandlerItr != end(typeHandlerMapper))
					paramTypeHandlerItr.value()( object, method.toStdString().data(), value);
        }
    }
}

RequestDispatcher &Request::getRequestDispatcher(const QString &page)
{
    delete requestDispatcher;
    requestDispatcher = new RequestDispatcher(configuration.getPath() + page);
    return *requestDispatcher;
}

Session &Request::getSession()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    qint64 currentTimeInt = QDateTime::currentMSecsSinceEpoch();
    qint64 expiration = configuration.getSessionExpirationTime();
    if(!session)
    {
        QByteArray sessionId  = httpParser->getSessionId();
        if(sessionId.isEmpty() || !sessions.contains(sessionId))
        {
            sessionId = QUuid::createUuid().toString().toLocal8Bit();
            response->addCookie(HttpCookie(HTTP::SESSION_ID(), sessionId));            
            session = new Session(sessionId, expiration);
            session->creationTime = currentTimeInt;            
            session->sessionExpirationTime = (currentTimeInt + expiration);
            sessions.insert(session->getId(), session);
        }
        else
        {
            session = sessions[sessionId];
        }
    }
    session->expired          = (currentTimeInt >= session->sessionExpirationTime);
    session->lastAccessedTime = currentTimeInt;
    if(!session->expired)
        session->sessionExpirationTime = (currentTimeInt + expiration);
    return *session;
}

CWF_END_NAMESPACE
