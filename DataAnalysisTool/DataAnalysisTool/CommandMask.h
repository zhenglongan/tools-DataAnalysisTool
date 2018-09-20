#ifndef COMMANDMASK_H
#define COMMANDMASK_H



typedef struct  LinkItem
{
	struct LinkItem* pNext;//��һ��
	struct LinkItem* pPrev;//��һ��

	//ȡֵ0��ʾû�����ε���Ŀ
	UINT8 MaskMasterCommand;//���εĲ���ͷ�������֣�ע�⣬���ͽ��ն����Σ�
	UINT8 MaskSlaveCommand;//���ε����ض������֣�ע�⣬���ͽ��ն����Σ�
	UINT32 MasterMaskedNumber;//�������͵ı����εİ���
	UINT32 SlaveMaskedNumber;//���ط��͵ı����εİ���

	UINT32 Number;//��ţ�����ˢ�����б�ؼ��е���ʾ����ʾ���б�ؼ��е�����

} CommandMaskListItem;

typedef struct  
{
	//�����ڵ�������ָ����
	UINT32 Number;
	CommandMaskListItem* pHeadItem;//ָ���ͷ
} CommandMaskList;

//�����洢�����������Ϣ
extern CommandMaskList MaskLink;

//��ʼ������
void MaskLinkInit(void);
//����һ���������������
INT8 MaskItemInset(UINT8 MasterCommand);
//ɾ��һ�ָ�������ּ���
INT8 MaskItemDelete(BYTE CommandWord);
//ɾ��һ�ָ���������е����
INT8 MaskItemDelete1(UINT32 Number);
INT8 MaskCheckItem(BYTE CommandWord);//��ѯ�Ƿ����ָ�������ֵ�������
void MaskDeleteLink(void);//ɾ����������



#endif
