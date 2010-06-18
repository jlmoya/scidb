#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc sci_DbConnect;
extern Gatefunc sci_DbDisconnect;
extern Gatefunc sci_DbQuery;
extern Gatefunc sci_DbLastError;
extern Gatefunc sci_DbTest;
extern Gatefunc sci_DbInitDatabaseModule;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,sci_DbConnect,"DbConnect"},
  {(Myinterfun)sci_gateway,sci_DbDisconnect,"DbDisconnect"},
  {(Myinterfun)sci_gateway,sci_DbQuery,"DbQuery"},
  {(Myinterfun)sci_gateway,sci_DbLastError,"DbLastError"},
  {(Myinterfun)sci_gateway,sci_DbTest,"DbTest"},
  {(Myinterfun)sci_gateway,sci_DbInitDatabaseModule,"DbInitDatabaseModule"},
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
