#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>
#include "MDI.h"

#include "gl.h"



void MDI::MDI_New(char *title){

	HWND hChild;

	if(g_iNumChild < g_iMaxChild)      // If the number of children hasn't reached the maximum
		g_iNumChild++;              // Increment the number of children
	else
		return;                   // Otherwise break out of the routine

	char szWindowTitle[20];
	sprintf(szWindowTitle, title);	

/* One way to create a child window is to send a WM_MDICREATE message to the client
window and pass it a pointer to a MDICREATESTRUCT. */

/*
	MDICREATESTRUCT mcs;
	mcs.szTitle = szWindowTitle;
	mcs.szClass = g_szChildClassName;
	mcs.hOwner  = g_hInst;
	mcs.x = mcs.cx = CW_USEDEFAULT;
	mcs.y = mcs.cy = CW_USEDEFAULT;
	mcs.style = MDIS_ALLCHILDSTYLES;

	hChild = (HWND)SenduMessage(g_hMDIClient, WM_MDICREATE, 0, (LONG)&mcs);
//*/

/* Another way to create a child window is to use CreateWindowEx and set the extended
windows style  to WS_EX_MDICHILD. I prefer this method as it will allow each child to be created in
seperate threads in the future. */

//*
	CREATESTRUCT cs;
	ZeroMemory(&cs, sizeof(CREATESTRUCT));

	hChild = CreateWindowEx(
	WS_EX_MDICHILD,
	szChildClassName,
	szWindowTitle,
	WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	hMDIClient,
	NULL,
	hInst,
	this
	);
//*/
	if(!hChild)
	{
		MessageBox(NULL, "Child creation failed.", "Error",
		MB_ICONEXCLAMATION | MB_OK);
	}

	g_child[g_iNumChild - 1].hWnd = hChild;         // Set child windows handle
	g_child[g_iNumChild - 1].hDC = GetDC(hChild);   // Set child windows device context

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat(g_child[g_iNumChild - 1].hDC, &pfd);
	SetPixelFormat(g_child[g_iNumChild - 1].hDC, iFormat, &pfd);               // Set pixel format for this child

	g_child[g_iNumChild - 1].hRC = wglCreateContext(g_child[g_iNumChild - 1].hDC);  // Set childs rendering context
	wglMakeCurrent(g_child[g_iNumChild - 1].hDC, g_child[g_iNumChild - 1].hRC);     // Make this child the current DC and RC    
	ShowWindow(hChild, SW_SHOW);
}

void MDI::Update(){
	if(g_iNumChild)                             // If we have some children
        {
            for(int iLoop = 0; iLoop < g_iNumChild; iLoop++)   // Loop through all children
            {
                if(!IsIconic(g_child[iLoop].hWnd))   // If child isn't iconic (minimized)
                {
                    wglMakeCurrent(g_child[iLoop].hDC, g_child[iLoop].hRC);  // Make them the current DC and RC
                    GetClientRect(g_child[iLoop].hWnd, &g_rectChild);
                    if(g_rectChild.right > 0 && g_rectChild.bottom > 0)
                        ReSizeGLScene(g_rectChild.right, g_rectChild.bottom);        // Set the childs projection matrix
                    //DrawGLScene();
					glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glFlush();
                    SwapBuffers(g_child[iLoop].hDC);
                }
            }
        }
}
