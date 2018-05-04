/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/
#ifndef CSTLCOMPILER_H
#define CSTLCOMPILER_H

#include "cppwebframework_global.h"

#include <QHash>

class QIODevice;
class QObject;

CWF_BEGIN_NAMESPACE
/**
* @brief This class compiles view pages with CSTL (C++ Server Pages Standard Tags Library).
*/
class CPPWEBFRAMEWORKSHARED_EXPORT CSTLCompiler
{
public:
    using ObjectMap = QHash<QString, QObject*>;

    /**
     * @brief Initialize the str, objects and isStrFileName properties.
     * @param QMap<QString, QObject *> &objects : Container objects that can be compiled into the view page.
     */
    CSTLCompiler(
            const QByteArray &content, 
            const QString &path, 
            ObjectMap &objects);

    CSTLCompiler(
            const QString& filePath, 
            const QString &path, 
            ObjectMap &objects);

    CSTLCompiler(
            QIODevice* input, 
            const QString &path, 
            ObjectMap &objects);

    /**
     * @brief Returns the compiled view page.
     * @return QByteArray : Compiled page.
     */
    QByteArray output();

private:
    const QString path;

    QHash<QString, QObject *> &objects;
    std::unique_ptr<QIODevice> mInput;

    QByteArray mOutput;

    void openInput();
    /**
     * @brief Process XML file to find special tags and expressions.
     * @param QXmlStreamReader &xml : The opened xml.
     * @return QByteArray : Returns the processed QByteArray or an error.
     */
    QByteArray processXml(QXmlStreamReader &xml);
    /**
     * @brief Process out tag from CSTL
     * @param QXmlStreamReader &xml : The opened xml.
     * @return QByteArray : Returns the processed QByteArray or an error.
     */
    QByteArray processOutTag( QXmlStreamAttributes &attr);

    /**
     * @brief Process for tag from CSTL
     * @param QXmlStreamReader &xml : The opened xml.
     * @return QByteArray : Returns the processed QByteArray or an error.
     */
    QByteArray processForTag(QXmlStreamReader &xml);
    /**
     * @brief Process if tag from CSTL
     * @param QXmlStreamReader &xml : The opened xml.
     * @return QByteArray : Returns the processed QByteArray or an error.
     */
    QByteArray processIfTag(QXmlStreamReader &xml);
    /**
     * @brief Extract a tag body.
     * @param QXmlStreamReader &xml : The opened xml.
     * @param const QString &tagName : Tag name to extract body.
     * @return QByteArray : Tag body.
     */
    QByteArray getBody(QXmlStreamReader &xml, const QString &tagName);
};

CWF_END_NAMESPACE

#endif // CSTLCOMPILER_H
