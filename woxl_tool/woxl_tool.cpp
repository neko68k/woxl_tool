// woxl_tool.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "woxl_tool.h"
#include"resource.h"
#include <Shellapi.h>
#include <CommDlg.h>
#include <shlobj.h>        //for Shell API
#include <Shlwapi.h> //for Shell API

#include "MDI.h"
#include "CMP.h"
#include "WAD.h"
#include "PRM.h"
#include "TRACK.h"


#define MAX_LOADSTRING 100


HWND g_hMainWindow;
//HWND hWnd;
HWND g_hMDIClient;


MDI *mdiClient = NULL;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void OpenFile();



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WOXL_TOOL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	/*LPWSTR* CommandLineToArgvW(
  __in   LPCWSTR lpCmdLine,
  __out  int *pNumArgs
);*/

	/*LPWSTR *argv;
	int argc;
	argv = CommandLineToArgvW(GetCommandLine(), &argc);
	*/
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WOXL_TOOL));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateMDISysAccel(g_hMDIClient, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		/*if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
		if(mdiClient)
			mdiClient->Update();
	}

	return (int) msg.wParam;
}

void OpenTrack()
{
       //standard use of the Shell API to browse for folders
       bool f_selected = false;
 
       char szDir [MAX_PATH];
       BROWSEINFO bi;        
       LPITEMIDLIST pidl;        
       LPMALLOC pMalloc;
       if (SUCCEEDED (::SHGetMalloc (&pMalloc)))
       {
              ::ZeroMemory (&bi,sizeof(bi)); 
               
 
              bi.lpszTitle = "Choose a Wipeout/Wipeout XL track folder:";
              bi.hwndOwner = g_hMainWindow;
              bi.pszDisplayName = 0;           
              bi.pidlRoot = 0;
              bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
              bi.lpfn = NULL;      //no customization function
              bi.lParam = NULL;    //no parameters to the customization function
 
              pidl = ::SHBrowseForFolder(&bi);           
              if (pidl)
              {
                     if (::SHGetPathFromIDList (pidl, szDir))
                     {
                           f_selected = true;
                     }
                
                     pMalloc -> Free(pidl);
                     pMalloc -> Release();
              }     
       }
	TRACK_Load(szDir);       
}

void OpenFile(){
	OPENFILENAMEA ofn;
	char szFileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    ofn.hwndOwner = g_hMainWindow;
    ofn.lpstrFilter = "Wipeout Primitive Model (*.PRM)\0*.prm\0Wipeout Texture Archive (*.CMP)\0*.cmp\0Wipeout Track Sections (*.TRS)\0*.trs\0Wipeout Track Vertices (*.TRV)\0*.trv\0Wipeout Track Faces (*.TRF)\0*.trf\0Wipeout WAD Archive (*.WAD)\0*.wad\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "cmp";

    if(GetOpenFileNameA(&ofn)){
		char *ext = strrchr(szFileName, '.');
		if(!strcmpi(ext, ".trs")){

		}

		if(!strcmpi(ext, ".prm")){
			PRM* prm = new PRM(szFileName);			
		}

		if(!strcmpi(ext, ".wad")){
			WAD_Load(szFileName);
		}

		if(!strcmpi(ext, ".cmp"))
			CMP_Extract(szFileName);
		//return 0;
	}

	mdiClient->MDI_New("Test");
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WOXL_TOOL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_3DSHADOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WOXL_TOOL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   

	hInst = hInstance; // Store instance handle in our global variable

	g_hMainWindow = CreateWindowEx(WS_EX_LEFT, szWindowClass, szTitle,
	WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	NULL, NULL, hInstance, NULL);

	if (!g_hMainWindow)
	{
		return FALSE;
	}

	ShowWindow(g_hMainWindow, nCmdShow);
	UpdateWindow(g_hMainWindow);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		case WM_CREATE:
			
			CLIENTCREATESTRUCT ccs;
			// Find window menu where children will be listed
			//HMENU hmenu;            // top-level menu
			//if ((hmenu = LoadMenu(hInst, "ShortcutExample")) == NULL) 
			
			//ccs.hWindowMenu = GetSubMenu(hmenu, 3);
			ccs.hWindowMenu = NULL;
			// If the menus document list for more than 9 children didn't upset the child ID then I would use the line below
			//ccs.hWindowMenu = (HMENU)GetSubMenu(GetMenu(hWnd), 2); // if you use less than 10 windows then use this line
			ccs.idFirstChild = ID_MDI_FIRSTCHILD;

			g_hMDIClient = CreateWindowEx(WS_EX_CLIENTEDGE, "MDICLIENT", NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,                    //WS_VSCROLL and WS_HSCROLL do not work properly yet
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			hWnd, (HMENU)ID_MDI_CLIENT, hInst, (LPVOID)&ccs);			
			ShowWindow(g_hMDIClient, SW_SHOW);			
			mdiClient = new MDI(g_hMainWindow, g_hMDIClient, hInst);			
			return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{		
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_OPEN:
			OpenFile();
			break;
		case ID_FILE_OPENTRACK:
			OpenTrack();
			break;
		default:
			return DefFrameProc(hWnd, g_hMDIClient, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefFrameProc(hWnd, g_hMDIClient, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
