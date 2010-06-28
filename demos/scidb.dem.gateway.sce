// ====================================================================
// Copyright INRIA 2008
// Igor GRIDCHYN
// This file is released into the public domain
// ====================================================================
demopath = get_absolute_file_path("scidb.dem.gateway.sce");

subdemolist = [	"demo DbConnect"              ,"DbConnect.dem.sce" ; 
			"demo DbDisconnect"		,"DbDisconnect.dem.sce";
			"demo DbQuery"			,"DbQuery.dem.sce";
			"demo DbFetchReal"		,"DbFetchReal.dem.sce";
			"demo DbFetchAllReal"		, "DbFetchAllReal.dem.sce";
			"demo DbFetchAllString"		, "DbFetchAllString.dem.sce";
			"demo DbFetchString"		, "DbFetchString.dem.sce";
			"demo DbFetchStruct"		, "DbFetchStruct.dem.sce";
			"demo DbFetchAllStruct"		, "DbFetchAllStruct.dem.sce";];

subdemolist(:,2) = demopath + subdemolist(:,2);
// ====================================================================
