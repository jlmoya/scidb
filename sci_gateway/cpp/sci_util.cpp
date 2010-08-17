#include "sci_util.h"

int sciGetQueryParam(char *fname, int iPos, QSqlQuery **query)
{
    int *piAddr;
    void *pvPtr;
    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    *query = (QSqlQuery*)pvPtr;

    return 0;
}

int getDatabaseParam(char *fname, int iPos, QSqlDatabase **db)
{
    int *piAddr;
    void *pvPtr;
    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    *db = (QSqlDatabase*)pvPtr;

    return 0;
}

int getFSQLParam(char *fname, int iPos, FuzzySQL **fsql)
{
    int *piAddr;
    void *pvPtr;
    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    *fsql = (FuzzySQL*)pvPtr;

    return 0;
}

int sciGetIntAt(char *fname, int iPos, int *piResult)
{
    int *piAddr;
    void *pvPtr;
    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int *piRows, *piCols;
    double *dResult;

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, piRows, piCols, &dResult);
    *piResult = (int)*dResult;

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    return 0;
}

int sciWriteMapIntoList(QMap<QString, QVariant> *mValues)
{
    QList<QString> lNames = mValues->keys();

    char **pstLabels = (char**)malloc(sizeof(char*)*(lNames.count() + 2));
    pstLabels[0] = "st";
    pstLabels[1] = "dims";
    double pdblDims[]  = {1,1};

    for(int j=0; j<lNames.count();j++)
    {
        pstLabels[j+2] = (char*)malloc(sizeof(char)*lNames.at(j).length());
        strcpy(pstLabels[j+2], lNames.at(j).toLatin1().data());
    }

    int *piStructAddress;

    createMList(pvApiCtx, Rhs + 1, lNames.count() + 2, &piStructAddress);
    createMatrixOfStringInList(pvApiCtx, Rhs + 1, piStructAddress, 1, 1, lNames.count() + 2, pstLabels);
    createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, piStructAddress, 2, 1, 2, pdblDims);

    for(int i=0; i < lNames.count(); i++)
    {
        sciWriteVarIntoList(piStructAddress, i+3, mValues->values().at(i));
    }

    return 0;
}

int sciWriteVarIntoList(int *piList, int position, QVariant vValue)
{
    switch(vValue.type())
    {
        case QVariant::Bool:
        {
            bool bField = vValue.toBool();
            int iBool;
            if(bField)
                iBool = 1;
            else
                iBool = 0;

            createMatrixOfBooleanInList(pvApiCtx, Rhs+1, piList, position, 1, 1, &iBool);
            break;
        }

        case QVariant::Int:
        {
            int iField = vValue.toInt();

            int *res = (int*)malloc(sizeof(int));
            *res = iField;

            createMatrixOfInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);
            break;
        }

        case QVariant::UInt:
        {
            unsigned int iField = vValue.toUInt();

            unsigned int *res = (unsigned int*)malloc(sizeof(unsigned int));
            *res = iField;

            createMatrixOfUnsignedInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);
            break;
        }

        case QVariant::LongLong:
        {
            int iField = vValue.toInt();

            int *res = (int*)malloc(sizeof(int));
            *res = iField;

            createMatrixOfInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);
            break;
        }

        case QVariant::ULongLong:
        {
            unsigned int iField = vValue.toUInt();

            unsigned int *res = (unsigned int*)malloc(sizeof(unsigned int));
            *res = iField;

            createMatrixOfUnsignedInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);

            break;
        }

        case QVariant::Double:
        {
            double dField = vValue.toDouble();

            double *res = (double*)malloc(sizeof(double));
            *res = dField;

            createMatrixOfDoubleInList(pvApiCtx, Rhs+1, piList, position, 1, 1, res);
            break;
        }

        case QVariant::String:
        {
            char *pcField = vValue.toString().toLatin1().data();

            createMatrixOfStringInList(pvApiCtx, Rhs+1, piList, position, 1, 1, &pcField);
            break;
        }

        default:
        {
            sciprint("Unknown var type with code: %d. Writing as string.\n", vValue.type());

            char *pcField = vValue.toString().toLatin1().data();

            createMatrixOfStringInList(pvApiCtx, Rhs+1, piList, position, 1, 1, &pcField);

            break;
        }
    }

    return 0;
}

