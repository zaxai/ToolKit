// CExcelPrint.cpp: 实现文件
//

#include "pch.h"
#include "ToolKit.h"
#include "CExcelPrint.h"
#include "afxdialogex.h"


// CExcelPrint 对话框

IMPLEMENT_DYNAMIC(CExcelPrint, CDialogEx)

CExcelPrint::CExcelPrint(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXCELPRINT, pParent)
	, m_lRow(0)
	, m_lColumn(0)
	, m_lCopies(1)
{

}

CExcelPrint::~CExcelPrint()
{
}

void CExcelPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);
	DDX_Control(pDX, IDC_EDIT_ROW, m_editRow);
	DDX_Control(pDX, IDC_EDIT_COLUMN, m_editColumn);
	DDX_Control(pDX, IDC_EDIT_COPIES, m_editCopies);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_editInput);
}


BEGIN_MESSAGE_MAP(CExcelPrint, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CExcelPrint::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CExcelPrint::OnBnClickedButtonPrint)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CExcelPrint 消息处理程序


BOOL CExcelPrint::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editRow.SetWindowText(_T("0"));
	m_editColumn.SetWindowText(_T("0"));
	m_editCopies.SetWindowText(_T("1"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CExcelPrint::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter;
	strFilter = _T("Excel files(*.xlsx)|*.xlsx|Excel files(*.xls)|*.xls|"); 
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, strFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		m_strFilePath = fileDlg.GetPathName();
		m_editPath.SetWindowText(m_strFilePath);
	}
}


void CExcelPrint::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strRow,strColumn,strCopies;
	m_editRow.GetWindowText(strRow);
	m_editColumn.GetWindowText(strColumn);
	m_editCopies.GetWindowText(strCopies);
	m_editInput.GetWindowText(m_strInput);
	m_lRow = _ttol(strRow);
	m_lColumn = _ttol(strColumn);
	m_lCopies = _ttol(strCopies);
	ZUtil::StrSplit(m_strInput, m_vec_strText, _T("\r\n"), m_strInput.Right(2) == _T("\r\n"));
	if (m_threadPrint.joinable())
		m_threadPrint.join();
	m_threadPrint = std::thread(&CExcelPrint::PrintThread, this);
}


void CExcelPrint::PrintThread()
{
	CString strError;
	ZOperateExcelFile::InitOle();
	ZOperateExcelFile operateexcel;
	if (operateexcel.InitExcel(&strError))
	{
		if (operateexcel.OpenExcelFile(m_strFilePath))
		{
			if (operateexcel.LoadSheet(0))
			{
				for (const CString& str : m_vec_strText)
				{
					operateexcel.SetCellString(m_lRow,m_lColumn,str);
					operateexcel.Save();
					operateexcel.PrintOut(m_lCopies);
				}
			}
			operateexcel.CloseExcelFile();
		}
		operateexcel.ReleaseExcel();
	}
	ZOperateExcelFile::UninitOle();
}


void CExcelPrint::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_threadPrint.joinable())
		m_threadPrint.join();
}
