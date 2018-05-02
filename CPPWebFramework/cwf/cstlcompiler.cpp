/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "cstlcompiler.h"
#include "cstlcompilerattributes.h"
#include "cstlcompilerfor.h"
#include "cstlcompilerobject.h"
#include "cstlcompilerif.h"
#include "cstlcompilerimport.h"
#include "constants.h"

#include <QHash>
#include <QSet>
#include <QStringBuilder>
#include <vector>

using namespace std;

CWF_BEGIN_NAMESPACE

CSTLCompiler::CSTLCompiler(const QByteArray &str, const QString &path,
                           QMap<QString, QObject *> &objects,
                           bool isStrFileName) : str(str),
                                                 path(path),
                                                 objects(objects),
                                                 isStrFileName(isStrFileName)
{
    if(isStrFileName)
        isView = str.toLower().endsWith(".view");
}

QByteArray CSTLCompiler::openFile(QXmlStreamReader &xml)
{
    if(isStrFileName)
    {
        QFile file( QString::fromUtf8(str));
        if(!file.open(QIODevice::ReadOnly))
            return "<html><body>" + file.errorString().toUtf8() + "</body></html>";

        QByteArray content(file.readAll());
        file.close();
        if(isView)
        {
            xml.addData(content);
        }
        else
        {
            return content;
        }
    }
    else
    {
        xml.addData(str);
    }

    if (xml.hasError())
        return "<html><body>XML ERROR: " + xml.errorString().toUtf8() + "</body></html>";
    return "";
}

QByteArray CSTLCompiler::processOutTag(QMap<QString, QString> &attr)
{
    int size = attr.size();
    if(size < 1)
        return "";
    else if(size > 1)
        return "***ERROR - OUT TAG DOES NOT HAS MORE THAN PROPERTY***";

    return CSTLCompilerAttributes(objects).buildAttributes(attr, false).toUtf8();
}

QByteArray CSTLCompiler::getBody(QXmlStreamReader &xml, const QString &tagName)
{
    QByteArray content( XML::HEADER().toUtf8());
    QStringRef text;
    QStringRef name;
    CSTLCompilerAttributes compilerAttributes(objects);

    for( xml.readNext(); !xml.atEnd(); xml.readNext())
    {
        name = xml.name();
        text = xml.text();

        if(xml.isStartElement())
        {
            QString att;
            QMap<QString, QString> attributes =  compilerAttributes.getAttributes(xml.attributes());

            for( auto it = attributes.begin(); it != attributes.end(); ++it)
                att += QLatin1Char(' ') % it.key() % QLatin1String("=\"")
                    % it.value() % QLatin1Char('\"');

            content += '<' + name.toUtf8().toLower() + att + '>' + text.toUtf8();
        }
        else if(xml.isEndElement())
        {
            if (name == tagName)
                break;

            content += text.toUtf8() + "</" + name.toUtf8().toLower() + '>';
        }
        else
            content += text.toUtf8();
    }

    return content;
}

