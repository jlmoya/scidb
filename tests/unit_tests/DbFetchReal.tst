// This file is released into the public domain
//=================================
// load toolbox_skeleton
if funptr('DbFetchReal') == 0 then
  root_tlbx_path = SCI+'\contrib\scidb\';
  exec(root_tlbx_path + 'loader.sce',-1); 
end
//=================================
connStr = struct ('provider', 'SQLITE', 'database', 'reals')
conn=DbConnect(connStr)
qr=DbQuery("SELECT * FROM reals")
real = DbFetchReal(qr)
if real(1) <> 1.1 | real(2) <> 2.2 then pause,end
//=================================