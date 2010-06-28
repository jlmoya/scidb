// ====================================================================
// Copyright DIGITEO 2008
// Igor GRIDCHYN
// This file is released into the public domain
// ====================================================================
mode(-1);
lines(0);

disp('connStr = struct (''provider'', ''QSQLITE'', ''database'', ''addressbook'')');
connStr = struct ('provider', 'QSQLITE', 'database', 'addressbook')
disp('Connecting to a database:');
disp('DbConnect(connStr)');
DbConnect(connStr)
disp('resultHandler = DbQuery(''SELECT * FROM addressbook'')');
resultHandler = DbQuery('SELECT * FROM addressbook');

disp('recordStrings = DbFetchString(resultHandler)');
recordStrings = DbFetchString(resultHandler);

disp('DbDisconnect()');
DbDisconnect()

// ====================================================================