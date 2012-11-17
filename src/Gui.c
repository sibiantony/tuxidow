/****************************************************************
 
FILE : GUI.C
DETAILS : IMPLEMENTS ALL GUI FUNCTIONS
CONTACT : tuxidow@yahoo.com

*****************************************************************/

#include "gui.h" 

/**************************************************************************
   Local Function Prototypes
**************************************************************************/

extern TCHAR  g_szClassName[] = TEXT("VListVwClass");
extern char	  *filetype[]={	"Unknown",
							"File",
							"Folder",
							"Char_device",
							"Block_device",
							"Named Pipe",
							"Socket",
							"Symbolic Link",
							"Linux Partition"};

extern  int display_maxindex = 0,display_current_index = 0;

TBBUTTON tbButtons[] = {
	{ 0, IDS_BACK, TBSTATE_ENABLED , TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 0L, IDS_BACK},
	{ 0 , -1, TBSTATE_ENABLED, BTNS_SEP , 0L, -1},
	{ 1, IDS_FWD, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 0L, IDS_FWD},
	{ 0 , -1, TBSTATE_ENABLED, BTNS_SEP , 0L, -1},
	{ 2, IDS_UP, TBSTATE_ENABLED , TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 0L, IDS_UP},
	{ 0 , -1, TBSTATE_ENABLED, BTNS_SEP , 0L, -1},
	{ 3, IDS_HOME, TBSTATE_ENABLED , TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 0L, IDS_HOME}
	
};

typedef struct tag_dlghdr { 
		HWND hwndTab;       // tab control 
		HWND hwndDisplay;   // current child dialog box 
		RECT rcDisplay;     // display rectangle for the tab control 
		DLGTEMPLATE *apRes[C_PAGES]; 
} DLGHDR;

HFONT g_hfFont = NULL;
COLORREF g_rgbText = RGB(0, 0, 0);

/******************************************************************************

   InitApplication

******************************************************************************/

BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASSEX  wcex;
	ATOM        aReturn;

	DebugReport(SEPARATOR,"Initialising GUI..\n");

	wcex.cbSize          = sizeof(WNDCLASSEX);
	wcex.style           = CS_DBLCLKS;
	wcex.lpfnWndProc     = (WNDPROC)MainWndProc;
	wcex.cbClsExtra      = 0;
	wcex.cbWndExtra      = 0;
	wcex.hInstance       = hInstance;
	wcex.hCursor         = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName    = MAKEINTRESOURCE(IDM_MAIN_MENU);
	wcex.lpszClassName   = g_szClassName;
	wcex.hIcon           = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TUXIDOW));
	wcex.hIconSm		 = LoadImage(hInstance , MAKEINTRESOURCE(IDI_TUXIDOW), IMAGE_ICON, 16, 16, 0);
	aReturn = RegisterClassEx(&wcex);

	if(0 == aReturn)
	{  
		WNDCLASS wc;

		wc.style          = 0;
		wc.lpfnWndProc    = (WNDPROC)MainWndProc;
		wc.cbClsExtra     = 0;
		wc.cbWndExtra     = 0;
		wc.hInstance      = hInstance;
		wc.hIcon          = LoadIcon(hInstance , MAKEINTRESOURCE(IDI_TUXIDOW));
		wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName   = MAKEINTRESOURCE(IDM_MAIN_MENU);
		wc.lpszClassName  = g_szClassName;

		aReturn = RegisterClass(&wc);
	}
	if(!aReturn) DebugReport(0,"Could not Register Window class\n");
	return aReturn;
}

/******************************************************************************

   InitInstance

******************************************************************************/

