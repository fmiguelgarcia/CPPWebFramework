/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "cstlcompilerif.h"
#include "constants.h"

#include <QHash>
#include <QStringBuilder>

CWF_BEGIN_NAMESPACE

CSTLCompilerIf::CSTLCompilerIf(const QXmlStreamAttributes &attr)
{
	const int size = attr.size();
    if(size == 0 || size == 1)
    {
        relationalOperator = RelationalOperator::ERROR;
        attributes.insert(
			CSTL::TAG::PROPERTY::ERROR(),
			QStringLiteral("***ERROR IF TAG - IF TAG NEEDS TWO ATTRIBUTES ***"));
    }
    else if(size == 2)
    {
        for(int i = 0; i < size; ++i)
        {
            const QString name(std::move(attr[i].name().toString().toLower()));
            const QString value(std::move(attr[i].value().toString()));

            if(name == CSTL::TAG::PROPERTY::VAR())
            {
                attributes.insert(CSTL::TAG::PROPERTY::VAR(), value);
            }
            else
            {
				static const QHash<QString, RelationalOperator> relMap = {
					{ CSTL::TAG::PROPERTY::IF::EQUAL(), RelationalOperator::EQUAL},
					{ CSTL::TAG::PROPERTY::IF::DIFFERENT(), RelationalOperator::DIFFERENT},
					{ CSTL::TAG::PROPERTY::IF::GREATER(), RelationalOperator::GREATER},
					{ CSTL::TAG::PROPERTY::IF::GREATER_EQUAL(), RelationalOperator::GREATER_EQUAL},
					{ CSTL::TAG::PROPERTY::IF::LESS(), RelationalOperator::LESS},
					{ CSTL::TAG::PROPERTY::IF::LESS_EQUAL(), RelationalOperator::LESS_EQUAL}
				};

				relationalOperator = relMap.value( name, RelationalOperator::ERROR);

				if( relationalOperator == RelationalOperator::ERROR)
                {
					attributes.insert(
						CSTL::TAG::PROPERTY::ERROR(),
						QLatin1Literal("***ERROR IF TAG - IF TAG DOESN'T PERMITS AN ATTRIBUTE CALLED \"") % name % QLatin1Literal("\"***"));
                    break;
                }

                value.toDouble(&isNumber);
                attributes.insert(CSTL::TAG::PROPERTY::CONDITION(), value);
            }
        }
    }
    else if(size > 2)
    {
        relationalOperator = RelationalOperator::ERROR;
        attributes.insert(
			CSTL::TAG::PROPERTY::ERROR(),
			QStringLiteral("***ERROR IF TAG - IF TAG DOESN'T PERMITS MORE THAN ONE ATTRIBUTE ***"));
    }
}

CWF_END_NAMESPACE
