#pragma once
#ifndef PXVATLS_H
	//#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif




//#pragma once
//#include "resource.h"	
//
//// CShowImageDialog 对话框
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
//	CShowImageDialog(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CShowImageDialog();
//
//// 对话框数据
//	enum { IDD =IDD_SHOWIMGDIALOG };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//
//	DECLARE_MESSAGE_MAP()
//};
