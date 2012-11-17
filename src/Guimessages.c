/****************************************************************
 
FILE : GUIMESSAGES.C
DETAILS : HANDLES ALL GUI MESSAGES
CONTACT : tuxidow@yahoo.com

*****************************************************************/


#include "guimessages.h"


/******************************************************************************

   MainWndProc

******************************************************************************/

LRESULT CALLBACK MainWndProc( HWND hWnd,
                              UINT uMessage,
                              WPARAM wParam,
                              LPARAM lParam)
{
static HWND hwndListView;
int pathnamlen=0,i;
switch (uMessage)
   {
case WM_CREATE:
	{		
			// create the TreeView control
			hwndListView = CreateListView(g_hInst, hWnd);
			
			//initialize the TreeView control
			InitListView(hwndListView);
			
			BuildRebar(hWnd);

			g_hbitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SIDEIMAGE));
			if(g_hbitmap == NULL)
				MessageBox(hWnd, "Could not load IDB_SIDEIMAGE !", "Error", MB_OK | MB_ICONEXCLAMATION);

			
			g_hbitmapTuxidow = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_TUXIDOW));
			if(g_hbitmapTuxidow == NULL)
				MessageBox(hWnd, "Could not load IDB_TUXIDOW !", "Error", MB_OK | MB_ICONEXCLAMATION);

			DisplayPartitionIcons(hwndListView);
			hdcPropertiesText = GetDC(hWnd);
			ChooseTextFont(hWnd);

	}
      
      break;

   case WM_NOTIFY:
      return HandleNotifyMessages(hWnd, lParam);
   
   case WM_SIZE:
	  MoveRebar(hWnd);
	  if(( CURRENT_VIEW == LVS_ICON ) || (CURRENT_VIEW == LVS_SMALLICON))
		{
			DWORD dwStyle = GetWindowLong(hwndListView, GWL_STYLE);
			SetWindowLong(hwndListView, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_LIST);
			SetWindowLong(hwndListView, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | CURRENT_VIEW);		
		}
		ResizeListView(hwndListView, hWnd);
      break;

   case WM_INITMENUPOPUP:
      UpdateListViewMenu(hwndListView, GetMenu(hWnd));
      break;
   
   case WM_CONTEXTMENU:
	   if(isListFileChosen == TRUE)
			{if(DoFileSelectionMenu(hWnd, wParam, lParam))
             return FALSE;
			}
	   else
		   {if(DoListViewMenu(hWnd, wParam, lParam))
             return FALSE;
			}
      break;

   case WM_COMMAND:
      switch (GET_WM_COMMAND_ID(wParam, lParam))
         {
         case IDM_LARGE_ICONS:
			CURRENT_VIEW = LVS_ICON ;
            SwitchView(hwndListView, LVS_ICON);
            break;
         
         case IDM_SMALL_ICONS:
		    CURRENT_VIEW = LVS_SMALLICON ;
            SwitchView(hwndListView, LVS_SMALLICON);
            break;
         
         case IDM_LIST:
			CURRENT_VIEW = LVS_LIST ;
            SwitchView(hwndListView, LVS_LIST);
            break;
         
         case IDM_REPORT:
			CURRENT_VIEW = LVS_REPORT ;
            SwitchView(hwndListView, LVS_REPORT);
            break;

		 case ID_VIEW_PARTITIONINFO:
			 DisplayPartitionInfo(hWnd);
			 break;

		 case IDM_FILE_VIEW:
			 DoMenuFileView(hWnd , hwndListView);
			 break;

		 case IDM_FILE_COPY:
			 DoMenuFileCopy(hWnd , hwndListView);
			 break;

		 case IDM_FILE_PROPERTIES:
			 DoMenuFileProperties(hWnd , hwndListView);
			 break;

         case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
         
         case IDM_ABOUT:
			 DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDlgProc);
            break;   

		 case IDS_BACK:
					DebugReport(0,"BTNCLK : BACK\n");
					if(display_current_index!=0)
					{
				        display_current_index--;
					    disk = peslba[present_partition].disk_no;
					    printinode(disk,peslba[present_partition].lba,displaystack[display_current_index]);	
						InsertListViewItems(hwndListView);
						SetWindowText(hwndAddress,pathname[display_current_index]);
					}
					break;

		 case IDS_FWD:
					DebugReport(0,"BTNCLK : FORWARD\n");
					if(display_current_index!=display_maxindex)
					{
						display_current_index++;
						disk = peslba[present_partition].disk_no;
					    printinode(disk,peslba[present_partition].lba,displaystack[display_current_index]);	
						//printinode(disk,peslba[present_partition],displaystack[display_current_index]);
						InsertListViewItems(hwndListView);
						SetWindowText(hwndAddress,pathname[display_current_index]);
					}
					break;
		 case IDS_UP:
					DebugReport(0,"BTNCLK : UP\n");
			 if(guifile[0].file_type!=LINUX_DRIVE)
			 {
				if(present_inode!=0)
				{
					if(++display_current_index!=100)
					{
						display_maxindex=display_current_index;	
						displaystack[display_current_index]=guifile[0].inode;
						strcpy(pathname[display_current_index],pathname[display_current_index-1]);
						pathnamlen = (int)strlen(pathname[display_current_index]);
						i = pathnamlen-2 ;
						while(pathname[display_current_index][i]!='/' && i>(address_len))	
							i=i-1;
						pathname[display_current_index][(i)>address_len?(i+1):address_len]='\0';					
					}
				    //printinode(disk,peslba[present_partition],guifile[0].inode);
					disk = peslba[present_partition].disk_no;
					printinode(disk,peslba[present_partition].lba,guifile[0].inode);	

					InsertListViewItems(hwndListView);
					SetWindowText(hwndAddress,pathname[display_current_index]);
				}
				else
				{
					DisplayPartitionIcons(hwndListView);  
					display_current_index=0;
					display_maxindex=0;
				}
			 }		
					break;	
		 case IDS_HOME:
				DebugReport(0,"\nBTNCLK : HOME\n");
				DisplayPartitionIcons(hwndListView);  
				break;

		 case ID_ACCLBACK :
			SendMessage(hWnd , WM_COMMAND , IDS_BACK , 0);
			 break;

		 case ID_ACCLFWD  :
			 SendMessage(hWnd , WM_COMMAND , IDS_FWD , 0);
			 break;

		 case ID_ACCLUP   :
			 SendMessage(hWnd , WM_COMMAND , IDS_UP , 0);
			 break;

         }
      break;
   case WM_CHAR:
	   {
		   switch(wParam)
		   {
		   case 13: MessageBox (NULL, TEXT("Up"), TEXT("Command"), MB_OK );
			   break;
		   }
	   }
	   break ;
	case WM_PAINT:
		{
			// Just a note, never use a MessageBox from inside WM_PAINT
			// The box will cause more WM_PAINT messages and you'll probably end up
			// stuck in a loop

			BITMAP bm;
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hWnd, &ps);
			
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmOld = SelectObject(hdcMem, g_hbitmap);

			GetObject(g_hbitmap, sizeof(bm), &bm);
			BitBlt(hdc, 0, LISTWINDOW_Y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
			
			hbmOld = SelectObject(hdcMem, g_hbitmapTuxidow);

			GetObject(g_hbitmapTuxidow, sizeof(bm), &bm);
			BitBlt(hdc, 5, LISTWINDOW_Y + 300, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);

			EndPaint(hWnd, &ps);
		}

		break;

   case WM_DESTROY:
      PostQuitMessage(0);
      break;

   default:
      break;
   }
