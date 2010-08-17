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
