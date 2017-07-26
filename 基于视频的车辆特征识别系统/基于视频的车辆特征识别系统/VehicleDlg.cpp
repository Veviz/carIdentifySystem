
// VehicleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Vehicle.h"
#include "VehicleDlg.h"
#include "afxdialogex.h"
#include "ShowImageDialog.h"
#include "CarInfoDialog.h"
#include "PlateDialg.h"
#include "resource.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVehicleDlg 对话框




CVehicleDlg::CVehicleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVehicleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_color = _T("");
	m_carType = _T("");
	OriginalImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,3);			//3通道
	BackgroundImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,3);		//3通道
	SubImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,1);				//单通道
	ConturImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,1);			//单通道
	NoBgImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,3);				//3通道
	spaceOccupy=0;
	lenghtHeightRatio=0;
	tailHeightRatio=0;
	//
	conturSeq=new IplImage* [100];
	capture=NULL;
	capture1=NULL;
	curFrame=NULL;
	preFrame=NULL;
	Threshold=0;
	foregroundImg=NULL;
	grayBgImg=NULL;
	laplaceImg=NULL;
	RobertImg=NULL;	
	element=NULL;
	conturNumber=0;
	j=0;

	
}
CVehicleDlg::~CVehicleDlg()
{
	delete [] conturSeq;
	conturSeq=NULL;
	if(RobertImg)
		cvReleaseImage(&RobertImg);
	RobertImg=NULL;
	if(laplaceImg)
		cvReleaseImage(&laplaceImg);
	laplaceImg=NULL;
	if(foregroundImg)
		cvReleaseImage(&foregroundImg);
	foregroundImg=NULL;
	if(capture)
		cvReleaseCapture(&capture);
	capture=NULL;
	if(capture1)
		cvReleaseCapture(&capture1);
	capture1=NULL;

	if(OriginalImg)
		cvReleaseImage(&OriginalImg);
	OriginalImg=NULL;
	if(BackgroundImg)
		cvReleaseImage(&BackgroundImg);
	BackgroundImg=NULL;
	if(SubImg)
		cvReleaseImage(&SubImg);
	SubImg=NULL;
	if(ConturImg)
		cvReleaseImage(&ConturImg);
	ConturImg=NULL;
	if(NoBgImg)
		cvReleaseImage(&NoBgImg);
	NoBgImg=NULL;
}
void CVehicleDlg::DoDataExchange(CDataExchange* pDX)
{

	//
	
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COLOR, m_color);
	DDX_Text(pDX, IDC_CARTYPE, m_carType);

}

BEGIN_MESSAGE_MAP(CVehicleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOKSTART, &CVehicleDlg::OnBnClickedOkstart)
	ON_STN_DBLCLK(IDC_ORIGINALIMG, &CVehicleDlg::OnStnDblclickOriginalimg)
	ON_STN_DBLCLK(IDC_BACKGROUNDIMG, &CVehicleDlg::OnStnDblclickBackgroundimg)
	ON_STN_DBLCLK(IDC_SUBIMG, &CVehicleDlg::OnStnDblclickSubimg)
	ON_STN_DBLCLK(IDC_CONTURIMG, &CVehicleDlg::OnStnDblclickConturimg)
	ON_STN_DBLCLK(IDC_NOBGIMG, &CVehicleDlg::OnStnDblclickNobgimg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PLAY, &CVehicleDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PLATE, &CVehicleDlg::OnBnClickedPlate)
END_MESSAGE_MAP()


// CVehicleDlg 消息处理程序

