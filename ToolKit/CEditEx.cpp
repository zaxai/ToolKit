// CEditEx.cpp: 实现文件
//

#include "pch.h"
#include "CEditEx.h"


// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx()
{

}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
END_MESSAGE_MAP()



// CEditEx 消息处理程序




BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		UINT nKey = pMsg->wParam;
		if (nKey == _T('A'))
		{
			if (::GetKeyState(VK_CONTROL) < 0)
			{
				SetSel(0, -1);
				return TRUE;
			}
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}
