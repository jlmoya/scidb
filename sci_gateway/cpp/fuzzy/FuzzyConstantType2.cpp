#include "FuzzyConstantType2.h"

FuzzyConstantType2::FuzzyConstantType2(QString str, FMB fuzzyMetaBase)
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
        data[1] = ((QString)args.at(1)).toDouble();

        return;
    }

    if(str.startsWith('{'))
    {
        if(str.contains('/'))
        {
			type = 11;

            QStringList args = str.mid(1, str.length() - 2).split(',');

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
			type = 10;

            QStringList args = str.mid(1, str.length() - 2).split(',');            

            data = new double[args.count()];

            for (int i = 0; i < args.size(); ++i)
                 data[i] = ((QString) args.at(i)).toDouble();

            return;
        }
    }

    //!!! need to fetch label ID !!!
    if(str.startsWith('$'))
    {
		FuzzyLabel lbl = fuzzyMetaBase.GetFuzzyLabel(str.mid(1, str.length() - 1));

        type = 4;

		data = new double[args.count()];
		
		data[0] = lbl.alpha();
		data[1] = lbl.beta();
		data[2] = lbl.gamma();
		data[3] = lbl.delta();
    }

    //!!! need to get the margin !!!
    if (str.startsWith('#'))
    {
        type = 6;

        return;
    }

    if (str.contains("+-"))
    {
        type = 8;

        QStringList args = str.split("+-");

        data = new double[4];

        double d = ((QString) args.at(0)).toDouble(),
            m = ((QString) args.at(1)).toDouble();

        data[0] = d;
        data[1] = d-m;
        data[2] = d+m;
        data[3] = m;

        return;
    }

    bool *res;
    double crisp = str.toDouble(res);

    if (*res)
    {
        type = 3;

        data = new double[1];
        data[0] = crisp;

        return;
    }

    type = -1;    
}
