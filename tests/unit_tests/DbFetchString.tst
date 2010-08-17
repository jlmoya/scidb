// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

connStr = struct ("provider", "SQLITE", ..
                   "database", "reals" );

conn    = DbConnect(connStr);
query   = DbQuery("SELECT * FROM reals");
strs    = DbFetchString(query);

if strs(1) <> "1.1" ..
 | strs(2) <> "2.2" then pause,end
