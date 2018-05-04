/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "cstlcompilerfor.h"
#include "constants.h"

#include <QSet>
#include <QStringBuilder>

namespace 
{
    void parseItemsTag( CSTLCompilerFor* cFor, const QXmlStreamAttribute& attr)
    {
    }
    
    void parseTagValueToInteger( const QXmlStreamAttribute& attr, int& target, int defaultValue = 0)
    {
        bool ok;
        target = attr.value().toInt( &ok);
        if( !ok)
            target = defaultValue;
    }

    void parseFromTag( CSTLCompilerFor* cFor, const QXmlStreamAttribute& attr)
    {
        parseTagValueToInteger( cFor->from, attr);
    }

    void parseToTag( CSTLCompilerFor* cFor, const QXmlStreamAttribute& attr)
    {
        parseTagValueToInteger( cFor->to, attr);
    }

    void parseIncrementTag( CSTLCompilerFor* cFor, const QXmlStreamAttribute& attr)
    {
        parseTagValueToInteger( cFor->increment, attr, 1);
    }

    void parseItemsTag( CSTLCompilerFor* cFor, const QXmlStreamAttribute& attr)
    {
        cFor->items = attr.value().toString();
    }

    void parseVarTag( CSTLCompilerFor* cFor, const QXmlStreamAttribute& attr)
    {
    }
}

CWF_BEGIN_NAMESPACE

CSTLCompilerFor::CSTLCompilerFor(const QXmlStreamAttributes &attributes)
{
    using AttrHandler = std::function< void( CSTLCompilerFor*, QXmlStreamAttribute&)>;
    static const QHash<QString, AttrHandler> validForAttributes = {
        { CSTL::TAG::PROPERTY::FOR::ITEMS(), & parseItemsTag },
        { CSTL::TAG::PROPERTY::VAR(), & parseVarTag },
        { CSTL::TAG::PROPERTY::FOR::FROM(), & parseFromTag },
        { CSTL::TAG::PROPERTY::FOR::TO(), & parseToTag },
        { CSTL::TAG::PROPERTY::FOR::INCREMENT(), & parseIncrementTag }
    };

    for( const QXmlStreamAttribute& attr : attributes)
    {
        const QString name = attr.name().toString().toLower();
        auto attrHandlerItr = validForAttributes.find( name);
        if( attrHandlerItr != end( validForAttributes))
            attrHandlerItr.value()( this, attr);
        else
        {
            mErrors.push_back(
                QLatin1Literal("***ERROR FOR TAG - FOR TAG DOESN'T PERMITS AN ATTRIBUTE CALLED ")
                % name % QLatin1Literal("***"));
        }
    }
}

CWF_END_NAMESPACE