HWND InitInstance(   HINSTANCE hInstance,
                     int nCmdShow)
{
	HWND     hWnd;
	INITCOMMONCONTROLSEX iccex;
	TCHAR    szTitle[MAX_PATH] = "";

	g_hInst = hInstance;
	displaystack[0] = 2;	//Initializing the displaystack with the first inode number.

	LoadString(g_hInst, IDS_APPTITLE, szTitle, sizeof(szTitle));

	/* Create a main window for this application instance.  */
	hWnd = CreateWindowEx(  0,
                        g_szClassName,
                        szTitle,
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

	/* If window could not be created, return "failure" */
	if (!hWnd)
		return FALSE;

	/* Make the window visible; update its client area; and return "success" */
	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	//required for common controls
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_COOL_CLASSES;
	if(! InitCommonControlsEx(&iccex) )
	{
		DebugReport(0,"Could not Initialise Common controls \n");
		return FALSE;
	}
	//Load the Accelerator Table.
	hAccTable = LoadAccelerators(g_hInst , MAKEINTRESOURCE(IDR_ACCELERATOR1));
	
	if(!(hWnd))
		DebugReport(0,"Could not Create Window..\n");
	return hWnd;
}

int find_fil_typ(char name[])
{
	int l,i=0,coun=0;
	char test[10];
	l = (int)strlen(name);
	i = l-1;
	while(name[i]!='.'&&coun<=4)
	{test[coun++]=name[i--];}test[coun]='\0';
	if(!(strcmp(test,"gpj")&&strcmp(test,"gnp")&&strcmp(test,"pmb")
			&&strcmp(test,"agt")&&strcmp(test,"oci")&&strcmp(test,"mpx")))
	return(7);
	else if(!strcmp(test,"vaw"))
	return(10);
	else if(!strcmp(test,"3pm"))
	return(20);
	else if(!strcmp(test,"iva")&&strcmp(test,"tad")&&strcmp(test,"vom"))
	return(8);
	else if(!(strcmp(test,"rar")&&(strcmp(test,"piz"))&&(strcmp(test,"zg"))&&(strcmp(test,"2zb"))))
	return(9);
	else if(!(strcmp(test,"mpr")))
	return(16);
	else if(!(strcmp(test,"txt")))
	return(6);
	else if(!(strcmp(test,"c")))
	return(15);
	else if(!(strcmp(test,"ppc")))
	return(14);
	else if(!(strcmp(test,"h")))
	return(17);
	else if(!(strcmp(test,"fdp")))
	return(19);
	else if(!(strcmp(test,"nur"))&&(strcmp(test,"hs"))&&(strcmp(test,"fnoc")))
	return(18);
	else if(!(strcmp(test,"lmth"))&&(strcmp(test,"lmx"))&&(strcmp(test,"lmgs")))
	return(21);
	else return(2);
}

HWND BuildRebar(HWND hwndParent)
{
	HWND     hwndRebar = NULL;

	hwndRebar = CreateWindowEx(   WS_EX_TOOLWINDOW, 
								REBARCLASSNAME, 
								NULL,
								WS_VISIBLE |WS_BORDER | 
								WS_CHILD | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS | 
								RBS_VARHEIGHT | RBS_BANDBORDERS |  
								CCS_NODIVIDER | CCS_NOPARENTALIGN |
								CCS_TOP |0,
								0, 0, 250, 100, 
								hwndParent, 
								(HMENU)ID_REBAR, 
								g_hInst, 
								NULL);
	if(hwndRebar)
	{
		//Create a toolbar band in the rebar control.
		CreateToolBarBand(hwndRebar);

		//Create an address bar band in the rebar control.
		CreateAddressBarBand(hwndRebar);
	}
	return hwndRebar;
}

/**************************************************************************

	CreateToolBarBand()

**************************************************************************/

HWND CreateToolBarBand(HWND hWndParent)
{
	REBARBANDINFO rbBand;

	hWndToolBar = CreateToolbarEx(hWndParent,
        				WS_CHILD | TBSTYLE_LIST |
						TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | 
						WS_CLIPCHILDREN |WS_CLIPSIBLINGS | 
						CCS_NODIVIDER | CCS_NORESIZE | 
						WS_VISIBLE,
        				ID_TOOLBAR, 
						NUM_TBBITMAPS,					//nBitmaps - set to zero initially
						(HINSTANCE)g_hInst, 
						IDB_TOOLBAR,				//wBMID - no resource identifiers
        				(LPCTBBUTTON)&tbButtons,
       					NUM_TBBUTTONS,					// nButtons - set to zero initially
						BMP_CX,BMP_CY,                  // width & height of buttons
						BMP_CX,BMP_CY,                  // width & height of bitmaps
						sizeof(TBBUTTON));

	if (hWndToolBar == NULL )
	{
		MessageBox (NULL, TEXT("Toolbar not created!"), NULL, MB_OK );
		return (HWND)NULL;
	}

	// Initialize REBARBANDINFO for all rebar bands
	rbBand.cbSize = sizeof(REBARBANDINFO);
	rbBand.fMask = RBBIM_COLORS |				// clrFore and clrBack are valid
				   RBBIM_CHILD |				// hwndChild is valid
			       RBBIM_CHILDSIZE |			// cxMinChild and cyMinChild are valid
			       RBBIM_STYLE |				// fStyle is valid
			       RBBIM_ID |					// wID is valid
			       RBBIM_BACKGROUND;			// hbmBack is valid
	rbBand.clrFore = GetSysColor(COLOR_BTNTEXT);
	rbBand.clrBack = GetSysColor(COLOR_BTNFACE);
	rbBand.fStyle = RBBS_NOVERT |				// do not display in vertical orientation
			    RBBS_CHILDEDGE |
			    RBBS_FIXEDBMP;
	rbBand.hbmBack = NULL;							//No Bitmaps are loaded
	rbBand.hwndChild = hWndToolBar;
	rbBand.wID = ID_TOOLBAR;
	rbBand.cxMinChild = MIN_TBCX;
	rbBand.cyMinChild = MIN_CY;

	// Insert band into rebar
	SendMessage(hWndParent, RB_INSERTBAND, (UINT) -1, (LPARAM) (LPREBARBANDINFO) &rbBand);

	return (hWndToolBar);
}

/**************************************************************************

	CreateAddressBarBand()

**************************************************************************/
void CreateAddressBarBand(HWND hwndRebar)
{
	REBARBANDINFO  rbbi;
	RECT           rc;
	LRESULT        lResult;

		//add a band that contains a combobox
	hwndAddress = CreateWindowEx(   0, 
									TEXT("combobox"), 
									NULL,
									WS_VISIBLE |WS_CHILD | 
									WS_TABSTOP |WS_VSCROLL |
									WS_CLIPCHILDREN | WS_CLIPSIBLINGS | 
									CBS_AUTOHSCROLL | CBS_DROPDOWN | 
									0,0,0, 
									300, 200, 
									hwndRebar, 
									(HMENU)ID_COMBOBOX, 
									g_hInst, 
									NULL);

	SendMessage(hwndAddress, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	  
	//SetWindowText(hwndAddress,pathname[display_current_index]);
	SetWindowText(hwndAddress,pathname[0]);

	GetWindowRect(hwndAddress, &rc);
	   
	ZeroMemory(&rbbi, sizeof(rbbi));
	rbbi.cbSize       = sizeof(REBARBANDINFO);
	rbbi.fMask        = RBBIM_SIZE | 
							RBBIM_CHILD | 
							RBBIM_CHILDSIZE | 
							RBBIM_ID | 
							RBBIM_STYLE | 
							RBBIM_TEXT |
							RBBIM_BACKGROUND |
							RBBIM_IMAGE |
							0;
	rbbi.cxMinChild   = rc.right - rc.left;
	rbbi.cyMinChild   = rc.bottom - rc.top;
	rbbi.cx           = 100;
	rbbi.fStyle       =  RBBS_BREAK |
							RBBS_CHILDEDGE | 
							RBBS_FIXEDBMP |
							0;
	rbbi.wID          = ID_COMBOBOX;
	rbbi.hwndChild    = hwndAddress;
	rbbi.lpText       = TEXT("Address");
	rbbi.cch          = 2;
	rbbi.hbmBack      = NULL;
	rbbi.iImage       = 0;

	lResult = SendMessage(hwndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)(LPREBARBANDINFO)&rbbi);

}
/**************************************************************************

   MoveRebar()
   
**************************************************************************/

void MoveRebar(HWND hWnd)
{
	RECT  rc;
	int   x,y,
		cx,cy;

	GetClientRect(hWnd, &rc);
	//align the rebar along the top of the window
		x = 0;
		y = 0;
		cx = rc.right - rc.left;
		cy = rc.bottom - rc.top;
	    
	MoveWindow( GetDlgItem(hWnd, ID_REBAR), 
				x, 
				y, 
				cx, 
				cy, 
				TRUE);
}


/******************************************************************************

   CreateListView

******************************************************************************/
HWND CreateListView(HINSTANCE hInstance, HWND hwndParent)
{
DWORD       dwStyle;
HWND        hwndListView;
HIMAGELIST  himlSmall;
HIMAGELIST  himlLarge;
BOOL        bSuccess = TRUE;

dwStyle =   WS_TABSTOP | 
            WS_CHILD | 
            WS_VISIBLE |
            LVS_AUTOARRANGE |
            LVS_REPORT | 
            LVS_OWNERDATA ;
            
hwndListView = CreateWindowEx(   0,			           // ex style
                                 WC_LISTVIEW,               // class name - defined in commctrl.h
                                 "",                        // dummy text
                                 dwStyle,                   // style
                                 0,                         // x position
                                 0,                         // y position
                                 0,							// width
                                 0,							// height
                                 hwndParent,                // parent
                                 (HMENU)ID_LISTVIEW,        // ID
                                 g_hInst,                   // instance
                                 NULL);                     // no extra data

if(!hwndListView)
   return NULL;

ResizeListView(hwndListView, hwndParent);

//set the image lists
himlSmall = ImageList_Create(16, 16, ILC_COLORDDB | ILC_MASK, 1, 0);
himlLarge = ImageList_Create(37,37, ILC_COLORDDB | ILC_MASK, 1, 0);

if (himlSmall && himlLarge)
   {
   HICON hIcon;

   //set up the small image list
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_FOLDER), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[0]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_DRIVE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[1]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_FILE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[2]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_SOCKET), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[3]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_CHARDEV), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[4]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_BLOCDEV), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[5]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_TEXT), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[6]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_JPG), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[7]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_MOVIE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[8]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_RAR), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[9]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_WAV), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[10]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_MP3), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[11]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_PIPLINK), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[12]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_PIPE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[13]=ImageList_AddIcon(himlSmall, hIcon); 
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_CPP), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[14]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_C), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[15]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_RPM), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[16]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_H), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[17]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_EXEC), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[18]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_PDF), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[19]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_MP3), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[20]=ImageList_AddIcon(himlSmall, hIcon);
	hIcon = LoadImage(g_hInst, MAKEINTRESOURCE(IDI_HTML), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	images[21]=ImageList_AddIcon(himlSmall, hIcon);


   //set up the large image list
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_FOLDER));
	images[0]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_DRIVE));
	images[1]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_FILE));
	images[2]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_SOCKET));
	images[3]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_CHARDEV));
	images[4]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_BLOCDEV));
	images[5]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_TEXT));
	images[6]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_JPG));
	images[7]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_MOVIE));
	images[8]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_RAR));
	images[9]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_WAV));
	images[10]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_MP3));
	images[11]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_PIPLINK));
	images[12]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_PIPE));
	images[13]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_CPP));
	images[14]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_C));
	images[15]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_RPM));
	images[16]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_H));
	images[17]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_EXEC));
	images[18]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_PDF));
	images[19]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_MP3));
	images[20]=ImageList_AddIcon(himlLarge, hIcon);
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_HTML));
	images[21]=ImageList_AddIcon(himlLarge, hIcon);

	ListView_SetImageList(hwndListView, himlSmall, LVSIL_SMALL);
	ListView_SetImageList(hwndListView, himlLarge, LVSIL_NORMAL);
   }