BOOL CVehicleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVehicleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVehicleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVehicleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVehicleDlg::OnBnClickedOkstart()
{
	//TODO: 在此添加控件通知处理程序代码
	
	int i=0;
	char strname[200];	
	CString strFileName;	
	CString strFilter=_T("ALL FILES|*.*|avi文件|*.avi||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if( IDOK != dlg.DoModal()) 
		return;
	strFileName=dlg.GetPathName();
	for( i=0;i<strFileName.GetLength();i++)
		strname[i]=strFileName[i];
	strname[i]='\0';
	if(i==1)//如果路径长度为0;
		return;
	imageProcess.BackgroundInit(strname);
	cvCopy(imageProcess.BackgroundImg,BackgroundImg);
	showImagetoHDC.ShowImage(imageProcess.BackgroundImg,IDC_BACKGROUNDIMG);
	capture=cvCreateFileCapture(strname);
	capture1=cvCreateFileCapture(strname);
	curFrame=cvQueryFrame(capture);
	preFrame;
	for(int i=0;i<5;i++)
	{
		
		preFrame=cvQueryFrame(capture1);
	}
	Threshold=0;
	foregroundImg=cvCreateImage(cvGetSize(curFrame),8,1);
	grayBgImg=cvCreateImage(cvGetSize(curFrame),8,1);
	laplaceImg = cvCreateImage(cvSize(foregroundImg->width, foregroundImg->height),  foregroundImg->depth, foregroundImg->nChannels);
	element = cvCreateStructuringElementEx(3,3,1,1, CV_SHAPE_ELLIPSE, 0);
	AfxGetMainWnd()->GetDlgItem(IDC_PLAY)->EnableWindow();
}



void CVehicleDlg::OnStnDblclickOriginalimg()
{
	// TODO: 在此添加控件通知处理程序代码
	CShowImageDialog *showDialog=new CShowImageDialog(OriginalImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickBackgroundimg()
{
	// TODO: 在此添加控件通知处理程序代码
	CShowImageDialog *showDialog=new CShowImageDialog(BackgroundImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickSubimg()
{
	// TODO: 在此添加控件通知处理程序代码
	CShowImageDialog *showDialog=new CShowImageDialog(SubImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickConturimg()
{
	// TODO: 在此添加控件通知处理程序代码
	CShowImageDialog *showDialog=new CShowImageDialog(ConturImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickNobgimg()
{
	// TODO: 在此添加控件通知处理程序代码
	CCarInfoDialog *carInfoDialog=new CCarInfoDialog(NoBgImg,m_color,m_carType,lenghtHeightRatio,spaceOccupy,tailHeightRatio);
	carInfoDialog->Create(IDD_CARINFODIALOG,NULL);
	carInfoDialog->ShowWindow(SW_SHOW);
	carInfoDialog->showImg();
}


void CVehicleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDS_SHOW_PIC == nIDEvent)
	{
		curFrame=cvQueryFrame(capture);
		preFrame=cvQueryFrame(capture1);
		if(!preFrame)
			OnBnClickedPlay();
		cvCopy(curFrame,OriginalImg);
		if(!(j%4))
			imageProcess.BackgroundUpdate(curFrame,preFrame);
		showImagetoHDC.ShowImage(curFrame,IDC_ORIGINALIMG);
		showImagetoHDC.ShowImage(imageProcess.BackgroundImg,IDC_BACKGROUNDIMG);
		cvCopy(imageProcess.BackgroundImg,BackgroundImg);
		if(!(j%3))
		{ 
			imageProcess.ForegroundImageExtraction(curFrame,imageProcess.BackgroundImg,foregroundImg);
			cvCopy(foregroundImg,SubImg);
			showImagetoHDC.ShowImage(foregroundImg,IDC_SUBIMG);
		
			imageProcess.Laplace(foregroundImg,laplaceImg);
			cvCalcHist( &laplaceImg,imageProcess.MyHistogram,0, 0 ); // 计算直方图
			Threshold=imageProcess.OSTUthred(imageProcess.MyHistogram);
			if(Threshold<35)
			{
				Threshold=35;
			}
			cvThreshold(laplaceImg,laplaceImg,Threshold, 255.0,CV_THRESH_BINARY);

			if(RobertImg)
				cvReleaseImage(&RobertImg);
			RobertImg=NULL;
			RobertImg = imageProcess.Robert(foregroundImg,foregroundImg->width,foregroundImg->height);
			cvCalcHist( &RobertImg,imageProcess.MyHistogram,0, 0 ); // 计算直方图
			Threshold=imageProcess.OSTUthred(imageProcess.MyHistogram);
			if(Threshold<25)
			{
				Threshold=25;
			}
			cvThreshold(RobertImg,RobertImg,Threshold, 255.0,CV_THRESH_BINARY);


			cvCalcHist( &foregroundImg,imageProcess.MyHistogram,0, 0 ); // 计算直方图
			Threshold=imageProcess.OSTUthred(imageProcess.MyHistogram);
			if(Threshold<25)
			{
				Threshold=25;
			}
			cvThreshold(foregroundImg,foregroundImg,Threshold, 255.0,CV_THRESH_BINARY);
			cvAnd(foregroundImg,laplaceImg,laplaceImg,NULL);
			cvOr(RobertImg,laplaceImg,RobertImg,NULL);
			cvDilate(RobertImg,RobertImg);
			cvMorphologyEx(RobertImg,RobertImg,NULL,element,CV_MOP_CLOSE,1);
			cvThreshold(RobertImg,RobertImg,100, 255.0,CV_THRESH_OTSU);
			cvNot(RobertImg,RobertImg);
			cvDilate(RobertImg,RobertImg,element,1);
			conturNumber=imageProcess.CarContursExtraction(RobertImg,conturSeq);	
			cvReleaseImage(&RobertImg);
			for(int i=0;i<conturNumber;i++)
			{
				imageProcess.ConvertGray(conturSeq[i],foregroundImg);
				cvReleaseImage(&conturSeq[i]);
				if(!imageProcess.Judge(foregroundImg))
					continue;
				imageProcess.RemoveNoise(foregroundImg,5,15);
				imageProcess.RemoveNoise(foregroundImg,15,5,1);
				cvCopy(foregroundImg,ConturImg);
				showImagetoHDC.ShowImage(foregroundImg,IDC_CONTURIMG);
				cvThreshold(foregroundImg,foregroundImg,250,255,CV_THRESH_BINARY);	
				IplConvKernel* element1 = cvCreateStructuringElementEx(3,3,1,1, CV_SHAPE_ELLIPSE, 0);
				cvDilate(foregroundImg,foregroundImg,element1,1);
				imageProcess.NoBackgroudImageExtraction(curFrame,foregroundImg);
				cvCopy(imageProcess.noBgImg,NoBgImg);
				imageProcess.ColorSvm();
				showImagetoHDC.ShowImage(imageProcess.noBgImg,IDC_NOBGIMG);
				spaceOccupy=imageProcess.SpaceOccupancyExtraction(foregroundImg);				
				lenghtHeightRatio=imageProcess.LengthHeightRatioExtration(foregroundImg);
				tailHeightRatio=imageProcess.TailCarHeightRatioExtration(foregroundImg);
				imageProcess.CarTypeJudge(spaceOccupy,lenghtHeightRatio,tailHeightRatio);
				m_color=imageProcess.Color;
				m_carType=imageProcess.CarType;
				Sleep(0);
				UpdateData(FALSE);
			}
		}
	}
	j++;
	CDialogEx::OnTimer(nIDEvent);
}


void CVehicleDlg::OnBnClickedPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	CString itmCaption;
	AfxGetMainWnd()->GetDlgItemTextA(IDC_PLAY,itmCaption);
	if(itmCaption=="播放")
	{
		AfxGetMainWnd()->SetDlgItemTextA(IDC_PLAY,"暂停");
		SetTimer(IDS_SHOW_PIC,50,NULL);
	}else
	{
		AfxGetMainWnd()->SetDlgItemTextA(IDC_PLAY,"播放");
		KillTimer(IDS_SHOW_PIC);
	}
}


void CVehicleDlg::OnBnClickedPlate()
{
	// TODO: 在此添加控件通知处理程序代码
	CPlateDialg *showDialog=new CPlateDialg();
	showDialog->Create(IDD_CAR_PLATE,NULL);
	showDialog->ShowWindow(SW_SHOW);
}
