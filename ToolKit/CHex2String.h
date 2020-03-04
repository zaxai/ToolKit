#pragma once


// CHex2String 对话框

class CHex2String : public CDialogEx
{
	DECLARE_DYNAMIC(CHex2String)

public:
	CHex2String(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHex2String();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEX2STRING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CButton m_btnRemoveSpace;
	CEditEx m_editInput;
	CEditEx m_editOutput;
public:
	afx_msg void OnBnClickedButtonHex2string();
	afx_msg void OnBnClickedButtonString2hex();
};