return hwndListView;
}

/******************************************************************************

   ResizeListView

******************************************************************************/
void ResizeListView(HWND hwndListView, HWND hwndParent)
{
	RECT  rc;

	GetClientRect(hwndParent, &rc);

	MoveWindow( hwndListView, 
				rc.left + LISTWINDOW_X,
				rc.top + LISTWINDOW_Y,
				rc.right - rc.left - LISTWINDOW_X,
				rc.bottom - rc.top - LISTWINDOW_Y,
				TRUE);

}

/******************************************************************************

   PositionHeader

   this needs to be called when the ListView is created, resized, the view is 
   changed or a WM_SYSPARAMETERCHANGE message is recieved

******************************************************************************/
void PositionHeader(HWND hwndListView)
{
	HWND  hwndHeader = GetWindow(hwndListView, GW_CHILD);
	DWORD dwStyle = GetWindowLong(hwndListView, GWL_STYLE);

	/*To ensure that the first item will be visible, create the control without 
	the LVS_NOSCROLL style and then add it here*/
	dwStyle |= LVS_NOSCROLL;
	SetWindowLong(hwndListView, GWL_STYLE, dwStyle);

	//only do this if we are in report view and were able to get the header hWnd
	if(((dwStyle & LVS_TYPEMASK) == LVS_REPORT) && hwndHeader)
	{
		RECT        rc;
		HD_LAYOUT   hdLayout;
		WINDOWPOS   wpos;

		GetClientRect(hwndListView, &rc);
		hdLayout.prc = &rc;
		hdLayout.pwpos = &wpos;

		Header_Layout(hwndHeader, &hdLayout);

		SetWindowPos(  hwndHeader, 
						wpos.hwndInsertAfter, 
						wpos.x, 
						wpos.y,
						wpos.cx, 
						wpos.cy, 
						wpos.flags | SWP_SHOWWINDOW);

		ListView_EnsureVisible(hwndListView, 0, FALSE);
	}
}

