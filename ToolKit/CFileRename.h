#pragma once


// CFileRename 对话框
#include <regex>

class CFileRename : public CDialogEx
{
	DECLARE_DYNAMIC(CFileRename)

public:
	CFileRename(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileRename();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILERENAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strFolderPath;
	ZListCtrl m_listShow;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonRename();
	void InsertFolder();
};
