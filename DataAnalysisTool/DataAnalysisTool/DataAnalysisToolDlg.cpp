
// DataAnalysisToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataAnalysisTool.h"
#include "DataAnalysisToolDlg.h"
#include "DlgProxy.h"

#include "analysis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//我的全局变量
CircularBuf circular_buf(10*1024);//定义缓存缓冲区，大小为64KB
BYTE display_buf[10*1024];//用于缓存显示

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
	m_ctrlComboCommPort.SetCurSel(5);//串口选择框默认选择COM1
	m_nPort=1;//默认选择COM1
	m_ctrlComboCommBaudRate.SetCurSel(9);//波特率默认选择115200
	m_ctrlComm.put_CommPort(m_nPort);
	OpenPortStatus=FALSE;
	if(m_ctrlComm.get_PortOpen())
	{
		m_ctrlComm.put_PortOpen(FALSE);//关闭串口
	}
	m_ctrlButtonClosePort.EnableWindow(m_ctrlComm.get_PortOpen());//根据串口是否打开，置“打开串口”、“关闭串口”有效或者无效
	m_ctrlButtonOpenPort.EnableWindow(!m_ctrlComm.get_PortOpen());
	
	SetTimer(TIMER_READ_PORT_DATA,50,NULL); //100毫秒定时器，获取环形缓冲区中的数据并解析后显示到界面中
	
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

	UpdateData(FALSE);//更新编辑框内容，即在接收显示框中显示接收数据


	analysis_init(&m_strEditDisplayData);//初始化协议解析模块
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
		m_nPort=m_ctrlComboCommPort.GetCurSel()+1;//获得组合框中的串口号
		m_ctrlComm.put_CommPort(m_nPort);//选择相应串口
		m_ctrlComm.put_InputMode(1);//设置接收数据拣出模式，即二进制模式
		m_ctrlComm.put_InBufferSize(16*1024);//设置串口接收缓冲区大小为16KB
		m_ctrlComm.put_OutBufferSize(128);//设置串口发送缓冲区大小为字节

		switch(m_ctrlComboCommPort.GetCurSel())//根据选择设置波特率
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
			MessageBox(_T("所选串口成功打开"),_T("打开成功"));

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
			MessageBox(_T("所选串口成功关闭"),_T("打开成功"));
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
	
	UpdateData(FALSE);//更新编辑框内容，即在接收显示框中显示接收数据


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

			UpdateData(FALSE);//更新编辑框内容，即在接收显示框中显示接收数据
			
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
		pWnd->MoveWindow(cx-280,cy-30,150,30);
	
	//串口接收数据清除按钮
	pWnd=GetDlgItem(IDC_BUTTON_CLEAR_DATA); 
	if(pWnd->GetSafeHwnd()) 
		pWnd->MoveWindow(7,cy-50,100,32);

	
	
}
