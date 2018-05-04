/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#ifndef CSTLCOMPILERFOR_H
#define CSTLCOMPILERFOR_H

#include <QXmlStreamAttributes>
#include "cppwebframework_global.h"

CWF_BEGIN_NAMESPACE
/**
 * @brief Extracts and valites all attibutes from a "for" tag.
 */
class CSTLCompilerFor
{
public:
    /**
     * @brief This constructor processes and validates the attributes of "for" tags.
     * @param const QXmlStreamAttributes &attr : XML tag attributes.
     */
    explicit CSTLCompilerFor(const QXmlStreamAttributes &attr);

public:
    std::vector<QString> mErrors;
};

CWF_END_NAMESPACE

#endif // CSTLCOMPILERFOR_H
