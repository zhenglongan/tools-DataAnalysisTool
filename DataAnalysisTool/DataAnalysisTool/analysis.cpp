#include "StdAfx.h"
#include "analysis.h"

BYTE rcv_buf[MAX_PACKET_LENGTH];

static volatile RcvState RxState;//通信接收状态
static volatile unsigned int discard_number;//无法解析的字符数


CString *pstrOut; //字符串输出指针，初始化时需要制定

static BYTE bcc_verify(BYTE* a,unsigned short len);//BCC校验函数
static void get_current_time(CString* pstr);//获取当前时间到字符串

void analysis_init(CString *pstr)//初始化相关变量
{
	RxState = RCV_STATE_HEAD;
	discard_number = 0;
	pstrOut = pstr;//制定输出字符串变量
	memset(rcv_buf,0x00,MAX_PACKET_LENGTH);
}


void analysis_rcv_FSM(BYTE dat)//解析数据帧：接收状态机函数
{
	static BYTE len;
	switch(RxState)
	{
		/* If we have received a character in the IDLE state(state 1) ,
		*just do nothing.
		*/
		case RCV_STATE_IDLE:				/* state 1 */
			//do nothing
			break;

		case RCV_STATE_HEAD:				/* state 2 */
			if(dat == DATA_START)
			{
				rcv_buf[0] = dat;
				RxState = RCV_STATE_LEN;//turn to state 3
				//AfxMessageBox(_T("解析HEAD正确"));
			}
			else
			{
				//丢弃
				discard_number++;
			}
			break;

		case RCV_STATE_LEN:					/* state 3 */
			if(dat>=3)
			{
				len=dat;
				rcv_buf[1] = dat;
				RxState = RCV_STATE_COMMAND;//turn to state 4
				//AfxMessageBox(_T("解析LEN正确"));
			}
			else
			{
				//丢弃
				discard_number+=2;//丢弃此字节和上一个字节
				RxState = RCV_STATE_HEAD;//turn to state 2
			}
			break;
		
		case RCV_STATE_COMMAND:				/* state 4 */
			if(dat>0x30)//接收的命令字范围，必须大于0x30
			{
				rcv_buf[2] = dat;//命令字
				if(len==3)
				{
					RxState = RCV_STATE_BCC;//turn to state 6,have no data area
					//AfxMessageBox(_T("解析COMMAND正确"));
				}
				else
				{
					RxState = RCV_STATE_DATA;//turn to state 6
				}
			}
			else
			{
				//丢弃
				discard_number+=3;//丢弃此字节和之前的所有字节
				RxState = RCV_STATE_HEAD;//turn to state 2
			}
			break;

		case RCV_STATE_DATA:				/* state 5 */
			rcv_buf[3+rcv_buf[1]-len]=dat;
			if(--len==3)//接收DATA部分完成
			{
				len=0;
				RxState = RCV_STATE_BCC;//turn to state 6
				//AfxMessageBox(_T("解析DATA完成"));
			}
			break;

		case RCV_STATE_BCC:					/* state 6 */
			if(dat == bcc_verify(rcv_buf,rcv_buf[1]))//校验区域为HEAD到DATA，长度为LEN
			{
				rcv_buf[rcv_buf[1]]=dat;//存入BCC
				RxState = RCV_STATE_TAIL;//turn to state 7
				//AfxMessageBox(_T("BCC正确"));
			}
			else
			{
				//丢弃
				discard_number = discard_number+rcv_buf[1]+1;//丢弃此字节和之前的所有字节
				RxState = RCV_STATE_HEAD;//turn to state 2
				//AfxMessageBox(_T("BCC错误"));
			}
			break;

		case RCV_STATE_TAIL:				/* state 7 */
			if(dat == DATA_END)
			{
				rcv_buf[rcv_buf[1]+1] = dat;
				RxState = RCV_STATE_HEAD;//turn to state 2
				//调用协议解析函数，程序运行到此处说明已经解析出了一条完整的数据包
				//AfxMessageBox(_T("通信帧接收正确"));
				analysis_protocol(rcv_buf,pstrOut);
				


				//RxState = RCV_STATE_HEAD;//turn to state 2
			}
			else
			{
				//丢弃
				discard_number = discard_number+rcv_buf[1]+2;//丢弃此字节和之前的所有字节
				RxState = RCV_STATE_HEAD;//turn to state 2
			}
			break;
		
		case RCV_STATE_CMPL:				/* state 8 */
			RxState = RCV_STATE_HEAD;//turn to state 2
			break;

		case CLOUD_STATE_RX_ERROR:			/* state 9 */
			RxState = RCV_STATE_HEAD;//turn to state 2
			break;
		
		
		
	}
}

