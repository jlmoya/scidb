// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

demopath = get_absolute_file_path("scidb.dem.gateway.sce");

subdemolist = [ "demo DbConnect"            , "DbConnect.dem.sce"           ;
                "demo DbDisconnect"         , "DbDisconnect.dem.sce"        ;
                "demo DbQuery"              , "DbQuery.dem.sce"             ;
                "demo DbFetchReal"          , "DbFetchReal.dem.sce"         ;
                "demo DbFetchAllReal"       , "DbFetchAllReal.dem.sce"      ;
                "demo DbFetchAllString"     , "DbFetchAllString.dem.sce"    ;
                "demo DbFetchString"        , "DbFetchString.dem.sce"       ;
                "demo DbFetchStruct"        , "DbFetchStruct.dem.sce"       ;
                "demo DbFetchAllStruct"     , "DbFetchAllStruct.dem.sce"    ;
                "demo DbFirst"              , "DbFirst.dem.sce"             ;
                "demo DbLastDbError"        , "DbLastDbError.dem.sce"       ;
                "demo DbAffectedRowsNumber" , "DbAffectedRowsNumber.dem.sce";
                "demo DbTables"             , "DbTables.dem.sce"            ];

subdemolist(:,2) = demopath + subdemolist(:,2);
