#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>


#include "MDI.h"

LRESULT CALLBACK MDI::MDIChildWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    switch(uMessage)
    {
        case WM_LBUTTONDOWN:
           // g_bMouseDrag = 1;                // Set mouse flag
            //g_iMouseLastX = LOWORD(lParam);  // Get mouse position
           // g_iMouseLastY = HIWORD(lParam);
           // SetCapture(hWnd);             // Capture the mouse
        break;

        case WM_MOUSEMOVE:
            /*if(g_bMouseDrag)                  // If mouse flag set
            {
                g_iMouseDeltaX = LOWORD(lParam) - g_iMouseLastX;   // Get mouse deltas
                g_iMouseDeltaY = HIWORD(lParam) - g_iMouseLastY;
                g_iMouseLastX = LOWORD(lParam);                 // Get new mouse position
                g_iMouseLastY = HIWORD(lParam);
            }*/
        break;

        case WM_LBUTTONUP:         // If left button is released
            //g_bMouseDrag = 0;         // Clear mouse flag
           // ReleaseCapture();      // Release the mouse capture
        break;

/*
        //This msg isn't posted if the mouse is captured
        case WM_NCMOUSEMOVE:
        {
            POINTS pts;
            if(g_bMouseDrag)
            {
                pts = MAKEPOINTS(lParam);
                g_iMouseDeltaX = pts.x - g_iMouseLastX;
                g_iMouseDeltaY = pts.y - g_iMouseLastY;
                g_iMouseLastX = pts.x;
                g_iMouseLastY = pts.y;
            }
        }
        break;
//*/

/* The following code paints the child window when necessary but isn't needed as we
repaint all of them each frame. */

/*
        case WM_PAINT:
        {
            int iThisChild;
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            iThisChild = GetDlgCtrlID(hWnd) - ID_MDI_FIRSTCHILD;
            wglMakeCurrent(g_child[iThisChild].hDC, g_child[iThisChild].hRC);
            GetClientRect(hWnd, &g_rectChild);
            if(g_rectChild.right > 0 && g_rectChild.bottom > 0)
                SetProjection(g_rectChild.right, g_rectChild.bottom);
            DrawGLScene();
            glFlush();
            SwapBuffers(hDC);
            EndPaint(hWnd, &ps);
        }
        break;
//*/
        case WM_CLOSE:
            g_iNumChild--;                       // Decrement the number of child windows
            if(g_iNumChild == 0)                 // If this is the last child window then just free the DC and RC
            {
                wglMakeCurrent( NULL, NULL );
                ReleaseDC(g_child[0].hWnd, g_child[0].hDC);
                wglDeleteContext(g_child[0].hRC);
            }
            else
            {
                int iLoop;
                int iThisChild;
                char szWindowTitle[20];
                iThisChild = GetDlgCtrlID(hWnd) - ID_MDI_FIRSTCHILD;
                ReleaseDC(g_child[iThisChild].hWnd, g_child[iThisChild].hDC);    // Free this childs DC and RC
                wglDeleteContext(g_child[iThisChild].hRC);
                if(iThisChild != g_iNumChild)                  // If this child isn't the last in the array of children
                {
                    for (iLoop = iThisChild; iLoop < (g_iNumChild); iLoop++)   // Loop from this child to the end of the array
                    {
                        g_child[iLoop] = g_child[iLoop + 1];       // Shift the children forward in the array
                        sprintf(szWindowTitle, "%d", iLoop);
                        SetWindowText(g_child[iLoop].hWnd, szWindowTitle);  // Renumber the children
                    }
                }
            }
        break;

        case WM_MDIACTIVATE:
        {
            HMENU hMenu, hFileMenu;
            UINT uEnableFlag;

            hMenu = GetMenu(hMainWindow);
            if(hWnd == (HWND)lParam)       //being activated
            {
                uEnableFlag = MF_ENABLED;
            }
            else
            {
                uEnableFlag = MF_GRAYED;    //being de-activated
            }
            EnableMenuItem(hMenu, 1, MF_BYPOSITION | uEnableFlag);
            EnableMenuItem(hMenu, 2, MF_BYPOSITION | uEnableFlag);

            hFileMenu = GetSubMenu(hMenu, 0);
            //EnableMenuItem(hFileMenu, CM_FILE_SAVE, MF_BYCOMMAND | uEnableFlag);
            //EnableMenuItem(hFileMenu, CM_FILE_SAVEAS, MF_BYCOMMAND | uEnableFlag);

            DrawMenuBar(hMainWindow);
        }
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
//              case CM_FILE_SAVE: etc
            }
        return 0;
    }
    return DefMDIChildProc(hWnd, uMessage, wParam, lParam);
}