// InitDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InitDialog.h"
#include "afxdialogex.h"


// CInitDialog �Ի���

IMPLEMENT_DYNAMIC(CInitDialog, CDialogEx)

CInitDialog::CInitDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInitDialog::IDD, pParent)
{
	
}

CInitDialog::~CInitDialog()
{
}

void CInitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INITPROGRESS, m_initProgress);
}
void CInitDialog::PostNcDestroy()
{
	delete this;
}

BEGIN_MESSAGE_MAP(CInitDialog, CDialogEx)
END_MESSAGE_MAP()


// CInitDialog ��Ϣ�������
