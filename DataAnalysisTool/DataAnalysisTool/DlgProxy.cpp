
// DlgProxy.cpp : ʵ���ļ�
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
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ������ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
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
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	// 	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CDataAnalysisToolDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CDataAnalysisToolDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDataAnalysisToolDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IDataAnalysisTool ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {966CD290-A1A0-4BDF-B684-80D49F934D70}
static const IID IID_IDataAnalysisTool =
{ 0x966CD290, 0xA1A0, 0x4BDF, { 0xB6, 0x84, 0x80, 0xD4, 0x9F, 0x93, 0x4D, 0x70 } };

BEGIN_INTERFACE_MAP(CDataAnalysisToolDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CDataAnalysisToolDlgAutoProxy, IID_IDataAnalysisTool, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {F4F88504-9F02-484F-94CF-F94B75F1C899}
IMPLEMENT_OLECREATE2(CDataAnalysisToolDlgAutoProxy, "DataAnalysisTool.Application", 0xf4f88504, 0x9f02, 0x484f, 0x94, 0xcf, 0xf9, 0x4b, 0x75, 0xf1, 0xc8, 0x99)


// CDataAnalysisToolDlgAutoProxy ��Ϣ�������
