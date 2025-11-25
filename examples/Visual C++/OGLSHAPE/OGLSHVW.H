// oglshvw.h : interface of the COglshapeView class
//
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
///////////////////////////////////////
// START CUSTOM CODE
///////////////////////////////////////
///////////////////////////////////////

#include <gl\gl.h>

enum {Points, Lines, LineStrip, LineLoop, Polygons, Nonconvex,
      Triangles, TriangleStrip, TriangleFan, Quads, QuadStrip
     };

///////////////////////////////////////
///////////////////////////////////////
// END CUSTOM CODE
///////////////////////////////////////
///////////////////////////////////////

class COglshapeView : public CView {
protected: // create from serialization only
    COglshapeView();
    DECLARE_DYNCREATE(COglshapeView)

    // Attributes
public:
    COglshapeDoc* GetDocument();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

protected:
    HGLRC m_hRC;
    int m_shape;

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COglshapeView)
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~COglshapeView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    ///////////////////////////////////////
    ///////////////////////////////////////
    // START CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    void DrawWithOpenGL();
    void DrawPoints();
    void DrawLines();
    void DrawLineStrip();
    void DrawLineLoop();
    void DrawPolygons();
    void DrawNonconvex();
    void DrawTriangles();
    void DrawTriangleStrip();
    void DrawTriangleFan();
    void DrawQuads();
    void DrawQuadStrip();

    ///////////////////////////////////////
    ///////////////////////////////////////
    // END CUSTOM CODE
    ///////////////////////////////////////
    ///////////////////////////////////////

    // Generated message map functions
protected:
    //{{AFX_MSG(COglshapeView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnDrawLineloop();
    afx_msg void OnUpdateDrawLineloop(CCmdUI* pCmdUI);
    afx_msg void OnDrawLines();
    afx_msg void OnUpdateDrawLines(CCmdUI* pCmdUI);
    afx_msg void OnDrawLinestrip();
    afx_msg void OnUpdateDrawLinestrip(CCmdUI* pCmdUI);
    afx_msg void OnDrawNonconvex();
    afx_msg void OnUpdateDrawNonconvex(CCmdUI* pCmdUI);
    afx_msg void OnDrawPoints();
    afx_msg void OnUpdateDrawPoints(CCmdUI* pCmdUI);
    afx_msg void OnDrawPolygons();
    afx_msg void OnUpdateDrawPolygons(CCmdUI* pCmdUI);
    afx_msg void OnDrawQuadrilaterals();
    afx_msg void OnUpdateDrawQuadrilaterals(CCmdUI* pCmdUI);
    afx_msg void OnDrawQuadstrip();
    afx_msg void OnUpdateDrawQuadstrip(CCmdUI* pCmdUI);
    afx_msg void OnDrawTrianglefan();
    afx_msg void OnUpdateDrawTrianglefan(CCmdUI* pCmdUI);
    afx_msg void OnDrawTriangles();
    afx_msg void OnUpdateDrawTriangles(CCmdUI* pCmdUI);
    afx_msg void OnDrawTrianglestrip();
    afx_msg void OnUpdateDrawTrianglestrip(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in oglshvw.cpp
inline COglshapeDoc* COglshapeView::GetDocument() {
    return (COglshapeDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////
