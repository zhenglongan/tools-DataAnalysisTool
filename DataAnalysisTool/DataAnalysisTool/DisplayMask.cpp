// DisplayMask.cpp : implementation file
//

#include "stdafx.h"
#include "DataAnalysisTool.h"
#include "DisplayMask.h"
#include "common.h"
#include "CommandMask.h"


// CDisplayMask dialog

IMPLEMENT_DYNAMIC(CDisplayMask, CDialog)

CDisplayMask::CDisplayMask(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayMask::IDD, pParent)
	, m_strMaskCommand(_T(""))
	, m_AddNewItemFlag(0)
	, m_AddNewCommandWord(0)
{

}

CDisplayMask::~CDisplayMask()
{
}

void CDisplayMask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MASK_TABLE, m_ctrlListMaskTable);
	DDX_Text(pDX, IDC_EDIT_ADD_MASK, m_strMaskCommand);
}


BEGIN_MESSAGE_MAP(CDisplayMask, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MASK, &CDisplayMask::OnBnClickedButtonAddMask)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_MASK, &CDisplayMask::OnBnClickedButtonRemoveMask)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_ALL, &CDisplayMask::OnBnClickedButtonRemoveAll)
END_MESSAGE_MAP()


// CDisplayMask message handlers

void CDisplayMask::OnBnClickedButtonAddMask()
{
	int len;
	UINT8 value;
	INT8 ret;

	// TODO: Add your control notification handler code here
	//int nEdit = 0;
	//nEdit = GetDlgItemInt(IDC_EDIT_ADD_MASK);
	UpdateData(TRUE);
	len = m_strMaskCommand.GetLength();
	if(len<=0)
	{
		MessageBox(_T("��������Ҫ���ε�ָ���ٵ���˰�ť"),_T("��ʾ"),MB_ICONINFORMATION);
		return;
	}
	else if(len==1)
	{
		if(m_strMaskCommand[0]>='0' && m_strMaskCommand[0]<='9')
		{
			value=m_strMaskCommand[0]-'0';
		}
		else if(m_strMaskCommand[0]>='a' && m_strMaskCommand[0]<='f')
		{
			value=m_strMaskCommand[0]-'a'+0x0A;
		}
		else if(m_strMaskCommand[0]>='A' && m_strMaskCommand[0]<='F')
		{
			value=m_strMaskCommand[0]-'A'+0x0A;
		}
		else
		{
			MessageBox(_T("������16����������Ҫ���������ַ�"),_T("��ʾ"),MB_ICONINFORMATION);
			return;
		}
	}
	else
	{
		//�ȶ���λ
		if(m_strMaskCommand[0]>='0' && m_strMaskCommand[0]<='9')
		{
			value=m_strMaskCommand[0]-'0';
		}
		else if(m_strMaskCommand[0]>='a' && m_strMaskCommand[0]<='f')
		{
			value=m_strMaskCommand[0]-'a'+0x0A;
		}
		else if(m_strMaskCommand[0]>='A' && m_strMaskCommand[0]<='F')
		{
			value=m_strMaskCommand[0]-'A'+0x0A;
		}
		else
		{
			MessageBox(_T("������16����������Ҫ���������ַ�"),_T("��ʾ"),MB_ICONINFORMATION);
			return;
		}

		//�ٶ���λ
		if(m_strMaskCommand[1]>='0' && m_strMaskCommand[1]<='9')
		{
			value = value*16+m_strMaskCommand[1]-'0';
		}
		else if(m_strMaskCommand[1]>='a' && m_strMaskCommand[1]<='f')
		{
			value = value*16+m_strMaskCommand[1]-'a'+0x0A;
		}
		else if(m_strMaskCommand[1]>='A' && m_strMaskCommand[1]<='F')
		{
			value = value*16+m_strMaskCommand[1]-'A'+0x0A;
		}
		else
		{
			MessageBox(_T("������16����������Ҫ���������ַ�"),_T("��ʾ"),MB_ICONINFORMATION);
			return;
		}

	}
	if(value<0xA0)
	{
		//�������е������ʾ�Ѿ���ȡ���������ֵ
		ret = MaskItemInset(value);//���뵽������
		//����ɹ����������
		if(ret==0)
		{
			//��֪��ʾ��ʱ����������Ҫ��ʾΪ��ɫ
			m_AddNewItemFlag=1;
			m_AddNewCommandWord=value;

			//����༭���е�����
			m_strMaskCommand.Empty();
			UpdateData(FALSE);
		}
		else if(ret==-1)
		{
			MessageBox(_T("����ʧ�ܣ���Ϊ�Ѿ������˴�ָ��"),_T("��ʾ"),MB_ICONINFORMATION);
		}
		//���б�����ʾ
		DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);
	}
	else
	{
		MessageBox(_T("�����ֲ��ܴ���0xA0"),_T("��ʾ"),MB_ICONINFORMATION);
	}

	//�������۽��ڱ༭��
	CEdit *pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_ADD_MASK);
	pEdit->SetFocus();

}

