#pragma once
#include "resource.h"

// CInitDialog 对话框

class CInitDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInitDialog)

public:
	CInitDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitDialog();

// 对话框数据
	enum { IDD = IDD_INITIONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_initProgress;
public:
	void PostNcDestroy();
};
