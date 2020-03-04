// CFileRename.cpp: 实现文件
//

#include "pch.h"
#include "ToolKit.h"
#include "CFileRename.h"
#include "afxdialogex.h"


// CFileRename 对话框

IMPLEMENT_DYNAMIC(CFileRename, CDialogEx)

CFileRename::CFileRename(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILERENAME, pParent)
{

}

CFileRename::~CFileRename()
{
}

void CFileRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_listShow);
}


BEGIN_MESSAGE_MAP(CFileRename, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CFileRename::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CFileRename::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, &CFileRename::OnBnClickedButtonRename)
END_MESSAGE_MAP()


// CFileRename 消息处理程序


BOOL CFileRename::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rc;
	m_listShow.GetClientRect(rc);
	m_listShow.SetExtendedStyle(m_listShow.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listShow.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rc.Width() / 10, 0);
	m_listShow.InsertColumn(1, _T("原文件名"), LVCFMT_LEFT, rc.Width() / 10 * 4, 1);
	m_listShow.InsertColumn(2, _T("现文件名"), LVCFMT_LEFT, rc.Width() / 10 * 4, 2);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CFileRename::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO  bi;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPCITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl)
	{
		TCHAR  szDisplayName[MAX_PATH] = { 0 };
		SHGetPathFromIDList(pidl, szDisplayName);
		m_strFolderPath = szDisplayName;
		SetDlgItemText(IDC_EDIT_PATH, m_strFolderPath);
		InsertFolder();
		GetDlgItem(IDC_BUTTON_RENAME)->EnableWindow(TRUE);
	}
}


void CFileRename::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	InsertFolder();
	GetDlgItem(IDC_BUTTON_RENAME)->EnableWindow(TRUE);
}


void CFileRename::OnBnClickedButtonRename()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strRegex, strReplace, strOriName, strCurName;
	GetDlgItemText(IDC_EDIT_REGEX, strRegex);
	GetDlgItemText(IDC_EDIT_REPLACE, strReplace);
	std::wregex re((LPCTSTR)strRegex);
	int nCount = m_listShow.GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		strOriName = m_listShow.GetItemText(i, 1);
		std::wstring str = std::regex_replace((LPCTSTR)strOriName, re, (LPCTSTR)strReplace);
		strCurName = str.c_str();
		try
		{
			CFile::Rename(m_strFolderPath + _T("\\") + strOriName, m_strFolderPath + _T("\\") + strCurName);
			m_listShow.SetItemText(i, 2, strCurName);
			if (strCurName != strOriName)
				m_listShow.SetRowBkColor(i, RGB(0, 255, 0));
		}
		catch (CFileException * p_ex)
		{
			CString strError;
			strError.Format(_T("Error:%d"), p_ex->m_cause);
			p_ex->Delete();
			m_listShow.SetItemText(i, 2, strError);
			m_listShow.SetRowBkColor(i, RGB(255, 0, 0));
		}
	}
	GetDlgItem(IDC_BUTTON_RENAME)->EnableWindow(FALSE);
}


void CFileRename::InsertFolder()
{
	// TODO: 在此处添加实现代码.
	m_listShow.DeleteAllItems();
	std::vector<CString> vec_str;
	ZUtil::BrowseCurrentDir(m_strFolderPath, vec_str);
	int nSize = vec_str.size();
	CString strText, strFileName;
	for (int i = 0; i < nSize; ++i)
	{
		strText.Format(_T("%d"), i + 1);
		strFileName = vec_str[i].Mid(vec_str[i].ReverseFind(_T('\\')) + 1);
		m_listShow.InsertItem(i, strText);
		m_listShow.SetItemText(i, 1, strFileName);
	}
}
