// oglshdoc.h : interface of the COglshapeDoc class
//
/////////////////////////////////////////////////////////////////////////////

class COglshapeDoc : public CDocument {
protected: // create from serialization only
    COglshapeDoc();
    DECLARE_DYNCREATE(COglshapeDoc)

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COglshapeDoc)
public:
    virtual BOOL OnNewDocument();
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~COglshapeDoc();
    virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // Generated message map functions
protected:
    //{{AFX_MSG(COglshapeDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
