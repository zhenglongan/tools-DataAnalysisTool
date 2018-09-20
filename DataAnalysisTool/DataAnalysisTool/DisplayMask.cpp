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
		MessageBox(_T("请先输入要屏蔽的指令再点击此按钮"),_T("提示"),MB_ICONINFORMATION);
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
			MessageBox(_T("请输入16进制数，不要输入其他字符"),_T("提示"),MB_ICONINFORMATION);
			return;
		}
	}
	else
	{
		//先读高位
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
			MessageBox(_T("请输入16进制数，不要输入其他字符"),_T("提示"),MB_ICONINFORMATION);
			return;
		}

		//再读低位
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
			MessageBox(_T("请输入16进制数，不要输入其他字符"),_T("提示"),MB_ICONINFORMATION);
			return;
		}

	}
	if(value<0xA0)
	{
		//程序运行到这里表示已经获取到了输入的值
		ret = MaskItemInset(value);//插入到链表中
		//插入成功，清除输入
		if(ret==0)
		{
			//告知显示的时候，新增项需要显示为深色
			m_AddNewItemFlag=1;
			m_AddNewCommandWord=value;

			//清除编辑框中的内容
			m_strMaskCommand.Empty();
			UpdateData(FALSE);
		}
		else if(ret==-1)
		{
			MessageBox(_T("插入失败，因为已经屏蔽了此指令"),_T("提示"),MB_ICONINFORMATION);
		}
		//在列表中显示
		DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);
	}
	else
	{
		MessageBox(_T("命令字不能大于0xA0"),_T("提示"),MB_ICONINFORMATION);
	}

	//光标继续聚焦在编辑框
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
	//	MessageBox(_T("第1行被选中"),_T("提示"),MB_ICONINFORMATION);
	//}
	//if(m_ctrlListMaskTable.GetCheck(1)==1)
	//{
	//	MessageBox(_T("第2行被选中"),_T("提示"),MB_ICONINFORMATION);
	//}

	//逐行扫描是否被勾中，勾中的项目将被删除,必须从下往上扫描，否则先删除上面的项以后
	//后续的行号就错乱了
	for(i=MaskLink.Number; i>0; i--)
	{
		if(m_ctrlListMaskTable.GetCheck(i-1)==1)
		{
			flag=1;
			MaskItemDelete1(i);//删除被勾选的项目
		}
	}
	//有被删除的项目，那么刷新显示
	if(flag == 1)
	{
		//在列表中刷新显示
		DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);
	}
	//没有被选中项目的时候，再考虑去删除编辑框输入的值
	if(flag==0)
	{
		UpdateData(TRUE);
		len = m_strMaskCommand.GetLength();
		if(len<=0)
		{
			MessageBox(_T("请先选中要删除的行或者输入要取消屏蔽的指令再点击此按钮"),_T("提示"),MB_ICONINFORMATION);
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
				MessageBox(_T("请输入16进制数，不要输入其他字符"),_T("提示"),MB_ICONINFORMATION);
				return;
			}
		}
		else
		{
			//先读高位
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
				MessageBox(_T("请输入16进制数，不要输入其他字符"),_T("提示"),MB_ICONINFORMATION);
				return;
			}

			//再读低位
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
				MessageBox(_T("请输入16进制数，不要输入其他字符"),_T("提示"),MB_ICONINFORMATION);
				return;
			}

		}

		if(value<0xA0)
		{
			//程序运行到这里表示已经获取到了输入的值
			ret = MaskItemDelete(value);//从链表中删除
			if(ret==-1)
			{
				MessageBox(_T("屏蔽列表已空，不需要再取消屏蔽"),_T("提示"),MB_ICONINFORMATION);
			}
			else if(ret==-2)
			{
				MessageBox(_T("屏蔽列表中不存在此指令"),_T("提示"),MB_ICONINFORMATION);
			}

			//在列表中显示
			DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);
		}
		else
		{
			MessageBox(_T("命令字不能大于0xA0"),_T("提示"),MB_ICONINFORMATION);
		}

		//光标继续聚焦在编辑框
		CEdit *pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_ADD_MASK);
		pEdit->SetFocus();
	}
}

void CDisplayMask::OnBnClickedButtonRemoveAll()
{
	// TODO: Add your control notification handler code here

	//删除所有项目
	MaskDeleteLink();
	//在列表中显示
	DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);

}

BOOL CDisplayMask::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctrlListMaskTable.DeleteAllItems();
	//设置列表控件风格为网格线，选中高亮，前面加一个可选勾
	m_ctrlListMaskTable.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES); 
	//设置表头
	CString strHeader[4] = { _T("序号"),_T("面板指令"),_T("主控指令"),_T("说明")};
	for (int nCol = 0; nCol < 4; nCol++)
	{
		m_ctrlListMaskTable.InsertColumn(nCol, strHeader[nCol], LVCFMT_LEFT, 130);
	}
	m_ctrlListMaskTable.SetBkColor(RGB_Default);

	m_ctrlListMaskTable.SetColumnWidth(0,50);
	m_ctrlListMaskTable.SetColumnWidth(1,80);
	m_ctrlListMaskTable.SetColumnWidth(2,80);
	m_ctrlListMaskTable.SetColumnWidth(3,380);
	
	//在列表中显示
	DisplayMaskList(&m_ctrlListMaskTable,&MaskLink);


	CEdit *pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_ADD_MASK);
	pEdit->SetLimitText(2);   //限制示例编辑框输入长度为2字节
	
	m_AddNewItemFlag=0;
	m_AddNewCommandWord=0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// 把链表中的屏蔽信息显示到列表控件中
bool CDisplayMask::DisplayMaskList(CXListCtrl* plist, CommandMaskList* pMaskLink)
{
	CString strTemp;
	CommandMaskListItem *pItem;
	UINT16 i;

	plist->DeleteAllItems();//先清除之前的显示
	if(pMaskLink->Number == 0)
	{
		return 0;
	}
	pItem = pMaskLink->pHeadItem;
	for(i=1; i<=pMaskLink->Number; i++)
	{
		//新增加的命令字用深色
		if( (m_AddNewItemFlag==1)&&(pItem->MaskMasterCommand==m_AddNewCommandWord) )
		{
			plist->SetItemColor(pItem->Number-1,RGB_White,RGB_Red);//墙红色
			m_AddNewItemFlag=0;
			m_AddNewCommandWord=0;
		}
		else//其他用粉色
		{
			plist->SetItemColor(pItem->Number-1,RGB_Black,RGB_Pink);//粉色
		}

		//第1列：序号
		strTemp.Format(_T("%d"),pItem->Number);
		plist->InsertItem(pItem->Number,strTemp);
		//第2列：面板指令
		strTemp.Format(_T("0x%02X"),pItem->MaskMasterCommand);
		plist->SetItemText(pItem->Number-1,1,strTemp);
		//第3列：主控指令
		strTemp.Format(_T("0x%02X"),pItem->MaskSlaveCommand);
		plist->SetItemText(pItem->Number-1,2,strTemp);
		//第4列：说明，后续再详细处理
		strTemp.Format(_T("0x%02X，暂未分析"),pItem->MaskMasterCommand);
		plist->SetItemText(pItem->Number-1,3,strTemp);

		pItem = pItem->pNext;
	}




	return false;
}
