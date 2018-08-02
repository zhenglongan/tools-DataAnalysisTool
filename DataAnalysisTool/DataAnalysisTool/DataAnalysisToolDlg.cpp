
// DataAnalysisToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataAnalysisTool.h"
#include "DataAnalysisToolDlg.h"
#include "DlgProxy.h"

#include "analysis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�ҵ�ȫ�ֱ���
CircularBuf circular_buf(10*1024);//���建�滺��������СΪ64KB
BYTE display_buf[10*1024];//���ڻ�����ʾ

#define TIMER_READ_PORT_DATA 1





// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDataAnalysisToolDlg �Ի���




IMPLEMENT_DYNAMIC(CDataAnalysisToolDlg, CDialog);

CDataAnalysisToolDlg::CDataAnalysisToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataAnalysisToolDlg::IDD, pParent)
	, m_strEditDisplayData(_T(""))
	, m_nPort(0)
	, OpenPortStatus(false)
	, m_nBaudRate(0)
	, m_strEditRxNumber(_T(""))
	, m_RxNumber(0)
	, m_strEditUncongnizedNumber(_T(""))
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CDataAnalysisToolDlg::~CDataAnalysisToolDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CDataAnalysisToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATA_DISPLAY, m_strEditDisplayData);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_ctrlButtonOpenPort);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ctrlButtonClosePort);
	DDX_Control(pDX, IDC_COMBO_PORT, m_ctrlComboCommPort);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_ctrlComboCommBaudRate);
	DDX_Text(pDX, IDC_EDIT1, m_strEditRxNumber);
	DDX_Control(pDX, IDC_EDIT_DATA_DISPLAY, m_ctrlEditDataDisplay);
	DDX_Text(pDX, IDC_EDIT2, m_strEditUncongnizedNumber);
}

BEGIN_MESSAGE_MAP(CDataAnalysisToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_about, &CDataAnalysisToolDlg::Onabout)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDataAnalysisToolDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDataAnalysisToolDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_DATA, &CDataAnalysisToolDlg::OnBnClickedButtonClearData)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDataAnalysisToolDlg ��Ϣ�������

