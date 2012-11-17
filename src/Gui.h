/****************************************************************
 
FILE : GUI.H
DETAILS : IMPLEMENTS ALL GUI FUNCTIONS
CONTACT : tuxidow@yahoo.com

*****************************************************************/

#include "resource.h"
#include "Tuxidow.h"
#include "guimessages.h"
#include "partit.h"
#include "Ext2FS.h"


#ifndef WIN32

#define GET_WM_COMMAND_ID(wp, lp)               (wp)
#define GET_WM_COMMAND_HWND(wp, lp)             (HWND)(LOWORD(lp))
#define GET_WM_COMMAND_CMD(wp, lp)              HIWORD(lp)
#endif

//Functions to Initialize the App.
BOOL InitApplication(HINSTANCE);
HWND InitInstance(HINSTANCE, int);

//Functions for ListView Contnrol.
BOOL InitListView(HWND);
HWND CreateListView(HINSTANCE, HWND);
BOOL InsertListViewItems(HWND);
void ResizeListView(HWND, HWND);
void SwitchView(HWND, DWORD);
void UpdateListViewMenu(HWND, HMENU);

//Functions for Rebar control .
HWND BuildRebar(HWND);
HWND CreateToolBarBand(HWND);
void CreateAddressBarBand(HWND);
void MoveRebar(HWND);

//Functions for Right click menus.
BOOL DoListViewMenu(HWND ,WPARAM ,LPARAM );
BOOL DoFileSelectionMenu( HWND , WPARAM ,LPARAM );
void DoMenuFileView(HWND , HWND);
void DoMenuFileCopy(HWND , HWND);
void DoMenuFileProperties(HWND , HWND);

//Functions for tab control in about dialog
VOID WINAPI OnSelChanged(HWND) ;
DLGTEMPLATE * WINAPI DoLockDlgRes(LPCSTR);
VOID WINAPI OnTabbedDialogInit(HWND);

//Other function prototypes .
int	 find_fil_typ(char *);
void DisplayPartitionInfo(HWND);
void DisplayPartitionIcons(HWND);
void ChooseTextFont(HWND);
void ClearPropertiesText();
void ShowCopyProgress(HWND , char *);



#define ID_LISTVIEW  1000
#define ID_BUTTON    2000
#define ID_COMBOBOX  2001
#define ID_REBAR     3000
#define ID_TOOLBAR   4000
#define BMP_CX	     32
#define BMP_CY	     32
#define MIN_TBCX	 200
#define MIN_CY		 38
#define NUM_TBBITMAPS	7
#define NUM_TBBUTTONS	7
#define LISTWINDOW_X	160
#define LISTWINDOW_Y	69
#define LINUX_DRIVE 8

#define PROPERTIES_LEFT   10
#define PROPERTIES_TOP    LISTWINDOW_Y + 160
#define PROPERTIES_RIGHT  150
#define PROPERTIES_BOTTOM LISTWINDOW_Y + 310

#define ITEM_COUNT (num_files - 1)
#define PROGRESS_RANGE  15
#define C_PAGES		2



/**************************************************************************
   Global Variables
**************************************************************************/

HANDLE   g_hInst;
HWND	 hWndToolBar;
HWND     hwndAddress;
HWND	 hProgressDlg;
HBITMAP	 g_hbitmap ;
HBITMAP	 g_hbitmapTuxidow ;
HDC		 hdcPropertiesText;
HACCEL	 hAccTable;


TCHAR    g_szClassName[] ;
DWORD	 CURRENT_VIEW;
char	 *filetype[9] ;

WORD	 isListFileChosen ;
DWORD    chosenFileIndex  ;
DWORD	 displaystack[100] ;
int		 display_maxindex , display_current_index ;
int		 images[10];
char	 error[11];


