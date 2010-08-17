/* ==================================================================== */
/* Igor GRIDCHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_util.h"
#include "sci_db.h"
/* ==================================================================== */

extern QString sDefaultConnection;

extern QMap<QString,QList<QString>> mslsProviderConnectionOptions;

extern QList<QString> lsCommonConnectionParameters;

extern QList<QString> lsProviders;

extern "C"
{
    int sci_DbFuzzyCreate(char *fname)
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
            fsql->FuzzyMetaBase()->CreateTableMetaInfo(fTab.name());
        }else if (!strcmp(sObjectType, "FCL"))
        {
            FuzzyCol fCol(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyCol(fCol.tableId(), fCol.fType(), fCol.len(), fCol.codeSig(),
                fCol.columnName(), fCol.com(), fCol.um());
        }else if (!strcmp(sObjectType, "FOB"))
        {
            FuzzyObject fObj(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyObject(fObj.columnId(), fObj.fuzzyName(), fObj.fuzzyType());
        }else if (!strcmp(sObjectType, "FLD"))
        {
            FuzzyLabel fLab(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyLabelDef(fLab.fuzzyId(), fLab.alpha(), fLab.beta(), fLab.gamma(), fLab.delta());
        }else if (!strcmp(sObjectType, "FAM"))
        {
            FuzzyApproxMuch fApprox(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyApproxMuch(fApprox.columnId(), fApprox.margin(), fApprox.much());
        }else if (!strcmp(sObjectType, "FND"))
        {
            FuzzyNearness fNns(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyNearness(fNns.fuzzyId1(), fNns.fuzzyId2(), fNns.degree());
        }else if (!strcmp(sObjectType, "FCC"))
        {
            FuzzyCompatibleCol FCompCol(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyCompatibleCol(FCompCol.columnId1(), FCompCol.columnId2());
        }else if (!strcmp(sObjectType, "FQD"))
        {
            FuzzyQualifier fQual(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyQualifier(fQual.fuzzyId(), fQual.qualifier());
        }else if (!strcmp(sObjectType, "FDC"))
        {
            FuzzyDegreeCol fDegCol(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyDegreeCol(fDegCol.columnId1(), fDegCol.columnId2());
        }else if (!strcmp(sObjectType, "FDT"))
        {
            FuzzyDegreeTable fDegT(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyDegreeTable(fDegT.tableId(), fDegT.columnId(),fDegT.degreeType());
        }else if (!strcmp(sObjectType, "FTQ"))
        {
            FuzzyTableQuantifier fTabQ(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyTableQuantifier(fTabQ.tableId(), fTabQ.fuzzyName(), fTabQ.fuzzyType(),
                fTabQ.alpha(), fTabQ.beta(), fTabQ.gamma(), fTabQ.delta());
        }else if (!strcmp(sObjectType, "FSQ"))
        {
            FuzzySystemQuantifier fSysQ(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzySystemQuantifier(fSysQ.fuzzyName(), fSysQ.fuzzyType(), fSysQ.alpha(), fSysQ.beta(), fSysQ.gamma(), fSysQ.delta());
        }else if (!strcmp(sObjectType, "FDS"))
        {
            FuzzyDegreeSig fDegSig(mValues);
            fsql->FuzzyMetaBase()->CreateFuzzyDegreeSig(fDegSig.significance());
        }
        else
        {
            Scierror(999, "%s: Unknown object type %s", fname, sObjectType);
        }

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
