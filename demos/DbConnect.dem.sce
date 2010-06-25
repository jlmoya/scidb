// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

mode(-1);
lines(0);

disp("conn = struct ('provider', 'QPSQL', 'user', 'postgres')");
conn = struct('provider', 'QPSQL', 'user', 'postgres')
disp("DbConnect(conn)");
DbConnect(conn)