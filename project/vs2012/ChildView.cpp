
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "SoftCG.h"
#include "ChildView.h"
#include "MainFrm.h"

#include <GLEW\glew.h>
#include <GLUT\glut.h>

#include <io.h>
#include <fcntl.h>

void InitConsole()
{
  int nRet= 0;
  FILE* fp;
  AllocConsole();
  nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
  fp = _fdopen(nRet, "w");
  *stdout = *fp;
  setvbuf(stdout, NULL, _IONBF, 0);
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

// CChildView

CChildView::CChildView()
	:app(NULL)
{
}

CChildView::~CChildView()
{
	if(::wglMakeCurrent (0,0) == FALSE)
    {
        MessageBox(_T("Could not make RC non-current"));
    }
    if(::wglDeleteContext (m_hRC)==FALSE)
    {
        MessageBox(_T("Could not delete RC"));
    }
    if(m_pDC)
    {
        delete m_pDC;
    }
    m_pDC = NULL;

	if(app)
	{
		delete app;
		app = NULL;
	}
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	
	/*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// 清除屏幕及深度缓存
	glBegin(GL_TRIANGLES);						// 绘制三角形
		glColor3f(1.0f,0.0f,0.0f);				// 设置当前色为红色
		glVertex3f( 0.0f, 100.0f, 0.0f);
		glColor3f(0.0f,1.0f,0.0f);				// 设置当前色为绿色
		glVertex3f(100.0f,100.0f, 0.0f);				// 左下
		glColor3f(0.0f, 0.0f, 0.3f * (rand() % 4) );				// 设置当前色为蓝色
		glVertex3f( 100.0f, 0, 0.0f);				// 右下
	glEnd();							// 三角形绘制结束
	*/

	SwapBuffers( m_pDC->GetSafeHdc());
}

BOOL CChildView::InitializeOpenGL()
{
    m_pDC = new CClientDC(this);
    if(m_pDC == NULL)
    {
        MessageBox(_T("Error Obtaining DC"));
        return FALSE;
    }
    if(!SetupPixelFormat())
    {
        return FALSE;
    }
    m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());
    if(m_hRC == 0)
    {
        MessageBox(_T("Error Creating RC"));
        return FALSE;
    }
    if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
    {
        MessageBox(_T("Error making RC Current"));
        return FALSE;
    }
    return TRUE;
}

BOOL CChildView::SetupPixelFormat()
{
  static PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |            // support OpenGL
        PFD_DOUBLEBUFFER,                // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        16,                             // 16-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
    if ( m_nPixelFormat == 0 )
    {
       return FALSE;
    }
    if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
    {
       return FALSE;
    }
    return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  InitConsole();
	InitializeOpenGL();
	glewInit();
	app = new App(600, 600);
	SetTimer(1, 20, NULL);

	return 0;
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, cx, 0, cy);
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵
	glLoadIdentity();							// 重置模型观察矩阵
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CWnd::OnEraseBkgnd(pDC);
	return true;
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnTimer(nIDEvent);
	app->Update(0.02);
	OnPaint();

  CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
  CString str;
  str.Format(_T("FPS: %.2f"), app->GetFPS());
  LPCTSTR re=str.AllocSysString();
  pFrame->GetStatusBar()->SetPaneText(0, re);
}