return DefWindowProc(hWnd, uMessage, wParam, lParam);
}


/******************************************************************************

   AboutDlgProc

******************************************************************************/

LRESULT CALLBACK AboutDlgProc(   HWND hDlg, 
                              UINT uMessage, 
                              WPARAM wParam, 
                              LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_INITDIALOG:
		{	OnTabbedDialogInit( hDlg);
				return TRUE;
		}
	case WM_NOTIFY:
		switch (((NMHDR FAR *) lParam)->code)
		{
		case TCN_SELCHANGE:
			OnSelChanged(hDlg);
			break;
		}
		break;
      
	case WM_COMMAND:
		switch(wParam)
			{
			case IDOK:
				EndDialog(hDlg, IDOK);
				break;

			case IDCANCEL:
				EndDialog(hDlg, IDOK);
				break;
			}
		return TRUE;
    } 
    
	return FALSE;
}
/**************************************************************************

   ListViewNotify()

**************************************************************************/

LRESULT HandleNotifyMessages(HWND hWnd, LPARAM lParam)
{
LPNMHDR  lpnmh = (LPNMHDR) lParam;
HWND     hwndListView = GetDlgItem(hWnd, ID_LISTVIEW);
int code;

switch(lpnmh->code)
   {
	case TTN_NEEDTEXT:
			{
				LPTOOLTIPTEXT lpToolTipText;
				char szBuf[MAX_PATH];

				// Display the ToolTip text.
				lpToolTipText = (LPTOOLTIPTEXT)lParam;
				LoadString (g_hInst, 
					(UINT)lpToolTipText->hdr.idFrom,   // string ID == cmd ID
					szBuf,
					sizeof (szBuf));
				lpToolTipText->lpszText = szBuf;
				break;
			}
			return 0;
   case LVN_GETDISPINFO:
      {
      LV_DISPINFO *lpdi = (LV_DISPINFO *)lParam;
      TCHAR szString[MAX_PATH];
	  TCHAR szString1[13];
      if(lpdi->item.iSubItem)
         {
         switch(lpdi->item.iSubItem)
			 {
			 case 1:wsprintf(szString, "%lu", guifile[lpdi->item.iItem+1].inode);
            lstrcpy(lpdi->item.pszText, szString);
			break;
			 case 2:
				 wsprintf(szString,filetype[guifile[lpdi->item.iItem+1].file_type]);
            lstrcpy(lpdi->item.pszText, szString);
			break;
			 case 3:
				 wsprintf(szString,"%lu",guifile[lpdi->item.iItem+1].isize);
            lstrcpy(lpdi->item.pszText, szString);
			break;
			case 4:
				strcpy(szString," ");
				strncpy(szString1,guifile[lpdi->item.iItem+1].acrights,13);				
            lstrcpy(lpdi->item.pszText, szString1);
			break;
      		 }
         }
      else
         {
         if(lpdi->item.mask & LVIF_TEXT)
            {
			wsprintf(szString, guifile[lpdi->item.iItem+1].filename);
            lstrcpy(lpdi->item.pszText, szString);
            }
		 
         if(lpdi->item.mask & LVIF_IMAGE)
            {            
				if(guifile[lpdi->item.iItem+1].file_type==1)
				{
					code=find_fil_typ(guifile[lpdi->item.iItem+1].filename);
						lpdi->item.iImage = images[code];				
				}			
				else if(guifile[lpdi->item.iItem+1].file_type==2)
					lpdi->item.iImage = images[0];
				else if(guifile[lpdi->item.iItem+1].file_type==3)
					lpdi->item.iImage = images[5];
				else if(guifile[lpdi->item.iItem+1].file_type==4)
					lpdi->item.iImage = images[4];
				else if(guifile[lpdi->item.iItem+1].file_type==6)
					lpdi->item.iImage = images[3];
				else if(guifile[lpdi->item.iItem+1].file_type==5)
					lpdi->item.iImage = images[13];
				else if(guifile[lpdi->item.iItem+1].file_type==7)
					lpdi->item.iImage = images[12];
				else if(guifile[lpdi->item.iItem+1].file_type==LINUX_DRIVE)
					lpdi->item.iImage = images[1];
			}
		 }
      }
      return 0;

   case LVN_ODCACHEHINT:
      {
      LPNMLVCACHEHINT   lpCacheHint = (LPNMLVCACHEHINT)lParam;
      /*
      This sample doesn't use this notification, but this is sent when the 
      ListView is about to ask for a range of items. On this notification, 
      you should load the specified items into your local cache. It is still 
      possible to get an LVN_GETDISPINFO for an item that has not been cached, 
      therefore, your application must take into account the chance of this 
      occurring.
      */
      }
      return 0;

   case LVN_ODFINDITEM:
      {
      LPNMLVFINDITEM lpFindItem = (LPNMLVFINDITEM)lParam;
      /*
      This sample doesn't use this notification, but this is sent when the 
      ListView needs a particular item. Return -1 if the item is not found.
      */
      }
      return 0;
 
   case NM_DBLCLK:
	   {
		 LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;
		 TCHAR current[MAX_PATH];
		 ClearPropertiesText();
		 if((pnm->iItem < ITEM_COUNT )&& (pnm->iItem!=-1))
		 {	
			 if(guifile[pnm->iItem+1].file_type==2)
			 {
				
				if(++display_current_index!=100)
				{
				strcpy(pathname[display_current_index],pathname[display_current_index-1]);
				strcat(pathname[display_current_index],guifile[pnm->iItem+1].filename);
				strcat(pathname[display_current_index],"/");
				display_maxindex=display_current_index;	
				displaystack[display_current_index]=guifile[pnm->iItem+1].inode;
				}
				disk = peslba[present_partition].disk_no;
				DebugReport(SEPARATOR,"DBLCLK  Folder : %s ,Inode number :%d \n",
					guifile[pnm->iItem+1].filename,guifile[pnm->iItem+1].inode);
			    printinode(disk,peslba[present_partition].lba,guifile[pnm->iItem+1].inode);	
				InsertListViewItems(hwndListView);	

			 }
			else if(guifile[pnm->iItem+1].file_type == 1)
			{
				DebugReport(SEPARATOR,"DBLCLK  File :%s , Inode number :%d \n",
					guifile[pnm->iItem+1].filename,guifile[pnm->iItem+1].inode);
				GetCurrentDirectory(MAX_PATH,current);
				_chdir("c:\\windows\\temp");
				ShowCopyProgress(hWnd , "C:\\Windows\\Temp\\ ");
				CopyExt2File(guifile[pnm->iItem+1].inode,peslba[present_partition].lba,guifile[pnm->iItem+1].filename);				 
				_chdir(current);
				wsprintf(current,"C:\\windows\\temp\\%s",guifile[pnm->iItem+1].filename);
				ShellExecute(hwndListView,NULL,current,NULL,"c:\\windows",SW_SHOWNORMAL);
				EndDialog(hProgressDlg , IDOK);
			}

			 if(guifile[pnm->iItem+1].file_type==LINUX_DRIVE&&strcmp(guifile[pnm->iItem+1].filename,"No support")&&strcmp(guifile[pnm->iItem+1].filename,"ReiserFS"))
				{
					DebugReport(SEPARATOR,"DBLCLK  Partition : %d , Disk :%d , LBA :%lu\n",
						guifile[pnm->iItem+1].inode,peslba[present_partition].disk_no,peslba[present_partition].lba);
					present_partition=guifile[pnm->iItem+1].inode;
					disk = peslba[present_partition].disk_no;
					printsuper(disk,peslba[present_partition].lba);
					printinode(disk,peslba[present_partition].lba,2);	
					InsertListViewItems(hwndListView);
				}	
			 else 
			 {
				 if(!strcmp(guifile[pnm->iItem+1].filename,"No support")||!strcmp(guifile[pnm->iItem+1].filename,"ReiserFS"))
				 { 
					 MessageBox(NULL,"Support for Ext2/3 FileSystem only","Errorrrr..",MB_OK);
					 SetWindowText(hwndAddress,"My Linux Partitions");
				 return 0;
				 }
			 }
			  SetWindowText(hwndAddress,pathname[display_current_index]);
		 }
		
	   }

	   return 0;
	   case NM_RETURN:
	   {
		 LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;	
		 TCHAR current[MAX_PATH];
		 pnm->iItem=ListView_GetSelectionMark(hwndListView);
		 if((pnm->iItem<ITEM_COUNT )&& (pnm->iItem!=-1))
		 {	
			 if(guifile[pnm->iItem+1].file_type==2)
			 {
				
				if(++display_current_index!=100)
				{
				strcpy(pathname[display_current_index],pathname[display_current_index-1]);
				strcat(pathname[display_current_index],guifile[pnm->iItem+1].filename);strcat(pathname[display_current_index],"/");
				display_maxindex=display_current_index;	
				displaystack[display_current_index]=guifile[pnm->iItem+1].inode;
				}
				disk = peslba[present_partition].disk_no;
				DebugReport(SEPARATOR,"NM_RETURN  Folder : %s ,Inode number :%d \n",
					guifile[pnm->iItem+1].filename,guifile[pnm->iItem+1].inode);
				printinode(disk,peslba[present_partition].lba,guifile[pnm->iItem+1].inode);
				InsertListViewItems(hwndListView);		
			 }
			 else 
				 if(guifile[pnm->iItem+1].file_type==1)
			 {
				 DebugReport(SEPARATOR,"NM_RETURN  File :%s , Inode number :%d \n",
					guifile[pnm->iItem+1].filename,guifile[pnm->iItem+1].inode);
				 GetCurrentDirectory(MAX_PATH,current);
				 _chdir("c:\\windows\\temp");
				 ShowCopyProgress(hWnd , "C:\\Windows\\Temp\\ ");
				 CopyExt2File(guifile[pnm->iItem+1].inode,peslba[present_partition].lba,guifile[pnm->iItem+1].filename);				 
				 _chdir(current);
				 wsprintf(current,"c:\\windows\\temp\\%s",guifile[pnm->iItem+1].filename);
				 ShellExecute(hwndListView,NULL,current,NULL,"c:\\windows",SW_SHOWNORMAL);
				 EndDialog(hProgressDlg , IDOK);
			 }
				 else

			 if(guifile[pnm->iItem+1].file_type==LINUX_DRIVE&&strcmp(guifile[pnm->iItem+1].filename,"No support")&&strcmp(guifile[pnm->iItem+1].filename,"ReiserFS"))
			 {
				DebugReport(SEPARATOR,"NM_RETURN  Partition : %d , Disk :%d , LBA :%lu\n",
						guifile[pnm->iItem+1].inode,peslba[present_partition].disk_no,peslba[present_partition].lba);
				present_partition=guifile[pnm->iItem+1].inode;
				disk = peslba[present_partition].disk_no;
				printsuper(disk,peslba[present_partition].lba);
				printinode(disk,peslba[present_partition].lba,2);	
				InsertListViewItems(hwndListView);
			 }
			 else
				 if(!strcmp(guifile[pnm->iItem+1].filename,"No support")||!strcmp(guifile[pnm->iItem+1].filename,"ReiserFS"))
				 { 
					 MessageBox(NULL,"Support for Ext2/3 FileSystem only","Errorrrr..",MB_OK);
					 SetWindowText(hwndAddress,"My Linux Partitions");
				 return 0;
				 }
			SetWindowText(hwndAddress,pathname[display_current_index]);
		 }
		 
	   }

	   return 0;
   case NM_RCLICK:
	   {
		LPNMITEMACTIVATE pnm = (LPNMITEMACTIVATE) lParam;
		
		if((pnm->iItem < ITEM_COUNT )&& (pnm->iItem!=-1))	
			{
				isListFileChosen = TRUE;
				chosenFileIndex  = pnm->iItem;
			}
		else 
			isListFileChosen = FALSE;
	   }
	   return 0;

   case NM_CLICK:
	  	{
			LPNMITEMACTIVATE pnm = (LPNMITEMACTIVATE) lParam;
			RECT rcClient;
			char filenametext[MAX_PATH];
			char filesizetext[20];
			
			ClearPropertiesText();
			if((pnm->iItem < ITEM_COUNT )&& (pnm->iItem!=-1))	
			{
				strcpy(filenametext,guifile[pnm->iItem+1].filename);
			
				rcClient.left = PROPERTIES_LEFT;
				rcClient.top = PROPERTIES_TOP;
				rcClient.right = PROPERTIES_RIGHT;			
				rcClient.bottom = PROPERTIES_BOTTOM;
				DrawText(hdcPropertiesText ,filenametext , -1,&rcClient,  DT_WORD_ELLIPSIS );
				rcClient.top = PROPERTIES_TOP + 20;

				if(guifile[pnm->iItem+1].file_type!=LINUX_DRIVE)
					wsprintf(filesizetext , "%d %s" , guifile[pnm->iItem+1].isize , "bytes");
				else 
					wsprintf(filesizetext , "%d %s" , guifile[pnm->iItem+1].isize , "Mbytes");

				DrawText(hdcPropertiesText ,filesizetext , -1,&rcClient,  DT_WORD_ELLIPSIS );
				rcClient.top = PROPERTIES_TOP + 40;
				DrawText(hdcPropertiesText ,guifile[pnm->iItem+1].acrights , -1,&rcClient,  DT_WORD_ELLIPSIS );
				rcClient.top = PROPERTIES_TOP + 60;
				DrawText(hdcPropertiesText ,filetype[guifile[pnm->iItem+1].file_type] , -1,&rcClient,  DT_WORD_ELLIPSIS );

			}
			
			
		}	
	  return 0;	
	
   }

return 0;
}


/******************************************************************************

   ProgressDlgProc

******************************************************************************/

LRESULT CALLBACK ProgressDlgProc(   HWND hDlg, 
                              UINT uMessage, 
                              WPARAM wParam, 
                              LPARAM lParam)

{
switch (uMessage)
   {
   case WM_INITDIALOG: 
      return TRUE;
 
   case WM_COMMAND:
      switch(wParam)
         {
         case IDOK:
            EndDialog(hDlg, IDOK);
            break;

         case IDCANCEL:
            EndDialog(hDlg, IDOK);
            break;
         }
      return TRUE;
    } 
    
return FALSE;
}

/******************************************************************************
   ChildDialogProc

******************************************************************************/
	LRESULT CALLBACK ChildDialogProc(HWND hDlg,UINT message,UINT wParam,LONG lParam)
	{
		switch (message)
		{
		case WM_INITDIALOG: 
			{
					ShowWindow(hDlg , SW_SHOW);
					return (TRUE);
			}
		}
		return (FALSE);   

	}
