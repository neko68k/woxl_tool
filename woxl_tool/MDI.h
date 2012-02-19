#ifndef __MDI_H__
#define __MDI_H__

#include <Windows.h>
#include <stdio.h>

typedef struct CHILD                        // Child structure for each child window
{
    HDC hDC;                                // Individual device context
    HGLRC hRC;                              // Individual rendering context
    HWND hWnd;                              // Handle to child window
	char title[MAX_PATH];
} CHILD;

#define ID_MDI_CLIENT      4999             // ID for client menu
#define ID_MDI_FIRSTCHILD  50000            // Start ID for child windows

class MDI{
private:
	int g_iMaxChild;                          // Maximum number of child windows wanted
	int g_iNumChild;   
	CHILD* g_child;  
	HWND hMainWindow;
	HWND hMDIClient;
	RECT rectChild; 
	HINSTANCE hInst;
	char szChildClassName[MAX_PATH];
	RECT g_rectChild; 

	LRESULT CALLBACK MDIChildWndProc(HWND hWnd, UINT uMessage, 
		WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK MDIChildWndProcPassthru(HWND hWnd, UINT uMessage, 
		WPARAM wParam, LPARAM lParam){
			MDI* pParent;

		   // Get pointer to window
		   if(uMessage == WM_CREATE)
		   {
			  pParent = (MDI*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			  SetWindowLongPtr(hWnd,GWL_USERDATA,(LONG_PTR)pParent);
		   }
		   else
		   {
			  pParent = (MDI*)GetWindowLongPtr(hWnd,GWL_USERDATA);
			  if(!pParent) return DefWindowProc(hWnd,uMessage,wParam,lParam);
		   }

		   //pParent->m_hWnd = hWnd;
		   return pParent->MDIChildWndProc(hWnd, uMessage,wParam,lParam);
			}

	void Create();

public:
	MDI(HWND hMain, HWND hClient, HINSTANCE hi){
		g_iMaxChild = 50;                          // Maximum number of child windows wanted
		g_iNumChild = 0;   
		hMainWindow = hMain;
		hMDIClient = hClient;
		hInst = hi;
		sprintf(szChildClassName, "OpenGL");
		g_child = new CHILD[g_iMaxChild];
		
		WNDCLASSEX wc;
		// Create an OpenGL compatible window class
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wc.lpfnWndProc = (WNDPROC)MDIChildWndProcPassthru;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = NULL;//LoadIcon(hInst, MAKEINTRESOURCE(IDI_CHILDICON));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_3DSHADOW+1);    // Background color (Only seen if OGL fails)
		wc.lpszMenuName = NULL;
		wc.lpszClassName = szChildClassName;
		wc.hIconSm = LoadIcon(hInst, NULL);//MAKEINTRESOURCE(IDI_CHILDICON));

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL,"Failed to register the child window.","Error",MB_OK|MB_ICONEXCLAMATION);
			return;
		}

		}
	~MDI(){}

	void MDI_New(char *title);
	void Update();
};



#endif