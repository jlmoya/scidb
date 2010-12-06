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

#include "FuzzyConstant.h"

FuzzyConstant::FuzzyConstant(QString str, FMB fuzzyMetaBase)
{
    if(str == "UNKNOWN")
    {
        type = 0;
        return;
    }

    if (str == "UNDEFINED")
    {
        type = 1;
        return;
    }

    if (str == "NULL")
    {
        type = 2;
    }

    if(str.startsWith("$["))
    {
        type = 7;
        str = str.mid(2, str.length() - 3);
        QStringList args = str.split(',');

        data = new double[4];

        data[0] = ((QString)args.at(0)).toDouble();
        data[1] = ((QString)args.at(1)).toDouble();
        data[2] = ((QString)args.at(2)).toDouble();
        data[3] = ((QString)args.at(3)).toDouble();

        return;
    }

    if (str.startsWith('['))
    {
        type = 5;
        str = str.mid(1, str.length()-2);
        QStringList args = str.split(',');

        data = new double[2];

        data[0] = ((QString)args.at(0)).toDouble();
        data[1] = ((QString)args.at(0)).toDouble();
        data[2] = ((QString)args.at(1)).toDouble();
        data[3] = ((QString)args.at(1)).toDouble();

        return;
    }

    if(str.startsWith('{'))
    {
        if(str.contains('/'))
        {

            QStringList args = str.mid(1, str.length() - 2).split(',');

            QStringList cplt = ((QString) args.at(0)).split('/');
            if (QString("0123456789").contains(cplt.at(2).at(0)))
            {
                type = 12;

                data = new double[args.count() * 2];

                for (int i = 0; i < args.size(); ++i)
                {
                    QStringList cpl = ((QString) args.at(i)).split('/');
                    data[2*i] = ((QString) cpl.at(0)).toDouble();
                    data[2*i + 1] = ((QString) cpl.at(1)).toDouble();
                }

                return;
            }
            else
            {
                type = 13;

                data = new double[args.count()];

                for (int i = 0; i < args.size(); ++i)
                {
                    QStringList cpl = ((QString) args.at(i)).split('/');
                    data[i] = ((QString) cpl.at(0)).toDouble();
                    labels.append(cpl.at(1));
                }

                return;
            }
        }

        else
        {
            QStringList args = str.mid(1, str.length() - 2).split(',');

            if (QString("0123456789").contains(args.at(0)))
            {
                type = 10;

                QStringList args = str.mid(1, str.length() - 2).split(',');

                data = new double[args.count()];

                for (int i = 0; i < args.size(); ++i)
                     data[i] = ((QString) args.at(i)).toDouble();

                return;
            }
            else
            {
                type = 11;

                QStringList args = str.mid(1, str.length() - 2).split(',');

                for (int i = 0; i < args.size(); ++i)
                    labels.append(args.at(i));

                return;
            }
        }
    }

    //!!! need to fetch label ID !!!
    if(str.startsWith('$'))
    {
        if (!fuzzyMetaBase._mFuzzyObjectsByName.contains(str.mid(1, str.length() - 1)))
        {
            type = -1;
            return;
        }

        FuzzyObject *obj = fuzzyMetaBase.GetFuzzyObject(str.mid(1, str.length() - 1));

        if (obj->fuzzyType() == 4)
        {
            FuzzyLabel *lbl = fuzzyMetaBase.GetFuzzyLabel(str.mid(1, str.length() - 1));
            type =4;

            data = new double[4];

            data[0] = lbl->alpha();
            data[1] = lbl->beta();
            data[2] = lbl->gamma();
            data[3] = lbl->delta();
        }
        else if (obj->fuzzyType() < 2 )
        {
            type = 15;

            labels.append(str.mid(1, str.length() - 1));
        }
        else if(obj->fuzzyType() == 2)
        {
            type = 16;

            labels.append(str.mid(1, str.length() - 1));
        }
    }

    //!!! need to get the margin !!!
    if (str.startsWith('#'))
    {
        type = 6;

        data = new double[4];

        data[0] = str.mid(1, str.length() - 1).toDouble();
        data[1] = str.mid(1, str.length() - 1).toDouble();

        return;
    }

    if (str.contains("+-"))
    {
        type = 8;

        QStringList args = str.split("+-");

        data = new double[4];

        double d = ((QString) args.at(0)).toDouble(),
            m = ((QString) args.at(1)).toDouble();

        //data[0] = d;
        //data[1] = d-m;
        //data[2] = d+m;
        //data[3] = m;

        //to enable handling like a trapezoid
        data[0] = d-m;
        data[1] = d;
        data[2] = d;
        data[3] = d+m;


        return;
    }

    bool *res = NULL;
    double crisp = str.toDouble(res);

    if (*res)
    {
        type = 3;

        data = new double[1];
        data[0] = crisp;

        return;
    }

    //bad constant
    type = -1;
}
