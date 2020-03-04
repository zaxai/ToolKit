
// MainFrm.h: CMainFrame 类的接口
//

#pragma once

#include "CHex2String.h"
#include "CCheckCode.h"
#include "CTxt2Excel.h"
#include "CStringMerge.h"
#include "CExcelMerge.h"
#include "CExcelPrint.h"
#include "CFileRename.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	CHex2String* m_p_hex2string;
	CCheckCode* m_p_checkcode;
	CTxt2Excel* m_p_txt2excel;
	CStringMerge* m_p_stringmerge;
	CExcelMerge* m_p_excelmerge;
	CExcelPrint* m_p_excelprint;
	CFileRename* m_p_filerename;
	void InitInCreate();
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnToolHex2string();
	afx_msg void OnToolCheckcode();
	afx_msg void OnToolTxt2excel();
	afx_msg void OnToolStringmerge();
	afx_msg void OnToolExcelmerge();
	afx_msg void OnToolExcelprint();
	afx_msg void OnToolFilerename();
	afx_msg void OnHelpUpdate();
};