/******************************************************************************

   InitListView

******************************************************************************/

BOOL InitListView(HWND hwndListView)
{
	LV_COLUMN   lvColumn;
	int         i;
	TCHAR       szString[5][20] = {"Name", "Inode", "File type", "Size", "Access Rights"};

	//empty the list
	ListView_DeleteAllItems(hwndListView);

	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 120;
	for(i = 0; i < 5; i++)
	{
		lvColumn.pszText = szString[i];
		ListView_InsertColumn(hwndListView, i, &lvColumn);
	}
	InsertListViewItems(hwndListView);
	return TRUE;
}

/******************************************************************************

   InsertListViewItems

******************************************************************************/

BOOL InsertListViewItems(HWND hwndListView)
{
	//empty the list
	ListView_DeleteAllItems(hwndListView);

	//set the number of items in the list
	ListView_SetItemCount(hwndListView, ITEM_COUNT);

	return TRUE;
}

/**************************************************************************

   SwitchView()

**************************************************************************/

void SwitchView(HWND hwndListView, DWORD dwView)
{
	DWORD dwStyle = GetWindowLong(hwndListView, GWL_STYLE);

	CURRENT_VIEW = dwView;
	SetWindowLong(hwndListView, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | dwView);
	ResizeListView(hwndListView, GetParent(hwndListView));
}

