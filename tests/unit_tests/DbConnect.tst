// Copyright (C) 2010 - Igor GRIDCHYN
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// load toolbox_skeleton
if funptr('DbConnect') == 0 then
  root_tlbx_path = SCI+'\contrib\scidb\';
  exec(root_tlbx_path + 'loader.sce',-1); 
end
//=================================
connStr = struct ('provider', 'SQLITE', 'database', 'reals')
conn=DbConnect(connStr)
if DbLastDbError() <> "" then pause,end
//=================================