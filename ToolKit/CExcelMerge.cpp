// CExcelMerge.cpp: 实现文件
//

#include "pch.h"
#include "ToolKit.h"
#include "CExcelMerge.h"
#include "afxdialogex.h"


// CExcelMerge 对话框

IMPLEMENT_DYNAMIC(CExcelMerge, CDialogEx)

CExcelMerge::CExcelMerge(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXCELMERGE, pParent)
	, m_bIsRemoveFirstRow(false)
	, m_strFolderPath(_T(""))
{

}

CExcelMerge::~CExcelMerge()
{
}

void CExcelMerge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_boxShow);
	DDX_Control(pDX, IDC_CHECK_REMOVEFIRSTROW, m_btnRemoveFirstRow);
}


BEGIN_MESSAGE_MAP(CExcelMerge, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CExcelMerge::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, &CExcelMerge::OnBnClickedButtonMerge)
	ON_BN_CLICKED(IDC_CHECK_REMOVEFIRSTROW, &CExcelMerge::OnBnClickedCheckRemovefirstrow)
	ON_MESSAGE(WM_MSGRECVPRO, &CExcelMerge::OnMsgrecvpro)
END_MESSAGE_MAP()


// CExcelMerge 消息处理程序


void CExcelMerge::OnBnClickedButtonOpen()
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
		SetDlgItemText(IDC_EDIT_INPUTPATH, _T(""));
		TCHAR  szDisplayName[MAX_PATH] = { 0 };
		SHGetPathFromIDList(pidl, szDisplayName);
		m_strFolderPath = szDisplayName;
		SetDlgItemText(IDC_EDIT_INPUTPATH, m_strFolderPath);
		InsertFolder();
	}
}


void CExcelMerge::OnBnClickedButtonMerge()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter, strExt;
	strExt = _T(".xlsx");
	strFilter = _T("Excel files(*.xlsx)|*.xlsx|");
	CFileDialog fileDlg(FALSE, strExt, _T("目标输出"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		SetDlgItemText(IDC_EDIT_OUTPUTPATH, _T(""));
		m_strDemoPath = ZUtil::GetExeCatalogPath() + _T("\\Demo.xlsx");
		m_strDestPath = fileDlg.GetPathName();
		m_vec2_strDataExport = m_vec2_strData;
		int nColumn = 0;
		if (m_vec2_strDataExport.size())
			nColumn = m_vec2_strDataExport[0].size();
		for (std::vector<CString>& vec_str : m_vec2_strDataExport)
			vec_str.resize(nColumn);
		if (StartExport())
		{
			GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MERGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_REMOVEFIRSTROW)->EnableWindow(FALSE);
		}
		else
			AfxMessageBox(_T("文件合并失败"));
	}
}


void CExcelMerge::OnBnClickedCheckRemovefirstrow()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsRemoveFirstRow = m_btnRemoveFirstRow.GetCheck();
}


void CExcelMerge::InsertFolder()
{
	m_vec_strPaths.clear();
	m_vec2_strData.clear();
	m_boxShow.ResetContent();
	std::vector<CString> vec_str;
	ZUtil::BrowseCurrentDir(m_strFolderPath, vec_str);
	int nSize = vec_str.size();
	CString strExt;
	for (const CString& str : vec_str)
	{
		strExt = str.Mid(str.ReverseFind(_T('.')));
		if (strExt == _T(".xls") || strExt == _T(".xlsx"))
			m_vec_strPaths.push_back(str);
	}
	m_itPaths = m_vec_strPaths.begin();
	if (m_itPaths != m_vec_strPaths.end())
	{
		GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MERGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEFIRSTROW)->EnableWindow(FALSE);
		m_strSrcPath = *m_itPaths;
		if (m_bIsRemoveFirstRow)
			ZExcelImport::m_nRowOffset = 1;
		StopImport();
		StartImport();
	}
}


void CExcelMerge::OnImportProgressUpdate(int nProgress)
{
	if (GetParent())
		GetParent()->PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_UPDATEPROGRESS);
}


void CExcelMerge::OnImportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_IMPORTRESULT);
}


void CExcelMerge::OnExportProgressUpdate(int nProgress)
{
	if (GetParent())
		GetParent()->PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_UPDATEPROGRESS);
}


void CExcelMerge::OnExportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_EXPORTRESULT);
}


afx_msg LRESULT CExcelMerge::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_IMPORTRESULT:
	{
		if (wParam)
		{
			for (const std::vector<CString>& vec_str : m_vec2_strDataImport)
				m_vec2_strData.push_back(vec_str);
			m_boxShow.AddString(*m_itPaths);
		}
		++m_itPaths;
		if (m_itPaths != m_vec_strPaths.end())
		{
			m_strSrcPath = *m_itPaths;
			if (m_bIsRemoveFirstRow)
				ZExcelImport::m_nRowOffset = 1;
			StopImport();
			StartImport();
		}
		else
		{
			GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MERGE)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_REMOVEFIRSTROW)->EnableWindow(TRUE);
		}
	}
	break;
	case MSGUSER_EXPORTRESULT:
	{
		GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MERGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEFIRSTROW)->EnableWindow(TRUE);
		if (wParam)
			SetDlgItemText(IDC_EDIT_OUTPUTPATH, m_strDestPath);
		else
			AfxMessageBox(_T("文件合并失败"));
	}
	break;
	}
	return 0;
}
