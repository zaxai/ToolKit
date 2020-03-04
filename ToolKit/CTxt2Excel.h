#pragma once


// CTxt2Excel 对话框

class CTxt2Excel : public CDialogEx, public ZExcelImport, public ZExcelExport
{
	DECLARE_DYNAMIC(CTxt2Excel)

public:
	CTxt2Excel(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTxt2Excel();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TXT2EXCEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_comboInputFormat;
	CComboBox m_comboOutputFormat;
	CListCtrl m_listShow;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnCbnSelchangeComboInputformat();
	afx_msg void OnCbnSelchangeComboOutputformat();
	virtual void OnImportProgressUpdate(int nProgress);
	virtual void OnImportResult(bool bResult);
	void ReadFromTxt(const CString& strSplit);
	void InsertList();
	virtual void OnExportProgressUpdate(int nProgress);
	virtual void OnExportResult(bool bResult);
	void WriteToTxt(const CString& strSplit);
	afx_msg void OnDestroy();
};
