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
    int sci_DbFuzzyGet(char *fname)
    {
        SciErr sciErr;
        QSqlDatabase *db;
        FuzzySQL *fsql;
        char *pcObjectType;
        int iObjectId;

        //arguments: FSQL-pointer, object type, {object id | object id1, object id2 (string or int) }
        // ( 3 variants total )

        CheckRhs(3,4);
        CheckLhs(0,1);

        getFSQLParam(fname, 1, &fsql);
        sciGetStringAt(fname, 2, &pcObjectType);


        if (Rhs == 3)
        {
            if (!strcmp(pcObjectType, "FSQ"))
            {
                char **psObjectName;
                sciGetStringAt(fname, 3, psObjectName);

                FuzzySystemQuantifier *fSystemQuantif = fsql->FuzzyMetaBase()->GetFuzzySystemQuantifier(*psObjectName);

                QMap<QString, QVariant> *mValues = fSystemQuantif->ValuesMap();
                sciWriteMapIntoList(mValues);
            }
            else
            {
                sciGetIntAt(fname, 3, &iObjectId);

                if (!strcmp(pcObjectType, "FMT"))
                {
                    FuzzyTableInfo *ftblInd = fsql->FuzzyMetaBase()->GetFuzzyTableInfo(iObjectId);

                    QMap<QString, QVariant> *mValues = ftblInd->ValuesMap();
                    sciWriteMapIntoList(mValues);
                } else if (!strcmp(pcObjectType, "FCL"))
                {
                    FuzzyCol *fCol = fsql->FuzzyMetaBase()->GetFuzzyCol(iObjectId);

                    QMap<QString, QVariant> *mValues = fCol->ValuesMap();
                    sciWriteMapIntoList(mValues);
                } else if (!strcmp(pcObjectType, "FOB"))
                {
                    FuzzyObject *fObj = fsql->FuzzyMetaBase()->GetFuzzyObject(iObjectId);

                    QMap<QString, QVariant> *mValues = fObj->ValuesMap();
                    sciWriteMapIntoList(mValues);
                }else if (!strcmp(pcObjectType, "FLD"))
                {
                    FuzzyLabel *fLab = fsql->FuzzyMetaBase()->GetFuzzyLabel(iObjectId);

                    QMap<QString, QVariant> *mValues = fLab->ValuesMap();
                    sciWriteMapIntoList(mValues);
                }else if (!strcmp(pcObjectType, "FAM"))
                {
                    FuzzyApproxMuch *fApprox = fsql->FuzzyMetaBase()->GetFuzzyApproxMuch(iObjectId);

                    QMap<QString, QVariant> *mValues = fApprox->ValuesMap();
                    sciWriteMapIntoList(mValues);
                }else if (!strcmp(pcObjectType, "FQD"))
                {
                    FuzzyQualifier *fQual = fsql->FuzzyMetaBase()->GetFuzzyQualifier(iObjectId);

                    QMap<QString, QVariant> *mValues = fQual->ValuesMap();
                    sciWriteMapIntoList(mValues);
                }else if (!strcmp(pcObjectType, "FDC"))
                {
                    int iCol = fsql->FuzzyMetaBase()->GetFuzzyDegreeCol(iObjectId);

                    QMap<QString, QVariant> *mValues = new QMap<QString, QVariant>();
                    mValues->insert("columnDegId", QVariant(iObjectId));
                    mValues->insert("columnId", QVariant(iCol));

                    sciWriteMapIntoList(mValues);
                }else if (!strcmp(pcObjectType, "FDT"))
                {
                    FuzzyDegreeTable *fDegTab = fsql->FuzzyMetaBase()->GetFuzzyDegreeTable(iObjectId);

                    QMap<QString, QVariant> *mValues = fDegTab->ValuesMap();
                    sciWriteMapIntoList(mValues);
                }
                else if (!strcmp(pcObjectType, "FDS"))
                {
                    FuzzyDegreeSig *fDegreeSig = fsql->FuzzyMetaBase()->GetFuzzyDegreeSig(iObjectId);

                    QMap<QString, QVariant> *mValues = fDegreeSig->ValuesMap();
                    sciWriteMapIntoList(mValues);
                }
                else if (!strcmp(pcObjectType, "FCC"))
                {
                    //get all compatible cols, including adapters and adaptees
                    QList<int> *fColAdapters = fsql->FuzzyMetaBase()->GetFuzzyCompatibleCols(iObjectId);

                    int *piColAdapters = new int[fColAdapters->count()];
                    for (int i = 0; i <  fColAdapters->count(); i++)
                    {
                        piColAdapters[i] = fColAdapters->at(i);
                    }

                    createMatrixOfInteger32(pvApiCtx, Rhs + 1, 1, fColAdapters->count(), piColAdapters);
                }
                else if (strcmp(pcObjectType, "FTQ") && strcmp(pcObjectType, "FND"))
                {
                    Scierror(999, "%s: Unknow object type %s", fname, pcObjectType);
                    return 0;
                }
                else
                {
                    Scierror(999, "%s: Wrong argument number for object type %s. 4 arguments expectd", fname, pcObjectType);
                    return 0;
                }
            }
        }
        else if (Rhs == 4)
        {
            if (!strcmp(pcObjectType, "FTQ"))
            {
                char **psStringId;
                sciGetStringAt(fname, 4, psStringId);

                FuzzyTableQuantifier *fTabQuantif = fsql->FuzzyMetaBase()->GetFuzzyTableQuantifier(iObjectId, *psStringId);

                QMap<QString, QVariant> *mValues = fTabQuantif->ValuesMap();
                sciWriteMapIntoList(mValues);
            } else if (!strcmp(pcObjectType, "FND"))
            {
                int iSecondId;

                sciGetIntAt(fname, 4, &iSecondId);

                FuzzyNearness *fNearness = fsql->FuzzyMetaBase()->GetFuzzyNearness(iObjectId, iSecondId);

                QMap<QString, QVariant> *mValues = fNearness->ValuesMap();
                sciWriteMapIntoList(mValues);
            } else if (strcmp(pcObjectType, "FMT") &&
                strcmp(pcObjectType, "FCL") &&
                strcmp(pcObjectType, "FOB") &&
                strcmp(pcObjectType, "FLD") &&
                strcmp(pcObjectType, "FAM") &&
                strcmp(pcObjectType, "FCC") &&
                strcmp(pcObjectType, "FQD") &&
                strcmp(pcObjectType, "FDC") &&
                strcmp(pcObjectType, "FDT") &&
                strcmp(pcObjectType, "FDS"))
            {
                Scierror(999, "%s: Unknow object type %s", fname, pcObjectType);
                return 0;
            }
            else
            {
                Scierror(999, "%s: Wrong argument number for object type %s. 3 arguments expectd", fname, pcObjectType);
                return 0;
            }
        }

        LhsVar(1) = Rhs + 1;

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
