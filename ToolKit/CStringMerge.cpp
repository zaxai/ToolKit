// CStringMerge.cpp: 实现文件
//

#include "pch.h"
#include "ToolKit.h"
#include "CStringMerge.h"
#include "afxdialogex.h"


// CStringMerge 对话框

IMPLEMENT_DYNAMIC(CStringMerge, CDialogEx)

CStringMerge::CStringMerge(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STRINGMERGE, pParent)
{

}

CStringMerge::~CStringMerge()
{
}

void CStringMerge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_editInput);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_editOutput);
}


BEGIN_MESSAGE_MAP(CStringMerge, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, &CStringMerge::OnBnClickedButtonMerge)
END_MESSAGE_MAP()


// CStringMerge 消息处理程序


void CStringMerge::OnBnClickedButtonMerge()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strInput, strOutput, strSplit;
	int nBaseRow;
	GetDlgItemText(IDC_EDIT_INPUT, strInput);
	GetDlgItemText(IDC_EDIT_SPLIT, strSplit);
	nBaseRow = GetDlgItemInt(IDC_EDIT_BASEROW);
	if (!nBaseRow)
	{
		AfxMessageBox(_T("请输入非0的行基数"));
		return;
	}
	std::vector<CString> vec_strRow;
	ZUtil::StrSplit(strInput, vec_strRow, _T("\r\n"), strInput.Right(2) == _T("\r\n"));
	int nRow = vec_strRow.size();
	for (int i = 0; i < nRow; ++i)
	{
		strOutput += vec_strRow[i];
		if (i < nRow - 1)
		{
			if ((i + 1) % nBaseRow)
				strOutput += strSplit;
			else
				strOutput += _T("\r\n---------------------\r\n");
		}
	}
	SetDlgItemText(IDC_EDIT_OUTPUT, strOutput);
}
