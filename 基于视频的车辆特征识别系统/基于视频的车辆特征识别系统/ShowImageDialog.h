//#pragma once
//#include "resource.h"	
//
//// CShowImageDialog �Ի���
//
//class CShowImageDialog : public CDialogEx
//{
//	DECLARE_DYNAMIC(CShowImageDialog)
//
//public:
//	CShowImageDialog(CWnd* pParent = NULL);   // ��׼���캯��
//	virtual ~CShowImageDialog();
//
//// �Ի�������
//	enum { IDD =IDD_SHOWIMGDIALOG };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
//
//	DECLARE_MESSAGE_MAP()
//};
#pragma once
#include "resource.h"	
#include "ShowImagetoHDC.h"
// CShowImageDialog �Ի���

class CShowImageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CShowImageDialog)

public:
	CShowImageDialog(CWnd* pParent = NULL);   // ��׼���캯��
	CShowImageDialog(IplImage * img,CWnd* pParent = NULL);
	virtual ~CShowImageDialog();

// �Ի�������
	enum { IDD =IDD_SHOWIMGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowImg();
	void PostNcDestroy();
public:
	IplImage * BigImg;
};
