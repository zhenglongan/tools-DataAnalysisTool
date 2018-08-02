#ifndef ANALYSIS_H
#define ANALYSIS_H

#define DATA_START   0x5A
#define DATA_END     0xFF


/*
 * �н�ģ��������붨��
 */
#define DOWNLOAD_STEP_CURVE			0x39
#define DOWNLOAD_STEP_CURVE_RET 	0x99

#define DOWNLOAD_STEP_CURVE_DONE		0x3A
#define DOWNLOAD_STEP_CURVE_DONE_RET 	0x9A

#define RFID_WRITE 				0x44
#define RFID_WRITE_RET 			0xa4

#define RFID_READ 				0x45
#define RFID_READ_RET 			0xa5

#define TEST_IOOUTPUT    		0x48//��·IO �������
#define TEST_IOOUTPUT_RET  		0xa8

#define READY_GO_SETOUT 		0x4e
#define READY_GO_SETOUT_RET 	0xae

#define CUT_COMMAND            0x61
#define CUT_COMMAND_RET        0xB1

#define AUTO_SELECT_PATTERN     0x66  
#define AUTO_SELECT_PATTERN_RET 0xb6

#define CONNECT      0x80
#define CONNECT_RET  0xE0
#define MISTAKE_RET  0xA0

#define CHANGE       0x81
#define CHANGE_RET   0xE1

#define QUERY        0x82
#define QUERY_RET    0xE2

#define PARA         0x83
#define PARA_RET     0xE3

#define PATTERN      0x84
#define PATTERN_RET  0xE4

#define SHIFT        0x85
#define SHIFT_RET    0xE5

#define STEP         0x86
#define STEP_RET     0xE6

#define VERSION      0x87
#define VERSION_RET  0xE7

#define MACHINE      0x88
#define MACHINE_RET  0xE8

#define COOR         0x89
#define COOR_RET     0xE9

#define INMOVE       0x8A
#define INMOVE_RET   0xEA

#define HIGH         0x8B
#define HIGH_RET     0xEB

#define SMOTOR       0x8C
#define SMOTOR_RET   0xEC

#define SPEED        0x8D
#define SPEED_RET    0xED

#define INPUT        0x8E
#define INPUT_RET    0xEE

#define OUTPUT       0x8F
#define OUTPUT_RET   0xEF



#define XYSENSOR     0x70
#define XYSENSOR_RET 0xD0

#define PCSENSOR     0x71
#define PCSENSOR_RET 0xD1

#define CSENSOR      0x72
#define CSENSOR_RET  0xD2

#define ISENSOR      0x73
#define ISENSOR_RET  0xD3

#define MOTOCWW      0x74
#define MOTOCWW_RET  0xD4

#define STEPMOVE     0x75
#define STEPMOVE_RET 0xD5

#define MOTOSTA      0x76
#define MOTOSTA_RET  0xD6

#define COORCOM      0x77
#define COORCOM_RET  0xD7

#define COORADJUST   			0x78
#define COORADJUST_RET 			0xD8



#define CUR_COORDINATES 		0x79
#define CUR_COORDINATES_RET 	0xD9

#define PREDIT_SHIFT 			0x7A
#define PREDIT_SHIFT_RET 		0xDA

#define STACOOR   				0x7B
#define STACOOR_RET   			0xDB

#define MOTORMECHANICANGLECHECK 0x7C
#define MOTORMECHANICANGLECHECK_RET 0xDC

#define MOTORMECHANICANGLEENTER 0x7D
#define MOTORMECHANICANGLEENTER_RET 0xDD

#define CURRENTSTITCHQUERY     	0x7E
#define CURRENTSTITCHQUERY_RET  0xDE

#define SEWSPEEDADJUST			0x7F
#define SEWSPEEDADJUST_RET		0XDF

#define ELEMENTSHIFT			0x60
#define ELEMENTSHIFT_RET		0xB0

//#define EDITCURRENTCOOR			0x61
//#define EDITCURRENTCOOR_RET		0xB1

#define CANCELALLCOOR			0x62
#define CANCELALLCOOR_RET		0xB2

#define STITCHUP				0x63
#define STITCHUP_RET			0xB3

