// CCheckCode.cpp: 实现文件
//

#include "pch.h"
#include "ToolKit.h"
#include "CCheckCode.h"
#include "afxdialogex.h"


// CCheckCode 对话框

IMPLEMENT_DYNAMIC(CCheckCode, CDialogEx)

CCheckCode::CCheckCode(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECKCODE, pParent)
	, m_bIsCalculating(false)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
	, m_nNum(0)
{

}

CCheckCode::~CCheckCode()
{
	StopCalculate();
}

void CCheckCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_editOutput);
}


BEGIN_MESSAGE_MAP(CCheckCode, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CCheckCode::OnBnClickedButtonCalculate)
	ON_MESSAGE(WM_MSGRECVPRO, &CCheckCode::OnMsgrecvpro)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCheckCode 消息处理程序


void CCheckCode::OnBnClickedButtonCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strOutput.Empty();
	SetDlgItemText(IDC_EDIT_OUTPUT, m_strOutput);
	GetDlgItemText(IDC_EDIT_INPUT, m_strInput);
	m_nNum = GetDlgItemInt(IDC_EDIT_NUM);
	StopCalculate();
	m_threadCalculate = std::thread(&CCheckCode::CalculateBarcode, this);
	GetDlgItem(IDC_BUTTON_CALCULATE)->EnableWindow(FALSE);
}


void CCheckCode::CalculateBarcode()
{
	m_bIsCalculating = true;
	CString strBarcode, strFullBarcode;
	int nProgress = 0;
	for (int i = 0; i < m_nNum && m_bIsCalculating; ++i)
	{
		strBarcode = BigNumAdd(m_strInput, i);
		strFullBarcode = strBarcode + GetCheckCode(strBarcode);
		m_strOutput += strFullBarcode + _T("\r\n");
		nProgress = (i + 1) * 100 / m_nNum;
		if (GetParent())
			GetParent()->PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_UPDATEPROGRESS);
	}
	PostMessage(WM_MSGRECVPRO, (WPARAM)m_bIsCalculating, MSGUSER_CALCULATERESULT);
	m_bIsCalculating = false;
}


void CCheckCode::StopCalculate()
{
	m_bIsCalculating = false;
	if (m_threadCalculate.joinable())
		m_threadCalculate.join();
}


CString CCheckCode::BigNumAdd(CString strBigNum, int i)
{
	CString str;
	str.Format(_T("%d"), i);
	return BigNumAdd(strBigNum, str);
}


CString CCheckCode::BigNumAdd(CString strFirst, CString strSecond)
{
	int nLength = strFirst.GetLength() > strSecond.GetLength() ? strFirst.GetLength() : strSecond.GetLength();
	++nLength;
	strFirst = CString(_T('0'), nLength - strFirst.GetLength()) + strFirst;
	strSecond = CString(_T('0'), nLength - strSecond.GetLength()) + strSecond;
	CString strBigNum(_T('0'), nLength),str;
	bool bHasCarry = false;
	int nFirst, nSecond;
	for (int i = nLength - 1; i >= 0; --i)
	{
		nFirst = _ttoi(strFirst.Mid(i, 1));
		nSecond = _ttoi(strSecond.Mid(i, 1));
		if (bHasCarry)
			++nFirst;
		bHasCarry = (nFirst + nSecond) / 10;
		str.Format(_T("%d"), (nFirst + nSecond) % 10);
		strBigNum.SetAt(i, str[0]);
	}
	if (strBigNum[0] == _T('0')&& strBigNum != _T("0"))
		strBigNum.Delete(0);
	return strBigNum;
}


CString CCheckCode::GetCheckCode(const CString & strBarcode)
{
	CString strCheckCode;
	int nLength = strBarcode.GetLength();
	int nSum = 0, nCheckCode;
	for (int i = 0; i < nLength; ++i)
	{
		if (i % 2)
			nSum += _ttoi(strBarcode.Mid(i, 1)) * 1;
		else
			nSum += _ttoi(strBarcode.Mid(i, 1)) * 3;
	}
	nCheckCode = nSum % 10;
	nCheckCode = 10 - nCheckCode;
	nCheckCode %= 10;
	strCheckCode.Format(_T("%d"), nCheckCode);
	return strCheckCode;
}


afx_msg LRESULT CCheckCode::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_CALCULATERESULT:
	{
		GetDlgItem(IDC_BUTTON_CALCULATE)->EnableWindow(TRUE);
		if (wParam)
		{
			SetDlgItemText(IDC_EDIT_OUTPUT, m_strOutput);
		}
		else
			SetDlgItemText(IDC_EDIT_OUTPUT, _T("计算已停止"));
	}
	break;
	}
	return 0;
}


void CCheckCode::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	StopCalculate();
}
