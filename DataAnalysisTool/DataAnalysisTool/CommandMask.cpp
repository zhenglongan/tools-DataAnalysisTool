#include "stdafx.h"
#include "CommandMask.h"

//链表，存储着屏蔽相关信息
CommandMaskList MaskLink;

//初始化链表
void MaskLinkInit(void)
{
	MaskLink.Number = 0;
	MaskLink.pHeadItem = NULL;//表头指向空
}

//插入一项（根据命令字排序）
INT8 MaskItemInset(UINT8 MasterCommand)
{
	CommandMaskListItem *pNewItem,*pItem;
	UINT8 flag = 0;

	if( MaskCheckItem(MasterCommand) == 0 )//如果已经存在
		return -1;//插入失败，因为已经存在了
	
	//不存在，那么需要插入
	pNewItem = new CommandMaskListItem;//为新的链表项分配空间

	if( MaskLink.Number == 0 )//之前链表为空
	{
		MaskLink.Number = 1;
		MaskLink.pHeadItem = pNewItem;//新项插入表头
		pNewItem->pNext=NULL;//新项前后都为空项
		pNewItem->pPrev=NULL;
	}
	else
	{
		pItem=MaskLink.pHeadItem;//从表头开始
		while(pItem!=NULL)//向下遍历寻找合适的插入地点
		{
			//如果要插入的项的主机命令字小于当前项，那么插入当前项之前
			if(MasterCommand < pItem->MaskMasterCommand)
			{
				//先插入新项
				pNewItem->pNext = pItem;
				pNewItem->pPrev = pItem->pPrev;
				//如果当前项是表头
				if(MaskLink.pHeadItem==pItem)
				{
					MaskLink.pHeadItem = pNewItem;//更新表头
				}
				else
				{
					//上一项的Next项改为新项
					pItem->pPrev->pNext = pNewItem;
				}
				//当前项的Previous项改为新项
				pItem->pPrev = pNewItem;
				MaskLink.Number++;

				flag =1;
				break;//跳出循环
			}
			pItem=pItem->pNext;//指向下一个
		}
		if(flag == 0)//如果还没有找到插入位置，那么插入表尾
		{
			pItem = MaskLink.pHeadItem;//指向表头
			while(pItem->pNext!=NULL)//找到表尾
			{
				pItem = pItem->pNext;
					
			}

			//插入表尾
			pItem->pNext = pNewItem;
			pNewItem->pPrev = pItem;
			pNewItem->pNext = NULL;
			MaskLink.Number++;
		}
	}

	//到这里表示已经插入完成了，把数据拷贝进新项吧
	pNewItem->MaskMasterCommand = MasterCommand;
	pNewItem->MaskSlaveCommand = MasterCommand+0x60;
	pNewItem->MasterMaskedNumber = 0;
	pNewItem->SlaveMaskedNumber = 0;
	
	UINT32 i = 1;
	pItem=MaskLink.pHeadItem;//从表头开始,重新编号
	while(pItem!=NULL)
	{
		pItem->Number = i;//12345...重新编号
		i++;

		pItem=pItem->pNext;//指向下一个
	}
	return 0;
}
//删除一项，指定命令字即可
//返回0，删除成功，返回-1，链表为空，返回-2，未找到指定项
INT8 MaskItemDelete(BYTE CommandWord)
{
	CommandMaskListItem *pItem;
	UINT8 flag = 0;

	if( MaskLink.Number == 0 )//链表为空
		return -1;//无法删除，链表为空

	pItem=MaskLink.pHeadItem;//从表头开始
	while(pItem!=NULL)//向下遍历寻找
	{
		//找到了
		if(pItem->MaskMasterCommand == CommandWord)
		{
			flag = 1;
			break;
		}

		pItem=pItem->pNext;//指向下一个
	}

	if(flag == 1)//已经找到，删除并返回
	{
		//整个链表都只有当前这一项
		if(MaskLink.Number == 1)
		{
			MaskLink.pHeadItem = NULL;
		}
		else
		{
			if(MaskLink.pHeadItem == pItem)//如果是表头被删除
			{
				MaskLink.pHeadItem = MaskLink.pHeadItem->pNext;//下一项成为新表头
				MaskLink.pHeadItem->pPrev = NULL;//新表头的上一项指向空
			}
			else
			{
				//当前项的上一项的下一项指向当前项的下一项，跨过了当前项了
				pItem->pPrev->pNext = pItem->pNext;
				//当前项的下一项不为空
				if(pItem->pNext!=NULL)
				{
					//当前项的下一项的上一项指向当前项的上一项
					pItem->pNext->pPrev = pItem->pPrev;
				}
			}
		}

		delete pItem;//释放内存
		MaskLink.Number--;

		UINT32 i = 1;
		pItem=MaskLink.pHeadItem;//从表头开始,重新编号
		while(pItem!=NULL)
		{
			pItem->Number = i;//12345...重新编号
			i++;

			pItem=pItem->pNext;//指向下一个
		}

		return 0;//已经找到并删除
	}
	else
		return -2;//未找到相关项目
}
//删除一项，指定在链表中的序号
//返回0，删除成功，返回-1，输入的序号不正确或者不存在，返回-2，未找到指定项
INT8 MaskItemDelete1(UINT32 Number)
{
	CommandMaskListItem *pItem;
	UINT32 i;
	INT8 ret;

	if(Number<1)
		return -1;//输入的序号不正确

	if(Number>MaskLink.Number)
		return -1;//输入的序号不存在
	
	pItem = MaskLink.pHeadItem;//先找到表头
	for(i=0;i<Number-1;i++)//找到要删除的项
	{
		pItem = pItem->pNext;
	}
	ret = MaskItemDelete(pItem->MaskMasterCommand);
	return ret;

}
//返回0，找到指定项，返回-1，链表为空，返回-2，未找到指定项
INT8 MaskCheckItem(BYTE CommandWord)//查询是否存在指定命令字的链表项
{
	CommandMaskListItem *pItem;
	UINT8 flag = 0;

	if( MaskLink.Number == 0 )//链表为空
		return -1;//链表为空

	pItem=MaskLink.pHeadItem;//从表头开始
	while(pItem!=NULL)//向下遍历寻找
	{
		//找到了
		if(pItem->MaskMasterCommand == CommandWord)
		{
			flag = 1;
			break;
		}

		pItem=pItem->pNext;//指向下一个
	}
	if(flag == 1)
		return 0;//找到了
	else
		return -2;//未找到
}
void MaskDeleteLink(void)//删除所有内容
{
	UINT32 i;
	CommandMaskListItem *pItem;

	i = MaskLink.Number;

	if(i==0)
		return;

	pItem = MaskLink.pHeadItem;

	while(1)//挨个删除
	{
		//删除表头
		//当前只有1项了
		if(MaskLink.Number == 1)
		{
			MaskLink.pHeadItem = NULL;
			delete pItem;//释放内存
			MaskLink.Number--;
			break;
		}
		else
		{
			MaskLink.pHeadItem = MaskLink.pHeadItem->pNext;//下一项成为新表头
			MaskLink.pHeadItem->pPrev = NULL;//新表头的上一项指向空
			delete pItem;//释放内存
			pItem = MaskLink.pHeadItem;
			MaskLink.Number--;
		}		
	}
}




