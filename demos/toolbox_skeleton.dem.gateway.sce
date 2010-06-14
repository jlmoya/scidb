// ====================================================================
// Copyright INRIA 2008
// Igor GRIDCHYN
// This file is released into the public domain
// ====================================================================
demopath = get_absolute_file_path("toolbox_skeleton.dem.gateway.sce");

subdemolist = ["demo DbConnect"               ,"DbConnect.dem.sce" ; ];

subdemolist(:,2) = demopath + subdemolist(:,2);
// ====================================================================
