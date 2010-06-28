// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//ilib_verbose(2);

scidb_names =  ["DbConnect"           , "sci_DbConnect"            ;
                "DbDisconnect"        , "sci_DbDisconnect"         ;
                "DbQuery"             , "sci_DbQuery"              ;
                "DbLastDbError"       , "sci_DbLastDbError"        ;
                "DbTest"              , "sci_DbTest"               ;
                "DbInitDatabaseModule", "sci_DbInitDatabaseModule" ;
                "DbFetchString"       , "sci_DbFetchString"        ;
                "DbFetchAllString"    , "sci_DbFetchAllString"     ;
                "DbFetchReal"         , "sci_DbFetchReal"          ;
                "DbFetchAllReal"      , "sci_DbFetchAllReal"       ;
                "DbFetchStruct"       , "sci_DbFetchStruct"        ;
                "DbFetchAllStruct"    , "sci_DbFetchAllStruct"     ;
                "DbTables"            , "sci_DbTables"             ;
                "DbAffectedRowsNumber", "sci_DbAffectedRowsNumber" ;
                "DbLastQueryError"    , "sci_DbLastQueryError"     ];


scidb_files =  ["sci_db.cxx"                  ;
                "sci_db.h"                    ;
                "sci_util.cpp"                ;
                "sci_util.h"                  ;
                "sci_test.cpp"                ;
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
                "sci_DbLastQueryError"        ];

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

WINDOWS  = (strcmpi(getos(),"windows") == 0);
MACOSX   = (strcmpi(getos(),"darwin" ) == 0);
LINUX    = (strcmpi(getos(),"linux"  ) == 0);

if WINDOWS then

    QT_libs = ["../../Qt/lib/windows"+ARCH+"/QtCore4"  ;
               "../../Qt/lib/windows"+ARCH+"/QtSql4"   ;
               "../../Qt/lib/windows"+ARCH+"/qsqlpsql4";
               "../../Qt/lib/windows"+ARCH+"/qsqlite4" ;
               "../../Qt/lib/windows"+ARCH+"/qsqlodbc4"];

elseif LINUX then

    QT_libs = ["../../Qt/lib/linux"+ARCH+"/libpq"  ;
               "../../Qt/lib/linux"+ARCH+"/libQtCLucene"   ;
               "../../Qt/lib/linux"+ARCH+"/libQtCore";
               "../../Qt/lib/linux"+ARCH+"/libQtSql" ;
               "../../Qt/lib/linux"+ARCH+"/libsqlite3";
               "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlite";
               "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlmysql" ;
               "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlpsql" ];

if getos() == "Windows" then
    QT_includes = "-I""" + get_absolute_file_path("builder_gateway_cpp.sce") + "../../Qt/include""";
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