/**************************************************************************

   DoListViewMenu()

**************************************************************************/

BOOL DoListViewMenu(  HWND hWnd, 
                     WPARAM wParam, 
                     LPARAM lParam)
{
	HWND  hwndListView = (HWND)wParam;
	HMENU hMenuLoad,
		hMenu;

	if(hwndListView != GetDlgItem(hWnd, ID_LISTVIEW))
	return FALSE;

	hMenuLoad = LoadMenu(g_hInst, MAKEINTRESOURCE(IDM_LISTVIEW_MENU));
	hMenu = GetSubMenu(hMenuLoad, 0);

	UpdateListViewMenu(hwndListView, hMenu);

	TrackPopupMenu(   hMenu,
					TPM_LEFTALIGN | TPM_RIGHTBUTTON,
					LOWORD(lParam),
					HIWORD(lParam),
					0,
					hWnd,
					NULL);

	DestroyMenu(hMenuLoad);

	return TRUE;
}

/**************************************************************************

   UpdateListViewMenu()

**************************************************************************/

void UpdateListViewMenu(HWND hwndListView, HMENU hMenu)
{
	UINT  uID;
	DWORD dwStyle;

	//uncheck all of these guys
	CheckMenuItem(hMenu, IDM_LARGE_ICONS,  MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_SMALL_ICONS,  MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_LIST,  MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDM_REPORT,  MF_BYCOMMAND | MF_UNCHECKED);

	//check the appropriate view menu item
	dwStyle = GetWindowLong(hwndListView, GWL_STYLE);
	switch(dwStyle & LVS_TYPEMASK)
	{
	case LVS_ICON:
		uID = IDM_LARGE_ICONS;
		break;
	      
	case LVS_SMALLICON:
		uID = IDM_SMALL_ICONS;
		break;
	      
	case LVS_LIST:
		uID = IDM_LIST;
		break;
	   
	case LVS_REPORT:
		uID = IDM_REPORT;
		break;
	}
	CheckMenuRadioItem(hMenu, IDM_LARGE_ICONS, IDM_REPORT, uID,  MF_BYCOMMAND | MF_CHECKED);

}
/**************************************************************************

   DoFileSelectionMenu()

**************************************************************************/

