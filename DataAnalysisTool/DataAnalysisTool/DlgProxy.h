
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CDataAnalysisToolDlg;


// CDataAnalysisToolDlgAutoProxy ����Ŀ��

class CDataAnalysisToolDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CDataAnalysisToolDlgAutoProxy)

	CDataAnalysisToolDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CDataAnalysisToolDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CDataAnalysisToolDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CDataAnalysisToolDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

