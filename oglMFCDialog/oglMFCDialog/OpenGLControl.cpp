#include "stdafx.h"
#include "OpenGLControl.h"
#include <auto_link_freeglut.hpp>
#include <GL/glut.h>

COpenGLControl::COpenGLControl(void)
{
	char* a[]= {" "};
	int i=1;
	glutInit(&i, a);
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 10.0f;	// Zoom on model in camera view
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY	= 0.0f;		// Rotation on model in camera view
	m_bIsMaximized = false;
	m_shoulder = 0;
	m_elbow = 0;
	m_base = 0;
}

COpenGLControl::~COpenGLControl(void)
{
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set our current view perspective
	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
		// If window resize token is "maximize"
		case SIZE_MAXIMIZED:
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly
			MoveWindow(6, 6, cx - 14, cy - 14);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;

			break;
		}

		// If window resize token is "restore"
		case SIZE_RESTORED:
		{
			// If the window is currently maximized
			if (m_bIsMaximized)
			{
				// Get the current window rect
				GetWindowRect(m_rect);

				// Move the window accordingly (to our stored old window)
				MoveWindow(m_oldWindow.left, m_oldWindow.top - 18, m_originalRect.Width() - 4, m_originalRect.Height() - 4);

				// Get the new window rect
				GetWindowRect(m_rect);

				// Store our old window as the new rect
				m_oldWindow = m_rect;
			}
		
			break;
		}
	}
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	oglInitialize();

	return 0;
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	// If the current view is perspective...
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void COpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			wglMakeCurrent(hdc, hrc);
			// Clear color and depth buffer bits
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw OpenGL scene
			oglDrawScene();

			// Swap buffers
			SwapBuffers(hdc);

			break;
		}

		default:
			break;
	}

	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX  = (float)point.x;
	m_fLastY  = (float)point.y;

	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		m_fRotX += (float)0.5f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.5f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.1f * diffY;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;
	}	

	CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, L"OpenGL", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow	= rect;
	m_originalRect	= rect;

	hWnd = parent;
}

void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;
	
	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	
	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Send draw request
	OnDraw(NULL);
}

void COpenGLControl::oglDrawScene(void)
{
	// Wireframe Mode
	GLUquadricObj *obj = gluNewQuadric();
	glClear (GL_COLOR_BUFFER_BIT);
	    
	glPushMatrix();
	//draw NO.13
	glPushMatrix();
	glTranslatef(5.705,1.5,0.0);
	glScalef(1.3,0.6,0.5);
	glutWireCube(1.0);
	glPopMatrix();
    //draw NO.14
	glPushMatrix();
	glTranslatef(6.115,1.8,0.0);
	glRotatef(90.0f,1.0,0.0,0.0);

	gluCylinder(obj,0.18,0.18,3.40,25,25) ;
	glPopMatrix();
	// draw NO.15
	glPushMatrix();
	glTranslatef(6.115,-1.06,0.0);
	glRotatef(90.0f,1.0,0.0,0.0);

	gluCylinder(obj,0.28,0.28,0.54,25,25) ;
	glPopMatrix();
	//draw NO.16
	glPushMatrix();
	glTranslatef(6.555,-1.9,0);
	glScalef(0.5,0.6,5.86);
	glutWireCube(1.0);	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.665,-1.9,0);
	glScalef(0.5,0.6,5.86);
	glutWireCube(1.0);	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.385,-1.9,-3.18);
	glScalef(2.86,0.6,0.5);
	glutWireCube(1.0);	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.385,-1.9,3.18);
	glScalef(2.86,0.6,0.5);
	glutWireCube(1.0);	
	glPopMatrix();

	//draw NO.12
	glPushMatrix();
	glTranslatef(5.33,0.72,0.0);
	glRotatef ((GLfloat) m_base, 0.0, 1.0, 0.0);
	glTranslatef(-5.33,0.72,0.0);
	glPushMatrix();
	glTranslatef(5.33,0.72,0.0);
	glRotatef(90.0f,1.0,0.0,0.0);

	gluCylinder(obj,0.275,0.275,0.38,25,25) ;
	glPopMatrix();
	
	//draw NO.9
	glPushMatrix();
	glTranslatef(1.97,1.0,0);
	glScalef(0.56,0.56,0.72);
	glutWireCube(1.0);	
	glPopMatrix();
    //draw NO.10
	glPushMatrix();
    glTranslatef(2.25,1.15,0.2);
	glRotatef(90.0f,0.0,1.0,0.0);
	gluCylinder(obj,0.01,0.01,2.8,25,25);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.25,1.15,-0.2);
	glRotatef(90.0f,0.0,1.0,0.0);
	gluCylinder(obj,0.01,0.01,2.8,25,25);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.25,0.85,0.2);
	glRotatef(90.0f,0.0,1.0,0.0);
	gluCylinder(obj,0.01,0.01,2.8,25,25);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.25,0.85,-0.2);
	glRotatef(90.0f,0.0,1.0,0.0);
	gluCylinder(obj,0.01,0.01,2.8,25,25);
	glPopMatrix();
    //draw NO.11
	glPushMatrix();
	glTranslatef(5.33,1.0,0.0);
	glScalef(0.5,0.5,0.5);
	glutWireCube(1.0);	
	glPopMatrix();
	//draw NO.8
	glPushMatrix();
	glTranslatef(1.97,0.22,0.0);
    glRotatef ((GLfloat) m_elbow, 0.0, 1.0, 0.0);
	glTranslatef(-1.97,0.22,0.0);
	glPushMatrix();
	glTranslatef(1.97,0.22,0.0);
	glRotatef(90.0f,1.0,0.0,0.0);

	gluCylinder(obj,0.2,0.2,0.3,25,25) ;
	glPopMatrix();
	//rotate angle of NO.1
	/*
	glTranslatef (0.75, 0.0, 0.0);
	glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
	glTranslatef (0.75, 0.0, 0.0); */
	// draw NO.1
	glPushMatrix();
	glTranslatef (0.48, 0.0, 0.0); 
	glScalef (0.36, 0.54, 0.4);
	
	glutWireCube (1.0);
	glPopMatrix();
	//draw NO.6
	glPushMatrix();
	glTranslatef(0.66,-0.20,0.15);
	glRotatef(90.0f,0.0,1.0,0.0);
	gluCylinder(obj,0.05,0.05,1.1,25,25) ;
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.66,-0.20,-0.15);
	glRotatef(90.0f,0.0,1.0,0.0);
	gluCylinder(obj,0.05,0.05,1.1,25,25) ;
	glPopMatrix();
	//draw no.7
	glPushMatrix();
	glTranslatef(1.97,-0.20,0);
	glScalef(0.42,0.24,0.42);
	glutWireCube(1.0);
	glPopMatrix();
	
	
	glRotatef ((GLfloat) m_shoulder, 1.0, 0.0, 0.0);
	//draw NO.2
	glPushMatrix();
	glRotatef(90.0f,0.0,1.0,0.0);
	
	gluCylinder(obj,0.2,0.2,0.3,25,25) ;
	glPopMatrix();
	//draw NO.4
	glPushMatrix();
	glTranslatef(-0.10,0.0,0.0);
    glScalef(0.20,0.26,0.46);
	glutWireCube(1.0);
	glPopMatrix();
    // draw NO.3
	glPushMatrix();
    glTranslatef(-0.81,0,-0.27);
	glScalef(1.62,0.26,0.08);
	glutWireCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.81,0,0.27);
	glScalef(1.62,0.26,0.08);
	glutWireCube(1.0);
	glPopMatrix();
    glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}