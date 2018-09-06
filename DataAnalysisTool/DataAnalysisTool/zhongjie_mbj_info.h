#pragma once

#define PLATFORM_SC      1 
#define PLATFORM_ASC     2
#define PLATFORM_ESC     3
#define PLATFORM_HSC     4
#define PLATFORM_MSC     5
#define PLATFORM_MASC    6
#define PLATFORM_TASC    7

#define PATTERN_MACHINE  '4'

#define JUKI_STYLE        '0'
#define BROTHER_STYLE     '1'
#define DUERKOPP_STYLE    '2'
#define MITSUBISHI_STYLE  '4'

#define ENGINE_TYPE0       0
#define ENGINE_TYPE1       1
#define ENGINE_TYPE2       2
#define ENGINE_TYPE3       3
#define ENGINE_TYPE4       4
#define ENGINE_TYPE5       5
#define ENGINE_DEFAULT     '0'

#define MAIN_MOTOR_DEFINE  '0'
#define MAIN_MOTOR_0830    '1'
#define MAIN_MOTOR_0850    '2'
#define MAIN_MOTOR_1635    '3'
#define MAIN_MOTOR_1730    '4'
#define MAIN_MOTOR_2530    '5'

#define XY_FRAMES_NONE          '1'
#define XY_FRAMES_OPEN          '2'
#define XY_FRAMES_CLOSE_400     '3'
#define XY_FRAMES_CLOSE_1000    '4'
#define XY_FRAMES_SERVO         '5'

#define EXPAND_RESERVE          '0'

#define SOFTWARE_PLATFORM_SC    1
#define SOFTWARE_PLATFORM_AC    2


//--------------------------------------------------------------------------------------
// system status definition
//--------------------------------------------------------------------------------------
typedef enum	//主控系统状态
{
    STATE_RESERVE				=(0),		/*!< reseved state. */
	STATE_FREE					=(1),			
	STATE_READY					=(2),			
	STATE_RUN					=(3),		
	STATE_ERROR					=(4),			
	STATE_PREWIND				=(5),			
	STATE_WIND					=(6),
	STATE_INPRESS				=(7),			
	STATE_POWEROFF				=(8),
	STATE_SINGLE				=(9),
	STATE_MANUAL				=(10),
	STATE_SETOUT				=(11),
	STATE_EMERSTOP				=(12),
	STATE_PREEDIT				=(13),
	STATE_EDIT					=(14),
	STATE_NOEDIT				=(15),
	STATE_FINISH				=(16),
	STATE_NEEDLE				=(17),
	STATE_WAITOFF				=(18),
	STATE_TRIM					=(19),
	STATE_SLACK					=(20),
	STATE_CHECKI03				=(21),
	STATE_CHECKI04				=(22),
	STATE_CHECKI05				=(23),
	STATE_CHECKI06				=(24),
	STATE_CHECKI07				=(25),
	STATE_CHECKI08				=(26),
	STATE_CHECKI10				=(27),
	STATE_EMERMOVE				=(28),
	STATE_BOARDTEST				=(29),
	STATE_RFID_WR				=(30),
	STATE_CALSEW				=(31),
	STATE_CHECKI11				=(32),

	STATE_DOWNLOAD_DSP1			=(35),
	STATE_DOWNLOAD_DSP2			=(36),
	STATE_DOWNLOAD_DSP_CURVE	=(37),
	STATE_MULTIPULE_IO			=(38),
	STATE_DOWNLOAD_DSP3			=(39),
	STATE_DOWNLOAD_DSP4			=(40),
	STATE_DOWNLOAD_SPFL			=(41),

	STATE_DOWNLOAD				=(72),
}SystemState;





typedef struct //步进版本结构体
{
	UINT8 MachineType;
	UINT8 FatherVersion;
	UINT8 ChildVersion;
	UINT8 SVNVersion;
	UINT8 DSPNumber;
}StepperVersion;
typedef struct //主控版本结构体
{
	UINT8 MachineType;
	UINT8 FatherVersion;
	UINT8 ChildVersion;
	UINT8 SVNVersion;
}MainVERSION;

typedef struct //主控版本结构体
{
	UINT8 status;
	UINT8 FatherVersion;
	UINT8 ChildVersion;
	UINT8 SVNVersion;
}SystemCurrentInfo;


typedef struct //花样数据结构体
{
	UINT8 func;        // fuction code
	INT8 xstep;        // X 
	INT8 ystep;        // Y
} PATTERN_DATA;

