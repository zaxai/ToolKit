// CTxt2Excel.cpp: 实现文件
//

#include "pch.h"
#include "ToolKit.h"
#include "CTxt2Excel.h"
#include "afxdialogex.h"
#include <locale>

// CTxt2Excel 对话框

IMPLEMENT_DYNAMIC(CTxt2Excel, CDialogEx)

CTxt2Excel::CTxt2Excel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TXT2EXCEL, pParent)
{

}

CTxt2Excel::~CTxt2Excel()
{
}

void CTxt2Excel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INPUTFORMAT, m_comboInputFormat);
	DDX_Control(pDX, IDC_COMBO_OUTPUTFORMAT, m_comboOutputFormat);
	DDX_Control(pDX, IDC_LIST_SHOW, m_listShow);
}


BEGIN_MESSAGE_MAP(CTxt2Excel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTxt2Excel::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CTxt2Excel::OnBnClickedButtonConvert)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUTFORMAT, &CTxt2Excel::OnCbnSelchangeComboInputformat)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUTFORMAT, &CTxt2Excel::OnCbnSelchangeComboOutputformat)
	ON_MESSAGE(WM_MSGRECVPRO, &CTxt2Excel::OnMsgrecvpro)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTxt2Excel 消息处理程序


BOOL CTxt2Excel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sz_strFormat[] = { _T("txt"),_T("xls"),_T("xlsx") };
	for (int i = 0; i < sizeof(sz_strFormat) / sizeof(CString); ++i)
	{
		m_comboInputFormat.AddString(sz_strFormat[i]);
		m_comboOutputFormat.AddString(sz_strFormat[i]);
	}
	m_comboInputFormat.SetCurSel(0);
	m_comboOutputFormat.SetCurSel(0);
	m_listShow.SetExtendedStyle(m_listShow.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTxt2Excel::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_comboInputFormat.GetCurSel();
	if (nSel != CB_ERR)
	{
		CString strFilter;
		switch (nSel)
		{
		case 0:strFilter = _T("Txt files(*.txt)|*.txt|"); break;
		case 1:strFilter = _T("Excel files(*.xls)|*.xls|"); break;
		case 2:strFilter = _T("Excel files(*.xlsx)|*.xlsx|"); break;
		}
		CFileDialog fileDlg(TRUE, NULL, NULL, 0, strFilter, this);
		if (IDOK == fileDlg.DoModal())
		{
			SetDlgItemText(IDC_EDIT_INPUTPATH, _T(""));
			m_strSrcPath = fileDlg.GetPathName();
			if (nSel == 0)
			{
				CString strSplit;
				GetDlgItemText(IDC_EDIT_INPUTSPLIT, strSplit);
				if (strSplit.IsEmpty())
					AfxMessageBox(_T("请输入分隔符"));
				else
				{
					GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(FALSE);
					ReadFromTxt(strSplit);
				}
			}
			else
			{
				if (StartImport())
				{
					GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(FALSE);
				}
				else
					AfxMessageBox(_T("文件打开失败"));
			}
		}
	}
}


void CTxt2Excel::OnBnClickedButtonConvert()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_comboOutputFormat.GetCurSel();
	if (nSel != CB_ERR)
	{
		CString strFilter,strExt;
		switch (nSel)
		{
		case 0:
			strExt = _T(".txt");
			strFilter = _T("Txt files(*.txt)|*.txt|"); 
			break;
		case 1:
			strExt = _T(".xls");
			strFilter = _T("Excel files(*.xls)|*.xls|"); 
			break;
		case 2:
			strExt = _T(".xlsx");
			strFilter = _T("Excel files(*.xlsx)|*.xlsx|"); 
			break;
		}
		CFileDialog fileDlg(FALSE, strExt, _T("目标输出"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);
		if (IDOK == fileDlg.DoModal())
		{
			SetDlgItemText(IDC_EDIT_OUTPUTPATH, _T(""));
			m_strDemoPath = ZUtil::GetExeCatalogPath() + _T("\\Demo.xlsx");
			m_strDestPath = fileDlg.GetPathName();
			m_vec2_strDataExport = m_vec2_strDataImport;
			if (nSel == 0)
			{
				CString strSplit;
				GetDlgItemText(IDC_EDIT_OUTPUTSPLIT, strSplit);
				if (strSplit.IsEmpty())
					AfxMessageBox(_T("请输入分隔符"));
				else
				{
					GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(FALSE);
					WriteToTxt(strSplit);
				}
			}
			else
			{
				if (StartExport())
				{
					GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(FALSE);
				}
				else
					AfxMessageBox(_T("文件转换失败"));
			}
		}
	}
}


void CTxt2Excel::OnCbnSelchangeComboInputformat()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboInputFormat.GetCurSel() == 0)
		GetDlgItem(IDC_EDIT_INPUTSPLIT)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT_INPUTSPLIT)->EnableWindow(FALSE);
}


