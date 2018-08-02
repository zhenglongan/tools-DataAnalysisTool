#pragma once

class CircularBuf
{
public:
	CircularBuf(void);
	CircularBuf(unsigned int BufLen);
	~CircularBuf(void);
private:
	//成员变量
	BYTE *pbufer;//缓存区指针
	unsigned int buf_length_all;//缓存区总长度
	unsigned int buf_length_used;//缓存区已用长度
	unsigned int read_index;//读取索引
	unsigned int write_index;//写入索引

	bool read_index_change(int num);//增加或者减少读取索引
	bool write_index_change(int num);//增加或者减少写入索引

public:
	//函数
	bool write(BYTE *pbuf,unsigned int len);//写入缓存区
	unsigned int read(BYTE *pbuf,unsigned int len);//读取缓存区

	unsigned int get_write_index(void);//获取读取索引
	unsigned int get_read_index(void);//获取写入索引
	unsigned int get_buf_length_all(void);//获取缓冲区总长度
	unsigned int get_buf_length_used(void);//获取缓冲区已用长度
	unsigned int get_buf_length_unused(void);//获取缓冲区未用长度
};