BOOL CDataAnalysisToolDlg::OnInitDialog()
{
	CString strtemp;
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//�������������������������������������Լ���ӵĳ������������������������������������������������������
	
	//���ڳ�ʼ��
	m_ctrlComboCommPort.SetCurSel(5);//����ѡ���Ĭ��ѡ��COM1
	m_nPort=1;//Ĭ��ѡ��COM1
	m_ctrlComboCommBaudRate.SetCurSel(9);//������Ĭ��ѡ��115200
	m_ctrlComm.put_CommPort(m_nPort);
	OpenPortStatus=FALSE;
	if(m_ctrlComm.get_PortOpen())
	{
		m_ctrlComm.put_PortOpen(FALSE);//�رմ���
	}
	m_ctrlButtonClosePort.EnableWindow(m_ctrlComm.get_PortOpen());//���ݴ����Ƿ�򿪣��á��򿪴��ڡ������رմ��ڡ���Ч������Ч
	m_ctrlButtonOpenPort.EnableWindow(!m_ctrlComm.get_PortOpen());
	
	SetTimer(TIMER_READ_PORT_DATA,50,NULL); //100���붨ʱ������ȡ���λ������е����ݲ���������ʾ��������
	
	//��ʾ���յ����ַ���Ϊ0
	m_RxNumber=0;////��ʼ�����յ����ַ�����Ϊ0
	m_strEditRxNumber.Format(_T("%d"),m_RxNumber);//ת��ΪCString�ַ�����ʾ
	/* ��ʾ���� */
	strtemp.Format(_T("%d"),0);//ת��ΪCString�ַ�����ʾ
	m_strEditRxNumber=_T("����:");
	m_strEditRxNumber+=strtemp;
	
	strtemp.Format(_T("%d"),0);//ת��ΪCString�ַ�����ʾ
	m_strEditUncongnizedNumber=_T("δʶ����:");
	m_strEditUncongnizedNumber+=strtemp;

	UpdateData(FALSE);//���±༭�����ݣ����ڽ�����ʾ������ʾ��������


	analysis_init(&m_strEditDisplayData);//��ʼ��Э�����ģ��
	//�������������������������������������Լ���ӵĳ����������������������������������������������������������







	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDataAnalysisToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDataAnalysisToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDataAnalysisToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳�����Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

void CDataAnalysisToolDlg::OnClose()
{
	if (CanExit())
		CDialog::OnClose();
}

void CDataAnalysisToolDlg::OnOK()
{
	if (CanExit())
		CDialog::OnOK();
}

void CDataAnalysisToolDlg::OnCancel()
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CDataAnalysisToolDlg::CanExit()
{
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void CDataAnalysisToolDlg::Onabout()
{
	// TODO: Add your command handler code here
	//AfxMessageBox(_T("�鿴��������Ϣ"));
	CAboutDlg About_dlg;
	About_dlg.DoModal();
}

void CDataAnalysisToolDlg::OnBnClickedButtonOpen()
{

	// TODO: Add your control notification handler code here
	//�������������������������������������Լ���ӵĳ����������������������������������������������������������
	CString strTemp;

	if(OpenPortStatus==FALSE)//�������δ�򿪣�ִ����������
	{
		m_nPort=m_ctrlComboCommPort.GetCurSel()+1;//�����Ͽ��еĴ��ں�
		m_ctrlComm.put_CommPort(m_nPort);//ѡ����Ӧ����
		m_ctrlComm.put_InputMode(1);//���ý������ݼ��ģʽ����������ģʽ
		m_ctrlComm.put_InBufferSize(16*1024);//���ô��ڽ��ջ�������СΪ16KB
		m_ctrlComm.put_OutBufferSize(128);//���ô��ڷ��ͻ�������СΪ�ֽ�

		switch(m_ctrlComboCommPort.GetCurSel())//����ѡ�����ò�����
		{
			case 0:
				m_nBaudRate=1200;
				break;
			case 1:
				m_nBaudRate=2400;
				break;
			case 2:
				m_nBaudRate=4800;
				break;
			case 3:
				m_nBaudRate=9600;
				break;
			case 4:
				m_nBaudRate=14400;
				break;
			case 5:
				m_nBaudRate=19200;
				break;
			case 6:
				m_nBaudRate=28800;
				break;
			case 7:
				m_nBaudRate=38400;
				break;
			case 8:
				m_nBaudRate=57600;
				break;
			case 9:
				m_nBaudRate=115200;
				break;
			case 10:
				m_nBaudRate=230400;
				break;
			case 11:
				m_nBaudRate=256000;
				break;
			default:
				m_nBaudRate=115200;
				break;
		}
		strTemp.Format(_T("%d,n,8,1"), m_nBaudRate);
		m_ctrlComm.put_Settings(strTemp);
		//m_ctrlComm.put_Settings(_T("115200,n,8,1"));//���ò�����Ϊbps��N��ʾ��У��λ��EΪżУ
											//�飬OΪ��У�飩λ����λһλֹͣλ
		
		m_ctrlComm.put_PortOpen(TRUE);//�򿪴���
		
		m_ctrlComm.put_RThreshold(1);//���ý����¼������ż�����ʾÿ�����յ�һ���������ֽ�ʱ����һ�������¼�
		m_ctrlComm.put_InputLen(0);//���ö�ȡ�������ݳ���Ϊ����ʾ�޳������ƣ�ÿ�ζ�ȡʱһ���Զ�ȡ���ջ������е�ȫ������

		m_ctrlComm.put_InBufferCount(0);//��ս��ջ���������������ջ������еĲ�������
		if(m_ctrlComm.get_PortOpen())
		{		
			m_ctrlButtonClosePort.EnableWindow(m_ctrlComm.get_PortOpen());//���մ��ڱ�־λ���á��򿪴��ڡ������رմ��ڡ���Ч������Ч
			m_ctrlButtonOpenPort.EnableWindow(!m_ctrlComm.get_PortOpen());
			OpenPortStatus=TRUE;
			MessageBox(_T("��ѡ���ڳɹ���"),_T("�򿪳ɹ�"));

		}
		else
		{
			OpenPortStatus=FALSE;
			MessageBox(_T("�����޷��򿪻��ѱ�ռ��"),_T("���ڴ�ʧ��"));
		}


	}

//�������������������������������������Լ���ӵĳ����������������������������������������������������������

}

void CDataAnalysisToolDlg::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	if(OpenPortStatus==TRUE)//������ڴ򿪣�ִ����������
	{
		m_ctrlComm.put_PortOpen(FALSE);//�رմ���
		if(!m_ctrlComm.get_PortOpen())
		{
			OpenPortStatus=FALSE;
			m_ctrlButtonClosePort.EnableWindow(OpenPortStatus);//���մ��ڱ�־λ���á��򿪴��ڡ������رմ��ڡ���Ч������Ч
			m_ctrlButtonOpenPort.EnableWindow(!OpenPortStatus);
			MessageBox(_T("��ѡ���ڳɹ��ر�"),_T("�򿪳ɹ�"));
		}
		else
		{
			MessageBox(_T("����δ�ɹ��ر�"),_T("�رճɹ�"));
		}
	}

	
}
BEGIN_EVENTSINK_MAP(CDataAnalysisToolDlg, CDialog)
	ON_EVENT(CDataAnalysisToolDlg, IDC_MSCOMM1, 1, CDataAnalysisToolDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

//���ڿؼ����պ���
void CDataAnalysisToolDlg::OnCommMscomm1()
{
	// TODO: Add your message handler code here

//�������������������������������������Լ���ӵĳ����������������������������������������������������������

	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	BYTE temp;
//	static char rxdata[2048];//����BYTE������
	CString strtemp;//
	if(m_ctrlComm.get_CommEvent()==2)//�¼�ֵΪ��ʾ���������¼�
	{
		variant_inp=m_ctrlComm.get_Input();//�����ջ�����
		safearray_inp=variant_inp;//VARIANT��ת����Cstring�ͱ���
		len=safearray_inp.GetOneDimSize();//�õ���Ч���ݳ���
		for(k=0;k<len;k++)
		{
			safearray_inp.GetElement(&k,&temp);//ת��ΪBYTE������
			circular_buf.write(&temp,1);//���뻷�λ�����

		}

	//	for(k=0;k<len;k++)//������ת��ΪCstring�ͱ���
	//	{
	//		BYTE bt=*(char*)(rxdata+k);//�ַ���
	////		strtemp.Format("%c",bt);//���ַ�������ʱ����strtemp���
	//		strtemp.Format(_T(" %02X"), bt);
	//		m_strEditDisplayData+=strtemp;//������ձ༭���Ӧ�ַ�����ӳ�������
	//	}
	
	}
	
	UpdateData(FALSE);//���±༭�����ݣ����ڽ�����ʾ������ʾ��������


//�������������������������������������Լ���ӵĳ������������������������������������������������������������

}

//�����ʾ�ļ������
void CDataAnalysisToolDlg::OnBnClickedButtonClearData()
{
	// TODO: Add your control notification handler code here
	CString strtemp;
	
	//���ַ���
	m_RxNumber=0;////��ʼ�����յ����ַ�����Ϊ0
	strtemp.Format(_T("%d"),m_RxNumber);//ת��ΪCString�ַ�����ʾ
	m_strEditRxNumber=_T("����:");
	m_strEditRxNumber+=strtemp;
	
	
	//δʶ���ַ���
	analysis_clear_discard_number();
	strtemp.Format(_T("%d"),analysis_get_discard_number());//ת��ΪCString�ַ�����ʾ
	m_strEditUncongnizedNumber=_T("δʶ����:");
	m_strEditUncongnizedNumber+=strtemp;
	
	//��ش���
	m_strEditDisplayData.Empty();
	UpdateData(FALSE);//���±༭�����ݣ����ڽ�����ʾ������ʾ��������
}

void CDataAnalysisToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	unsigned int datalen;
	unsigned int i;
	CString strtemp;

	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==TIMER_READ_PORT_DATA)
	{
		//MessageBox(_T("���붨ʱ��"),_T("��ʱ��"));

		

		datalen=circular_buf.get_buf_length_used();//��ȡ���ݳ���
		if(datalen!=0)//���λ�������Ϊ��
		{
			//m_strEditDisplayData+=_T("\r\n\r\nEnter:");
			//ȫ����ȡ����
			circular_buf.read(display_buf,datalen);
			
			//��ʼ��������ʾԭʼ���ݺͽ�������
			for(i=0;i<datalen;i++)//
			{
				analysis_rcv_FSM(display_buf[i]);//���ｫֱ�����

			}
			

			/* ��ʾ���� */
			m_RxNumber+=datalen;//������յ����ַ���
			strtemp.Format(_T("%d"),m_RxNumber);//ת��ΪCString�ַ�����ʾ
			m_strEditRxNumber=_T("����:");
			m_strEditRxNumber+=strtemp;
			
			strtemp.Format(_T("%d"),analysis_get_discard_number());//ת��ΪCString�ַ�����ʾ
			m_strEditUncongnizedNumber=_T("δʶ����:");
			m_strEditUncongnizedNumber+=strtemp;

			UpdateData(FALSE);//���±༭�����ݣ����ڽ�����ʾ������ʾ��������
			
			//��Ϊ����UpdateData(FALSE);��ѹ������Զ����������Ե��¹������������ع�������˸ɴ಻����
			//m_ctrlEditDataDisplay.LineScroll(m_ctrlEditDataDisplay.GetLineCount());//������ʾ�����һ��
			//this->SendDlgItemMessage(IDC_EDIT_DATA_DISPLAY,WM_VSCROLL, SB_BOTTOM,0); //������ʼ���ڵײ�

		}
		else//���λ�������
		{
			//ʲô������
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDataAnalysisToolDlg::AutoNextLine(CString strLineText)
{

	CEdit   *pEdit= (CEdit*)GetDlgItem(IDC_EDIT_DATA_DISPLAY);
	int   nLen   =   pEdit->GetWindowTextLength();
	pEdit-> SetFocus();
	pEdit-> SetSel(nLen,   nLen);
	pEdit-> ReplaceSel(strLineText);

}

//�ؼ�λ���Զ��ı�
void CDataAnalysisToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//������ʾ��
	CWnd* pWnd=GetDlgItem(IDC_EDIT_DATA_DISPLAY); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(130,40,cx-150,cy-100);

	//������ʾ����ı߽���ʾ��
	pWnd=GetDlgItem(IDC_STATIC); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(123,20,cx-137,cy-70);

	//���ڽ��������ַ�������ʾ��
	pWnd=GetDlgItem(IDC_EDIT1); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(cx-137,cy-30,150,30);

	//���ڽ������ַ�δʶ������ʾ��
	pWnd=GetDlgItem(IDC_EDIT2); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(cx-280,cy-30,150,30);
	
	//���ڽ������������ť
	pWnd=GetDlgItem(IDC_BUTTON_CLEAR_DATA); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(7,cy-50,100,32);

	
	
}
