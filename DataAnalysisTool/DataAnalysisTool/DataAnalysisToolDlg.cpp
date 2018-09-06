
// DataAnalysisToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataAnalysisTool.h"
#include "DataAnalysisToolDlg.h"
#include "DlgProxy.h"

#include "analysis.h"
#include "EnumSerial.h"//添加串口自动扫描功能

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//我的全局变量
CircularBuf circular_buf(10*1024);//定义缓存缓冲区，大小为64KB
BYTE display_buf[10*1024];//用于缓存显示
CString display_new_line;

CArray<SSerInfo,SSerInfo&> asi;


#define TIMER_READ_PORT_DATA 1





// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDataAnalysisToolDlg 对话框




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
	, m_strEditSlavePacketNumber(_T(""))
	, m_strEditMasterPacketNumber(_T(""))
	, strCurrentPortName(_T(""))
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CDataAnalysisToolDlg::~CDataAnalysisToolDlg()
{
	// 如果该对话框有自动化代理，则
	//  将此代理指向该对话框的后向指针设置为 NULL，以便
	//  此代理知道该对话框已被删除。
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
	DDX_Text(pDX, IDC_EDIT3, m_strEditSlavePacketNumber);
	DDX_Text(pDX, IDC_EDIT4, m_strEditMasterPacketNumber);
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
	ON_WM_DEVICECHANGE()
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, &CDataAnalysisToolDlg::OnCbnSelchangeComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CDataAnalysisToolDlg::OnCbnSelchangeComboBaudrate)
END_MESSAGE_MAP()


// CDataAnalysisToolDlg 消息处理程序

BOOL CDataAnalysisToolDlg::OnInitDialog()
{
	CString strtemp;
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//↓↓↓↓↓↓↓↓↓↓↓↓↓以下是我们自己添加的程序↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	
	//串口初始化
	//m_ctrlComboCommPort.SetCurSel(5);//串口选择框默认选择COM1
	//m_nPort=1;//默认选择COM1
	GetCom();//获取可用串口
	m_ctrlComboCommBaudRate.SetCurSel(9);//波特率默认选择115200
	//m_ctrlComm.put_CommPort(m_nPort);
	OpenPortStatus=FALSE;
	if(m_ctrlComm.get_PortOpen())
	{
		m_ctrlComm.put_PortOpen(FALSE);//关闭串口
	}
	m_ctrlButtonClosePort.EnableWindow(m_ctrlComm.get_PortOpen());//根据串口是否打开，置“打开串口”、“关闭串口”有效或者无效
	m_ctrlButtonOpenPort.EnableWindow(!m_ctrlComm.get_PortOpen());
	
	SetTimer(TIMER_READ_PORT_DATA,50,NULL); //100毫秒定时器，获取环形缓冲区中的数据并解析后显示到界面中
	
	m_ctrlEditDataDisplay.SetLimitText(UINT_MAX);//设置显示最大字符数,这样不会导致字符显示到一定数量后停止显示

	//显示接收到的字符数为0
	m_RxNumber=0;////初始化接收到的字符数量为0
	m_strEditRxNumber.Format(_T("%d"),m_RxNumber);//转换为CString字符串显示
	/* 显示设置 */
	strtemp.Format(_T("%d"),0);//转换为CString字符串显示
	m_strEditRxNumber=_T("总数:");
	m_strEditRxNumber+=strtemp;
	
	strtemp.Format(_T("%d"),0);//转换为CString字符串显示
	m_strEditUncongnizedNumber=_T("未识别数:");
	m_strEditUncongnizedNumber+=strtemp;

	strtemp.Format(_T("%d"),0);//转换为CString字符串显示
	m_strEditMasterPacketNumber=_T("主机指令:");
	m_strEditMasterPacketNumber+=strtemp;

	strtemp.Format(_T("%d"),0);//转换为CString字符串显示
	m_strEditSlavePacketNumber=_T("从机指令:");
	m_strEditSlavePacketNumber+=strtemp;

	UpdateData(FALSE);//更新编辑框内容，即在接收显示框中显示接收数据


	//analysis_init(&m_strEditDisplayData);//初始化协议解析模块
	display_new_line.Empty();
	analysis_init(&display_new_line);//初始化协议解析模块
	//↑↑↑↑↑↑↑↑↑↑↑↑↑以下是我们自己添加的程序↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑







	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataAnalysisToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDataAnalysisToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

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
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
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
	//AfxMessageBox(_T("查看软件相关信息"));
	CAboutDlg About_dlg;
	About_dlg.DoModal();
}