#define POINTSHIFT				0x64
#define POINTSHIFT_RET			0xB4

#define ENDSTYLECHOOSE			0x65
#define ENDSTYLECHOOSE_RET		0xB5

#define MOVETOCURRENTCOOR		0x66
#define MOVETOCURRENTCOOR_RET	0xB6

#define CURRENTPOINTSHIFT		0x67
#define CURRENTPOINTSHIFT_RET	0xB7

#define EDITELEMENTCOMMAND		0x68
#define EDITELEMENTCOMMAND_RET	0xB8

#define SEWTESTENDFLAGCHECK     0x69
#define SEWTESTENDFLAGCHECK_RET 0xB9

#define CANCELOVERTABPOINT     	0x6A
#define CANCELOVERTABPOINT_RET 	0xBA

#define PATTERNPOINTSHIFT		0x6B
#define PATTERNPOINTSHIFT_RET	0xBB

#define FOOTSTATEQUERY			0x6C
#define FOOTSTATEQUERY_RET		0xBC

#define INIFLAG					0x6D
#define INIFLAG_RET				0xBD

#define ORIGIN					0x6E
#define ORIGIN_RET				0xBE

#define STORE_OFFSET       		0x90
#define STORE_OFFSET_RET   		0xf0

#define DSP_DOWNLOAD_CODE      0x92
#define DSP_DOWNLOAD_CODE_RET  0xF2

#define DSP_DOWNLOAD_VERIFY      0x93
#define DSP_DOWNLOAD_VERIFY_RET  0xF3

#define SET_BASELINE_ALARM      0x94
#define SET_BASELINE_ALARM_RET  0xF4

#define PARA2                   0x95
#define PARA2_RET               0xF5

#define CHECKI11_TEST           0x96
#define CHECKI11_TEST_RET       0xF6

#define BOBBIN_CASE_INPUT       0x97
#define BOBBIN_CASE_INPUT_RET   0xF7

#define DISPLAY_SERVOPARA 		0x98    
#define DISPLAY_SERVOPARA_RET 	0xf8

#define SERVOPARA      			0x99
#define SERVOPARA_RET  			0xf9

#define SYS_PARAM_GROUP			0x9A
#define SYS_PARAM_GROUP_RET		0xFA

#define RESET_USERPARAM			0x9B
#define RESET_USERPARAM_RET		0xFB

#define QUERY_ID      				0x9c
#define QUERY_ID_RET  				0xFc

#define SET_MAIN_CONTROL_LOCK       0x9d
#define SET_MAIN_CONTROL_LOCK_RET   0xFd




 
//class ProtocolItem
//{
//public:
//	ProtocolItem(unsigned int BufLen);
//	~ProtocolItem(void);
//private:
//	//��Ա����
//	BYTE command_word;
//	BYTE command_word_return;
//
//
//
//
//public:
//
//};
#define MAX_PACKET_LENGTH 300

extern BYTE rcv_buf[];//���������ɵ�ͨ������

typedef enum	//ͨ�Ű�����״̬
{
    RCV_STATE_IDLE,			/*!< Receiver is in idle state. */
	
	RCV_STATE_HEAD,			/*!< Waiting for Packet Head. */
	RCV_STATE_LEN,			/*!< Waiting for length. */
	RCV_STATE_COMMAND,		/*!< Waiting for Command Word. */
	RCV_STATE_DATA,			/*!< Waiting for DATA. */
	RCV_STATE_BCC,			/*!< Waiting for BCC. */

	RCV_STATE_TAIL,			/*!< Waiting for TAIL. */

    RCV_STATE_CMPL,         /*!< Frame received completely. */
    CLOUD_STATE_RX_ERROR
}RcvState;


void analysis_rcv_FSM(BYTE dat);//����ͨ��֡

void analysis_protocol(BYTE* pcommdata,CString *pstrout);//������ȷ��ͨ��֡�������Ӧ�Ľ�����Ϣ



void analysis_init(CString *pstr);//��ʼ����ر�������Ҫ�ƶ�������������ַ���
unsigned int analysis_get_discard_number(void);//��ȡ�޷��������ַ���
void analysis_clear_discard_number(void);//�����޷��������ַ���


#endif