int sciStructStringFields(int *piAddr, QMap<QString, QString> *map, char *fname)
{
        SciErr sciErr;
        QList<QString> namesList;
        QList<QString> valuesList;

        int m1 = 0, n1 = 0;
        //int *piAddr = NULL;
        char *pStVarOne = NULL;
        int lenStVarOne = 0;
        int iType1 = 0;

        sciErr = getVarType(pvApiCtx, piAddr, &iType1);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }

        if ( iType1 != sci_mlist )
        {
          Scierror(999,"%s: Wrong type for input argument #%d: A mlist expected.\n",fname,1);
          return 0;
        }

        int iItemNumber  = 0;

        sciErr = getListItemNumber(pvApiCtx, piAddr, &iItemNumber);

        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        //sciprint("Number of items: (%d)\n", iItemNumber);

        int * piListItem = NULL;

        for(int i = 0 ; i < iItemNumber ; i++)
        {
            //sciprint("Getting List item Address...\n");
            sciErr = getListItemAddress(pvApiCtx, piAddr, i + 1, &piListItem);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            int iType = 0;
            sciErr = getVarType(pvApiCtx, piListItem, &iType);

            if(iType != sci_strings)
                continue;

            int iRows, iCols;

            //getting string
            //sciprint("Getting rows/ columns count...\n");

            sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, i + 1, &iRows, &iCols, NULL, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            int *piLen = (int*)malloc(sizeof(int) * iRows * iCols);
            sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, i + 1, &iRows, &iCols, piLen, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            char **pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
            for(int j = 0 ; j < iRows * iCols ; j++)
            {
                pstData[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));//+ 1 for null termination
            }

            sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, i + 1, &iRows, &iCols, piLen, pstData);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //sciprint("Item #%d : %s\n", i, pstData[0]);

            if(i==0)
            for(int j = 2 ; j < iRows * iCols ; j++)
            {
                //filling the list of param names
                namesList.append(pstData[j]);
                //sciprint("Item #%d:%d : %s\n", i, j, pstData[j]);
            }

            if(i>=2)
            {
                valuesList.append(pstData[0]);
            }
        }

        for (int i = 0; i < iItemNumber - 2; i++)
        {
            map->insert(namesList[i], valuesList[i]);
        }

        return 0;
}

int sciGetIntInList(char *fname, int *piListAddress, int iPos, int *piResult)
{
    SciErr sciErr;
    int iRows           = 0;
    int iCols           = 0;

    double* pdbl   = NULL;

    sciErr = getMatrixOfDoubleInList(pvApiCtx, piListAddress, iPos, &iRows, &iCols, &pdbl);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    *piResult = (int)pdbl[0];

    return 0;
}

int sciGetStringInList(char *fname, int *piListAddress, int iPos, char **psResult)
{
    int *piAddress, *piLen;
    int iRows, iCols;
    char **pstData;
    SciErr sciErr;

    getListItemAddress(pvApiCtx, piListAddress, iPos, &piAddress);

    sciErr = getMatrixOfStringInList(pvApiCtx, piListAddress, iPos, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    sciErr = getMatrixOfStringInList(pvApiCtx, piListAddress, iPos, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }

    sciErr = getMatrixOfStringInList(pvApiCtx, piListAddress, iPos, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    *psResult = pstData[0];
}

int sciGetStringAt(char *fname, int iPos, char **ppcResult)
{
    SciErr sciErr;
    int i;
    int iRows       = 0;
    int iCols       = 0;
    int* piLen      = NULL;
    char **pstData = NULL;
    int *piPos;

    sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piPos);

    sciErr = getMatrixOfString(pvApiCtx, piPos, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    sciErr = getMatrixOfString(pvApiCtx, piPos, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }

    sciErr = getMatrixOfString(pvApiCtx, piPos, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    *ppcResult = pstData[0];

    return 0;
}
int sciGetQSqlQueryAt(char *fname, int iPos, QSqlQuery **ppSqlQuery)
{
    SciErr sciErr;

        int m1 = 0, n1 = 0;
        int *piAddr = NULL;
        char *pStVarOne = NULL;
        int lenStVarOne = 0;
        int iType1 = 0;
        void *pvPtr;

        sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddr, &iType1);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }

        if ( iType1 != sci_pointer )
        {
          Scierror(999,"%s: Wrong type for input argument #%d: A pointer expected.\n", fname, 1);
          return 1;
        }

        sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);

        *ppSqlQuery = (QSqlQuery*)pvPtr;

        return 0;
}

