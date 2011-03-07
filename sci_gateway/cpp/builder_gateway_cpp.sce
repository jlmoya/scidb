// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//ilib_verbose(2);

WINDOWS  = (strcmpi(getos(),"windows") == 0);
MACOSX   = (strcmpi(getos(),"darwin" ) == 0);
LINUX    = (strcmpi(getos(),"linux"  ) == 0);


scidb_names =  ["DbConnect"           , "sci_DbConnect"            ;
                "DbDisconnect"        , "sci_DbDisconnect"         ;
                "DbQuery"             , "sci_DbQuery"              ;
                "DbLastDbError"       , "sci_DbLastDbError"        ;
                "DbInitDatabaseModule", "sci_DbInitDatabaseModule" ;
                "DbFetchString"       , "sci_DbFetchString"        ;
                "DbFetchAllString"    , "sci_DbFetchAllString"     ;
                "DbFetchReal"         , "sci_DbFetchReal"          ;
                "DbFetchAllReal"      , "sci_DbFetchAllReal"       ;
                "DbFetchStruct"       , "sci_DbFetchStruct"        ;
                "DbFetchAllStruct"    , "sci_DbFetchAllStruct"     ;
                "DbTables"            , "sci_DbTables"             ;
                "DbAffectedRowsNumber", "sci_DbAffectedRowsNumber" ;
                "DbLastQueryError"    , "sci_DbLastQueryError"     ;
                "DbFirst"             , "sci_DbFirst"              ;
                "DbLastInsertId"      , "sci_DbLastInsertId"       ;
                "DbFuzzyCreateFMB"    , "sci_DbFuzzyCreateFMB"     ;
                "DbFuzzyLoadFMB"      , "sci_DbFuzzyLoadFMB"       ;
                "DbFuzzyCreate"       , "sci_DbFuzzyCreate"        ;
                "DbFuzzyGet"          , "sci_DbFuzzyGet"           ;
                "DbFuzzyUpdate"       , "sci_DbFuzzyUpdate"        ;
                "DbFuzzyDelete"       , "sci_DbFuzzyDelete"        ;
                "DbFuzzyQuery"        , "sci_DbFuzzyQuery"         ;
                "DbFuzzyLastError"    , "sci_DbFuzzyLastError"     ;
                "DbFuzzyHasFMB"       , "sci_DbFuzzyHasFMB"        ];

scidb_files =  ["sci_db.cxx"                  ;
                "sci_util.cpp"                ;
                "sci_DbConnect.cxx"           ;
                "sci_DbDisconnect.cxx"        ;
                "sci_DbQuery.cxx"             ;
                "sci_DbFetchReal.cxx"         ;
                "sci_DbFetchString.cxx"       ;
                "sci_DbFetchStruct.cxx"       ;
                "sci_DbFetchAllString.cxx"    ;
                "sci_DbFetchAllReal.cxx"      ;
                "sci_DbFetchAllStruct.cxx"    ;
                "sci_DbLastDbError.cxx"       ;
                "sci_DbInitDatabaseModule.cxx";
                "sci_DbTables.cxx"            ;
                "sci_DbAffectedRowsNumber.cxx";
                "sci_DbLastQueryError.cxx"    ;
                "sci_DbLastInsertId.cxx"      ;
                "sci_DbFirst.cxx"             ;
                "sci_db.h"                    ;
                "sci_util.h"                  ;
                "sci_DbFuzzyCreateFMB.cpp"        ;
                "sci_DbFuzzyLoadFMB.cpp"          ;
                "sci_DbFuzzyCreate.cpp"           ;
                "sci_DbFuzzyGet.cpp"              ;
                "sci_DbFuzzyUpdate.cpp"           ;
                "sci_DbFuzzyDelete.cpp"           ;
                "sci_DbFuzzyQuery.cpp"            ;
                "sci_DbFuzzyHasFMB.cpp"           ;
                "sci_DbFuzzyLastError.cpp"        ;
                "fuzzysql.cpp"                    ;
                "fuzzysql.h"                      ;
                "FMB.cpp"                         ;
                "FMB.h"                           ;
                "FuzzyApproxMuch.cpp"             ;
                "FuzzyApproxMuch.h"               ;
                "FuzzyCol.cpp"                    ;
                "FuzzyCol.h"                      ;
                "FuzzyCompatibleCol.cpp"          ;
                "FuzzyCompatibleCol.h"            ;
                "FuzzyConstant.cpp"               ;
                "FuzzyConstant.h"                 ;
                "FuzzyDegreeCol.cpp"              ;
                "FuzzyDegreeCol.h"                ;
                "FuzzyDegreeSig.cpp"              ;
                "FuzzyDegreeSig.h"                ;
                "FuzzyDegreeTable.cpp"            ;
                "FuzzyDegreeTable.h"              ;
                "FuzzyLabel.cpp"                  ;
                "FuzzyLabel.h"                    ;
                "FuzzyNearness.cpp"               ;
                "FuzzyNearness.h"                 ;
                "FuzzyObject.cpp"                 ;
                "FuzzyObject.h"                   ;
                "FuzzyQualifier.cpp"              ;
                "FuzzyQualifier.h"                ;
                "FuzzySystemQuantifier.cpp"       ;
                "FuzzySystemQuantifier.h"         ;
                "FuzzyTableQuantifier.cpp"        ;
                "FuzzyTableQuantifier.h"          ;
                "FuzzyTableInfo.cpp"              ;
                "FuzzyTableInfo.h"                ;
                "TreeTokenNode.cpp"               ;
                "TreeTokenNode.h"                 ;];

