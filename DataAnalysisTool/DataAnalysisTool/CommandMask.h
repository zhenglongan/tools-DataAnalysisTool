#ifndef COMMANDMASK_H
#define COMMANDMASK_H



typedef struct  LinkItem
{
	struct LinkItem* pNext;//下一项
	struct LinkItem* pPrev;//上一项

	//取值0表示没有屏蔽的项目
	UINT8 MaskMasterCommand;//屏蔽的操作头端命令字（注意，发送接收都屏蔽）
	UINT8 MaskSlaveCommand;//屏蔽的主控端命令字（注意，发送接收都屏蔽）
	UINT32 MasterMaskedNumber;//主机发送的被屏蔽的包数
	UINT32 SlaveMaskedNumber;//主控发送的被屏蔽的包数

	UINT32 Number;//编号，用于刷新在列表控件中的显示，表示在列表控件中的行数

} CommandMaskListItem;

typedef struct  
{
	//链表内的总屏蔽指令数
	UINT32 Number;
	CommandMaskListItem* pHeadItem;//指向表头
} CommandMaskList;

//链表，存储着屏蔽相关信息
extern CommandMaskList MaskLink;

//初始化链表
void MaskLinkInit(void);
//插入一项（根据命令字排序）
INT8 MaskItemInset(UINT8 MasterCommand);
//删除一项，指定命令字即可
INT8 MaskItemDelete(BYTE CommandWord);
//删除一项，指定在链表中的序号
INT8 MaskItemDelete1(UINT32 Number);
INT8 MaskCheckItem(BYTE CommandWord);//查询是否存在指定命令字的链表项
void MaskDeleteLink(void);//删除所有内容



#endif