//--------------------------------------------------------------------------------------
//  Name:  verify
//  pars: *a
//  Returns: UINT8
//  Description: verify receive data
//--------------------------------------------------------------------------------------
static BYTE bcc_verify(BYTE* a,unsigned short len)
{
	unsigned short i;
	BYTE result;
	result=0x00;

	for(i=0;i<len;i++)
	{
		result ^= a[i];	
	}
	return result;
}
static void get_current_time(CString* pstr)//获取当前时间到字符串,精确到毫秒
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	pstr->Format(_T("%02d:%02d:%02d:%03d"),st.wHour,st.wMinute,st.wSecond,st.wMilliseconds) ;
}

unsigned int analysis_get_discard_number(void)//获取无法解析的字符数
{
	return discard_number;
}

void analysis_clear_discard_number(void)//清零无法解析的字符数
{
	discard_number=0;
}

void analysis_protocol(BYTE* pcommdata,CString *pstrout)//解析正确的通信帧并输出相应的解析信息
{
	bool packet_source;//0表示主机发送来的，1表示从机发送来的
	BYTE len,command_word;
	BYTE i;
	CString strtemp,strtime;

	get_current_time(&strtime);//获取系统时间到字符串strtime，格式为hh:mm:ss:mmm

	len=pcommdata[1];
	command_word=pcommdata[2];
	/********************************************************************************************
	 * 第1步，判断是主机还是从机指令,赋值给变量packet_source
	 *********************************************************************************************/

	//0x99有可能是从机的回复：DOWNLOAD_STEP_CURVE_RET
	//也有可能是主机指令：SERVOPARA
	if(command_word==0x99)
	{
		if(len==3)//len=3表示是从机的回复：DOWNLOAD_STEP_CURVE_RET
		{
			packet_source = 1;
		}
		else//是主机指令：SERVOPARA
		{
			packet_source = 0;
		}
	}
	//0x99有可能是从机的回复：DOWNLOAD_STEP_CURVE_DONE_RET
	//也有可能是主机指令：SYS_PARAM_GROUP
	else if(command_word==0x9A)
	{
		if(len==5)//len=3表示是从机的回复：DOWNLOAD_STEP_CURVE_DONE_RET
		{
			packet_source = 1;
		}
		else//是主机指令：SYS_PARAM_GROUP
		{
			packet_source = 0;
		}
	}
	else//其他的就好判断了
	{
		if(command_word<0xA0)//主机命令字小于0xA0,因为超过0xA0,从机回复命令字（主机命令字加上0x60）
		{
			packet_source = 0;
		}
		else
		{
			packet_source = 1;
		}
	}


	/********************************************************************************************
	 * 第2步，分开处理主机指令和从机指令
	 *********************************************************************************************/

	//------------------------------------------------主机指令------------------------------------------------
	if(packet_source==0)
	{
		*pstrout+=strtime;
		*pstrout+=_T(" Master-->");//首先显示时间和头部信息

		for(i=0;i<len+2;i++)//原文粘贴数据
		{
			strtemp.Format(_T(" %02X"), rcv_buf[i]);
			*pstrout+=strtemp;//加入接收编辑框对应字符串型映射变量中
		}
		//此处添加解析数据,暂时忽略
		//......


		//最后添加换行，完成分析并输出
		*pstrout+=_T("\r\n");
	}
	//------------------------------------------------从机指令-----------------------------------------------
	else
	{
		*pstrout+=strtime;
		*pstrout+=_T(" Slave-->");//首先显示时间和头部信息
		
		for(i=0;i<len+2;i++)//原文粘贴数据
		{
			strtemp.Format(_T(" %02X"), rcv_buf[i]);
			*pstrout+=strtemp;//加入接收编辑框对应字符串型映射变量中
		}
		//此处添加解析数据,暂时忽略
		//......


		//最后添加换行，完成分析并输出
		*pstrout+=_T("\r\n");
		*pstrout+=_T("\r\n");//从机多加一个换行，这样可以分辨出主从机指令的配对情况
	}

}