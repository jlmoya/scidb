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

#include "TreeTokenNode.h"

QString TreeTokenNode::Token()
{
    return _token;
}

TreeTokenNode::TreeTokenNode(QString expression)
{
    _left = NULL;
    _right = NULL;

    expression = expression.toUpper();

    int iBracketsCount = 0;

    for(int i=0; i<expression.length() - 3; i++)
    {
        if (expression.at(i) == '(')
        {
            iBracketsCount++;
        }
        else if (expression.at(i) == ')')
        {
            iBracketsCount--;
        }else if(expression.mid(i, 2) == "OR" && iBracketsCount == 0)
        {
            _left = new TreeTokenNode(expression.mid(0, i-1));
            _right = new TreeTokenNode(expression.mid(i+2));
            _token = "OR";
            break;
        }
    }

    if (_left == NULL)
    {
        for(int i=0; i<expression.length() - 3; i++)
        {
            if (expression.at(i) == '(')
            {
                iBracketsCount++;
            }
            else if (expression.at(i) == ')')
            {
                iBracketsCount--;
            }else if(expression.mid(i, 3) == "AND" && iBracketsCount == 0)
            {
                _left = new TreeTokenNode(expression.mid(0, i-1));
                _right = new TreeTokenNode(expression.mid(i+3));
                _token = "AND";
                break;
            }
        }
    }

    _token = expression;
}

TreeTokenNode::~TreeTokenNode(void)
{
}

QString TreeTokenNode::CdegCall()
{
    if (_left == NULL)
    {
        return _token;
    }

    if (_token == "OR")
    {
        return "min(" + _left->CdegCall() + ", " + _right->CdegCall() + ")";
    }
    else if(_token == "AND")
    {
        return "max(" + _left->CdegCall() + ", " + _right->CdegCall() + ")";
    }
    return NULL;
}
