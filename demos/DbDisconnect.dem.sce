// ====================================================================
// Copyright DIGITEO 2008
// Igor GRIDCHYN
// This file is released into the public domain
// ====================================================================
mode(-1);
lines(0);

disp('conn = struct (''provider'', ''QSQLITE'', ''database'', ''addressbook'')');
conn = struct('provider', 'QSQLITE', 'database', 'addressbook')
disp('Connecting to a database:');
disp('DbConnect(conn)');
DbConnect(conn)
disp('resultHandler = DbQuery(''SELECT * FROM addressbook'')');
resultHandler = DbQuery('SELECT * FROM addressbook');
disp('DbDisconnect()');
DbDisconnect()

// ====================================================================
