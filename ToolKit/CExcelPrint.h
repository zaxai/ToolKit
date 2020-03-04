#pragma once


// CExcelPrint 对话框

class CExcelPrint : public CDialogEx
{
	DECLARE_DYNAMIC(CExcelPrint)

public:
	CExcelPrint(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CExcelPrint();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXCELPRINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEditEx m_editPath;
	CEditEx m_editRow;
	CEditEx m_editColumn;
	CEditEx m_editCopies;
	CEditEx m_editInput;
	CString m_strFilePath;
	CString m_strInput;
	long m_lRow;
	long m_lColumn;
	long m_lCopies;
	std::vector<CString> m_vec_strText;
	std::thread m_threadPrint;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonPrint();
	void PrintThread();
	afx_msg void OnDestroy();
};