void CDataAnalysisToolDlg::OnBnClickedButtonOpen()
{

	// TODO: Add your control notification handler code here
	//↓↓↓↓↓↓↓↓↓↓↓↓↓以下是我们自己添加的程序↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	CString strTemp;

	if(OpenPortStatus==FALSE)//如果串口未打开，执行下列内容
	{
		m_nPort=asi[m_ctrlComboCommPort.GetCurSel()].cComNum;//获得组合框中的串口号

		m_ctrlComm.put_CommPort(m_nPort);//选择相应串口
		m_ctrlComm.put_InputMode(1);//设置接收数据拣出模式，即二进制模式
		m_ctrlComm.put_InBufferSize(16*1024);//设置串口接收缓冲区大小为16KB
		m_ctrlComm.put_OutBufferSize(128);//设置串口发送缓冲区大小为字节

		switch(m_ctrlComboCommBaudRate.GetCurSel())//根据选择设置波特率
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
		//m_ctrlComm.put_Settings(_T("115200,n,8,1"));//设置波特率为bps，N表示无校验位（E为偶校
											//验，O为奇校验）位数据位一位停止位
		
		m_ctrlComm.put_PortOpen(TRUE);//打开串口
		
		m_ctrlComm.put_RThreshold(1);//设置接收事件触发门槛，表示每当接收到一个及以上字节时引发一个接收事件
		m_ctrlComm.put_InputLen(0);//设置读取接收数据长度为，表示无长度限制，每次读取时一次性读取接收缓冲区中的全部数据

		m_ctrlComm.put_InBufferCount(0);//清空接收缓冲区，即清除接收缓冲区中的残留数据
		if(m_ctrlComm.get_PortOpen())
		{		
			m_ctrlButtonClosePort.EnableWindow(m_ctrlComm.get_PortOpen());//按照串口标志位，置“打开串口”、“关闭串口”有效或者无效
			m_ctrlButtonOpenPort.EnableWindow(!m_ctrlComm.get_PortOpen());
			OpenPortStatus=TRUE;
			//获取当前选中的串口设备名称
			m_ctrlComboCommPort.GetLBText(m_ctrlComboCommPort.GetCurSel(),strCurrentPortName);
			//MessageBox(_T("所选串口成功打开"),_T("打开成功"));

		}
		else
		{
			OpenPortStatus=FALSE;
			MessageBox(_T("串口无法打开或已被占用"),_T("串口打开失败"));
		}


	}

//↑↑↑↑↑↑↑↑↑↑↑↑↑以下是我们自己添加的程序↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

}