QByteArray CSTLCompiler::processForTag(QXmlStreamReader &xml)
{
    const QString hashOpenKey = QStringLiteral("#{");
    const QChar closeKey = QLatin1Char( '}');

    QByteArray htmlOut;
    CSTLCompilerFor forAttributes(xml.attributes());
    CSTLCompilerAttributes compilerAttributes(objects);


    if(forAttributes.attributes.contains(CSTL::TAG::PROPERTY::ERROR()))
    {
        getBody(xml, CSTL::TAG::FOR());
        htmlOut = forAttributes.attributes[CSTL::TAG::PROPERTY::ERROR()].toUtf8();
    }
    else
    {
        const QString items = forAttributes.attributes[CSTL::TAG::PROPERTY::FOR::ITEMS()]
            .remove( hashOpenKey)
            .remove( closeKey);

        if(objects.contains(items))
        {
            QObject *object = (QObject*)objects[items];

            if(!items.isEmpty())
            {
                const QString type = QString::fromLocal8Bit( object->metaObject()->className());
                if(type != CSTL::SUPPORTED_TYPES::CWF_QLISTOBJECT())
                {
                    htmlOut = "***ERROR - " + type.toUtf8() + " ISN'T A CWF::QListObject***";
                    getBody(xml, CSTL::TAG::FOR());
                }
                else
                {
                    QListObject *qListObject = static_cast<QListObject*>(object);
                    const QByteArray ret = getBody(xml, CSTL::TAG::FOR());
                    const QString var = forAttributes.attributes[CSTL::TAG::PROPERTY::VAR()]
                        .remove( hashOpenKey)
                        .remove( closeKey);

                    for(int iL = 0; iL < qListObject->size(); ++iL)
                    {
                        objects.insert(var, qListObject->operator [](iL) );
                        QXmlStreamReader forBody(ret);
                        htmlOut += processXml(forBody);
                        objects.remove(var);
                    }
                }
            }
        }
        else
        {
            int from      = forAttributes.attributes[CSTL::TAG::PROPERTY::FOR::FROM()].toInt();
            int to        = forAttributes.attributes[CSTL::TAG::PROPERTY::FOR::TO()].toInt();
            int increment = forAttributes.attributes[CSTL::TAG::PROPERTY::FOR::INCREMENT()].toInt();
            QByteArray tagBody = getBody(xml, CSTL::TAG::FOR());
            QString &var = forAttributes.attributes[CSTL::TAG::PROPERTY::VAR()];
            for(int i = from; i <= to; i += increment)
            {
                QString copy = QString::fromUtf8( tagBody);
                CSTLCompilerObject obj;
                obj.setValue(QString::number(i));
                objects.insert(var, &obj);

                copy.remove(XML::HEADER());
                QString outPutText;
                compilerAttributes.compile(copy, outPutText);
                copy = XML::HEADER() % QLatin1Literal("<out>") % outPutText 
                    % QLatin1Literal("</out>");

                QXmlStreamReader forBody(copy);
                htmlOut += processXml(forBody);
                objects.remove(var);
            }
        }
    }
    return htmlOut;
}

QByteArray CSTLCompiler::processIfTag(QXmlStreamReader &xml)
{
    QByteArray htmlOut;
    CSTLCompilerIf ifAttributes(xml.attributes());

    if(ifAttributes.relationalOperator == RelationalOperator::ERROR)
    {
        getBody(xml, CSTL::TAG::IF());
        htmlOut = ifAttributes.attributes[CSTL::TAG::PROPERTY::ERROR()].toUtf8();
    }
    else
    {
        QString var(ifAttributes.attributes[CSTL::TAG::PROPERTY::VAR()]);
			QString condition(ifAttributes.attributes[CSTL::TAG::PROPERTY::CONDITION()]);
        CSTLCompilerObject conditionObj, varObj;
        bool removeVar = false, removeCondition = false;
        if(!objects.contains(var))
        {
            removeVar = true;
            varObj.setValue(var);
            objects.insert(var, &varObj);
        }
        if(!objects.contains(condition))
        {
            removeCondition = true;
            conditionObj.setValue(condition);
            objects.insert(condition, &conditionObj);
        }

        CSTLCompilerAttributes compilerAttributes(objects);
        QString tagBody = QString::fromUtf8( getBody(xml, CSTL::TAG::IF()));
        compilerAttributes.compileAttributes(ifAttributes.attributes);

        bool isTrue = false;

		const QString attrVar = ifAttributes.attributes[CSTL::TAG::PROPERTY::VAR()];
		const QString attrCondition = ifAttributes.attributes[CSTL::TAG::PROPERTY::CONDITION()];

		switch( ifAttributes.relationalOperator)
		{
			case RelationalOperator::EQUAL:
				isTrue = (attrVar == attrCondition);
				break;
			case RelationalOperator::DIFFERENT:
				isTrue = (attrVar != attrCondition);
				break;
			case RelationalOperator::GREATER:
				if(ifAttributes.isNumber)
					isTrue = attrVar.toDouble() > attrCondition.toDouble();
				else
					isTrue = attrVar > attrCondition;
				break;
			case RelationalOperator::GREATER_EQUAL:
				if(ifAttributes.isNumber)
					isTrue = attrVar.toDouble() >= attrCondition.toDouble();
				else
					isTrue = attrVar >= attrCondition;
				break;
			case RelationalOperator::LESS:
				if(ifAttributes.isNumber)
					isTrue = attrVar.toDouble() < attrCondition.toDouble();
				else
					isTrue = attrVar < attrCondition;
				break;
			case RelationalOperator::LESS_EQUAL:
				if(ifAttributes.isNumber)
					isTrue = attrVar.toDouble() <= attrCondition.toDouble();
				else
					isTrue = attrVar <= attrCondition;
				break;
			case RelationalOperator::ERROR:
				break;
        }

        if(isTrue)
        {
            if(!tagBody.contains( QLatin1Char('<'))
                || !tagBody.contains( QLatin1String("</")))
            {
                tagBody.replace(XML::HEADER(), QString());
                QString outPutText;
                compilerAttributes.compile(tagBody, outPutText);
                tagBody = XML::HEADER() % QLatin1Literal("<out>") % outPutText 
						 % QLatin1Literal("</out>");
            }
            QXmlStreamReader forBody(tagBody);
            htmlOut += processXml(forBody);
        }

        if(removeVar)
            objects.remove(var);
        if(removeCondition)
            objects.remove(condition);
    }
    return htmlOut;
}

