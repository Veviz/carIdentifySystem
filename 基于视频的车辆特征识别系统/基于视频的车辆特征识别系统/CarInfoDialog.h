
#pragma once
#include "resource.h"	
#include "ShowImagetoHDC.h"
// CCarInfoDialog �Ի���

class CCarInfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCarInfoDialog)

public:
	CCarInfoDialog(CWnd* pParent = NULL);   // ��׼���캯��
	CCarInfoDialog(IplImage * img,CString color,CString cartype,double lengthheight,double spaceoccupy,double tailheight,CWnd* pParent = NULL);
	virtual ~CCarInfoDialog();

// �Ի�������
	enum { IDD = IDD_CARINFODIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
