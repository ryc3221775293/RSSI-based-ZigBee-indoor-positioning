
// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "Positioning.h"
#include "DlgProxy.h"
#include "PositioningDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPositioningDlgAutoProxy

IMPLEMENT_DYNCREATE(CPositioningDlgAutoProxy, CCmdTarget)

CPositioningDlgAutoProxy::CPositioningDlgAutoProxy()
{
	EnableAutomation();
	
	// 为使应用程序在自动化对象处于活动状态时一直保持 
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CPositioningDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CPositioningDlg)))
		{
			m_pDialog = reinterpret_cast<CPositioningDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CPositioningDlgAutoProxy::~CPositioningDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CPositioningDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CPositioningDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPositioningDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IPositioning 的支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {4E815C12-68E2-4CAD-87AC-E323D6996D59}
static const IID IID_IPositioning =
{ 0x4E815C12, 0x68E2, 0x4CAD, { 0x87, 0xAC, 0xE3, 0x23, 0xD6, 0x99, 0x6D, 0x59 } };

BEGIN_INTERFACE_MAP(CPositioningDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CPositioningDlgAutoProxy, IID_IPositioning, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {C367FA16-669C-4D4C-BA6B-44157B76C329}
IMPLEMENT_OLECREATE2(CPositioningDlgAutoProxy, "Positioning.Application", 0xc367fa16, 0x669c, 0x4d4c, 0xba, 0x6b, 0x44, 0x15, 0x7b, 0x76, 0xc3, 0x29)


// CPositioningDlgAutoProxy 消息处理程序
