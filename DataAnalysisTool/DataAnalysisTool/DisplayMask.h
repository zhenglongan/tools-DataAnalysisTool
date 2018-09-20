#pragma once
#include "afxcmn.h"
#include "XListCtrl.h"
#include "CommandMask.h"

// CDisplayMask dialog

class CDisplayMask : public CDialog
{
	DECLARE_DYNAMIC(CDisplayMask)

public:
	CDisplayMask(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayMask();

// Dialog Data
	enum { IDD = IDD_DIALOG_SET_DISPLAY_MASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// ��������ʾ��ͨ��ָ���б�ؼ�
	CXListCtrl m_ctrlListMaskTable;
	afx_msg void OnBnClickedButtonAddMask();
	afx_msg void OnBnClickedButtonRemoveMask();
	afx_msg void OnBnClickedButtonRemoveAll();
	virtual BOOL OnInitDialog();
	// �������е�������Ϣ��ʾ���б�ؼ���
	bool DisplayMaskList(CXListCtrl* plist, CommandMaskList* pMaskLink);
	CString m_strMaskCommand;
	// 1��ʾ������һ��������Ŀ�����ʱ����ʾ��Ҫ����ɫ����
	UINT8 m_AddNewItemFlag;
	// �������������������
	UINT8 m_AddNewCommandWord;
};
