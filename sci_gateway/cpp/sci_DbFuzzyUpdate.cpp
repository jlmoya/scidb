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

#include "sci_util.h"
#include "sci_db.h"

extern QString sDefaultConnection;

extern QMap<QString,QList<QString>> mslsProviderConnectionOptions;

extern QList<QString> lsCommonConnectionParameters;

extern QList<QString> lsProviders;

extern "C"
{
    int sci_DbFuzzyUpdate(char *fname)
    {
        SciErr sciErr;
        QSqlDatabase *db;
        FuzzySQL *fsql;
        char *sObjectType;
        int *piListPosition;

        CheckRhs(3,3);
        CheckLhs(0,1);

        getFSQLParam(fname, 1, &fsql);
        sciGetStringAt(fname, 2, &sObjectType);

        QMap<QString, QVariant> *mValues = new QMap<QString, QVariant>();
        getVarAddressFromPosition(pvApiCtx, 3, &piListPosition);
        sciGetValuesMapFromStruct(fname, piListPosition, mValues);

        if (!strcmp(sObjectType, "FMT"))
        {
            FuzzyTableInfo fTab(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyTableInfo(fTab);
        }else if (!strcmp(sObjectType, "FCL"))
        {
            FuzzyCol fCol(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyColumn(fCol);
        }else if (!strcmp(sObjectType, "FOB"))
        {
            FuzzyObject fObj(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyObject(fObj);
        }else if (!strcmp(sObjectType, "FLD"))
        {
            FuzzyLabel fLab(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyLabel(fLab);
        }else if (!strcmp(sObjectType, "FAM"))
        {
            FuzzyApproxMuch fApprox(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyApproxMuch(fApprox);
        }else if (!strcmp(sObjectType, "FND"))
        {
            FuzzyNearness fNns(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyNearness(fNns);
        }else if (!strcmp(sObjectType, "FCC"))
        {
            Scierror(999, "%s: Object of type %s cannot be updatet", fname, sObjectType);
            return 0;
        }else if (!strcmp(sObjectType, "FQD"))
        {
            FuzzyQualifier fQual(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyQualifier(fQual);
        }else if (!strcmp(sObjectType, "FDC"))
        {
            Scierror(999, "%s: Object of type %s cannot be updatet", fname, sObjectType);
            return 0;
        }else if (!strcmp(sObjectType, "FDT"))
        {
            Scierror(999, "%s: Object of type %s cannot be updatet", fname, sObjectType);
            return 0;
        }else if (!strcmp(sObjectType, "FTQ"))
        {
            FuzzyTableQuantifier fTabQ(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyTableQuantifier(fTabQ);
        }else if (!strcmp(sObjectType, "FSQ"))
        {
            FuzzySystemQuantifier fSysQ(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzySystemQuantifier(fSysQ);
        }else if (!strcmp(sObjectType, "FDS"))
        {
            FuzzyDegreeSig fDegSig(mValues);
            fsql->FuzzyMetaBase()->UpdateFuzzyDegreeSig(fDegSig);
        }
        else
        {
            Scierror(999, "%s: Unknown object type %s", fname, sObjectType);
            return 0;
        }

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
