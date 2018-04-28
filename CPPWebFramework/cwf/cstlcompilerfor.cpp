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

CWF_BEGIN_NAMESPACE

CSTLCompilerFor::CSTLCompilerFor(const QXmlStreamAttributes &attr)
{
	static const QSet<QString> validNames = {
		CSTL::TAG::PROPERTY::FOR::ITEMS(),
		CSTL::TAG::PROPERTY::VAR(),
		CSTL::TAG::PROPERTY::FOR::FROM(),
		CSTL::TAG::PROPERTY::FOR::TO(),
		CSTL::TAG::PROPERTY::FOR::INCREMENT()
	};

    for(int i = 0; i < attr.size(); ++i)
    {
        const QString name(attr[i].name().toString().toLower());
        const QString value(attr[i].value().toString());

		if( !validNames.contains( name))
        {
            attributes.insert(
				CSTL::TAG::PROPERTY::ERROR(),
				QLatin1Literal("***ERROR FOR TAG - FOR TAG DOESN'T PERMITS AN ATTRIBUTE CALLED ")
					% name % QLatin1Literal("***"));
            return;
        }
        attributes.insert(name, value);
    }

	if( !attributes.contains(CSTL::TAG::PROPERTY::FOR::ITEMS())
		|| !attributes.contains(CSTL::TAG::PROPERTY::VAR()))
    {
        bool from, to, increment;
        attributes[CSTL::TAG::PROPERTY::FOR::FROM()].toInt(&from);
        attributes[CSTL::TAG::PROPERTY::FOR::TO()].toInt(&to);
        attributes[CSTL::TAG::PROPERTY::FOR::INCREMENT()].toInt(&increment);

        if(!(from && to && increment) || !attributes.contains(CSTL::TAG::PROPERTY::VAR()))
        {
            attributes.insert(
				CSTL::TAG::PROPERTY::ERROR(),
				QStringLiteral("***ERROR FOR TAG - USE THE CORRECT ATTRIBUTES (FROM, TO, INCREMENT, VAR OR ITEMS, VAR)***"));
        }
    }
}

CWF_END_NAMESPACE