BOOL DoFileSelectionMenu(  HWND hWnd, 
						WPARAM wParam, 
						LPARAM lParam)
{
	HWND  hwndListView = (HWND)wParam;
	HMENU hMenuLoad,
		hMenu;

	if(hwndListView != GetDlgItem(hWnd, ID_LISTVIEW))
	return FALSE;

	hMenuLoad = LoadMenu(g_hInst, MAKEINTRESOURCE(IDM_FILE_MENU));
	hMenu = GetSubMenu(hMenuLoad, 0);

	TrackPopupMenu(   hMenu,
					TPM_LEFTALIGN | TPM_RIGHTBUTTON,
					LOWORD(lParam),
					HIWORD(lParam),
					0,
					hWnd,
					NULL);

	DestroyMenu(hMenuLoad);

	return TRUE;
}
/**************************************************************************

DoMenuFileView()

***************************************************************************/

void DoMenuFileView(HWND hWnd ,HWND hwndListView)
{
	 TCHAR current[MAX_PATH];
	 if( guifile[chosenFileIndex + 1].file_type == 1)
	 {
	 GetCurrentDirectory(MAX_PATH,current);
				 _chdir("c:\\windows\\temp");
				 CopyExt2File(guifile[chosenFileIndex+1].inode,peslba[present_partition].lba,guifile[chosenFileIndex+1].filename);				 
				 _chdir(current);
				 wsprintf(current,"C:\\windows\\temp\\%s",guifile[chosenFileIndex+1].filename);
				 ShellExecute(hwndListView,NULL,current,NULL,"c:\\windows",SW_SHOWNORMAL);}
	 else
		 if(guifile[chosenFileIndex + 1].file_type == 2)
		 {
			if(++display_current_index!=100)
			{
			strcpy(pathname[display_current_index],pathname[display_current_index-1]);
			strcat(pathname[display_current_index],guifile[chosenFileIndex+1].filename);strcat(pathname[display_current_index],"/");
			display_maxindex=display_current_index;	
			displaystack[display_current_index]=guifile[chosenFileIndex+1].inode;
			}
			SetWindowText(hwndAddress,pathname[display_current_index]);
			printinode(disk,peslba[present_partition].lba,guifile[chosenFileIndex + 1].inode);
			InitListView(hwndListView);
		 }
}

/**************************************************************************

DoMenuFileCopy()

***************************************************************************/

void DoMenuFileCopy(HWND hWnd , HWND hwndListView)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "",currentdirectory[MAX_PATH]="";

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner   = hWnd;
	ofn.hInstance   = g_hInst;
	strcpy(szFileName, guifile[chosenFileIndex +1].filename);
	ofn.lpstrFile   = szFileName;
	ofn.nMaxFile	= MAX_PATH;
	ofn.lpstrTitle  = "Save";
	ofn.Flags		= OFN_EXPLORER | OFN_OVERWRITEPROMPT;
	GetCurrentDirectory(MAX_PATH,currentdirectory);

	if(guifile[chosenFileIndex + 1].file_type !=1 && guifile[chosenFileIndex + 1].file_type !=2)
	{
		MessageBox(hWnd,"Copy support for this file type not provided","Error",MB_OK | MB_ICONEXCLAMATION);
	}

	else

	if(GetSaveFileName(&ofn) != 0)
		{

			ShowCopyProgress(hWnd , ofn.lpstrFile);
			
			if(guifile[chosenFileIndex + 1].file_type==1)
				CopyExt2File(guifile[chosenFileIndex + 1].inode,peslba[present_partition].lba,ofn.lpstrFile);
			else
			if(guifile[chosenFileIndex + 1].file_type==2)
			{	
			_mkdir(ofn.lpstrFile);
			_chdir(ofn.lpstrFile);			
		    CopyFolder(guifile[chosenFileIndex + 1].inode , peslba[present_partition].lba);
			_chdir(currentdirectory);
			}
		   EndDialog(hProgressDlg ,IDOK);

		}	
}

