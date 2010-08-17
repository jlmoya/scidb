/*
 * Copyright (C) 2010 - Igor GRIDCHYN
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#pragma once
#include "QtIncludes.h"

class TreeTokenNode
{
QString _token;
TreeTokenNode *_left, *_right;

public:
    QString Token();

    TreeTokenNode(QString expression);
    ~TreeTokenNode(void);

    QString CdegCall();
};
