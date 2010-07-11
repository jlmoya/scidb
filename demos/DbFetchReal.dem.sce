// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

chdir(get_absolute_file_path("DbFetchReal.dem.sce"));

disp("connStr = struct(""provider"", ""QODBC"", ""database"", ""DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=Dbt.mdb""");
connStr = struct("provider", "QODBC", "database", "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=Dbt.mdb")
disp("Connecting to a database:");
disp("DbConnect(conn)");
DbConnect(connStr)
disp("resultHandler = DbQuery(""SELECT * FROM test"")");
resultHandler = DbQuery("SELECT * FROM test");

disp("reals = DbFetchReal(resultHandler)");
reals = DbFetchReal(resultHandler)

disp(reals, "reals=");

disp("DbDisconnect()");
DbDisconnect()