/**************************************************************************

DoMenuFileProperties()

***************************************************************************/

void DoMenuFileProperties(HWND hWnd , HWND hwndListView)
{
	TCHAR szString1[250];
	strcpy(szString1," ");
	if(guifile[chosenFileIndex+1].file_type!=LINUX_DRIVE)
		wsprintf(szString1,"\nName                       %s\nInode no.                 %ld\nType                         %s\nSize                          %lu bytes\nAccess Rights           %s",guifile[chosenFileIndex+1].filename,guifile[chosenFileIndex+1].inode,filetype[guifile[chosenFileIndex+1].file_type],guifile[chosenFileIndex+1].isize,guifile[chosenFileIndex+1].acrights);
	else
		wsprintf(szString1,"\nName                       %s\nInode no.                 %ld\nType                         %s\nSize                          %lu Mbytes",guifile[chosenFileIndex+1].filename,2,filetype[guifile[chosenFileIndex+1].file_type],guifile[chosenFileIndex+1].isize);
    MessageBox(hWnd, szString1, "               ...Properties...               ", MB_OK);
}

/**************************************************************************

      DisplayPartitionInfo

***************************************************************************/
void DisplayPartitionInfo(HWND hWnd)
{
	TCHAR szString[150],szString1[1000];int i;
	strcpy(szString1,"");
	for(i=0;i<partition_index;i++)
	{
		wsprintf(szString,"Parition %d      %s\n\n",i+1,partitioninfo[i]);
		 strcat(szString1,szString);
	}
   MessageBox(hWnd,szString1, "               ...Partition Info...               ", MB_OK);
}

/**************************************************************************

      DisplayPartitionIcons

***************************************************************************/
void DisplayPartitionIcons(HWND hwndListView)
{
	int i;
	SetWindowText(hwndAddress,"My Linux Partitions");
	display_current_index=0;
	for(i=1;i<=numlin;i++)
	{
		guifile[i].file_type=LINUX_DRIVE;
		disk = peslba[i-1].disk_no ;
		strcpy(guifile[i].filename,printsuper(disk,peslba[i-1].lba));
		strcpy(guifile[i].acrights,"\0");
		guifile[i].isize = peslba[i-1].numsectors/2048;
		guifile[i].inode = i-1;
	}
	num_files=numlin+1;
	InsertListViewItems(hwndListView);
    sprintf(error,"%d %s",guifile[1].inode,guifile[1].filename);
}

void ClearPropertiesText()
{
	WORD i ; 
	RECT rcClient;
	char clearText[PROPERTIES_RIGHT];
	for( i =0 ;i<PROPERTIES_RIGHT -1 ; i++)
		clearText[i] = ' ';
	clearText[i] = '\0';
	rcClient.left = PROPERTIES_LEFT;
	rcClient.top = PROPERTIES_TOP;
	rcClient.right = PROPERTIES_RIGHT;			
	rcClient.bottom = PROPERTIES_BOTTOM;
	DrawText(hdcPropertiesText ,clearText , -1,&rcClient, 0);
	rcClient.top = PROPERTIES_TOP + 20;
	DrawText(hdcPropertiesText ,clearText , -1,&rcClient , 0);
	rcClient.top = PROPERTIES_TOP + 40;
	DrawText(hdcPropertiesText ,clearText , -1,&rcClient , 0);
	rcClient.top = PROPERTIES_TOP + 60;
	DrawText(hdcPropertiesText ,clearText , -1,&rcClient , 0);

}