QByteArray CSTLCompiler::processXml(QXmlStreamReader &xml)
{
    QByteArray htmlOut;
    while(!xml.atEnd())
    {
        CSTLCompilerAttributes compilerAttributes(objects);
        QString name(std::move(xml.name().toString().toLower()));
        QString text(std::move(xml.text().toString()));
        QString tagAttributes;
        QMap<QString, QString> attr;

        if( xml.isStartElement())
        {
            static const vector<uint> tagsHashList = {
                qHash( CSTL::TAG::OUT()),
                qHash( CSTL::TAG::FOR()),
                qHash( CSTL::TAG::IF()),
                qHash( CSTL::TAG::IMPORT())
            };
            const uint nameHash = qHash( name);
            
            if( nameHash == tagsHashList[0]) // CSTL::TAG::OUT()
            {
                attr = std::move(compilerAttributes.getAttributes(xml.attributes()));
                htmlOut += processOutTag(attr);
                name.clear();
            }
            else if( nameHash == tagsHashList[1]) // CSTL::TAG::FOR()
            {
                htmlOut += processForTag(xml);
                name.clear();
            }
            else if( nameHash == tagsHashList[2]) // CSTL::TAG::IF() 
            {
                htmlOut += processIfTag(xml);
                name.clear();
            }
            else if( nameHash == tagsHashList[3]) // CSTL::TAG::IMPORT() 
            {
                CSTLCompilerImport importUrl(xml.attributes(), path);
                if(!importUrl.attributes.contains(CSTL::TAG::PROPERTY::ERROR()))
                    htmlOut += importUrl.attributes[CSTL::TAG::PROPERTY::IMPORT::URL()].toUtf8();
                else
                    htmlOut += importUrl.attributes[CSTL::TAG::PROPERTY::ERROR()].toUtf8();
                name.clear();
            }
        }
        else
        {
            static const QSet<QString> validNames = {
                CSTL::TAG::OUT(),
                CSTL::TAG::IF(),
                CSTL::TAG::FOR()
            };

            if( validNames.contains( name) == false)
            {
                attr = std::move(compilerAttributes.getAttributes(xml.attributes()));
                tagAttributes = compilerAttributes.buildAttributes(attr);
                if(xml.isStartElement())
                    name = "<" + name + tagAttributes + ">";
                else if(xml.isEndElement())
                    name = "</" + name + ">";
            }
            else
                name.clear();
        }

        //process(text, number);
        htmlOut += name + text;
        xml.readNext();
        if(xml.hasError())
        {
            return "<html><body>XML ERROR: " + xml.errorString().toUtf8() + "</body></html>";
        }
    }
    return htmlOut;
}

QByteArray CSTLCompiler::output()
{
    QXmlStreamReader xml;
    QByteArray htmlOutput(std::move(openFile(xml)));
    if(isView)
    {
        if(htmlOutput.isEmpty())
            return processXml(xml);
    }
    return htmlOutput;
}

CWF_END_NAMESPACE
