// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

connStr2 = struct ("provider", "QPSQL", ..
                   "user", "postgres" , ..
                   "database", "test" );

conn2    = DbConnect(connStr2);
qr2      = DbQuery("SELECT * FROM dtest");
strs     = DbFetchString(qr2);

if strs(1) <> "3.2" ..
 | strs(2) <> "4.3" then pause,end
