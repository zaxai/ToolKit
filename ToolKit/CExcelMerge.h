#pragma once


// CExcelMerge 对话框

class CExcelMerge : public CDialogEx, public ZExcelImport, public ZExcelExport
{
	DECLARE_DYNAMIC(CExcelMerge)

public:
	CExcelMerge(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CExcelMerge();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXCELMERGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_boxShow;
	CButton m_btnRemoveFirstRow;
	bool m_bIsRemoveFirstRow;
	CString m_strFolderPath;
	std::vector<CString> m_vec_strPaths;
	std::vector<CString>::iterator m_itPaths;
	std::vector<std::vector<CString>> m_vec2_strData;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonMerge();
	afx_msg void OnBnClickedCheckRemovefirstrow();
	void InsertFolder();
	virtual void OnImportProgressUpdate(int nProgress);
	virtual void OnImportResult(bool bResult);
	virtual void OnExportProgressUpdate(int nProgress);
	virtual void OnExportResult(bool bResult);
};
