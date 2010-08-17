// This file is released into the public domain
//=================================
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