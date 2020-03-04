
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "ToolKit.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_TOOL_HEX2STRING, &CMainFrame::OnToolHex2string)
	ON_COMMAND(ID_TOOL_CHECKCODE, &CMainFrame::OnToolCheckcode)
	ON_COMMAND(ID_TOOL_TXT2EXCEL, &CMainFrame::OnToolTxt2excel)
	ON_MESSAGE(WM_MSGRECVPRO, &CMainFrame::OnMsgrecvpro)
	ON_COMMAND(ID_TOOL_STRINGMERGE, &CMainFrame::OnToolStringmerge)
	ON_COMMAND(ID_TOOL_EXCELMERGE, &CMainFrame::OnToolExcelmerge)
	ON_COMMAND(ID_TOOL_EXCELPRINT, &CMainFrame::OnToolExcelprint)
	ON_COMMAND(ID_TOOL_FILERENAME, &CMainFrame::OnToolFilerename)
	ON_COMMAND(ID_HELP_UPDATE, &CMainFrame::OnHelpUpdate)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_PROGRESS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
	: m_p_hex2string(nullptr)
	, m_p_checkcode(nullptr)
	, m_p_txt2excel(nullptr)
	, m_p_stringmerge(nullptr)
	, m_p_excelmerge(nullptr)
	, m_p_excelprint(nullptr)
	, m_p_filerename(nullptr)
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WIN_XP);
}

CMainFrame::~CMainFrame()
{
	if (m_p_hex2string)
		delete m_p_hex2string;
	if (m_p_checkcode)
		delete m_p_checkcode;
	if (m_p_txt2excel)
		delete m_p_txt2excel;
	if (m_p_stringmerge)
		delete m_p_stringmerge;
	if (m_p_excelmerge)
		delete m_p_excelmerge;
	if (m_p_excelprint)
		delete m_p_excelprint;
	if (m_p_filerename)
		delete m_p_filerename;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);
	m_wndToolBar.EnableTextLabels(TRUE);//工具栏同时显示图标和文字

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作: 
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_TOOL_HEX2STRING);
	lstBasicCommands.AddTail(ID_TOOL_CHECKCODE);
	lstBasicCommands.AddTail(ID_TOOL_TXT2EXCEL);
	lstBasicCommands.AddTail(ID_TOOL_STRINGMERGE);
	lstBasicCommands.AddTail(ID_TOOL_EXCELMERGE); 
	lstBasicCommands.AddTail(ID_TOOL_EXCELPRINT);
	lstBasicCommands.AddTail(ID_TOOL_FILERENAME);
	lstBasicCommands.AddTail(ID_HELP_UPDATE);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);
	InitInCreate();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::InitInCreate()
{
	// TODO: 在此处添加实现代码.
	m_wndStatusBar.EnablePaneProgressBar(1, 100, TRUE);
	m_wndStatusBar.SetPaneWidth(1, 500);
	m_wndStatusBar.SetPaneProgress(1, 0);
}


afx_msg LRESULT CMainFrame::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_UPDATEPROGRESS:
	{
		static int nLastProgress = 0;
		if (wParam != nLastProgress)
		{
			m_wndStatusBar.SetPaneProgress(1, wParam);
			nLastProgress = wParam;
		}
	}
	break;
	}
	return 0;
}


void CMainFrame::OnToolHex2string()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_hex2string)
	{
		m_p_hex2string = new CHex2String();
		m_p_hex2string->Create(IDD_HEX2STRING, this);
	}
	m_p_hex2string->ShowWindow(SW_SHOW);
}


void CMainFrame::OnToolCheckcode()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_checkcode)
	{
		m_p_checkcode = new CCheckCode();
		m_p_checkcode->Create(IDD_CHECKCODE, this);
	}
	m_p_checkcode->ShowWindow(SW_SHOW);
}


void CMainFrame::OnToolTxt2excel()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_txt2excel)
	{
		m_p_txt2excel = new CTxt2Excel();
		m_p_txt2excel->Create(IDD_TXT2EXCEL, this);
	}
	m_p_txt2excel->ShowWindow(SW_SHOW);
}


void CMainFrame::OnToolStringmerge()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_stringmerge)
	{
		m_p_stringmerge = new CStringMerge();
		m_p_stringmerge->Create(IDD_STRINGMERGE, this);
	}
	m_p_stringmerge->ShowWindow(SW_SHOW);
}


void CMainFrame::OnToolExcelmerge()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_excelmerge)
	{
		m_p_excelmerge = new CExcelMerge();
		m_p_excelmerge->Create(IDD_EXCELMERGE, this);
	}
	m_p_excelmerge->ShowWindow(SW_SHOW);
}


void CMainFrame::OnToolExcelprint()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_excelprint)
	{
		m_p_excelprint = new CExcelPrint();
		m_p_excelprint->Create(IDD_EXCELPRINT, this);
	}
	m_p_excelprint->ShowWindow(SW_SHOW);
}


void CMainFrame::OnToolFilerename()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_filerename)
	{
		m_p_filerename = new CFileRename();
		m_p_filerename->Create(IDD_FILERENAME, this);
	}
	m_p_filerename->ShowWindow(SW_SHOW);
}


void CMainFrame::OnHelpUpdate()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL, _T("open"), _T("http://www.zaxai.com/resource.php?resid=187"), NULL, NULL, SW_SHOW);
}
