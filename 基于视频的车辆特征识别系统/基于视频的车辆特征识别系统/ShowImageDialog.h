//#pragma once
//#include "resource.h"	
//
//// CShowImageDialog 对话框
//
//class CShowImageDialog : public CDialogEx
//{
//	DECLARE_DYNAMIC(CShowImageDialog)
//
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
#pragma once
#include "resource.h"	
#include "ShowImagetoHDC.h"
// CShowImageDialog 对话框

class CShowImageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CShowImageDialog)

public:
	CShowImageDialog(CWnd* pParent = NULL);   // 标准构造函数
	CShowImageDialog(IplImage * img,CWnd* pParent = NULL);
	virtual ~CShowImageDialog();

// 对话框数据
	enum { IDD =IDD_SHOWIMGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ShowImg();
	void PostNcDestroy();
public:
	IplImage * BigImg;
};