int transposeDoubleMatrix(double *pdMatr, int iRows, int iCols, double **ppdRes)
{
    *ppdRes = (double*)malloc( sizeof(double) * iRows * iCols);

    for(int i=0; i < iRows; i++)
    {
        for(int j=0; j < iCols; j++)
        {
            (*ppdRes)[i*iCols + j] = pdMatr[j*iRows + i];
        }
    }

    return 0;
}

int transposeStringMatrix(char **pdMatr, int iRows, int iCols, char ***ppdRes)
{
    *ppdRes = (char**)malloc( sizeof(char*) * iRows * iCols);

    for(int i=0; i < iRows; i++)
    {
        for(int j=0; j < iCols; j++)
        {
            (*ppdRes)[i*iCols + j] = (char*)malloc(strlen(pdMatr[j*iRows + i]));
            strcpy((*ppdRes)[i*iCols + j], pdMatr[j*iRows + i]);
        }
    }

    return 0;
}
int getConnectionStringMembers(QString sConnectionString, QMap<QString, QString> **mConnectionStringMembers)
{
    try
    {
        QStringList slCSparts = sConnectionString.split(';', QString::SkipEmptyParts);
        *mConnectionStringMembers = new QMap<QString, QString>();

        for(int i=0; i<slCSparts.size(); i++)
        {
            QStringList slKeyValue = slCSparts.at(i).split('=');

            QString key = slKeyValue.at(0);
            while (key.at(0) == ' ')
            {
                key.remove(0,1);
            }
            while (key.at(key.length() - 1) == ' ')
            {
                key.remove(key.length() - 1,1);
            }

            QString value = slKeyValue.at(1);
            while (value.at(0) == ' ')
            {
                value.remove(0,1);
            }
            while (value.at(value.length() - 1) == ' ')
            {
                value.remove(value.length() - 1,1);
            }

            (**mConnectionStringMembers).insert(key, slKeyValue.at(1));
        }

        return 0;
    }
    catch(int ex)
    {
        return ex;
    }
}

int sciGetValuesMapFromStruct(char *fname, int *piListPos, QMap<QString, QVariant> *mValues)
{
    int iNumberOfItems;
    SciErr sciErr;
    int iCols, iRows;
    char **pstData;
    int *piAddr;
    int *piLen;

    sciErr = getListItemNumber(pvApiCtx, piListPos, &iNumberOfItems);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        sciprint("This variable is not a list");
        return 0;
    }

    getListItemAddress(pvApiCtx, piListPos, 1, &piAddr);


    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    for (int i = 3; i <= iNumberOfItems; i++)
    {
        int iIntItem;;
        char *sStringItem;
        int iType;
        int *piListItemAddress;

        getListItemAddress(pvApiCtx, piListPos, i, &piListItemAddress);

        sciErr = getVarType(pvApiCtx, piListItemAddress, &iType);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        switch(iType)
        {
            case sci_matrix :
                sciGetIntInList(fname, piListPos, i, &iIntItem);
                mValues->insert(pstData[i-1], QVariant(iIntItem));
                break;

            case sci_strings :
                sciGetStringInList(fname, piListPos, i, &sStringItem);
                mValues->insert(pstData[i-1], QVariant(sStringItem));
                break;
        }
    }
}
