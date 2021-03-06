// oglshvw.cpp : implementation of the COglshapeView class
//

#include "stdafx.h"
#include "oglshape.h"

#include "oglshdoc.h"
#include "oglshvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COglshapeView

IMPLEMENT_DYNCREATE(COglshapeView, CView)

BEGIN_MESSAGE_MAP(COglshapeView, CView)
    //{{AFX_MSG_MAP(COglshapeView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_COMMAND(ID_DRAW_LINELOOP, OnDrawLineloop)
    ON_UPDATE_COMMAND_UI(ID_DRAW_LINELOOP, OnUpdateDrawLineloop)
    ON_COMMAND(ID_DRAW_LINES, OnDrawLines)
    ON_UPDATE_COMMAND_UI(ID_DRAW_LINES, OnUpdateDrawLines)
    ON_COMMAND(ID_DRAW_LINESTRIP, OnDrawLinestrip)
    ON_UPDATE_COMMAND_UI(ID_DRAW_LINESTRIP, OnUpdateDrawLinestrip)
    ON_COMMAND(ID_DRAW_NONCONVEX, OnDrawNonconvex)
    ON_UPDATE_COMMAND_UI(ID_DRAW_NONCONVEX, OnUpdateDrawNonconvex)
    ON_COMMAND(ID_DRAW_POINTS, OnDrawPoints)
    ON_UPDATE_COMMAND_UI(ID_DRAW_POINTS, OnUpdateDrawPoints)
    ON_COMMAND(ID_DRAW_POLYGONS, OnDrawPolygons)
    ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGONS, OnUpdateDrawPolygons)
    ON_COMMAND(ID_DRAW_QUADRILATERALS, OnDrawQuadrilaterals)
    ON_UPDATE_COMMAND_UI(ID_DRAW_QUADRILATERALS, OnUpdateDrawQuadrilaterals)
    ON_COMMAND(ID_DRAW_QUADSTRIP, OnDrawQuadstrip)
    ON_UPDATE_COMMAND_UI(ID_DRAW_QUADSTRIP, OnUpdateDrawQuadstrip)
    ON_COMMAND(ID_DRAW_TRIANGLEFAN, OnDrawTrianglefan)
    ON_UPDATE_COMMAND_UI(ID_DRAW_TRIANGLEFAN, OnUpdateDrawTrianglefan)
    ON_COMMAND(ID_DRAW_TRIANGLES, OnDrawTriangles)
    ON_UPDATE_COMMAND_UI(ID_DRAW_TRIANGLES, OnUpdateDrawTriangles)
    ON_COMMAND(ID_DRAW_TRIANGLESTRIP, OnDrawTrianglestrip)
    ON_UPDATE_COMMAND_UI(ID_DRAW_TRIANGLESTRIP, OnUpdateDrawTrianglestrip)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COglshapeView construction/destruction

COglshapeView::COglshapeView() {
    // TODO: add construction code here

}

COglshapeView::~COglshapeView() {
}

/////////////////////////////////////////////////////////////////////////////
// COglshapeView drawing

void COglshapeView::OnDraw(CDC* pDC) {
    COglshapeDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // TODO: add draw code for native data here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    wglMakeCurrent(pDC->m_hDC, m_hRC);
    DrawWithOpenGL();
    wglMakeCurrent(pDC->m_hDC, NULL);

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////
// COglshapeView diagnostics

#ifdef _DEBUG
void COglshapeView::AssertValid() const {
    CView::AssertValid();
}

void COglshapeView::Dump(CDumpContext& dc) const {
    CView::Dump(dc);
}

COglshapeDoc* COglshapeView::GetDocument() { // non-debug version is inline
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COglshapeDoc)));
    return (COglshapeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COglshapeView message handlers

int COglshapeView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CView::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    // TODO: Add your specialized creation code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), // Structure size.
        1,                             // Structure version number.
        PFD_DRAW_TO_WINDOW |           // Property flags.
        PFD_SUPPORT_OPENGL,
        PFD_TYPE_RGBA,
        24,                            // 24-bit color.
        0, 0, 0, 0, 0, 0,              // Not concerned with these.
        0, 0, 0, 0, 0, 0, 0,           // No alpha or accum buffer.
        32,                            // 32-bit depth buffer.
        0, 0,                          // No stencil or aux buffer.
        PFD_MAIN_PLANE,                // Main layer type.
        0,                             // Reserved.
        0, 0, 0                        // Unsupported.
    };

    CClientDC clientDC(this);

    int pixelFormat =
        ChoosePixelFormat(clientDC.m_hDC, &pfd);
    BOOL success =
        SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd);

    m_hRC = wglCreateContext(clientDC.m_hDC);

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    return 0;
}

