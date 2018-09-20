#include "stdafx.h"
#include "CommandMask.h"

//�����洢�����������Ϣ
CommandMaskList MaskLink;

//��ʼ������
void MaskLinkInit(void)
{
	MaskLink.Number = 0;
	MaskLink.pHeadItem = NULL;//��ͷָ���
}

//����һ���������������
INT8 MaskItemInset(UINT8 MasterCommand)
{
	CommandMaskListItem *pNewItem,*pItem;
	UINT8 flag = 0;

	if( MaskCheckItem(MasterCommand) == 0 )//����Ѿ�����
		return -1;//����ʧ�ܣ���Ϊ�Ѿ�������
	
	//�����ڣ���ô��Ҫ����
	pNewItem = new CommandMaskListItem;//Ϊ�µ����������ռ�

	if( MaskLink.Number == 0 )//֮ǰ����Ϊ��
	{
		MaskLink.Number = 1;
		MaskLink.pHeadItem = pNewItem;//��������ͷ
		pNewItem->pNext=NULL;//����ǰ��Ϊ����
		pNewItem->pPrev=NULL;
	}
	else
	{
		pItem=MaskLink.pHeadItem;//�ӱ�ͷ��ʼ
		while(pItem!=NULL)//���±���Ѱ�Һ��ʵĲ���ص�
		{
			//���Ҫ������������������С�ڵ�ǰ���ô���뵱ǰ��֮ǰ
			if(MasterCommand < pItem->MaskMasterCommand)
			{
				//�Ȳ�������
				pNewItem->pNext = pItem;
				pNewItem->pPrev = pItem->pPrev;
				//�����ǰ���Ǳ�ͷ
				if(MaskLink.pHeadItem==pItem)
				{
					MaskLink.pHeadItem = pNewItem;//���±�ͷ
				}
				else
				{
					//��һ���Next���Ϊ����
					pItem->pPrev->pNext = pNewItem;
				}
				//��ǰ���Previous���Ϊ����
				pItem->pPrev = pNewItem;
				MaskLink.Number++;

				flag =1;
				break;//����ѭ��
			}
			pItem=pItem->pNext;//ָ����һ��
		}
		if(flag == 0)//�����û���ҵ�����λ�ã���ô�����β
		{
			pItem = MaskLink.pHeadItem;//ָ���ͷ
			while(pItem->pNext!=NULL)//�ҵ���β
			{
				pItem = pItem->pNext;
					
			}

			//�����β
			pItem->pNext = pNewItem;
			pNewItem->pPrev = pItem;
			pNewItem->pNext = NULL;
			MaskLink.Number++;
		}
	}

	//�������ʾ�Ѿ���������ˣ������ݿ����������
	pNewItem->MaskMasterCommand = MasterCommand;
	pNewItem->MaskSlaveCommand = MasterCommand+0x60;
	pNewItem->MasterMaskedNumber = 0;
	pNewItem->SlaveMaskedNumber = 0;
	
	UINT32 i = 1;
	pItem=MaskLink.pHeadItem;//�ӱ�ͷ��ʼ,���±��
	while(pItem!=NULL)
	{
		pItem->Number = i;//12345...���±��
		i++;

		pItem=pItem->pNext;//ָ����һ��
	}
	return 0;
}
//ɾ��һ�ָ�������ּ���
//����0��ɾ���ɹ�������-1������Ϊ�գ�����-2��δ�ҵ�ָ����
INT8 MaskItemDelete(BYTE CommandWord)
{
	CommandMaskListItem *pItem;
	UINT8 flag = 0;

	if( MaskLink.Number == 0 )//����Ϊ��
		return -1;//�޷�ɾ��������Ϊ��

	pItem=MaskLink.pHeadItem;//�ӱ�ͷ��ʼ
	while(pItem!=NULL)//���±���Ѱ��
	{
		//�ҵ���
		if(pItem->MaskMasterCommand == CommandWord)
		{
			flag = 1;
			break;
		}

		pItem=pItem->pNext;//ָ����һ��
	}

	if(flag == 1)//�Ѿ��ҵ���ɾ��������
	{
		//��������ֻ�е�ǰ��һ��
		if(MaskLink.Number == 1)
		{
			MaskLink.pHeadItem = NULL;
		}
		else
		{
			if(MaskLink.pHeadItem == pItem)//����Ǳ�ͷ��ɾ��
			{
				MaskLink.pHeadItem = MaskLink.pHeadItem->pNext;//��һ���Ϊ�±�ͷ
				MaskLink.pHeadItem->pPrev = NULL;//�±�ͷ����һ��ָ���
			}
			else
			{
				//��ǰ�����һ�����һ��ָ��ǰ�����һ�����˵�ǰ����
				pItem->pPrev->pNext = pItem->pNext;
				//��ǰ�����һ�Ϊ��
				if(pItem->pNext!=NULL)
				{
					//��ǰ�����һ�����һ��ָ��ǰ�����һ��
					pItem->pNext->pPrev = pItem->pPrev;
				}
			}
		}

		delete pItem;//�ͷ��ڴ�
		MaskLink.Number--;

		UINT32 i = 1;
		pItem=MaskLink.pHeadItem;//�ӱ�ͷ��ʼ,���±��
		while(pItem!=NULL)
		{
			pItem->Number = i;//12345...���±��
			i++;

			pItem=pItem->pNext;//ָ����һ��
		}

		return 0;//�Ѿ��ҵ���ɾ��
	}
	else
		return -2;//δ�ҵ������Ŀ
}
//ɾ��һ�ָ���������е����
//����0��ɾ���ɹ�������-1���������Ų���ȷ���߲����ڣ�����-2��δ�ҵ�ָ����
INT8 MaskItemDelete1(UINT32 Number)
{
	CommandMaskListItem *pItem;
	UINT32 i;
	INT8 ret;

	if(Number<1)
		return -1;//�������Ų���ȷ

	if(Number>MaskLink.Number)
		return -1;//�������Ų�����
	
	pItem = MaskLink.pHeadItem;//���ҵ���ͷ
	for(i=0;i<Number-1;i++)//�ҵ�Ҫɾ������
	{
		pItem = pItem->pNext;
	}
	ret = MaskItemDelete(pItem->MaskMasterCommand);
	return ret;

}
//����0���ҵ�ָ�������-1������Ϊ�գ�����-2��δ�ҵ�ָ����
INT8 MaskCheckItem(BYTE CommandWord)//��ѯ�Ƿ����ָ�������ֵ�������
{
	CommandMaskListItem *pItem;
	UINT8 flag = 0;

	if( MaskLink.Number == 0 )//����Ϊ��
		return -1;//����Ϊ��

	pItem=MaskLink.pHeadItem;//�ӱ�ͷ��ʼ
	while(pItem!=NULL)//���±���Ѱ��
	{
		//�ҵ���
		if(pItem->MaskMasterCommand == CommandWord)
		{
			flag = 1;
			break;
		}

		pItem=pItem->pNext;//ָ����һ��
	}
	if(flag == 1)
		return 0;//�ҵ���
	else
		return -2;//δ�ҵ�
}
void MaskDeleteLink(void)//ɾ����������
{
	UINT32 i;
	CommandMaskListItem *pItem;

	i = MaskLink.Number;

	if(i==0)
		return;

	pItem = MaskLink.pHeadItem;

	while(1)//����ɾ��
	{
		//ɾ����ͷ
		//��ǰֻ��1����
		if(MaskLink.Number == 1)
		{
			MaskLink.pHeadItem = NULL;
			delete pItem;//�ͷ��ڴ�
			MaskLink.Number--;
			break;
		}
		else
		{
			MaskLink.pHeadItem = MaskLink.pHeadItem->pNext;//��һ���Ϊ�±�ͷ
			MaskLink.pHeadItem->pPrev = NULL;//�±�ͷ����һ��ָ���
			delete pItem;//�ͷ��ڴ�
			pItem = MaskLink.pHeadItem;
			MaskLink.Number--;
		}		
	}
}




