// CHex2String.cpp: 实现文件
//

#include "pch.h"
#include "ToolKit.h"
#include "CHex2String.h"
#include "afxdialogex.h"


// CHex2String 对话框

IMPLEMENT_DYNAMIC(CHex2String, CDialogEx)

CHex2String::CHex2String(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEX2STRING, pParent)
{

}

CHex2String::~CHex2String()
{
}

void CHex2String::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_REMOVESPACE, m_btnRemoveSpace);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_editInput);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_editOutput);
}


BEGIN_MESSAGE_MAP(CHex2String, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_HEX2STRING, &CHex2String::OnBnClickedButtonHex2string)
	ON_BN_CLICKED(IDC_BUTTON_STRING2HEX, &CHex2String::OnBnClickedButtonString2hex)
END_MESSAGE_MAP()


// CHex2String 消息处理程序


void CHex2String::OnBnClickedButtonHex2string()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strInput, strOutput, strByte,strChar;
	GetDlgItemText(IDC_EDIT_INPUT, strInput);
	if (m_btnRemoveSpace.GetCheck())
		strInput.Remove(_T(' '));
	int nLength = strInput.GetLength();
	for (int i = 0; i < nLength; i = i + 2)
	{
		strByte = strInput.Mid(i, 2);
		strChar.Format(_T("%c"), _tcstol(strByte, NULL, 16));
		strOutput += strChar;
	}
	SetDlgItemText(IDC_EDIT_OUTPUT, strOutput);
}


void CHex2String::OnBnClickedButtonString2hex()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strInput, strOutput, strByte;
	TCHAR ch;
	GetDlgItemText(IDC_EDIT_INPUT, strInput);
	if (m_btnRemoveSpace.GetCheck())
		strInput.Remove(_T(' '));
	int nLength = strInput.GetLength();
	for (int i = 0; i < nLength; ++i)
	{
		ch = strInput[i];
		strByte.Format(_T("%X"), ch);
		strOutput += strByte;
	}
	SetDlgItemText(IDC_EDIT_OUTPUT, strOutput);
}
