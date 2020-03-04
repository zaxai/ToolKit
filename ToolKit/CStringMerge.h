#pragma once


// CStringMerge 对话框

class CStringMerge : public CDialogEx
{
	DECLARE_DYNAMIC(CStringMerge)

public:
	CStringMerge(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStringMerge();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STRINGMERGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEditEx m_editInput;
	CEditEx m_editOutput;
public:
	afx_msg void OnBnClickedButtonMerge();
};
