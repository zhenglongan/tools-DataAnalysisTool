
// DataAnalysisToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "mscomm1.h"
#include "CircularBuf.h"

class CDataAnalysisToolDlgAutoProxy;

// CDataAnalysisToolDlg 对话框
class CDataAnalysisToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataAnalysisToolDlg);
	friend class CDataAnalysisToolDlgAutoProxy;

// 构造
public:
	CDataAnalysisToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CDataAnalysisToolDlg();

// 对话框数据
	enum { IDD = IDD_DATAANALYSISTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CDataAnalysisToolDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void Onabout();
	CString m_strEditDisplayData;
	CButton m_ctrlButtonOpenPort;
	CButton m_ctrlButtonClosePort;
	CComboBox m_ctrlComboCommPort;
	// 存储COM端口号
	int m_nPort;
	// 串口控件控制型变量
	CMscomm1 m_ctrlComm;
	afx_msg void OnBnClickedButtonOpen();
	// 串口状态标识，TRUE为打开，FALSE为关闭
	bool OpenPortStatus;
	afx_msg void OnBnClickedButtonClose();
	// 1200;2400;4800;9600;14400;19200;28800;38400;57600;115200;230400;256000
	CComboBox m_ctrlComboCommBaudRate;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnBnClickedButtonClearData();
	// 波特率
	unsigned int m_nBaudRate;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 接收到的字符数
	CString m_strEditRxNumber;
	// 接收到的字符数，这是数据型变量
	unsigned int m_RxNumber;
	CEdit m_ctrlEditDataDisplay;
	void AutoNextLine(CString strLineText);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_strEditUncongnizedNumber;
};
