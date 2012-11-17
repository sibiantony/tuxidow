/****************************************************************
 
FILE : GUIMESSAGES.H
DETAILS : HANDLES ALL GUI MESSAGES
CONTACT : tuxidow@yahoo.com

*****************************************************************/
#ifndef __MESSAGES_H
#define __MESSAGES_H

#include "Tuxidow.h"
#include "gui.h"

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT HandleNotifyMessages(HWND , LPARAM );
LRESULT CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ProgressDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildDialogProc(HWND,UINT,UINT ,LONG);
#endif

