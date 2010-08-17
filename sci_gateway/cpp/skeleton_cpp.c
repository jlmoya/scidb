#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc sci_DbConnect;
extern Gatefunc sci_DbDisconnect;
extern Gatefunc sci_DbQuery;
extern Gatefunc sci_DbLastDbError;
extern Gatefunc sci_DbLastQueryError;
extern Gatefunc sci_DbTest;
extern Gatefunc sci_DbInitDatabaseModule;
extern Gatefunc sci_DbFetchString;
extern Gatefunc sci_DbFetchAllString;
extern Gatefunc sci_DbFetchReal;
extern Gatefunc sci_DbFetchAllReal;
extern Gatefunc sci_DbFetchStruct;
extern Gatefunc sci_DbFetchAllStruct;
extern Gatefunc sci_DbTables;
extern Gatefunc sci_DbAffectedRowsNumber;
extern Gatefunc sci_DbFirst;
extern Gatefunc sci_DbLastInsertId;

extern Gatefunc sci_DbFuzzyCreateFMB;
extern Gatefunc sci_DbFuzzyLoadFMB;
extern Gatefunc sci_DbFuzzyCreate;
extern Gatefunc sci_DbFuzzyGet;
extern Gatefunc sci_DbFuzzyUpdate;
extern Gatefunc sci_DbFuzzyDelete;
extern Gatefunc sci_DbFuzzyQuery;
extern Gatefunc sci_DbFuzzyHasFMB;
extern Gatefunc sci_DbFuzzyLastError;

static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,sci_DbConnect,"DbConnect"},
  {(Myinterfun)sci_gateway,sci_DbDisconnect,"DbDisconnect"},
  {(Myinterfun)sci_gateway,sci_DbQuery,"DbQuery"},
  {(Myinterfun)sci_gateway,sci_DbLastDbError,"DbLastDbError"},
  {(Myinterfun)sci_gateway,sci_DbTest,"DbTest"},
  {(Myinterfun)sci_gateway,sci_DbInitDatabaseModule,"DbInitDatabaseModule"},
  {(Myinterfun)sci_gateway,sci_DbFetchString,"DbFetchString"},
  {(Myinterfun)sci_gateway,sci_DbFetchAllString,"DbFetchAllString"},
  {(Myinterfun)sci_gateway,sci_DbFetchReal,"DbFetchReal"},
  {(Myinterfun)sci_gateway,sci_DbFetchAllReal,"DbFetchAllReal"},
  {(Myinterfun)sci_gateway,sci_DbFetchStruct,"DbFetchStruct"},
  {(Myinterfun)sci_gateway,sci_DbFetchAllStruct,"DbFetchAllStruct"},
  {(Myinterfun)sci_gateway,sci_DbTables,"DbTables"},
  {(Myinterfun)sci_gateway,sci_DbAffectedRowsNumber,"DbAffectedRowsNumber"},
  {(Myinterfun)sci_gateway,sci_DbLastQueryError,"DbLastQueryError"},
  {(Myinterfun)sci_gateway,sci_DbFirst,"DbFirst"},
  {(Myinterfun)sci_gateway,sci_DbLastInsertId,"DbLastInsertId"},

	{(Myinterfun)sci_gateway,sci_DbFuzzyCreateFMB,"DbFuzzyCreateFMB"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyLoadFMB,"DbFuzzyLoadFMB"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyHasFMB,"DbFuzzyHasFMB"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyCreate,"DbFuzzyCreate"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyGet,"DbFuzzyGet"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyUpdate,"DbFuzzyUpdate"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyDelete,"DbFuzzyDelete"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyQuery,"DbFuzzyQuery"},
	{(Myinterfun)sci_gateway,sci_DbFuzzyLastError,"DbFuzzyLastError"}

};
 
int C2F(skeleton_cpp)()
{
  Rhs = Max(0, Rhs);
  if (*(Tab[Fin-1].f) != NULL) 
  {
     pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  }
  return 0;
}
