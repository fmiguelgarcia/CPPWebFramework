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

#include <QStringBuilder>

using namespace CWF;

namespace
{
	QByteArray processXmlTagOut(
		CSTLCompiler& compiler,
		QXmlStreamReader& xml,
		CSTLCompilerAttributes& compilerAttributes,
		QString& name)
	{
		QByteArray data;
		if( xml.isStartElement())
		{
			QMap<QString, QString> attr(
			compilerAttributes.getAttributes(
				xml.attributes()));
			name.clear();
			data = compiler.processOutTag(attr);
		}
		return data;
	}
}

CWF_BEGIN_NAMESPACE

CSTLCompiler::CSTLCompiler(const QByteArray &str, const QString &path,
                           QMap<QString, QObject *> &objects,
                           bool isStrFileName) : str(str),
                                                 path(path),
                                                 objects(objects),
                                                 isStrFileName(isStrFileName)
{
    if(isStrFileName)
        isView = str.toLower().endsWith( ".view");
}

QByteArray CSTLCompiler::openFile(QXmlStreamReader &xml)
{
	QByteArray content;

    if(isStrFileName)
    {
        QFile file(str);
        if(!file.open(QIODevice::ReadOnly))
            content = "<html><body>" + file.errorString().toUtf8() + "</body></html>";

		else
		{
			content = std::move(file.readAll());
			file.close();
			if(isView)
				xml.addData(content);
        }
    }
    else
    {
        xml.addData(str);
    }

    if (xml.hasError())
        content = "<html><body>XML ERROR: " + xml.errorString().toUtf8() + "</body></html>";

    return content;
}

QByteArray CSTLCompiler::processOutTag(QMap<QString, QString> &attr)
{
	QByteArray data;

	const int size = attr.size();
    if(size < 1)
        return QByteArray();
    else if(size > 1)
        return "***ERROR - OUT TAG DOES NOT HAVE MORE THAN PROPERTY***";

    return CSTLCompilerAttributes(objects).buildAttributes(attr, false).toUtf8();
}

QByteArray CSTLCompiler::getBody(QXmlStreamReader &xml, const QString &tagName)
{
    QByteArray name, text, content(XML::HEADER());
    QString att;
    CSTLCompilerAttributes compilerAttributes(objects);
    QMap<QString, QString> attributes;
    QMap<QString, QString>::iterator it;

    xml.readNext();
    while(!xml.atEnd())
    {
        name = xml.name().toUtf8().toLower();
        text = xml.text().toUtf8();

        if(xml.isStartElement())
        {
            attributes = std::move(compilerAttributes.getAttributes(xml.attributes()));
            for(it = attributes.begin(); it != attributes.end(); ++it)
                att += " " + it.key() + "=\"" + it.value() + "\"";
            content += "<" + name + att + ">" + text;
            attributes.clear();
            att.clear();
        }
        else if(xml.isEndElement() && name == tagName)
            break;
        else if(xml.isEndElement())
            content += text + "</" + name + ">";
        else
            content += text;
        xml.readNext();
    }
    return content;
}

QByteArray CSTLCompiler::processForTag(QXmlStreamReader &xml)
{
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
        QString items(forAttributes.attributes[CSTL::TAG::PROPERTY::FOR::ITEMS()]);
        items.replace("#{", "").replace("}", "");
        if(objects.contains(items))
        {
            QObject *object = (QObject*)objects[items];
            QString type(object->metaObject()->className());

            if(!items.isEmpty())
            {
                if(type != CSTL::SUPPORTED_TYPES::CWF_QLISTOBJECT())
                {
                    htmlOut = "***ERROR - " + type.toUtf8() + " ISN'T A CWF::QListObject***";
                    getBody(xml, CSTL::TAG::FOR());
                }
                else
                {
                    QListObject *qListObject = static_cast<QListObject*>(object);
                    QString ret(std::move(getBody(xml, CSTL::TAG::FOR())));
                    QString var(forAttributes.attributes[CSTL::TAG::PROPERTY::VAR()]);
                    var.replace("#{", "").replace("}", "");
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
            QString tagBody(std::move(getBody(xml, CSTL::TAG::FOR())));
            QString &var = forAttributes.attributes[CSTL::TAG::PROPERTY::VAR()];
            for(int i = from; i <= to; i += increment)
            {
                QString copy(tagBody);
                CSTLCompilerObject obj;
                obj.setValue(QString::number(i));
                objects.insert(var, &obj);

                copy.replace(XML::HEADER(), QString());
                QString outPutText;
                compilerAttributes.compile(copy, outPutText);
                copy = XML::HEADER()
					% QLatin1Literal("<out>")
					% outPutText
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
        QString tagBody(std::move(getBody(xml, CSTL::TAG::IF())));
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
            if(!tagBody.contains("<") || !tagBody.contains("</"))
            {
                tagBody.replace(XML::HEADER(), "");
                QString outPutText;
                compilerAttributes.compile(tagBody, outPutText);
                tagBody = XML::HEADER() + "<out>" + outPutText + "</out>";
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
	using XmlElementHandler = std::function< QByteArray(
		CSTLCompiler&,
		QXmlStreamReader&,
		CSTLCompilerAttributes&,
		QString&)>;
	static const QHash<QString, XmlElementHandler> nameHandlerMapper = {
		{
			CSTL::TAG::OUT(),
			& processXmlTagOut
		},
		{
			CSTL::TAG::FOR(),
			[]( CSTLCompiler& compiler, QXmlStreamReader& xml, CSTLCompilerAttributes& compilerAttributes, QString& name){
			}
		}
	};

    QByteArray htmlOut;
    while(!xml.atEnd())
    {
        CSTLCompilerAttributes compilerAttributes(objects);
        QString name(std::move(xml.name().toString().toLower()));
        QString text(std::move(xml.text().toString()));
        QString tagAttributes;
        QMap<QString, QString> attr;


        else if(name == CSTL::TAG::FOR() && xml.isStartElement())
        {
            htmlOut += processForTag(xml);
            name.clear();
        }
        else if(name == CSTL::TAG::IF() && xml.isStartElement())
        {
            htmlOut += processIfTag(xml);
            name.clear();
        }
        else if(name == CSTL::TAG::IMPORT() && xml.isStartElement())
        {
            CSTLCompilerImport importUrl(xml.attributes(), path);
            if(!importUrl.attributes.contains(CSTL::TAG::PROPERTY::ERROR()))
                htmlOut += importUrl.attributes[CSTL::TAG::PROPERTY::IMPORT::URL()].toUtf8();
            else
                htmlOut += importUrl.attributes[CSTL::TAG::PROPERTY::ERROR()].toUtf8();
            name.clear();

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
            {
                name.clear();
            }
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