void CDisplayMask::OnBnClickedButtonRemoveMask()
{
	// TODO: Add your control notification handler code here
	int len;
	UINT8 value;
	INT8 ret;
	UINT8 flag=0;
	UINT32 i;

	//if(m_ctrlListMaskTable.GetCheck(0)==1)
	//{
	//	MessageBox(_T("��1�б�ѡ��"),_T("��ʾ"),MB_ICONINFORMATION);
	//}
	//if(m_ctrlListMaskTable.GetCheck(1)==1)
	//{
	//	MessageBox(_T("��2�б�ѡ��"),_T("��ʾ"),MB_ICONINFORMATION);
	//}

	//����ɨ���Ƿ񱻹��У����е���Ŀ����ɾ��,�����������ɨ�裬������ɾ����������Ժ�
	//�������кžʹ�����
	for(i=MaskLink.Number; i>0; i--)
	{
		if(m_ctrlListMaskTable.GetCheck(i-1)==1)
		{
			flag=1;
			MaskItemDelete1(i);//ɾ������ѡ����Ŀ
		}
	}
	//�б�ɾ������Ŀ����ôˢ����ʾ
	if(flag == 1)
	{
		//���б���ˢ����ʾ
		DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);
	}
	//û�б�ѡ����Ŀ��ʱ���ٿ���ȥɾ���༭�������ֵ
	if(flag==0)
	{
		UpdateData(TRUE);
		len = m_strMaskCommand.GetLength();
		if(len<=0)
		{
			MessageBox(_T("����ѡ��Ҫɾ�����л�������Ҫȡ�����ε�ָ���ٵ���˰�ť"),_T("��ʾ"),MB_ICONINFORMATION);
			return;
		}
		else if(len==1)
		{
			if(m_strMaskCommand[0]>='0' && m_strMaskCommand[0]<='9')
			{
				value=m_strMaskCommand[0]-'0';
			}
			else if(m_strMaskCommand[0]>='a' && m_strMaskCommand[0]<='f')
			{
				value=m_strMaskCommand[0]-'a'+0x0A;
			}
			else if(m_strMaskCommand[0]>='A' && m_strMaskCommand[0]<='F')
			{
				value=m_strMaskCommand[0]-'A'+0x0A;
			}
			else
			{
				MessageBox(_T("������16����������Ҫ���������ַ�"),_T("��ʾ"),MB_ICONINFORMATION);
				return;
			}
		}
		else
		{
			//�ȶ���λ
			if(m_strMaskCommand[0]>='0' && m_strMaskCommand[0]<='9')
			{
				value=m_strMaskCommand[0]-'0';
			}
			else if(m_strMaskCommand[0]>='a' && m_strMaskCommand[0]<='f')
			{
				value=m_strMaskCommand[0]-'a'+0x0A;
			}
			else if(m_strMaskCommand[0]>='A' && m_strMaskCommand[0]<='F')
			{
				value=m_strMaskCommand[0]-'A'+0x0A;
			}
			else
			{
				MessageBox(_T("������16����������Ҫ���������ַ�"),_T("��ʾ"),MB_ICONINFORMATION);
				return;
			}

			//�ٶ���λ
			if(m_strMaskCommand[1]>='0' && m_strMaskCommand[1]<='9')
			{
				value = value*16+m_strMaskCommand[1]-'0';
			}
			else if(m_strMaskCommand[1]>='a' && m_strMaskCommand[1]<='f')
			{
				value = value*16+m_strMaskCommand[1]-'a'+0x0A;
			}
			else if(m_strMaskCommand[1]>='A' && m_strMaskCommand[1]<='F')
			{
				value = value*16+m_strMaskCommand[1]-'A'+0x0A;
			}
			else
			{
				MessageBox(_T("������16����������Ҫ���������ַ�"),_T("��ʾ"),MB_ICONINFORMATION);
				return;
			}

		}

		if(value<0xA0)
		{
			//�������е������ʾ�Ѿ���ȡ���������ֵ
			ret = MaskItemDelete(value);//��������ɾ��
			if(ret==-1)
			{
				MessageBox(_T("�����б��ѿգ�����Ҫ��ȡ������"),_T("��ʾ"),MB_ICONINFORMATION);
			}
			else if(ret==-2)
			{
				MessageBox(_T("�����б��в����ڴ�ָ��"),_T("��ʾ"),MB_ICONINFORMATION);
			}

			//���б�����ʾ
			DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);
		}
		else
		{
			MessageBox(_T("�����ֲ��ܴ���0xA0"),_T("��ʾ"),MB_ICONINFORMATION);
		}

		//�������۽��ڱ༭��
		CEdit *pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_ADD_MASK);
		pEdit->SetFocus();
	}
}

