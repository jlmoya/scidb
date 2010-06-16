// ====================================================================
// Allan CORNET
// DIGITEO 2008
// This file is released into the public domain
// ====================================================================
//ilib_verbose(2);

tbx_build_gateway('skeleton_cpp', ['DbConnect', 'sci_DbConnect';'DbDisconnect', 'sci_DbDisconnect'; 'DbQuery', 'sci_DbQuery'; 'DbLastError', 'sci_DbLastError';'DbTest', 'sci_DbTest';
  ], ['sci_db.cxx', 'sci_util.cpp', 'sci_util.h', 'sci_test.cpp'], ..
                  get_absolute_file_path('builder_gateway_cpp.sce'), ..
			['QtCore4', 'QtSql4', 'qsqlpsql4', 'qsqlite4']);

clear tbx_build_gateway;