if WINDOWS then
  scidb_files = [scidb_files; "dllMainScidb.c"];
end
                

scidb_gateway_path = get_absolute_file_path("builder_gateway_cpp.sce");

// Guess the platform

[dynamic_info,static_info] = getdebuginfo();
arch_info = static_info(grep(static_info,"/^Compiler Architecture:/","r"))

if ~isempty(arch_info) & (regexp(arch_info,"/\sX64$/","o") <> []) then
    ARCH = "64";
else
    ARCH = "32";
end

OSNAME = convstr(getos(),"l");


if ~WINDOWS then

  QT_libs = ["../../Qt/lib/linux"+ARCH+"/libpq"  ;
              "../../Qt/lib/linux"+ARCH+"/libQtCLucene"   ;
              "../../Qt/lib/linux"+ARCH+"/libQtCore";
              "../../Qt/lib/linux"+ARCH+"/libQtSql" ;
              "../../Qt/lib/linux"+ARCH+"/libsqlite3";
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlite"  ;
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlmysql";
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlpsql" ;
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlite"  ;
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqldb2"  ;
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqloci"  ;
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlibase";
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlmysql"];
else
  QT_libs = ["../../src/cpp/libfuzzysql"];
end

if getos() == "Windows" then
    pathQtInclude = fullpath(get_absolute_file_path("builder_gateway_cpp.sce")+ "../../Qt/include");
    pathQtCoreInclude = fullpath(get_absolute_file_path("builder_gateway_cpp.sce")+ "../../Qt/include/QtCore");
    pathQtSqlInclude = fullpath(get_absolute_file_path("builder_gateway_cpp.sce")+ "../../Qt/include/QtSql");
	pathFuzzyInclude = fullpath(get_absolute_file_path("builder_gateway_cpp.sce")+ "../../src/cpp");
    QT_includes = "-I""" + pathQtInclude + """ -I""" + pathQtCoreInclude + """ -I""" + pathQtSqlInclude + """ -I""" + pathFuzzyInclude + """ ";
else
    QT_includes = "-I" + get_absolute_file_path("builder_gateway_cpp.sce") + "../../Qt/include";
end

tbx_build_gateway("scidb",            ..
                  scidb_names,        ..
                  scidb_files ,       ..
                  scidb_gateway_path, ..
                  QT_libs,            ..
                  "",                 ..
                  QT_includes);

clear tbx_build_gateway;
