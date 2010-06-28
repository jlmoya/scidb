// ====================================================================
// Copyright DIGITEO 2008
// Igor GRIDCHYN
// This file is released into the public domain
// ====================================================================
mode(-1);
lines(0);

disp('connStr = struct(''provider'', ''QODBC'', ''database'', ''DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=DbTest.mdb''');
connStr = struct('provider', 'QODBC', 'database', 'DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=Dbt.mdb')
disp('Connecting to a database:');
disp('DbConnect(conn)');
DbConnect(connStr)
disp('resultHandler = DbQuery(''SELECT * FROM addressbook'')');
resultHandler = DbQuery('SELECT * FROM addressbook');

disp('allReals = DbFetchAllReal(resultHandler)');
allReals = DbFetchAllReal(resultHandler)

disp('DbDisconnect()');
DbDisconnect()

// ====================================================================