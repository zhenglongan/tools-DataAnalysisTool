
// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "DataAnalysisTool.h"
#include "DlgProxy.h"
#include "DataAnalysisToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataAnalysisToolDlgAutoProxy

IMPLEMENT_DYNCREATE(CDataAnalysisToolDlgAutoProxy, CCmdTarget)

CDataAnalysisToolDlgAutoProxy::CDataAnalysisToolDlgAutoProxy()
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
		ASSERT_KINDOF(CDataAnalysisToolDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CDataAnalysisToolDlg)))
		{
			m_pDialog = reinterpret_cast<CDataAnalysisToolDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CDataAnalysisToolDlgAutoProxy::~CDataAnalysisToolDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CDataAnalysisToolDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CDataAnalysisToolDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDataAnalysisToolDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IDataAnalysisTool 的支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {966CD290-A1A0-4BDF-B684-80D49F934D70}
static const IID IID_IDataAnalysisTool =
{ 0x966CD290, 0xA1A0, 0x4BDF, { 0xB6, 0x84, 0x80, 0xD4, 0x9F, 0x93, 0x4D, 0x70 } };

BEGIN_INTERFACE_MAP(CDataAnalysisToolDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CDataAnalysisToolDlgAutoProxy, IID_IDataAnalysisTool, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {F4F88504-9F02-484F-94CF-F94B75F1C899}
IMPLEMENT_OLECREATE2(CDataAnalysisToolDlgAutoProxy, "DataAnalysisTool.Application", 0xf4f88504, 0x9f02, 0x484f, 0x94, 0xcf, 0xf9, 0x4b, 0x75, 0xf1, 0xc8, 0x99)


// CDataAnalysisToolDlgAutoProxy 消息处理程序
