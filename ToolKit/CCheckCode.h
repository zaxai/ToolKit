#pragma once


// CCheckCode 对话框

class CCheckCode : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckCode)

public:
	CCheckCode(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCheckCode();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKCODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEditEx m_editOutput;
	std::thread m_threadCalculate;
	bool m_bIsCalculating;
	CString m_strInput;
	CString m_strOutput;
	int m_nNum;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonCalculate();
	void CalculateBarcode();
	void StopCalculate();
	CString BigNumAdd(CString strBigNum, int i);
	CString BigNumAdd(CString strFirst, CString strSecond);
	CString GetCheckCode(const CString& strBarcode);
	afx_msg void OnDestroy();
};
