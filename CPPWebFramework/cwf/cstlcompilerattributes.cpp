/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "cstlcompilerattributes.h"
#include "constants.h"
#include <QVariant>
#include "metaclassparser.h"

#include <QStringBuilder>

namespace
{
	template<class T>
	void invokeMethodAndPush( QObject* object, const char* method, QString& value, bool& ok)
	{
		T returnValue;
		ok = QMetaObject::invokeMethod(object, method, Qt::DirectConnection, Q_RETURN_ARG(T, returnValue));
		value.push_back(returnValue);
	}

	template< class T>
	void invokeMethodAndNumber( QObject* object, const char* method, QString& value, bool& ok)
	{
		T returnValue;
		ok = QMetaObject::invokeMethod(object, method, Qt::DirectConnection, Q_RETURN_ARG(T, returnValue));
		value = std::move( QString::number( returnValue));
	}
}

CWF_BEGIN_NAMESPACE

CSTLCompilerAttributes::CSTLCompilerAttributes(const QMap<QString, QObject *> &objects) : objects(objects)
{
}

QString CSTLCompilerAttributes::buildAttributes(
    QMap<QString, QString> &attr,
    const bool keyValue)
{
    using AttrItr = QMap<QString, QString>::iterator;
    using AttrHandler = std::function< QString( AttrItr& )>;

    AttrHandler attrHandler;
    QString htmlOut;

    if(keyValue)
        attrHandler = []( AttrItr& it){
            return QLatin1Char(' ') % it.key() % QLatin1Literal("=\"")
                % it.value() % QLatin1Char('\"');
        };
    else
        attrHandler = []( AttrItr& it){ return it.value(); };

    compileAttributes(attr);
    
    for( auto it = attr.begin(); it != attr.end(); ++it)
        htmlOut += attrHandler( it);

    return htmlOut;
}

void CSTLCompilerAttributes::compileAttributes(QMap<QString, QString> &attr)
{
    for( auto it = attr.begin(); it != attr.end(); ++it)
    {
        QString outPutText;
        QString &value = it.value();
        compile(value, outPutText);
        value = outPutText;
        /*
        //Compile at runtime
        if(value.startsWith("#{") && value.endsWith("}"))
        {
            value.replace("#{", "").replace("}", "");
            Properties prop(value);
            if(!objects.contains(prop.m_class))
            {
                value = "***ERROR - OBJECT " + prop.m_class + " DOES NOT EXIST.***";
            }
            else
            {
                QObject *object = objects[prop.m_class];
                bool ok = QMetaObject::invokeMethod(object, prop.m_method.toStdString().data(), Qt::DirectConnection, Q_RETURN_ARG(QString, value));
                if(!ok)
                {
                    value = "***ERROR - METHOD " + prop.m_method + " .***";
                }
            }
        }
        */
    }
}

void CSTLCompilerAttributes::compile(QString &text, QString &outPutText)
{
    bool start = false;
    const int size = text.size();
    QString expr;

    for(int i = 0; i < size; ++i)
    {
        QChar ch = text[i];
        if(ch == QLatin1Char('#'))
        {
            if((i + 1) < size)
            {
                QChar c = text[i + 1];
                if(c == QLatin1Char('{'))
                {
                    start = true;
                }
            }
        }
        else if(ch == QLatin1Char('}'))
        {
            if(start)
            {
                start = false;
                expr.replace("#{", "");
                Properties prop(expr);
                if(!objects.contains(prop.m_class))
                {
                    outPutText += "***ERROR - OBJECT " + prop.m_class + " DOES NOT EXIST.***";
                }
                else
                {
                    QString value;
                    QObject *object = objects[prop.m_class];
                    bool ok = false;
                    MetaClassParser metaClassParser(object);
                    QString methodReturnType(std::move(metaClassParser.getReturnType(prop.m_method + "()")));

                    if(!methodReturnType.isEmpty())
                    {
						using RetHandlerFuncType = std::function< void( QObject*, const char*, QString&, bool&)>;
						static const QHash<QString, RetHandlerFuncType> returnHandler {
							{
								CSTL::SUPPORTED_TYPES::QSTRING(),
								[]( QObject* object, const char* method, QString& value, bool& ok){
									ok = QMetaObject::invokeMethod(object, method, Qt::DirectConnection, Q_RETURN_ARG(QString, value));
								}
							},
							{
								CSTL::SUPPORTED_TYPES::STD_STRING(),
								[]( QObject* object, const char* method, QString& value, bool& ok){
									std::string returnValue;
									ok = QMetaObject::invokeMethod(object, method, Qt::DirectConnection, Q_RETURN_ARG(std::string, returnValue));
									value = std::move( QString::fromStdString( returnValue));
								}
							},
							{
								CSTL::SUPPORTED_TYPES::BOOL(),
								& invokeMethodAndNumber<bool>
							},
							{
								CSTL::SUPPORTED_TYPES::CHAR(),
								& invokeMethodAndPush<char>
							},
							{
								CSTL::SUPPORTED_TYPES::UNSIGNED_CHAR(),
								& invokeMethodAndPush<unsigned char>
							},
							{
								CSTL::SUPPORTED_TYPES::SHORT(),
								& invokeMethodAndNumber<short>
							},
							{
								CSTL::SUPPORTED_TYPES::UNSIGNED_SHORT(),
								& invokeMethodAndNumber<unsigned short>
							},
							{
								CSTL::SUPPORTED_TYPES::INT(),
								& invokeMethodAndNumber<int>
							},
							{
								CSTL::SUPPORTED_TYPES::UNSIGNED_INT(),
								& invokeMethodAndNumber<unsigned int>
							},
							{
								CSTL::SUPPORTED_TYPES::LONG(),
								& invokeMethodAndNumber<long>
							},
							{
								CSTL::SUPPORTED_TYPES::UNSIGNED_LONG(),
								& invokeMethodAndNumber<unsigned long>
							},
							{
								CSTL::SUPPORTED_TYPES::LONG_LONG(),
								& invokeMethodAndNumber<long long>
							},
							{
								CSTL::SUPPORTED_TYPES::UNSIGNED_LONG_LONG(),
								& invokeMethodAndNumber<unsigned long long>
							},
							{
								CSTL::SUPPORTED_TYPES::FLOAT(),
								& invokeMethodAndNumber<float>
							},
							{
								CSTL::SUPPORTED_TYPES::DOUBLE(),
								& invokeMethodAndNumber<double>
							}
						};

						auto itr = returnHandler.constFind( methodReturnType);
						if( itr != std::end( returnHandler))
						{
							const std::string method = prop.m_method.toStdString();
							itr.value()( object, method.c_str(), value, ok);
                        }

                        if(!ok)
                        {
                            outPutText += "***ERROR - METHOD " + prop.m_method + " .***";
                        }
                        else
                        {
                            outPutText += value;
                        }
                    }
                    else
                    {
                        outPutText += "***ERROR - METHOD NOT FOUND " + prop.m_method + " .***";
                    }
                }
                expr.clear();
                continue;
            }
        }
        if(start)
        {
            expr.push_back(ch);
        }
        else
        {
            outPutText.push_back(ch);
        }
    }
}

QMap<QString, QString> CSTLCompilerAttributes::getAttributes(const QXmlStreamAttributes &attributes)
{
    QMap<QString, QString> attr;
    for(int i = 0; i < attributes.size(); ++i)
    {
        QString name(std::move(attributes[i].name().toString()));
        QString value(std::move(attributes[i].value().toString()));
        attr.insert(name, value);
    }
    return attr;
}

CWF_END_NAMESPACE
