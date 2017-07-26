
// VehicleDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CVehicleDlg �Ի���




CVehicleDlg::CVehicleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVehicleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_color = _T("");
	m_carType = _T("");
	OriginalImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,3);			//3ͨ��
	BackgroundImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,3);		//3ͨ��
	SubImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,1);				//��ͨ��
	ConturImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,1);			//��ͨ��
	NoBgImg=cvCreateImage(cvSize(856,480),IPL_DEPTH_8U,3);				//3ͨ��
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


// CVehicleDlg ��Ϣ�������

BOOL CVehicleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVehicleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVehicleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVehicleDlg::OnBnClickedOkstart()
{
	//TODO: �ڴ���ӿؼ�֪ͨ����������
	
	int i=0;
	char strname[200];	
	CString strFileName;	
	CString strFilter=_T("ALL FILES|*.*|avi�ļ�|*.avi||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if( IDOK != dlg.DoModal()) 
		return;
	strFileName=dlg.GetPathName();
	for( i=0;i<strFileName.GetLength();i++)
		strname[i]=strFileName[i];
	strname[i]='\0';
	if(i==1)//���·������Ϊ0;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CShowImageDialog *showDialog=new CShowImageDialog(OriginalImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickBackgroundimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CShowImageDialog *showDialog=new CShowImageDialog(BackgroundImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickSubimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CShowImageDialog *showDialog=new CShowImageDialog(SubImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickConturimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CShowImageDialog *showDialog=new CShowImageDialog(ConturImg);
	showDialog->Create(IDD_SHOWIMGDIALOG,NULL);
	showDialog->ShowWindow(SW_SHOW);
	showDialog->ShowImg();
}


void CVehicleDlg::OnStnDblclickNobgimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCarInfoDialog *carInfoDialog=new CCarInfoDialog(NoBgImg,m_color,m_carType,lenghtHeightRatio,spaceOccupy,tailHeightRatio);
	carInfoDialog->Create(IDD_CARINFODIALOG,NULL);
	carInfoDialog->ShowWindow(SW_SHOW);
	carInfoDialog->showImg();
}


void CVehicleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			cvCalcHist( &laplaceImg,imageProcess.MyHistogram,0, 0 ); // ����ֱ��ͼ
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
			cvCalcHist( &RobertImg,imageProcess.MyHistogram,0, 0 ); // ����ֱ��ͼ
			Threshold=imageProcess.OSTUthred(imageProcess.MyHistogram);
			if(Threshold<25)
			{
				Threshold=25;
			}
			cvThreshold(RobertImg,RobertImg,Threshold, 255.0,CV_THRESH_BINARY);


			cvCalcHist( &foregroundImg,imageProcess.MyHistogram,0, 0 ); // ����ֱ��ͼ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString itmCaption;
	AfxGetMainWnd()->GetDlgItemTextA(IDC_PLAY,itmCaption);
	if(itmCaption=="����")
	{
		AfxGetMainWnd()->SetDlgItemTextA(IDC_PLAY,"��ͣ");
		SetTimer(IDS_SHOW_PIC,50,NULL);
	}else
	{
		AfxGetMainWnd()->SetDlgItemTextA(IDC_PLAY,"����");
		KillTimer(IDS_SHOW_PIC);
	}
}


void CVehicleDlg::OnBnClickedPlate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPlateDialg *showDialog=new CPlateDialg();
	showDialog->Create(IDD_CAR_PLATE,NULL);
	showDialog->ShowWindow(SW_SHOW);
}
