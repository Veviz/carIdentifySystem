#pragma once
#include "resource.h"

// CInitDialog �Ի���

class CInitDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInitDialog)

public:
	CInitDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInitDialog();

// �Ի�������
	enum { IDD = IDD_INITIONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_initProgress;
public:
	void PostNcDestroy();
};
