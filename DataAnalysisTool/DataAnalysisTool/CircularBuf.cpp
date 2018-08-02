#include "StdAfx.h"
#include "CircularBuf.h"

CircularBuf::CircularBuf(void)
{
	buf_length_all=128*1024;
	pbufer=new BYTE[buf_length_all];//Ĭ��128KB
	buf_length_used=0;//��ʹ��0
	read_index=0;//��ȡ����Ϊ0
	write_index=0;//д������Ϊ0
}

CircularBuf::~CircularBuf(void)
{
	delete []pbufer;//�ͷ��ڴ�
}
CircularBuf::CircularBuf(unsigned int BufLen)
{
	buf_length_all=BufLen;
	pbufer=new BYTE[BufLen];//ʹ��ָ��ֵ
	buf_length_used=0;//��ʹ��0
	read_index=0;//��ȡ����Ϊ0
	write_index=0;//д������Ϊ0
}
//����
bool CircularBuf::write(BYTE *pbuf,unsigned int len)//д�뻺����
{
	unsigned int i;

	if(len>(buf_length_all-buf_length_used))//Ҫд��ĳ����Ѿ������˻�������ʣ�೤��
		return 1;

	buf_length_used+=len;//�������ó���
	//д�뻺����
	for(i=0;i<len;i++)
	{
		*(pbufer+write_index)=*(pbuf+i);
		write_index_change(1);//ָ����һ�����������ı����ó��ȵ�ֵ
	}
	return 0;
}
unsigned int CircularBuf::read(BYTE *pbuf,unsigned int len)//��ȡ������
{
	unsigned int i;
	unsigned int used=buf_length_used;

	if(len>used||used==0)//Ҫ��ȡ�ĳ����Ѿ������˻����������ó��ȣ����߻������ѿ�
		return 1;

	buf_length_used-=len;//�������ó���
	//д�뻺����
	for(i=0;i<len;i++)
	{
		*(pbuf+i)=*(pbufer+read_index);
		read_index_change(1);//ָ����һ�����������ı����ó��ȵ�ֵ
	}

	return used;
}

unsigned int CircularBuf::get_write_index(void)//��ȡ��ȡ����
{
	return write_index;
}
unsigned int CircularBuf::get_read_index(void)//��ȡд������
{
	return read_index;
}

bool CircularBuf::read_index_change(int num)//���ӻ��߼��ٶ�ȡ����
{
	unsigned int temp=0;
	if(num>0)//��ȡ����ֵ
	{
		temp=num;
	}
	else if(num<0)
	{
		temp=-num;
	}
	if(temp>buf_length_all)//������Χ���ش���
		return 1;

	//�ı��ȡ����������������Ҳ�����Ǽ���
	if(num<0)
	{
		temp=num+buf_length_all;
	}
	read_index=(read_index+temp)%buf_length_all;//����������϶�Ϊ1

	return 0;
}

bool CircularBuf::write_index_change(int num)//���ӻ��߼���д������
{
	unsigned int temp=0;
	if(num>0)//��ȡ����ֵ
	{
		temp=num;
	}
	else if(num<0)
	{
		temp=-num;
	}
	if(temp>buf_length_all)//������Χ���ش���
		return 1;

	//�ı��ȡ����������������Ҳ�����Ǽ���
	if(num<0)
		temp=num+buf_length_all;
	write_index=(write_index+temp)%buf_length_all;

	return 0;
}
//Ϊ�˱���ؼ���������д��������������ȡ�������ṩ������ĵ����ߵģ���Ա��������ʹ�����������������ǿ���ֱ�ӷ�����Ӧ����
unsigned int CircularBuf::get_buf_length_all(void)//��ȡ�������ܳ���
{
	return buf_length_all;
}

unsigned int CircularBuf::get_buf_length_used(void)//��ȡ���������ó���
{
	return buf_length_used;
}

unsigned int CircularBuf::get_buf_length_unused(void)//��ȡ������δ�ó���
{
	return (buf_length_all-buf_length_used);
}