typedef struct	//系统第1组参数结构体,总长为205B
{
	UINT16 DSP1_para_1F;		//1,2
	UINT16 DSP1_para_20;		//3,4
	UINT16 DSP1_para_21;		//5,6
	UINT16 DSP1_para_22;		//7,8
	UINT16 DSP1_para_23;		//9,10
	UINT16 DSP1_para_27;		//11,12
	UINT16 DSP1_para_28H;		//13,14
	UINT16 DSP1_para_28M1;		//15,16
	UINT16 DSP1_para_28M2;		//17,18
	UINT16 DSP1_para_28L;		//19,20
	
	UINT16 DSP2_para_1F;		//21,22
	UINT16 DSP2_para_20;		//23,24
	UINT16 DSP2_para_21;		//25,26
	UINT16 DSP2_para_22;		//27,28
	UINT16 DSP2_para_23;		//29,30

	UINT16 DSP2_para_27;		//31,32
	UINT16 DSP2_para_28H;		//33,34
	UINT16 DSP2_para_28M1;		//35,36
	UINT16 DSP2_para_28M2;		//37,38
	UINT16 DSP2_para_28L;		//39,40
	
	UINT8  dsp1A_half_current;	//41
	UINT8  dsp1B_half_current;	//42
	UINT8  dsp2A_half_current;	//43
	UINT8  dsp2B_half_current;	//44
		
	UINT8  platform_type;		//45	
	UINT8  mainmotor_type;      //46
	UINT8  x_origin_mode;		//47
	UINT8  yj_org_direction;    //48
	UINT8  Corner_deceleration_speed;//49
    UINT8  wipper_type;			//50
	UINT8  x_sensor_open_level; //51
	UINT8  y_sensor_open_level;	//52
	UINT8  laser_function_enable;//53
	UINT8  dvb_open_level;		//54
	UINT8  last_8_speed;		//55
	UINT8  last_7_speed;		//56
	UINT8  last_6_speed;		//57
	UINT8  last_5_speed;		//58
	UINT8  last_4_speed;		//59
	UINT8  last_3_speed;		//60
	UINT8  last_2_speed;		//61
	UINT8  last_1_speed;		//62
	UINT8  dva_open_level;		//63
	
	UINT16  dsp1_step_crc;		//64,65
	UINT16  dsp2_step_crc;		//66,67
	
	UINT16 y_backward_dis;		//68,69
    UINT16 x_take_offset;		//70,71
	UINT16 x_take_offset2;		//72,73
	UINT16 left_barcode_position;	//74,75
	UINT16 right_barcode_position;	//76,77
	UINT16 catch_delay_time;	//78,79
	UINT16 y_barcode_position;	//80,81
	UINT16 blow_air_counter;	//82,83
	UINT16 cut_air_counter;		//84,85
	UINT8  yj_go_origin_enable;			//86
	UINT8  second_origin_footer_status; //87
	UINT8  go_special_position_mode;	//88
	
	UINT16  dsp3_step_crc;		//89,90
	UINT16  dsp4_step_crc;		//91,92
	UINT8   zx_org_direction;	//93
	
	UINT16 DSP3_para_1F;		//94,95
	UINT16 DSP3_para_20;		//96,97
	UINT16 DSP3_para_21;		//98,99
	UINT16 DSP3_para_22;		//100,101
	UINT16 DSP3_para_23;		//102,103
	UINT16 DSP3_para_27;		//104,105
	UINT16 DSP3_para_28H;		//106,107
	UINT16 DSP3_para_28M1;		//108,109
	UINT16 DSP3_para_28M2;		//110,111
	UINT16 DSP3_para_28L;		//112,113
	
	UINT8  dsp3A_half_current;	//114
	UINT8  dsp3B_half_current;	//115
	UINT8  dsp3A_current;		//116
	UINT8  dsp3B_current;		//117
	UINT8  dsp3_enable;			//118
	
	UINT8  dsp3a_motor_dir;		//119
	UINT8  dsp3b_motor_dir;		//120
	UINT8  dsp3a_sensor_dir;	//121	
	UINT8  dsp3b_sensor_dir;	//122
	
	UINT8  bobbin_platform_speed;//123
	 INT8  bobbin_shake_distance;//124
	UINT8  bobbin_shake_time;	 //125
	
	UINT8  rotate_cutter_delaytime1;//126
	UINT8  rotate_cutter_delaytime2;//127	
	UINT8  rotate_cutter_movetime;//128
	
	UINT8  rotate_cutter_working_mode;//129
	UINT8  rotate_cutter_detect_valve_down;//130 
	UINT8  x_motor_dir;			//131
	UINT8  y_motor_dir;			//132
	UINT8  zx_motor_dir;		//133
	
	UINT16 cutter_speed;		//134,135
	UINT8  speed_limit_switch;  //136
	UINT8  speed_percent;		//137
	UINT8  one_key_run_function;//138
	UINT8  thread_break_backward_switch;//139
	UINT8  thread_break_backward_stitchs;//140
	UINT8  slow_start_mode;		//141
	UINT8  stitch1_speed;		//142
	UINT8  stitch2_speed;		//143
	UINT8  stitch3_speed;		//144
	UINT8  stitch4_speed;		//145
	UINT8  stitch5_speed;		//146
	UINT8  first_5_adjust;		//147
	
	UINT8  Corner_deceleration_speed1; //148 拐点后第四针
	UINT8  Corner_deceleration_speed2; //149 拐点后第三针
	UINT8  Corner_deceleration_speed3; //150 拐点后第二针
	UINT8  Corner_deceleration_speed4; //151 拐点后第一针
	
	UINT8  slow_start_stitchs;	//152 慢速启动针数
	UINT8  slow_start_speed[15];//153-167 慢速启动速度，速度由低到高
	UINT8  adjust_mode_enable;  //168 是否使用XY动框角度和时间在不同速度时单独可调

	UINT8 reserved[205-168];//保留，暂未使用
}SYSTEM_PARA;

typedef struct	//
{
	UINT8 x_time_adjust[40];	//1-40 X动框时间微调，128以上表示负数，第1个表示转速100，第2个表示200，以此类推
	UINT8 x_angle_adjust[40];	//41-80 X动框角度微调，128以上表示负数
	UINT8 y_time_adjust[40];	//81-120 Y动框时间微调，128以上表示负数
	UINT8 y_angle_adjust[40];	//121-160 Y动框角度微调，128以上表示负数

	UINT8 reserved[205-160];//保留，暂未使用
}SYSTEM_PARA5;



class zhongjie_mbj_info
{
public:
	zhongjie_mbj_info(void);
	~zhongjie_mbj_info(void);


public:
	SYSTEM_PARA sys_para1;//系统参数第1组
	//第2-4组暂时空置

	SYSTEM_PARA5 sys_para5;//系统参数第5组,
	StepperVersion stepper_version1;
	StepperVersion stepper_version2;
	StepperVersion stepper_version3;
	StepperVersion stepper_version4;
	MainVERSION main_version;//主控版本
};