void COglshapeView::OnDestroy() {
    CView::OnDestroy();

    // TODO: Add your message handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    wglDeleteContext(m_hRC);

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

BOOL COglshapeView::PreCreateWindow(CREATESTRUCT& cs) {
    // TODO: Add your specialized code here and/or call the base class

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    return CView::PreCreateWindow(cs);
}

void COglshapeView::OnDrawLineloop() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = LineLoop;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawLineloop(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == LineLoop) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawLines() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = Lines;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawLines(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == Lines) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawLinestrip() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = LineStrip;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawLinestrip(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == LineStrip) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawNonconvex() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = Nonconvex;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawNonconvex(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == Nonconvex) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawPoints() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = Points;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawPoints(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == Points) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawPolygons() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = Polygons;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawPolygons(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == Polygons) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawQuadrilaterals() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = Quads;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawQuadrilaterals(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == Quads) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawQuadstrip() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = QuadStrip;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawQuadstrip(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == QuadStrip) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawTrianglefan() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = TriangleFan;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawTrianglefan(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == TriangleFan) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawTriangles() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = Triangles;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawTriangles(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == Triangles) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnDrawTrianglestrip() {
    // TODO: Add your command handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    m_shape = TriangleStrip;
    Invalidate();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::OnUpdateDrawTrianglestrip(CCmdUI* pCmdUI) {
    // TODO: Add your command update UI handler code here

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    if (m_shape == TriangleStrip) {
        pCmdUI->SetCheck(TRUE);
    } else {
        pCmdUI->SetCheck(FALSE);
    }

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

///////////////////////////////////////
///////////////////////////////////////
// START CUSTOM CODE
///////////////////////////////////////
///////////////////////////////////////

void COglshapeView::DrawWithOpenGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    switch(m_shape) {
        case Points        :
            DrawPoints();
            break;

        case Lines         :
            DrawLines();
            break;

        case LineStrip     :
            DrawLineStrip();
            break;

        case LineLoop      :
            DrawLineLoop();
            break;

        case Polygons      :
            DrawPolygons();
            break;

        case Triangles     :
            DrawTriangles();
            break;

        case TriangleStrip :
            DrawTriangleStrip();
            break;

        case TriangleFan   :
            DrawTriangleFan();
            break;

        case Quads         :
            DrawQuads();
            break;

        case QuadStrip     :
            DrawQuadStrip();
            break;

        case Nonconvex     :
            DrawNonconvex();
            break;
    }

    glFlush();
}

void COglshapeView::DrawPoints() {
    GLfloat ps[2];
    glGetFloatv(GL_POINT_SIZE_RANGE, ps);

    glPointSize(ps[1]);

    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.75f, 0.75f);
    glVertex2f(-0.75f, -0.75f);
    glVertex2f(-0.75f, 0.75f);
    glVertex2f(0.75f, -0.75f);
    glEnd();
}

void COglshapeView::DrawLines() {
    GLfloat lw[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, lw);

    if (lw[1] >= 4.0f) {
        glLineWidth(4.0f);
    } else {
        glLineWidth(lw[1]);
    }

    glBegin(GL_LINES);

    glVertex2f(-0.75f, 0.75f);
    glVertex2f(0.75f, 0.75f);

    glVertex2f(-0.75f, 0.25f);
    glVertex2f(0.75f, 0.25f);

    glEnd();

    glLineStipple(1, 0x4444);
    glEnable(GL_LINE_STIPPLE);

    glBegin(GL_LINES);

    glVertex2f(-0.75f, -0.25f);
    glVertex2f(0.75f, -0.25f);

    glVertex2f(-0.75f, -0.75f);
    glVertex2f(0.75f, -0.75f);

    glEnd();

    glDisable(GL_LINE_STIPPLE);
}

void COglshapeView::DrawLineStrip() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::DrawLineLoop() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::DrawPolygons() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::DrawNonconvex() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::DrawTriangles() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::DrawTriangleStrip() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

}

void COglshapeView::DrawTriangleFan() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::DrawQuads() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

void COglshapeView::DrawQuadStrip() {
    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////


    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////
}

///////////////////////////////////////
///////////////////////////////////////
// END CUSTOM CODE
///////////////////////////////////////
///////////////////////