void CDataAnalysisToolDlg::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	if(OpenPortStatus==TRUE)//如果串口打开，执行下列内容
	{
		m_ctrlComm.put_PortOpen(FALSE);//关闭串口
		if(!m_ctrlComm.get_PortOpen())
		{
			OpenPortStatus=FALSE;
			m_ctrlButtonClosePort.EnableWindow(OpenPortStatus);//按照串口标志位，置“打开串口”、“关闭串口”有效或者无效
			m_ctrlButtonOpenPort.EnableWindow(!OpenPortStatus);
			//清空当前选中的串口设备名称
			strCurrentPortName.Empty();
			//MessageBox(_T("所选串口成功关闭"),_T("打开成功"));
		}
		else
		{
			MessageBox(_T("串口未成功关闭"),_T("关闭成功"));
		}
	}

	
}
BEGIN_EVENTSINK_MAP(CDataAnalysisToolDlg, CDialog)
	ON_EVENT(CDataAnalysisToolDlg, IDC_MSCOMM1, 1, CDataAnalysisToolDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

//串口控件接收函数
void CDataAnalysisToolDlg::OnCommMscomm1()
{
	// TODO: Add your message handler code here

//↓↓↓↓↓↓↓↓↓↓↓↓↓以下是我们自己添加的程序↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	BYTE temp;
//	static char rxdata[2048];//设置BYTE型数组
	CString strtemp;//
	if(m_ctrlComm.get_CommEvent()==2)//事件值为表示接收数据事件
	{
		variant_inp=m_ctrlComm.get_Input();//读接收缓冲区
		safearray_inp=variant_inp;//VARIANT型转换成Cstring型变量
		len=safearray_inp.GetOneDimSize();//得到有效数据长度
		for(k=0;k<len;k++)
		{
			safearray_inp.GetElement(&k,&temp);//转换为BYTE型数组
			circular_buf.write(&temp,1);//存入环形缓冲区

		}

	//	for(k=0;k<len;k++)//将数组转换为Cstring型变量
	//	{
	//		BYTE bt=*(char*)(rxdata+k);//字符型
	////		strtemp.Format("%c",bt);//将字符送入临时变量strtemp存放
	//		strtemp.Format(_T(" %02X"), bt);
	//		m_strEditDisplayData+=strtemp;//加入接收编辑框对应字符串型映射变量中
	//	}
	
	}
	
	//UpdateData(FALSE);//更新编辑框内容，即在接收显示框中显示接收数据


//↑↑↑↑↑↑↑↑↑↑↑↑↑以下是我们自己添加的程序↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

}

//清除显示的监控数据
void CDataAnalysisToolDlg::OnBnClickedButtonClearData()
{
	// TODO: Add your control notification handler code here
	CString strtemp;
	
	//总字符数
	m_RxNumber=0;////初始化接收到的字符数量为0
	strtemp.Format(_T("%d"),m_RxNumber);//转换为CString字符串显示
	m_strEditRxNumber=_T("总数:");
	m_strEditRxNumber+=strtemp;
	
	
	//未识别字符数
	analysis_clear_discard_number();
	strtemp.Format(_T("%d"),analysis_get_discard_number());//转换为CString字符串显示
	m_strEditUncongnizedNumber=_T("未识别数:");
	m_strEditUncongnizedNumber+=strtemp;

	//正确解析的主机包个数
	analysis_clear_correct_master_pocket_number();
	strtemp.Format(_T("%d"),analysis_get_correct_master_pocket_number());//转换为CString字符串显示
	m_strEditUncongnizedNumber=_T("主机指令:");
	m_strEditUncongnizedNumber+=strtemp;

	//正确解析的从机包个数
	analysis_clear_correct_slave_pocket_number();
	strtemp.Format(_T("%d"),analysis_get_correct_slave_pocket_number());//转换为CString字符串显示
	m_strEditUncongnizedNumber=_T("从机指令:");
	m_strEditUncongnizedNumber+=strtemp;
		
	//已接收到的正确主机通信包个数
	strtemp.Format(_T("%d"),0);//转换为CString字符串显示
	m_strEditMasterPacketNumber=_T("主机指令:");
	m_strEditMasterPacketNumber+=strtemp;

	//已接收到的正确从机通信包个数
	strtemp.Format(_T("%d"),0);//转换为CString字符串显示
	m_strEditSlavePacketNumber=_T("从机指令:");
	m_strEditSlavePacketNumber+=strtemp;

	//监控窗口
	m_strEditDisplayData.Empty();
	UpdateData(FALSE);//更新编辑框内容，即在接收显示框中显示接收数据
}

void CDataAnalysisToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	unsigned int datalen;
	unsigned int i;
	CString strtemp;

	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==TIMER_READ_PORT_DATA)
	{
		//MessageBox(_T("进入定时器"),_T("定时器"));

		

		datalen=circular_buf.get_buf_length_used();//获取数据长度
		if(datalen!=0)//环形缓冲区不为空
		{
			//m_strEditDisplayData+=_T("\r\n\r\nEnter:");
			//全部读取出来
			circular_buf.read(display_buf,datalen);
			
			//开始分析并显示原始数据和解析数据
			for(i=0;i<datalen;i++)//
			{
				analysis_rcv_FSM(display_buf[i]);//这里将直接输出

			}
			

			/* 显示设置 */
			m_RxNumber+=datalen;//计算接收到的字符数
			strtemp.Format(_T("%d"),m_RxNumber);//转换为CString字符串显示
			m_strEditRxNumber=_T("总数:");
			m_strEditRxNumber+=strtemp;
			
			strtemp.Format(_T("%d"),analysis_get_discard_number());//转换为CString字符串显示
			m_strEditUncongnizedNumber=_T("未识别数:");
			m_strEditUncongnizedNumber+=strtemp;

			//正确解析的主机包个数
			strtemp.Format(_T("%d"),analysis_get_correct_master_pocket_number());//转换为CString字符串显示
			m_strEditMasterPacketNumber=_T("主机指令:");
			m_strEditMasterPacketNumber+=strtemp;

			//正确解析的从机包个数
			strtemp.Format(_T("%d"),analysis_get_correct_slave_pocket_number());//转换为CString字符串显示
			m_strEditSlavePacketNumber=_T("从机指令:");
			m_strEditSlavePacketNumber+=strtemp;

			if(display_new_line.IsEmpty()==0)
			{
				AutoNextLine(display_new_line);//追加解析内容
				display_new_line.Empty();//清空临时变量
			}
			//UpdateData(FALSE);//更新编辑框内容，即在接收显示框中显示接收数据
			UpdateEditDisplay();

			//因为函数UpdateData(FALSE);会把滚动条自动上拉，所以导致滚动条上下来回滚动，因此干脆不用了
			//m_ctrlEditDataDisplay.LineScroll(m_ctrlEditDataDisplay.GetLineCount());//滚动显示到最后一行
			//this->SendDlgItemMessage(IDC_EDIT_DATA_DISPLAY,WM_VSCROLL, SB_BOTTOM,0); //滚动条始终在底部

		}
		else//环形缓冲区空
		{
			//什么都不做
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

//控件位置自动改变
void CDataAnalysisToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//数据显示框
	CWnd* pWnd=GetDlgItem(IDC_EDIT_DATA_DISPLAY); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(130,40,cx-150,cy-100);

	//数据显示框外的边界提示框
	pWnd=GetDlgItem(IDC_STATIC); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(123,20,cx-137,cy-70);

	//串口接收数据字符总数显示框
	pWnd=GetDlgItem(IDC_EDIT1); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(cx-137,cy-30,150,30);

	//串口接收数字符未识别数显示框
	pWnd=GetDlgItem(IDC_EDIT2); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(cx-137-143,cy-30,150,30);
	
	//已接收到的正确从机通信包个数显示框
	pWnd=GetDlgItem(IDC_EDIT3); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(cx-137-143-143,cy-30,150,30);

	//已接收到的正确主机通信包个数显示框
	pWnd=GetDlgItem(IDC_EDIT4); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(cx-137-143-143-143,cy-30,150,30);

	//串口接收数据清除按钮
	pWnd=GetDlgItem(IDC_BUTTON_CLEAR_DATA); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(7,cy-50,100,32);
	
	
}

// 更新显示框的显示
void CDataAnalysisToolDlg::UpdateEditDisplay(void)
{
	CEdit   *pEdit;

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowText(m_strEditRxNumber);//正确解析的主机包个数
	
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	pEdit->SetWindowText(m_strEditUncongnizedNumber);//串口接收数字符未识别数显示框

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	pEdit->SetWindowText(m_strEditSlavePacketNumber);//已接收到的正确从机通信包个数显示框

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT4);
	pEdit->SetWindowText(m_strEditMasterPacketNumber);//已接收到的正确主机通信包个数显示框


}