void ChooseTextFont(HWND hwnd)
{
	HDC hdc;
	long lfHeight;
					
	hdc = GetDC(NULL);
	lfHeight = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(NULL, hdc);

	g_hfFont = /*GetStockObject(DEFAULT_GUI_FONT);//*/CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Lucida Console");
	SetTextColor(hdcPropertiesText, g_rgbText);
	SelectObject(hdcPropertiesText, g_hfFont);

}

void ShowCopyProgress(HWND hWnd , char * filename)
{	
	hProgressDlg = CreateDialog(g_hInst ,
						MAKEINTRESOURCE(IDD_PROGRESS) , 
						hWnd ,
						ProgressDlgProc) ;
	if(hProgressDlg == NULL)
		MessageBox(hWnd ,"Error retreiving handle to Progress Dialog","",MB_OK);
					
	hwndPB = GetDlgItem(hProgressDlg , IDC_PROGRESS_CONTROL);

	if(hwndPB == NULL)
		MessageBox(hWnd ,"Error retreiving handle to Progress bar control","",MB_OK);

	ShowWindow(hProgressDlg , SW_SHOWNA);
	UpdateWindow(hProgressDlg);

	SendMessage(hwndPB, PBM_SETRANGE, 0,
						MAKELPARAM(0, PROGRESS_RANGE)); 
	SendMessage(hwndPB, PBM_SETSTEP, (WPARAM) 1, 0);

	SetDlgItemText(hProgressDlg , IDC_COPYFILENAME , filename );
}


VOID WINAPI OnTabbedDialogInit(HWND hwndDlg) 
{ 
    DLGHDR *pHdr = (DLGHDR *) LocalAlloc(LPTR, sizeof(DLGHDR)); 
	TCITEM tie = {TCIF_TEXT  ,-1 , 0}; 
 
    // Save a pointer to the DLGHDR structure. 
    SetWindowLong(hwndDlg, GWL_USERDATA, (LONG) pHdr); 
    // Create the tab control. 
	pHdr->hwndTab = GetDlgItem(hwndDlg , IDC_TABCONTROL);
 
    // Add a tab for each of the two child dialog boxes. 
    tie.pszText = "   About Tuxidow   "; 
    TabCtrl_InsertItem(pHdr->hwndTab, 0, &tie); 
    tie.pszText = "    Copyright    "; 
    TabCtrl_InsertItem(pHdr->hwndTab, 1, &tie); 
 
    // Lock the resources for the three child dialog boxes. 
    pHdr->apRes[0] = DoLockDlgRes(MAKEINTRESOURCE(IDD_ABOUTUS)); 
    pHdr->apRes[1] = DoLockDlgRes(MAKEINTRESOURCE(IDD_COPYRIGHT)); 
   
    OnSelChanged(hwndDlg); 
} 

// DoLockDlgRes - loads and locks a dialog box template resource. 
// Returns the address of the locked resource. 
// lpszResName - name of the resource 
DLGTEMPLATE * WINAPI DoLockDlgRes(LPCSTR lpszResName) 
{ 
    HRSRC hrsrc = FindResource(NULL, lpszResName, RT_DIALOG); 
    HGLOBAL hglb = LoadResource(g_hInst, hrsrc); 
    return (DLGTEMPLATE *) LockResource(hglb); 
} 

VOID WINAPI OnSelChanged(HWND hwndDlg) 
{ 
    DLGHDR *pHdr = (DLGHDR *) GetWindowLong( 
        hwndDlg, GWL_USERDATA); 
	int iSel = TabCtrl_GetCurSel(pHdr->hwndTab); 
 
    // Destroy the current child dialog box, if any. 
    if (pHdr->hwndDisplay != NULL) 
        DestroyWindow(pHdr->hwndDisplay); 
 
    // Create the new child dialog box. 
    pHdr->hwndDisplay = CreateDialogIndirect(g_hInst, 
        pHdr->apRes[iSel], hwndDlg, ChildDialogProc);
	
} 