void CDisplayMask::OnBnClickedButtonRemoveAll()
{
	// TODO: Add your control notification handler code here

	//ɾ��������Ŀ
	MaskDeleteLink();
	//���б�����ʾ
	DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);

}

BOOL CDisplayMask::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctrlListMaskTable.DeleteAllItems();
	//�����б�ؼ����Ϊ�����ߣ�ѡ�и�����ǰ���һ����ѡ��
	m_ctrlListMaskTable.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES); 
	//���ñ�ͷ
	CString strHeader[4] = { _T("���"),_T("���ָ��"),_T("����ָ��"),_T("˵��")};
	for (int nCol = 0; nCol < 4; nCol++)
	{
		m_ctrlListMaskTable.InsertColumn(nCol, strHeader[nCol], LVCFMT_LEFT, 130);
	}
	m_ctrlListMaskTable.SetBkColor(RGB_Default);

	m_ctrlListMaskTable.SetColumnWidth(0,50);
	m_ctrlListMaskTable.SetColumnWidth(1,80);
	m_ctrlListMaskTable.SetColumnWidth(2,80);
	m_ctrlListMaskTable.SetColumnWidth(3,380);
	
	//���б�����ʾ
	DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);


	CEdit *pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_ADD_MASK);
	pEdit->SetLimitText(2);   //����ʾ���༭�����볤��Ϊ2�ֽ�
	
	m_AddNewItemFlag=0;
	m_AddNewCommandWord=0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// �������е�������Ϣ��ʾ���б�ؼ���
bool CDisplayMask::DisplayMaskList(CXListCtrl* plist, CommandMaskList* pMaskLink)
{
	CString strTemp;
	CommandMaskListItem *pItem;
	UINT16 i;

	plist->DeleteAllItems();//�����֮ǰ����ʾ
	if(pMaskLink->Number == 0)
	{
		return 0;
	}
	pItem = pMaskLink->pHeadItem;
	for(i=1; i<=pMaskLink->Number; i++)
	{
		//�����ӵ�����������ɫ
		if( (m_AddNewItemFlag==1)&&(pItem->MaskMasterCommand==m_AddNewCommandWord) )
		{
			plist->SetItemColor(pItem->Number-1,RGB_White,RGB_Red);//ǽ��ɫ
			m_AddNewItemFlag=0;
			m_AddNewCommandWord=0;
		}
		else//�����÷�ɫ
		{
			plist->SetItemColor(pItem->Number-1,RGB_Black,RGB_Pink);//��ɫ
		}

		//��1�У����
		strTemp.Format(_T("%d"),pItem->Number);
		plist->InsertItem(pItem->Number,strTemp);
		//��2�У����ָ��
		strTemp.Format(_T("0x%02X"),pItem->MaskMasterCommand);
		plist->SetItemText(pItem->Number-1,1,strTemp);
		//��3�У�����ָ��
		strTemp.Format(_T("0x%02X"),pItem->MaskSlaveCommand);
		plist->SetItemText(pItem->Number-1,2,strTemp);
		//��4�У�˵������������ϸ����
		strTemp.Format(_T("0x%02X����δ����"),pItem->MaskMasterCommand);
		plist->SetItemText(pItem->Number-1,3,strTemp);

		pItem = pItem->pNext;
	}




	return false;
}
