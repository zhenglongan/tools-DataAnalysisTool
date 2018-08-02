#include "StdAfx.h"
#include "analysis.h"

BYTE rcv_buf[MAX_PACKET_LENGTH];

static volatile RcvState RxState;//ͨ�Ž���״̬
static volatile unsigned int discard_number;//�޷��������ַ���


CString *pstrOut; //�ַ������ָ�룬��ʼ��ʱ��Ҫ�ƶ�

static BYTE bcc_verify(BYTE* a,unsigned short len);//BCCУ�麯��
static void get_current_time(CString* pstr);//��ȡ��ǰʱ�䵽�ַ���

void analysis_init(CString *pstr)//��ʼ����ر���
{
	RxState = RCV_STATE_HEAD;
	discard_number = 0;
	pstrOut = pstr;//�ƶ�����ַ�������
	memset(rcv_buf,0x00,MAX_PACKET_LENGTH);
}


void analysis_rcv_FSM(BYTE dat)//��������֡������״̬������
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
				//AfxMessageBox(_T("����HEAD��ȷ"));
			}
			else
			{
				//����
				discard_number++;
			}
			break;

		case RCV_STATE_LEN:					/* state 3 */
			if(dat>=3)
			{
				len=dat;
				rcv_buf[1] = dat;
				RxState = RCV_STATE_COMMAND;//turn to state 4
				//AfxMessageBox(_T("����LEN��ȷ"));
			}
			else
			{
				//����
				discard_number+=2;//�������ֽں���һ���ֽ�
				RxState = RCV_STATE_HEAD;//turn to state 2
			}
			break;
		
		case RCV_STATE_COMMAND:				/* state 4 */
			if(dat>0x30)//���յ������ַ�Χ���������0x30
			{
				rcv_buf[2] = dat;//������
				if(len==3)
				{
					RxState = RCV_STATE_BCC;//turn to state 6,have no data area
					//AfxMessageBox(_T("����COMMAND��ȷ"));
				}
				else
				{
					RxState = RCV_STATE_DATA;//turn to state 6
				}
			}
			else
			{
				//����
				discard_number+=3;//�������ֽں�֮ǰ�������ֽ�
				RxState = RCV_STATE_HEAD;//turn to state 2
			}
			break;

		case RCV_STATE_DATA:				/* state 5 */
			rcv_buf[3+rcv_buf[1]-len]=dat;
			if(--len==3)//����DATA�������
			{
				len=0;
				RxState = RCV_STATE_BCC;//turn to state 6
				//AfxMessageBox(_T("����DATA���"));
			}
			break;

		case RCV_STATE_BCC:					/* state 6 */
			if(dat == bcc_verify(rcv_buf,rcv_buf[1]))//У������ΪHEAD��DATA������ΪLEN
			{
				rcv_buf[rcv_buf[1]]=dat;//����BCC
				RxState = RCV_STATE_TAIL;//turn to state 7
				//AfxMessageBox(_T("BCC��ȷ"));
			}
			else
			{
				//����
				discard_number = discard_number+rcv_buf[1]+1;//�������ֽں�֮ǰ�������ֽ�
				RxState = RCV_STATE_HEAD;//turn to state 2
				//AfxMessageBox(_T("BCC����"));
			}
			break;

		case RCV_STATE_TAIL:				/* state 7 */
			if(dat == DATA_END)
			{
				rcv_buf[rcv_buf[1]+1] = dat;
				RxState = RCV_STATE_HEAD;//turn to state 2
				//����Э������������������е��˴�˵���Ѿ���������һ�����������ݰ�
				//AfxMessageBox(_T("ͨ��֡������ȷ"));
				analysis_protocol(rcv_buf,pstrOut);
				


				//RxState = RCV_STATE_HEAD;//turn to state 2
			}
			else
			{
				//����
				discard_number = discard_number+rcv_buf[1]+2;//�������ֽں�֮ǰ�������ֽ�
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
static void get_current_time(CString* pstr)//��ȡ��ǰʱ�䵽�ַ���,��ȷ������
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	pstr->Format(_T("%02d:%02d:%02d:%03d"),st.wHour,st.wMinute,st.wSecond,st.wMilliseconds) ;
}