// 获取PC当前可用的所有串口号（获取串口名字，然后从名字中计算出串口号，COM18的串口号是18
void CDataAnalysisToolDlg::GetCom(void)
{
	////程序启动时获取全部可用串口
	//HANDLE hCom;
	//int i,num,k;
	//CString str;
	//BOOL flag;

	//((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->ResetContent();
	//flag = FALSE;
	//num = 0;
	//for (i = 1;i <= 16;i++)
	//{//此程序支持16个串口
	//	str.Format(_T("\\\\.\\COM%d"),i);
	//	hCom = CreateFile(str, 0, 0, 0, 
	//	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//	if(INVALID_HANDLE_VALUE != hCom )
	//	{//能打开该串口，则添加该串口
	//		CloseHandle(hCom);
	//		str = str.Mid(4);
	//		((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->AddString(str);
	//		if (flag == FALSE)
	//		{
	//			flag = TRUE;
	//			num = i;
	//		}
	//	}
	//}
	//i = ((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->GetCount();
	//if (i == 0)
	//{//若找不到可用串口则禁用“打开串口”功能
	//	((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(FALSE);
	//}
	//else
	//{
	//	k = ((CComboBox *)GetDlgItem((IDC_COMBO_PORT)))->GetCount();
	//	((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->SetCurSel(k - 1);
	//	//mCom.BindCommPort(num);
	//}


	//////////////从注册表中读取用户的串口信息/////////
	//HKEY hKEY;
	//CString temp;
	//LPCTSTR data_Set=_T("HARDWARE\\DEVICEMAP\\SERIALCOMM"); 
	//((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->ResetContent();

	//if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set, 0, KEY_READ, &hKEY))	//打开注册表
	//{
	//	AfxMessageBox(_T("注册表错误: 无法打开有关的hKEY!")); 
	//	return;
	//} 
	//long i;
	//LPBYTE Data_Get=new BYTE [10];
	//for (i=0; ;i++) //中間為空,沒有進行i值的比較
	//{
	//	TCHAR szValueName[_MAX_PATH + 1];
	//	DWORD dwValueName = sizeof(szValueName);
	//	LPDWORD lpType = 0;
	//	
	//	DWORD cbData = 10;
	//	long ret = (RegEnumValue(hKEY,i,szValueName,&dwValueName,
	//		NULL,lpType,Data_Get,&cbData));
	//	if (ret)	  
	//		break;

	//	temp.Format(_T("%s"),Data_Get);
	//	((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->AddString(temp);
	//	
	//	//Port_Num = i+1;
	//
	//}
	//delete[] Data_Get;
	//::RegCloseKey(hKEY);	//关闭注册表
	//////////////////////////////////////////////////////////

	int dTemp;
	CString strTemp;

	// Populate the list of serial ports.
	EnumSerialPorts(asi,FALSE/*include all*/);//只能获取名字，而不能获取端口号
	m_ctrlComboCommPort.ResetContent();
	//自己挨个计算出端口号
	for (int ii=0; ii<asi.GetSize(); ii++) 
	{
		//查找第一个"COM"字符串出现的地方
		dTemp=asi[ii].strFriendlyName.Find(_T("COM"));
		if(dTemp>0)//find successed!
		{
			//后面的两位数字为串口号，目前暂时不支持COM99以上，因为这种情况几乎不会出现
			if(asi[ii].strFriendlyName[dTemp+3]>='0' && asi[ii].strFriendlyName[dTemp+3]<='9')
			{
				if(asi[ii].strFriendlyName[dTemp+4]>='0' && asi[ii].strFriendlyName[dTemp+4]<='9')
				{
					asi[ii].cComNum=(asi[ii].strFriendlyName[dTemp+3]-'0')*10 + (asi[ii].strFriendlyName[dTemp+4]-'0');

				}
				else
				{
					asi[ii].cComNum=asi[ii].strFriendlyName[dTemp+3]-'0';
				}
			}
			else
			{
				asi[ii].cComNum=0;//Error
			}
		}
		else
		{
			asi[ii].cComNum=0;//Error
		}
		m_ctrlComboCommPort.AddString(asi[ii].strFriendlyName);//
		//m_ctrlComboCommPort.AddString(asi[ii].strPortName);
		//m_ctrlComboCommPort.AddString(asi[ii].strDevPath);
		//m_ctrlComboCommPort.AddString(asi[ii].strPortDesc);
	}
	//重置选择框的宽度，这样不会显示不全
	ResetCtrlWidth(&m_ctrlComboCommPort);// reset combobox width
	if(asi.GetSize())
	{
		m_ctrlComboCommPort.SetCurSel(0);
	}
}
//重置选择框的宽度，这样不会显示不全
void CDataAnalysisToolDlg::ResetCtrlWidth(CComboBox* lpCtrl)
{
	CString str;  
	int dx = 0;  
	CSize sz;  
	CDC *pDC = lpCtrl->GetDC();  
	CFont *pOldFont = pDC->SelectObject(lpCtrl->GetFont());  
	for (int i = 0; i < lpCtrl->GetCount(); i++)  
	{  
		lpCtrl->GetLBText(i, str);  
		sz = pDC->GetTextExtent(str);  
		if (sz.cx > dx)  
		{  
			dx = sz.cx;  
		}  
	}  
	lpCtrl->SetDroppedWidth(dx+24);  
	pDC->SelectObject(pOldFont);  
	lpCtrl->ReleaseDC(pDC); 
}
//当有设备拔出或者有新设备插入PC中将进入此函数
BOOL CDataAnalysisToolDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	CString str;
	BYTE result=0;
	//str.Format(_T("设备出现变化，nEventType=%d,dwData=%d."),nEventType,dwData);
	//AfxMessageBox(str);
	switch (nEventType)//查看设备变化的具体类型
	{  
		case 0x8004/*DBT_DEVICEREMOVECOMPLETE*/://移除设备
			GetCom();//重新扫描串口
			if(OpenPortStatus==TRUE)//如果串口已经打开
			{
				if(m_ctrlComboCommPort.GetCount()==0)//已经没有可用的串口设备了
				{
					str=_T("当前使用的端口： ");
					str+=strCurrentPortName;
					str+=_T("\r\n已经被拔出！监控停止！");
					MessageBox(str,_T("警告！"),MB_ICONSTOP);
					OnBnClickedButtonClose();//关闭串口
				}
				else//
				{
				
					for(int i=0;i<m_ctrlComboCommPort.GetCount();i++)
					{
						//之前打开的串口还在，那么重新选中此项
						if(strCurrentPortName==asi[i].strFriendlyName)
						{
							m_ctrlComboCommPort.SetCurSel(i);
							result=1;
							break;
						}
						else
						{
						}
					}
					//之前打开的串口已经没有了
					if(result==0)
					{
						str=_T("当前使用的端口： ");
						str+=strCurrentPortName;
						str+=_T("\r\n已经被拔出！监控停止！");
						MessageBox(str,_T("警告！"),MB_ICONSTOP);
						OnBnClickedButtonClose();//关闭串口
					}
				}
			}
			//AfxMessageBox(_T("设备移除，重新扫描串口"));
			break;
		case 0x8000/*DBT_DEVICEREMOVECOMPLETE*/://增加设备
			GetCom();//重新扫描串口
			if(OpenPortStatus==TRUE)//如果串口已经打开
			{
				for(int i=0;i<m_ctrlComboCommPort.GetCount();i++)
				{
					//之前打开的串口还在，那么重新选中此项
					if(strCurrentPortName==asi[i].strFriendlyName)
					{
						m_ctrlComboCommPort.SetCurSel(i);
						result=1;
						break;
					}
					else
					{
					}
				}
			}
			//AfxMessageBox(_T("设备新增，重新扫描串口"));
			break;

		default:
			break;
	}
	return TRUE; 
}
//当串口选择更改后将进入此函数
void CDataAnalysisToolDlg::OnCbnSelchangeComboPort()
{
	// TODO: Add your control notification handler code here
	CString str;

	//AfxMessageBox(_T("选择的端口号改变了"));

	if(OpenPortStatus==TRUE)//如果串口已经打开
	{
		//获取当前选中的串口设备名称
		m_ctrlComboCommPort.GetLBText(m_ctrlComboCommPort.GetCurSel(),str);
		if(strCurrentPortName!=str)
		{
			for(int i=0;i<m_ctrlComboCommPort.GetCount();i++)
			{
				//重新选中之前的串口号
				if(strCurrentPortName==asi[i].strFriendlyName)
				{
					m_ctrlComboCommPort.SetCurSel(i);
					break;
				}
				else
				{
				}
			}
			str=_T("请先关闭当前端口： ");
			str+=strCurrentPortName;
			str+=_T("\r\n后再选择其他端口！");
			MessageBox(str,_T("提示"),MB_ICONINFORMATION);
			//AfxMessageBox(_T("请先关闭当前端口后再选择其他端口！"));
		}
	}
}
//当波特率选择更改后将进入此函数
void CDataAnalysisToolDlg::OnCbnSelchangeComboBaudrate()
{
// TODO: Add your control notification handler code here
	unsigned int BoadRate;
	BYTE i;
	CString str;

	if(OpenPortStatus==TRUE)//如果串口已经打开
	{
		switch(m_ctrlComboCommBaudRate.GetCurSel())//根据选择设置波特率
		{
			case 0:
				BoadRate=1200;
				break;
			case 1:
				BoadRate=2400;
				break;
			case 2:
				BoadRate=4800;
				break;
			case 3:
				BoadRate=9600;
				break;
			case 4:
				BoadRate=14400;
				break;
			case 5:
				BoadRate=19200;
				break;
			case 6:
				BoadRate=28800;
				break;
			case 7:
				BoadRate=38400;
				break;
			case 8:
				BoadRate=57600;
				break;
			case 9:
				BoadRate=115200;
				break;
			case 10:
				BoadRate=230400;
				break;
			case 11:
				BoadRate=256000;
				break;
			default:
				BoadRate=115200;
				break;
		}

		if(m_nBaudRate!=BoadRate)
		{
			switch(m_nBaudRate)//根据选择设置波特率
			{
				case 1200:
					i=0;
					break;
				case 2400:
					i=1;
					break;
				case 4800:
					i=2;
					break;
				case 9600:
					i=3;
					break;
				case 14400:
					i=4;
					break;
				case 19200:
					i=5;
					break;
				case 28800:
					i=6;
					break;
				case 38400:
					i=7;
					break;
				case 57600:
					i=8;
					break;
				case 115200:
					i=9;
					break;
				case 230400:
					i=10;
					break;
				case 256000:
					i=11;
					break;
				default:
					i=0;
					break;
			}
			m_ctrlComboCommBaudRate.SetCurSel(i);

			str=_T("请先关闭当前端口： ");
			str+=strCurrentPortName;
			str+=_T("\r\n后再更改波特率！");
			MessageBox(str,_T("提示"),MB_ICONINFORMATION);
		}
	}
}
