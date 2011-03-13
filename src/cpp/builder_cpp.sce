// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

WINDOWS  = (strcmpi(getos(),"windows") == 0);
MACOSX   = (strcmpi(getos(),"darwin" ) == 0);
LINUX    = (strcmpi(getos(),"linux"  ) == 0);

src_cpp_path = get_absolute_file_path('builder_cpp.sce');

// Guess the platform

[dynamic_info,static_info] = getdebuginfo();
arch_info = static_info(grep(static_info,"/^Compiler Architecture:/","r"))

if ~isempty(arch_info) & (regexp(arch_info,"/\sX64$/","o") <> []) then
    ARCH = "64";
else
    ARCH = "32";
end

scidb_cpp_files =  ["fuzzysql.cpp"                    ;
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

if getos() == "Windows" then
    pathQtInclude = fullpath(get_absolute_file_path("builder_cpp.sce")+ "../../Qt/include");
    pathQtCoreInclude = fullpath(get_absolute_file_path("builder_cpp.sce")+ "../../Qt/include/QtCore");
    pathQtSqlInclude = fullpath(get_absolute_file_path("builder_cpp.sce")+ "../../Qt/include/QtSql");
    QT_includes = "-I""" + pathQtInclude + """ -I""" + pathQtCoreInclude + """ -I""" + pathQtSqlInclude + """ ";
else
    QT_includes = "-I" + get_absolute_file_path("builder_cpp.sce") + "../../Qt/include";
end				
				
if ~WINDOWS then

  QT_libs = ["../../Qt/lib/linux"+ARCH+"/libpq"  ;
              "../../Qt/lib/linux"+ARCH+"/libQtCLucene"   ;
              "../../Qt/lib/linux"+ARCH+"/libQtCore";
              "../../Qt/lib/linux"+ARCH+"/libQtSql" ;
              "../../Qt/lib/linux"+ARCH+"/libsqlite3";
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlite"  ;
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlmysql";
              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlpsql" ;
//              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqldb2"  ;
//              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqloci"  ;
//              "../../Qt/lib/linux"+ARCH+"/sqldrivers/libqsqlibase";
	    ];
else
  //TODO: add other libraries later if they will be needed
  QT_libs = ["../../Qt/lib/windows32/QtCore4";
			"../../Qt/lib/windows32/QtSql4"];
end
				
tbx_build_src(['fuzzysql'], scidb_cpp_files, 'c', ..
              src_cpp_path, QT_libs, '', QT_includes);

clear tbx_build_src;
clear src_c_path;
