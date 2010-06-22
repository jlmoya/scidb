// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

ilib_verbose(2);

scidb_names =  ["DbConnect"           , "sci_DbConnect"            ;
                "DbDisconnect"        , "sci_DbDisconnect"         ;
                "DbQuery"             , "sci_DbQuery"              ;
                "DbLastError"         , "sci_DbLastError"          ;
                "DbTest"              , "sci_DbTest"               ;
                "DbInitDatabaseModule", "sci_DbInitDatabaseModule" ;
                "DbFetchString"       , "sci_DbFetchString"        ;
                "DbFetchAllString"    , "sci_DbFetchAllString"     ;
                "DbFetchReal"         , "sci_DbFetchReal"          ;
                "DbFetchAllReal"      , "sci_DbFetchAllReal"       ;
                "DbFetchStruct"       , "sci_DbFetchStruct"        ];

scidb_files =  ["sci_db.cxx"    ;
                "sci_util.cpp"  ;
                "sci_util.h"    ;
                "sci_test.cpp"  ];

scidb_gateway_path = get_absolute_file_path("builder_gateway_cpp.sce");

QT_libs = ["../../Qt/lib/QtCore4"  ;
           "../../Qt/lib/QtSql4"   ;
           "../../Qt/lib/qsqlpsql4";
           "../../Qt/lib/qsqlite4"];

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
