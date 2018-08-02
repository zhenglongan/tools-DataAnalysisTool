#pragma once

class CircularBuf
{
public:
	CircularBuf(void);
	CircularBuf(unsigned int BufLen);
	~CircularBuf(void);
private:
	//��Ա����
	BYTE *pbufer;//������ָ��
	unsigned int buf_length_all;//�������ܳ���
	unsigned int buf_length_used;//���������ó���
	unsigned int read_index;//��ȡ����
	unsigned int write_index;//д������

	bool read_index_change(int num);//���ӻ��߼��ٶ�ȡ����
	bool write_index_change(int num);//���ӻ��߼���д������

public:
	//����
	bool write(BYTE *pbuf,unsigned int len);//д�뻺����
	unsigned int read(BYTE *pbuf,unsigned int len);//��ȡ������

	unsigned int get_write_index(void);//��ȡ��ȡ����
	unsigned int get_read_index(void);//��ȡд������
	unsigned int get_buf_length_all(void);//��ȡ�������ܳ���
	unsigned int get_buf_length_used(void);//��ȡ���������ó���
	unsigned int get_buf_length_unused(void);//��ȡ������δ�ó���
};
