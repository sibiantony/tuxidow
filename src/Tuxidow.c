/****************************************************************
 
FILE : TUXIDOW.C
DETAILS : APPLICATION IS INITIALISED , CHECKS CURRENT WINDOWS VERSION
CONTACT : tuxidow@yahoo.com

*****************************************************************/


#include "Tuxidow.h"

/******************************************************************************

   WinMain

******************************************************************************/

int PASCAL WinMain(  HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
	MSG  msg;
	HWND hwndMain ;

	if(!(DEBUGPTR = fopen("DebugReport.txt","w")))
		if(!(DEBUGPTR = fopen("C:\\DebugReport.txt","w")))
			PrintError("Could not create DebugReport file\n");

	DebugReport(0,"----------Debug Started----------\n\n");
	if(!FindCurrentOS ())
		PrintError("Incompatible Windows Version.    \n\n    Designed for 2000/XP.  \n"); 
	CheckHardDisks();
	DebugReport(SEPARATOR,"Checking Linux Partitions...\n");
	FindExt2FSDrives();

	if(!hPrevInstance)
		if(!InitApplication(hInstance))
			return FALSE;
	
	/* Perform initializations that apply to a specific instance */
	if (!(hwndMain = InitInstance(hInstance, nCmdShow)))
		return FALSE;
	/* Acquire and dispatch messages until a WM_QUIT uMessage is received. */
	DebugReport(SEPARATOR,"Entering Message Loop\n");
	while(GetMessage( &msg, NULL, 0x00, 0x00 ))
	{
		if (!TranslateAccelerator( hwndMain,// handle to receiving window 
						hAccTable,			// handle to active accelerator table 
						&msg))				// message data 
		{ 
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		} 

	}
	return (int)msg.wParam;
}

void PrintError(char *msg)
{
	MessageBox(NULL , msg , "Error" , MB_OK | MB_ICONEXCLAMATION ) ;
	exit(0);
}

//Find the Operating system version.
//At present this only works in Windows 2000/XP
BOOL FindCurrentOS()
{

	DebugReport(0,"Checking OS version..\n");
	//Microsoft Windows Server 2003 family
	if ( _winmajor == 5 && _winminor == 2 ) 
	{	
		DebugReport(0,"Microsoft Windows Server 2003 family\n");
		DebugReport(0,"_winmajor = %d,_winminor = %d\n",_winmajor,_winminor);
		return TRUE;
	}

	//Microsoft Windows XP 
	if ( _winmajor == 5 && _winminor == 1 )
	{	
		DebugReport(0,"Microsoft Windows XP\n");
		DebugReport(0,"_winmajor = %d,_winminor = %d\n",_winmajor,_winminor);
		return TRUE;
	}

	//Microsoft Windows 2000 
    if ( _winmajor == 5 && _winminor== 0 )
		
	{
		DebugReport(0,"Microsoft Windows 2000\n");
		DebugReport(0,"_winmajor = %d,_winminor = %d\n",_winmajor,_winminor);
		return TRUE;
	}

	if( _winmajor > 5) 
	{	
		DebugReport(0,"Microsoft Windows LongHorn\n");
		DebugReport(0,"_winmajor = %d,_winminor = %d\n",_winmajor,_winminor);
		return TRUE;
	}

	DebugReport(0,"_winmajor = %d,_winminor = %d",_winmajor,_winminor);
	DebugReport(0,"Incompatible Operating system Version..\n");
	return FALSE;
             
}


void DebugReport(int separator, const char *format,...)
{  
	va_list argptr;	

	if(DEBUG_TUXIDOW)
	{
		if(separator)
			fprintf(DEBUGPTR,"%s","____________________________________________\n");
    	
		va_start(argptr, format);
		vfprintf(DEBUGPTR,format,argptr);
    	va_end(argptr);

	}
}

