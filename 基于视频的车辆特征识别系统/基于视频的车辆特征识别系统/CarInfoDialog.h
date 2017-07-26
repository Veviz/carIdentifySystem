
#pragma once
#include "resource.h"	
#include "ShowImagetoHDC.h"
// CCarInfoDialog 对话框

class CCarInfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCarInfoDialog)

public:
	CCarInfoDialog(CWnd* pParent = NULL);   // 标准构造函数
	CCarInfoDialog(IplImage * img,CString color,CString cartype,double lengthheight,double spaceoccupy,double tailheight,CWnd* pParent = NULL);
	virtual ~CCarInfoDialog();

// 对话框数据
	enum { IDD = IDD_CARINFODIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_color;
	CString m_cartype;
	double m_lengthheight;
	double m_spaceoccupy;
	double m_tailheight;
	IplImage * CurImg;
public:
	void showImg();
	void PostNcDestroy();

};
