#pragma once
#ifndef PXVATLS_H
	//#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif




//#pragma once
//#include "resource.h"	
//
//// CShowImageDialog �Ի���
//
//class CShowImageDialog : public CDialogEx
//{
//	DECLARE_DYNAMIC(CShowImageDialog)
//
void findChar(char C,char *str)
{
	if(C=='L')
		sprintf(str,"R8251");
	if(C=='J')
		sprintf(str,"R4200");
	if(C=='H')
		sprintf(str,"96171");
}
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
