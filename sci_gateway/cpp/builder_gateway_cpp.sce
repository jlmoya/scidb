// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//ilib_verbose(2);

scidb_names =  ["DbConnect"    , "sci_DbConnect"    ;
                "DbDisconnect" , "sci_DbDisconnect" ;
                "DbQuery"      , "sci_DbQuery"      ;
                "DbLastError"  , "sci_DbLastError"  ;
                "DbTest"       , "sci_DbTest"       ];

scidb_files =  ["sci_db.cxx"    ;
                "sci_util.cpp"  ;
                "sci_util.h"    ;
                "sci_test.cpp"  ];

scidb_gateway_path = get_absolute_file_path("builder_gateway_cpp.sce");

scidb_libs = ["QtCore4"  ;
              "QtSql4"   ;
              "qsqlpsql4";
              "qsqlite4"];

tbx_build_gateway("scidb",           ..
                  scidb_names,       ..
                  scidb_files,       ..
                  scidb_gateway_path,..
                  scidb_libs);

clear tbx_build_gateway;
