#include "StdAfx.h"
#include "CircularBuf.h"

CircularBuf::CircularBuf(void)
{
	buf_length_all=128*1024;
	pbufer=new BYTE[buf_length_all];//默认128KB
	buf_length_used=0;//已使用0
	read_index=0;//读取索引为0
	write_index=0;//写入索引为0
}

CircularBuf::~CircularBuf(void)
{
	delete []pbufer;//释放内存
}
CircularBuf::CircularBuf(unsigned int BufLen)
{
	buf_length_all=BufLen;
	pbufer=new BYTE[BufLen];//使用指定值
	buf_length_used=0;//已使用0
	read_index=0;//读取索引为0
	write_index=0;//写入索引为0
}
//函数
bool CircularBuf::write(BYTE *pbuf,unsigned int len)//写入缓存区
{
	unsigned int i;

	if(len>(buf_length_all-buf_length_used))//要写入的长度已经超过了缓冲区的剩余长度
		return 1;

	buf_length_used+=len;//更新已用长度
	//写入缓存区
	for(i=0;i<len;i++)
	{
		*(pbufer+write_index)=*(pbuf+i);
		write_index_change(1);//指向下一个索引，不改变已用长度的值
	}
	return 0;
}
unsigned int CircularBuf::read(BYTE *pbuf,unsigned int len)//读取缓存区
{
	unsigned int i;
	unsigned int used=buf_length_used;

	if(len>used||used==0)//要读取的长度已经超过了缓冲区的已用长度，或者缓冲区已空
		return 1;

	buf_length_used-=len;//更新已用长度
	//写入缓存区
	for(i=0;i<len;i++)
	{
		*(pbuf+i)=*(pbufer+read_index);
		read_index_change(1);//指向下一个索引，不改变已用长度的值
	}

	return used;
}

unsigned int CircularBuf::get_write_index(void)//获取读取索引
{
	return write_index;
}
unsigned int CircularBuf::get_read_index(void)//获取写入索引
{
	return read_index;
}

bool CircularBuf::read_index_change(int num)//增加或者减少读取索引
{
	unsigned int temp=0;
	if(num>0)//获取绝对值
	{
		temp=num;
	}
	else if(num<0)
	{
		temp=-num;
	}
	if(temp>buf_length_all)//超过范围返回错误
		return 1;

	//改变读取索引，可以是增加也可以是减少
	if(num<0)
	{
		temp=num+buf_length_all;
	}
	read_index=(read_index+temp)%buf_length_all;//将两种情况合二为1

	return 0;
}

bool CircularBuf::write_index_change(int num)//增加或者减少写入索引
{
	unsigned int temp=0;
	if(num>0)//获取绝对值
	{
		temp=num;
	}
	else if(num<0)
	{
		temp=-num;
	}
	if(temp>buf_length_all)//超过范围返回错误
		return 1;

	//改变读取索引，可以是增加也可以是减少
	if(num<0)
		temp=num+buf_length_all;
	write_index=(write_index+temp)%buf_length_all;

	return 0;
}
//为了避免关键变量被改写，下面这三个获取函数是提供给对象的调用者的，成员函数不会使用这三个函数，它们可以直接访问相应变量
unsigned int CircularBuf::get_buf_length_all(void)//获取缓冲区总长度
{
	return buf_length_all;
}

unsigned int CircularBuf::get_buf_length_used(void)//获取缓冲区已用长度
{
	return buf_length_used;
}

unsigned int CircularBuf::get_buf_length_unused(void)//获取缓冲区未用长度
{
	return (buf_length_all-buf_length_used);
}