void CTxt2Excel::OnCbnSelchangeComboOutputformat()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboOutputFormat.GetCurSel() == 0)
		GetDlgItem(IDC_EDIT_OUTPUTSPLIT)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT_OUTPUTSPLIT)->EnableWindow(FALSE);
}


void CTxt2Excel::OnImportProgressUpdate(int nProgress)
{
	if(GetParent())
		GetParent()->PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_UPDATEPROGRESS);
}


void CTxt2Excel::OnImportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_IMPORTRESULT);
}


void CTxt2Excel::ReadFromTxt(const CString & strSplit)
{
	CString strTxt;
	ZUtil::ReadFile2CString(m_strSrcPath, strTxt);
	m_vec2_strDataImport.clear();
	std::vector<CString> vec_strRow;
	ZUtil::StrSplit(strTxt, vec_strRow, _T("\r\n"), strTxt.Right(2) == _T("\r\n"));
	int nRow = vec_strRow.size();
	m_vec2_strDataImport.resize(nRow);
	for (int i = 0; i < nRow; ++i)
	{
		ZUtil::StrSplit(vec_strRow[i], m_vec2_strDataImport[i], strSplit, vec_strRow[i].Right(strSplit.GetLength()) == strSplit);
	}
	if (nRow)
	{
		int nColumn = m_vec2_strDataImport[0].size();
		for (int i = 0; i < nRow; ++i)
		{
			if (m_vec2_strDataImport[i].size() == nColumn)
				continue;
			else
				m_vec2_strDataImport[i].resize(nColumn);
		}
	}
	if (GetParent())
		GetParent()->PostMessage(WM_MSGRECVPRO, (WPARAM)100, MSGUSER_UPDATEPROGRESS);
	PostMessage(WM_MSGRECVPRO, (WPARAM)1, MSGUSER_IMPORTRESULT);
}


void CTxt2Excel::InsertList()
{
	m_listShow.DeleteAllItems();
	if (m_listShow.GetHeaderCtrl())
	{
		int nCount = m_listShow.GetHeaderCtrl()->GetItemCount();
		while (nCount--)
			m_listShow.DeleteColumn(0);
	}
	int nRow = m_vec2_strDataImport.size();
	if (nRow)
	{
		int nColumn = m_vec2_strDataImport[0].size();
		CRect rc;
		m_listShow.GetClientRect(rc);
		CString strText;
		for (int i = 0; i < nColumn + 1; ++i)
		{
			if (i == 0)
				strText = _T("序号");
			else
				strText.Format(_T("%d"), i);
			m_listShow.InsertColumn(i, strText, LVCFMT_CENTER, (rc.Width() - 20) / (nColumn + 1), i);
		}
		for (int i = 0; i < nRow; ++i)
		{
			strText.Format(_T("%d"), i + 1);
			m_listShow.InsertItem(i, strText);
			for (int j = 0; j < nColumn; ++j)
			{
				m_listShow.SetItemText(i, j + 1, m_vec2_strDataImport[i][j]);
			}
		}
	}
}


void CTxt2Excel::OnExportProgressUpdate(int nProgress)
{
	if (GetParent())
		GetParent()->PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_UPDATEPROGRESS);
}


void CTxt2Excel::OnExportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_EXPORTRESULT);
}


void CTxt2Excel::WriteToTxt(const CString& strSplit)
{
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");
	CStdioFile file;
	BOOL nResult = 0;
	nResult = file.Open(m_strDestPath, CFile::modeWrite | CFile::typeText | CFile::modeCreate);
	if (nResult)
	{
		int nRow = m_vec2_strDataExport.size();
		for (int i = 0; i < nRow; ++i)
		{
			CString strRow;
			int nColumn = m_vec2_strDataExport[i].size();
			for (int j = 0; j < nColumn; ++j)
				strRow += m_vec2_strDataExport[i][j] + strSplit;
			strRow += _T('\n');
			file.WriteString(strRow);
		}
		if (GetParent())
			GetParent()->PostMessage(WM_MSGRECVPRO, (WPARAM)100, MSGUSER_UPDATEPROGRESS);
	}
	PostMessage(WM_MSGRECVPRO, (WPARAM)nResult, MSGUSER_EXPORTRESULT);
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
}


afx_msg LRESULT CTxt2Excel::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_IMPORTRESULT:
	{
		GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(TRUE);
		if (wParam)
		{
			InsertList();
			SetDlgItemText(IDC_EDIT_INPUTPATH, m_strSrcPath);
		}
		else
			AfxMessageBox(_T("文件打开失败"));
	}
	break;
	case MSGUSER_EXPORTRESULT:
	{
		GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(TRUE);
		if (wParam)
			SetDlgItemText(IDC_EDIT_OUTPUTPATH, m_strDestPath);
		else
			AfxMessageBox(_T("文件转换失败"));
	}
	break;
	}
	return 0;
}


void CTxt2Excel::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	StopImport();
	StopExport();
}
