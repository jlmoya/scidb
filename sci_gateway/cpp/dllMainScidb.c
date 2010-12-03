/*
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*/
/*--------------------------------------------------------------------------*/ 
#include <stdio.h> 
#include <windows.h> 
/*--------------------------------------------------------------------------*/ 

#ifdef _WIN64
#pragma comment(lib, "../../Qt/lib/windows64/libpq.lib")
#pragma comment(lib, "../../Qt/lib/windows64/sqlite.lib")
#pragma comment(lib, "../../Qt/lib/windows64/sqlite3.lib")
#pragma comment(lib, "../../Qt/lib/windows64/QtCore4.lib")
#pragma comment(lib, "../../Qt/lib/windows64/QtSql4.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqlpsql4.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqlite4.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqlodbc4.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqlsite24.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqldb24.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqloci4.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqlibase4.lib")
#pragma comment(lib, "../../Qt/lib/windows64/qsqlmysql4.lib")
#else
#pragma comment(lib, "../../Qt/lib/windows32/libpq.lib")
#pragma comment(lib, "../../Qt/lib/windows32/sqlite.lib")
#pragma comment(lib, "../../Qt/lib/windows32/sqlite3.lib")
#pragma comment(lib, "../../Qt/lib/windows32/QtCore4.lib")
#pragma comment(lib, "../../Qt/lib/windows32/QtSql4.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqlpsql4.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqlite4.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqlodbc4.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqlite24.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqldb24.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqloci4.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqlibase4.lib")
#pragma comment(lib, "../../Qt/lib/windows32/qsqlmysql4.lib")
#endif


/*--------------------------------------------------------------------------*/ 
int WINAPI DllMain (HINSTANCE hInstance , DWORD reason, PVOID pvReserved)
{
	switch (reason) 
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return 1;
}
/*--------------------------------------------------------------------------*/ 