unsigned int analysis_get_discard_number(void)//��ȡ�޷��������ַ���
{
	return discard_number;
}

void analysis_clear_discard_number(void)//�����޷��������ַ���
{
	discard_number=0;
}

void analysis_protocol(BYTE* pcommdata,CString *pstrout)//������ȷ��ͨ��֡�������Ӧ�Ľ�����Ϣ
{
	bool packet_source;//0��ʾ�����������ģ�1��ʾ�ӻ���������
	BYTE len,command_word;
	BYTE i;
	CString strtemp,strtime;

	get_current_time(&strtime);//��ȡϵͳʱ�䵽�ַ���strtime����ʽΪhh:mm:ss:mmm

	len=pcommdata[1];
	command_word=pcommdata[2];
	/********************************************************************************************
	 * ��1�����ж����������Ǵӻ�ָ��,��ֵ������packet_source
	 *********************************************************************************************/

	//0x99�п����Ǵӻ��Ļظ���DOWNLOAD_STEP_CURVE_RET
	//Ҳ�п���������ָ�SERVOPARA
	if(command_word==0x99)
	{
		if(len==3)//len=3��ʾ�Ǵӻ��Ļظ���DOWNLOAD_STEP_CURVE_RET
		{
			packet_source = 1;
		}
		else//������ָ�SERVOPARA
		{
			packet_source = 0;
		}
	}
	//0x99�п����Ǵӻ��Ļظ���DOWNLOAD_STEP_CURVE_DONE_RET
	//Ҳ�п���������ָ�SYS_PARAM_GROUP
	else if(command_word==0x9A)
	{
		if(len==5)//len=3��ʾ�Ǵӻ��Ļظ���DOWNLOAD_STEP_CURVE_DONE_RET
		{
			packet_source = 1;
		}
		else//������ָ�SYS_PARAM_GROUP
		{
			packet_source = 0;
		}
	}
	else//�����ľͺ��ж���
	{
		if(command_word<0xA0)//����������С��0xA0,��Ϊ����0xA0,�ӻ��ظ������֣����������ּ���0x60��
		{
			packet_source = 0;
		}
		else
		{
			packet_source = 1;
		}
	}


	/********************************************************************************************
	 * ��2�����ֿ���������ָ��ʹӻ�ָ��
	 *********************************************************************************************/

	//------------------------------------------------����ָ��------------------------------------------------
	if(packet_source==0)
	{
		*pstrout+=strtime;
		*pstrout+=_T(" Master-->");//������ʾʱ���ͷ����Ϣ

		for(i=0;i<len+2;i++)//ԭ��ճ������
		{
			strtemp.Format(_T(" %02X"), rcv_buf[i]);
			*pstrout+=strtemp;//������ձ༭���Ӧ�ַ�����ӳ�������
		}
		//�˴���ӽ�������,��ʱ����
		//......


		//�����ӻ��У���ɷ��������
		*pstrout+=_T("\r\n");
	}
	//------------------------------------------------�ӻ�ָ��-----------------------------------------------
	else
	{
		*pstrout+=strtime;
		*pstrout+=_T(" Slave-->");//������ʾʱ���ͷ����Ϣ
		
		for(i=0;i<len+2;i++)//ԭ��ճ������
		{
			strtemp.Format(_T(" %02X"), rcv_buf[i]);
			*pstrout+=strtemp;//������ձ༭���Ӧ�ַ�����ӳ�������
		}
		//�˴���ӽ�������,��ʱ����
		//......


		//�����ӻ��У���ɷ��������
		*pstrout+=_T("\r\n");
		*pstrout+=_T("\r\n");//�ӻ����һ�����У��������Էֱ�����ӻ�ָ���������
	}

}