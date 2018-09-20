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
	// 被屏蔽显示的通信指令列表控件
	CXListCtrl m_ctrlListMaskTable;
	afx_msg void OnBnClickedButtonAddMask();
	afx_msg void OnBnClickedButtonRemoveMask();
	afx_msg void OnBnClickedButtonRemoveAll();
	virtual BOOL OnInitDialog();
	// 把链表中的屏蔽信息显示到列表控件中
	bool DisplayMaskList(CXListCtrl* plist, CommandMaskList* pMaskLink);
	CString m_strMaskCommand;
	// 1表示新增了一个屏蔽项目，这个时候显示需要把颜色变深
	UINT8 m_AddNewItemFlag;
	// 新增加屏蔽项的命令字
	UINT8 m_AddNewCommandWord;
};
