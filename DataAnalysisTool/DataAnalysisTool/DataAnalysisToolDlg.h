
// DataAnalysisToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "mscomm1.h"
#include "CircularBuf.h"
#include "analysis.h"

class CDataAnalysisToolDlgAutoProxy;

// CDataAnalysisToolDlg �Ի���
class CDataAnalysisToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataAnalysisToolDlg);
	friend class CDataAnalysisToolDlgAutoProxy;

// ����
public:
	CDataAnalysisToolDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CDataAnalysisToolDlg();

// �Ի�������
	enum { IDD = IDD_DATAANALYSISTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CDataAnalysisToolDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// ���ɵ���Ϣӳ�亯��
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
	// �洢COM�˿ں�
	int m_nPort;
	// ���ڿؼ������ͱ���
	CMscomm1 m_ctrlComm;
	afx_msg void OnBnClickedButtonOpen();
	// ����״̬��ʶ��TRUEΪ�򿪣�FALSEΪ�ر�
	bool OpenPortStatus;
	afx_msg void OnBnClickedButtonClose();
	// 1200;2400;4800;9600;14400;19200;28800;38400;57600;115200;230400;256000
	CComboBox m_ctrlComboCommBaudRate;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnBnClickedButtonClearData();
	// ������
	unsigned int m_nBaudRate;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ���յ����ַ���
	CString m_strEditRxNumber;
	// ���յ����ַ��������������ͱ���
	unsigned int m_RxNumber;
	CEdit m_ctrlEditDataDisplay;
	void AutoNextLine(CString strLineText);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_strEditUncongnizedNumber;
	// �ӻ����յ�������ͨ�Ű�����
	CString m_strEditSlavePacketNumber;
	// ���յ�����������ָ�������
	CString m_strEditMasterPacketNumber;
	// ������ʾ��
	void UpdateEditDisplay(void);
	// ��ȡ��ǰ���ô��ں�
	void GetCom(void);
	// reset combobox width
	void ResetCtrlWidth(CComboBox* lpCtrl);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	// ��ǰѡ�еĴ�������
	CString strCurrentPortName;
	afx_msg void OnCbnSelchangeComboPort();
	afx_msg void OnCbnSelchangeComboBaudrate();
	// ������ʾϵͳ״̬ͼ��
	CStatic m_pictrueControlSystemStatus;
	// ��ʾϵͳ״̬ͼƬ
	void ShowSystemStatusPicture(DWORD bitmap);

	HACCEL hAccel;//��ݼ�
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// ����ϵͳ״̬��ֵȥ����ͼ�����ʾ
	void ProcessSystemStateIcon(SYS_STRU* p_sys);
	// X��������
	CEdit X_coor;
	// Y��������
	CEdit Y_coor;
	// ����ϵͳ��ǰ����ֵ����ʾ
	void refresh_coor(INT32 x,INT32 y);
	afx_msg void On32771();
};
