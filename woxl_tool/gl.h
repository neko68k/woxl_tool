#ifndef __GLA_H__
#define __GLA_H__

#include "stdafx.h"
#include <gl/GL.h>

int InitGL(void);
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
void KillGLWindow(void);

GLvoid ReSizeGLScene(GLsizei width, GLsizei height);

#endif