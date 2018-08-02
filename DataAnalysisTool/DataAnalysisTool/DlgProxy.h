
// DlgProxy.h: 头文件
//

#pragma once

class CDataAnalysisToolDlg;


// CDataAnalysisToolDlgAutoProxy 命令目标

class CDataAnalysisToolDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CDataAnalysisToolDlgAutoProxy)

	CDataAnalysisToolDlgAutoProxy();           // 动态创建所使用的受保护的构造函数

// 属性
public:
	CDataAnalysisToolDlg* m_pDialog;

// 操作
public:

// 重写
	public:
	virtual void OnFinalRelease();

// 实现
protected:
	virtual ~CDataAnalysisToolDlgAutoProxy();

	// 生成的消息映射函数

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CDataAnalysisToolDlgAutoProxy)

	// 生成的 OLE 调度映射函数

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

