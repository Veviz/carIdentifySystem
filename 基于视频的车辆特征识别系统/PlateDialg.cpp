// PlateDialg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlateDialg.h"
#include "afxdialogex.h"
#include "pxvatls.h"

// CPlateDialg 对话框

IMPLEMENT_DYNAMIC(CPlateDialg, CDialogEx)

CPlateDialg::CPlateDialg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlateDialg::IDD, pParent)
{

	m_result = _T("");
}

CPlateDialg::~CPlateDialg()
{
}

void CPlateDialg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RESULTEDIT, m_result);
}


BEGIN_MESSAGE_MAP(CPlateDialg, CDialogEx)
	ON_BN_CLICKED(ID_OPENIMAGE, &CPlateDialg::OnBnClickedOpenimage)
	ON_BN_CLICKED(ID_PLATE_FIND, &CPlateDialg::OnBnClickedPlateFind)
	ON_BN_CLICKED(IDC_PLATE_DIV, &CPlateDialg::OnBnClickedPlateDiv)
	ON_BN_CLICKED(IDC_RESULT, &CPlateDialg::OnBnClickedResult)
END_MESSAGE_MAP()


// CPlateDialg 消息处理程序

void CPlateDialg::showImg()
{//此函数用于在picture control中显示图片
	CDC* pDC=GetDlgItem(IDD_CAR_PLATE)->GetDC();				// 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();									// 获取 HDC(设备句柄) 来进行绘图操作

    CRect rect;
	GetDlgItem(IDD_CAR_PLATE)->GetClientRect(&rect);
    int rw = rect.right - rect.left;								// 求出图片控件的宽和高
    int rh = rect.bottom - rect.top;
	IplImage *reSizeImg=cvCreateImage(cvSize(rw,rh),CurImg->depth,CurImg->nChannels);
	cvResize(CurImg,reSizeImg);
	SetRect(rect,0,0,rw,rh );
	CvvImage cvvImage;
	cvvImage.CopyOf(reSizeImg);										// 赋值给父类图像成员指针
    cvvImage.DrawToHDC(hDC,&rect );										// 调用父类绘图函数绘制图片
	cvReleaseImage(&reSizeImg);
    ReleaseDC(pDC);
}
void CPlateDialg::PostNcDestroy()
{
	delete this;
}
void CPlateDialg::OnBnClickedOpenimage()//打开图像
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter; 
	CString FileName;
	CSimpleArray<GUID> aguidFileTypes; 
	HRESULT hResult;// 获取CImage支持的图像文件的过滤字符串
	hResult = m_Image.GetExporterFilterString(strFilter,aguidFileTypes, _T( "All Image Files") );
	 if (FAILED(hResult)) 
	{ 
		MessageBox(_T("GetExporterFilter调用失败！"));
		return; 
	}
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if(IDOK != dlg.DoModal()) 
		return;
	m_Image.Destroy(); // 将外部图像文件装载到CImage对象中
	hResult = m_Image.Load(dlg.GetPathName());
	if (FAILED(hResult)) 
	{
		MessageBox(_T("调用图像文件失败！")); 
		return;
	}
	// 设置主窗口标题栏内容 
	CString str; 
	int i;
	str.LoadString(AFX_IDS_APP_TITLE);
	AfxGetMainWnd()->SetWindowText(str + _T(" - ") +dlg.GetPathName());
	FileName=dlg.GetPathName();
	Model=0;
	//Invalidate(); // 强制调用OnDraw
	CurImg=cvLoadImage(FileName,1);
	showImg();
}


void CPlateDialg::OnBnClickedPlateFind()//车牌定位
{
	// TODO: 在此添加控件通知处理程序代码


	//===========车牌定位模块=============
	//1.  灰度化图片
	//2.  Sobel边沿检测
	//3.  二值化处理
	//4.  削弱背景干扰
	//5.  水平投影
	//6.  垂直投影
	//#################################################################################################
	int i,j;                                        //循环变量
	int pixel;                                  //像素变量，记录图像每个像素的值
	bool find=false;
	CvScalar pb,pa;                                    //用于获取图像的像素的对象
	IplImage *src=CurImg;//src指针用于指向原图片
	IplImage *OprImg;
	//1.灰度化图片
	IplImage *gray=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	cvCvtColor(src,gray,CV_BGR2GRAY);
	OprImg=gray;
	//////////////////////////////////////////////////////////////////////////////

	//2.Sobel 边缘检测
	IplImage *sobel=NULL;
	sobel=cvCreateImage(cvGetSize(gray),IPL_DEPTH_16S,1);
    cvSobel(gray,sobel,1,0,3);
    IplImage *temper=cvCreateImage(cvGetSize(sobel),IPL_DEPTH_8U,1);
    cvConvertScaleAbs(sobel,temper,1,0);
	OprImg=temper;
	
	//*****************************************************************************************************************
    //3.二值化处理
	IplImage *bi = cvCreateImage(cvGetSize(OprImg),IPL_DEPTH_8U,1);
    cvThreshold(OprImg,bi, 0,255,   CV_THRESH_OTSU | CV_THRESH_BINARY);
    OprImg=bi;
	//*****************************************************************************************************************

	////////////////////////////////////////////////////////////////
	//4.削弱背景干扰
	IplImage * backGround=cvCreateImage(cvGetSize(OprImg),IPL_DEPTH_8U,1);
	for(i=0;i<backGround->height-1;i++)                          
		for(j=0;j<backGround->width-1;j++)
		{
			pb=cvGet2D(OprImg,i,j);
			pa=cvGet2D(OprImg,i,j+1);

			pixel=pb.val[0]-pa.val[0];

			if(pixel<0)
				pixel=-pixel;
			if(pixel>255)
				pixel=255;
			pb.val[0]=pixel;
			cvSet2D(backGround,i,j,pb);
		}
	OprImg=backGround;

	//======================车牌定位模块 ,此模块主要使用投影法进行操作
	//1.  水平投影
	//2.  垂直投影
	//####################################################################################################
	//1.水平投影
	find=false;
	int tempe;
	bottom=0,top=OprImg->height;
	for(i=OprImg->height*0.7;i<OprImg->height;i++)
	{
		pixel=0;
		for(j=0;j<OprImg->width;j++)
		{
			pb=cvGet2D(OprImg,i,j);
			tempe=pb.val[0];
			if(tempe==255)
				pixel++;
		}
		if((!find) && (pixel>20))
		{
			bottom=i;
			find=true;
		}
		if(find && (pixel<20))
		{
			top=i;
			break;
		}
	}
	bottom-=25;
	top+=10;

	for(i=0;i<bottom;i++)
	{
		for(j=0;j<src->width;j++)
		{
			pb=cvGet2D(src,i,j);
			pb.val[0]=0;
			pb.val[1]=0;
			pb.val[2]=0;
			cvSet2D(src,i,j,pb);
			cvSet2D(OprImg,i,j,pb);
		}
	}
	for(i=top;i<src->height;i++)
	{
		for(j=0;j<src->width;j++)
		{
			pb=cvGet2D(src,i,j);
			pb.val[0]=0;
			pb.val[1]=0;
			pb.val[2]=0;
			cvSet2D(src,i,j,pb);
			cvSet2D(OprImg,i,j,pb);
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////
	//2.垂直投影
	lBorder=0,rBorder=OprImg->width;
	for(j=OprImg->width*0.35;j<OprImg->width;j++)
	{
		pixel=0;
		for(i=bottom;i<top;i++)
		{
			pb=cvGet2D(OprImg,i,j);
			tempe=pb.val[0];
			if(tempe==255)
				pixel++;
		}
		if(pixel>15)
		{	
			lBorder=j-25;
		    rBorder=lBorder+170;
			break;
		}
	}


	for(j=0;j<lBorder;j++)
	{
		for(i=bottom;i<top;i++)
		{
			pb=cvGet2D(src,i,j);
			pb.val[0]=0;
			pb.val[1]=0;
			pb.val[2]=0;
			cvSet2D(src,i,j,pb);
		}
	}
	for(j=rBorder;j<OprImg->width;j++)
	{
		for(i=bottom;i<top;i++)
		{
			pb=cvGet2D(src,i,j);
			pb.val[0]=0;
			pb.val[1]=0;
			pb.val[2]=0;
			cvSet2D(src,i,j,pb);
		}
	}
	CurImg=src;
	showImg();
}


void CPlateDialg::OnBnClickedPlateDiv()//字符分割
{
	// TODO: 在此添加控件通知处理程序代码
	//===========================车牌图像预处理模块
	//1.  将图片大小改为车牌图片大小
	//2.  灰度化、二值化处理
	//3.  Canny 边沿检测
	//4.  Hough 变换
	//5.  仿射变换
	//6.  去除车牌边框
	//7.  通过垂直投影，找到每个车牌字符的左右边沿
	//8.  根据每个字符的左右边沿，将字符逐个分割，并把每一个字符都单独重新创建一张图片保存
	//####################################################################################################
	int i,j;                                        //循环变量
	int pixel;                                  //像素变量，记录图像每个像素的值
	bool find=false;
	CvScalar pb,pa;                                    //用于获取图像的像素的对象
	IplImage *src=CurImg;//src指针用于指向原图片
	IplImage *OprImg;
	IplImage *image;
	image=cvCreateImage(cvSize(rBorder-lBorder,top-bottom),IPL_DEPTH_8U,3);
	int x,y;
	for(i=bottom,x=0;i<top;i++,x++)
	{	
		for(j=lBorder,y=0;j<rBorder;j++,y++)
		{
			pb=cvGet2D(src,i,j);
			pa.val[0]=pb.val[0];
			pa.val[1]=pb.val[1];
			pa.val[2]=pb.val[2];
			cvSet2D(image,x,y,pa);
		}
	}
	//================================灰度化处理==================================
	IplImage *linshi=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	cvCvtColor(image,linshi,CV_RGB2GRAY);
	//================================二值化处理===================================
    cvThreshold(linshi,linshi, 0,255,   CV_THRESH_OTSU | CV_THRESH_BINARY);
	src=linshi;
	//==================================Canny 检测==================================
	IplImage *canny=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	cvCanny(src,canny,50,150,3);
	//==================================Hough 变换==================================
	CvSeq* lines = NULL;
	CvMemStorage* storage = NULL;
	float theta=0;
	storage = cvCreateMemStorage (0);
	lines = cvHoughLines2 (canny, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 80, 200, 10);
	for (int i = 0; i < lines->total; i++)
	{
		 float* line = (float*)cvGetSeqElem(lines, i);
		if(line[1] <10 && line[1]>-10)
			theta=line[1];
	}
	//=========================================仿射变换==========================
	IplImage * Img_tmp=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	float m[6]; 
	theta=4;
	CvMat M = cvMat( 2, 3, CV_32F, m );//一个2*3的矩阵
	CvPoint2D32f center;//点的类型
	center.x=float (src->width/2.0+0.5);//寻找翻转的中心
	center.y=float (src->height/2.0+0.5);   
	cv2DRotationMatrix( center, theta,1, &M);
	cvWarpAffine(src,Img_tmp, &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );
	src=Img_tmp;
	//===========================================去除上下边框=====================
	bool tag=false;
	int img_top=0,img_bottom=0;
	int img_lBorder=0,img_rBorder=src->width;
	int a,b;
	for(a=0;a<src->height;a++)
	{
		pixel=0;
		for(b=0;b<src->width;b++)
		{
			pb=cvGet2D(src,a,b);
			if(pb.val[0]>0)
				pixel++;
		}
		if(!tag && pixel>20)
		{
			img_bottom=a;
			tag=true;
		}
		if(tag && pixel<=20 )
		{
			img_top=a;
			if(img_top-img_bottom<src->height*0.4)
			{
					tag=false;
					img_top=0;
			}
			else
				 break;
		}
	}
	for(a=0;a<img_bottom;a++)
		for(b=0;b<src->width;b++)
		{
			pb=cvGet2D(src,a,b);
			pb.val[0]=0;
			cvSet2D(src,a,b,pb);
		}
	for(a=img_top;a<src->height;a++)
		for(b=0;b<src->width;b++)
		{
			pb=cvGet2D(src,a,b);
			pb.val[0]=0;
			cvSet2D(src,a,b,pb);
		}
	//==================车牌去噪===============
	cvSmooth(src,src,CV_MEDIAN);

	//###################################################################################################
	int l[8]={0};             //记录每一个字符的左边界
	int r[8]={0};             //记录每一个字符的右边界
	int pixelSum[2]={0};
	int Count[8]={0};
	a=0,b=0;
	int count=0;
	int charaWidth;         //每一个字符的宽度保持一致
	int width;             //每个字符的宽度和最大宽度的差值
	for(j=0;j<src->width;j++)//通过水平投影，将每一个字符分割开来
	{
		pixelSum[1]=0;
		for(i=0;i<src->height;i++)
		{
			pb=cvGet2D(src,i,j);
			pixelSum[1]+=pb.val[0];
			if(pb.val[0]==255)
				Count[count]++;
		}
		if(pixelSum[0]<10 && pixelSum[1]>=10)
		{
			l[a]=j;
			a++;
		}
		else if(pixelSum[0]>10 && pixelSum[1]<10)
		{
			r[b]=j-1;
			b++;
			count++;
			if(Count[count-1]<50)
			{
				count--;
				a--;
				b--;
			}
		}
		pixelSum[0]=pixelSum[1];
	}

	//统一规定每个字符的宽度为所有字符中宽度最大的字符宽度
	charaWidth=-1;
	for(int aa=0;aa<7;aa++)
	{
		if(charaWidth<r[aa]-l[aa])
			charaWidth=r[aa]-l[aa];
	}
	//****************************************************************************************************
	chara1=cvCreateImage(cvSize(r[0]-l[0],img_top-img_bottom),IPL_DEPTH_8U,1);
	int i1,j1;
	for(i1=img_bottom,x=0;i1<img_top;i1++,x++)
	{
		for(j1=l[0],y=0;j1<r[0];j1++,y++)
			{
				pb=cvGet2D(src,i1,j1);
				pa.val[0]=pb.val[0];
				cvSet2D(chara1,x,y,pa);
			}
	}
	//在二值化的图片上面绘出第一个字符的位置
	//第一个字符的四个点的坐标为：A(l[0],img_bottom),B(r[0],img_bottom),C(l[0],img_top),D(r[0],img_top)
	cvLine(Img_tmp,cvPoint(l[0],img_bottom),cvPoint(r[0],img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[0],img_bottom),cvPoint(r[0],img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[0],img_top),cvPoint(l[0],img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[0],img_top),cvPoint(l[0],img_bottom),cvScalar(255,255,255),1,8,0);
	//****************************************************************************************************

	//****************************************************************************************************
	chara2=cvCreateImage(cvSize(charaWidth,img_top-img_bottom),IPL_DEPTH_8U,1);
	width=(charaWidth-(r[1]-l[1]))/2.0;
	int i2,j2;
	for(i2=img_bottom,x=0;i2<img_top;i2++,x++)
	{
		for(j2=l[1]-width,y=0;j2<r[1]+width;j2++,y++)
			{
				pb=cvGet2D(src,i2,j2);
				pa.val[0]=pb.val[0];
				cvSet2D(chara2,x,y,pa);
			}
	}
	//第二个字符的四个点的坐标为：A(l[1]-width,img_bottom),B(r[1]+width,img_bottom),C(l[1]-width,img_top),D(r[1]+width,img_top)
	cvLine(Img_tmp,cvPoint(l[1]-width,img_bottom),cvPoint(r[1]+width,img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[1]+width,img_bottom),cvPoint(r[1]+width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[1]+width,img_top),cvPoint(l[1]-width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[1]-width,img_top),cvPoint(l[1]-width,img_bottom),cvScalar(255,255,255),1,8,0);

	//****************************************************************************************************
	chara3=cvCreateImage(cvSize(charaWidth,img_top-img_bottom),IPL_DEPTH_8U,1);
	width=(charaWidth-(r[2]-l[2]))/2.0;
	int i3,j3;
	for(i3=img_bottom,x=0;i3<img_top;i3++,x++)
	{
		for(j3=l[2]-width,y=0;j3<r[2]+width;j3++,y++)
			{
				pb=cvGet2D(src,i3,j3);
				pa.val[0]=pb.val[0];
				cvSet2D(chara3,x,y,pa);
			}
	}
	//第三个字符的四个点的坐标为：A(l[2]-width,img_bottom),B(r[2]+width,img_bottom),C(l[2]-width,img_top),D(r[2]+width,img_top)
	cvLine(Img_tmp,cvPoint(l[2]-width,img_bottom),cvPoint(r[2]+width,img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[2]+width,img_bottom),cvPoint(r[2]+width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[2]+width,img_top),cvPoint(l[2]-width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[2]-width,img_top),cvPoint(l[2]-width,img_bottom),cvScalar(255,255,255),1,8,0);
	//****************************************************************************************************

	//****************************************************************************************************
	chara4=cvCreateImage(cvSize(charaWidth,img_top-img_bottom),IPL_DEPTH_8U,1);
	width=(charaWidth-(r[3]-l[3]))/2.0;
	int i4,j4;
	for(i4=img_bottom,x=0;i4<img_top;i4++,x++)
	{
		for(j4=l[3]-width,y=0;j4<r[3]+width;j4++,y++)
			{
				pb=cvGet2D(src,i4,j4);
				pa.val[0]=pb.val[0];
				cvSet2D(chara4,x,y,pa);
			}
	}
	//第四个字符的四个点的坐标为：A(l[3]-width,img_bottom),B(r[3]+width,img_bottom),C(l[3]-width,img_top),D(r[3]+width,img_top)
	cvLine(Img_tmp,cvPoint(l[3]-width,img_bottom),cvPoint(r[3]+width,img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[3]+width,img_bottom),cvPoint(r[3]+width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[3]+width,img_top),cvPoint(l[3]-width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[3]-width,img_top),cvPoint(l[3]-width,img_bottom),cvScalar(255,255,255),1,8,0);
	//****************************************************************************************************

	//****************************************************************************************************
	chara5=cvCreateImage(cvSize(charaWidth,img_top-img_bottom),IPL_DEPTH_8U,1);
	width=(charaWidth-(r[4]-l[4]))/2.0;
	int i5,j5;
	for(i5=img_bottom,x=0;i5<img_top;i5++,x++)
	{
		for(j5=l[4]-width,y=0;j5<r[4]+width;j5++,y++)
			{
				pb=cvGet2D(src,i5,j5);
				pa.val[0]=pb.val[0];
				cvSet2D(chara5,x,y,pa);
			}
	}
	//第五个字符的四个点的坐标为：A(l[4]-width,img_bottom),B(r[4]+width,img_bottom),C(l[4]-width,img_top),D(r[4]+width,img_top)
	cvLine(Img_tmp,cvPoint(l[4]-width,img_bottom),cvPoint(r[4]+width,img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[4]+width,img_bottom),cvPoint(r[4]+width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[4]+width,img_top),cvPoint(l[4]-width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[4]-width,img_top),cvPoint(l[4]-width,img_bottom),cvScalar(255,255,255),1,8,0);
	//****************************************************************************************************

	//*****************************************************************************************************
	chara6=cvCreateImage(cvSize(charaWidth,img_top-img_bottom),IPL_DEPTH_8U,1);
	width=(charaWidth-(r[5]-l[5]))/2.0;
	int i6,j6;
	for(i6=img_bottom,x=0;i6<img_top;i6++,x++)
	{
		for(j6=l[5]-width,y=0;j6<r[5]+width;j6++,y++)
			{
				pb=cvGet2D(src,i6,j6);
				pa.val[0]=pb.val[0];
				cvSet2D(chara6,x,y,pa);
			}
	}
	//第六个字符的四个点的坐标为：A(l[5]-width,img_bottom),B(r[5]+width,img_bottom),C(l[5]-width,img_top),D(r[5]+width,img_top)
	cvLine(Img_tmp,cvPoint(l[5]-width,img_bottom),cvPoint(r[5]+width,img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[5]+width,img_bottom),cvPoint(r[5]+width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[5]+width,img_top),cvPoint(l[5]-width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[5]-width,img_top),cvPoint(l[5]-width,img_bottom),cvScalar(255,255,255),1,8,0);
	//******************************************************************************************************

	//*********************************************************************************************************
	chara7=cvCreateImage(cvSize(charaWidth,img_top-img_bottom),IPL_DEPTH_8U,1);
	width=(charaWidth-(r[6]-l[6]))/2.0;
	int i7,j7;
	for(i7=img_bottom,x=0;i7<img_top;i7++,x++)
	{
		for(j7=l[6]-width,y=0;j7<r[6]+width;j7++,y++)
			{
				pb=cvGet2D(src,i7,j7);
				pa.val[0]=pb.val[0];
				cvSet2D(chara7,x,y,pa);
			}
	}
	//第七个字符的四个点的坐标为：A(l[6]-width,img_bottom),B(r[6]+width,img_bottom),C(l[6]-width,img_top),D(r[6]+width,img_top)
	cvLine(Img_tmp,cvPoint(l[6]-width,img_bottom),cvPoint(r[6]+width,img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[6]+width,img_bottom),cvPoint(r[6]+width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[6]+width,img_top),cvPoint(l[6]-width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[6]-width,img_top),cvPoint(l[6]-width,img_bottom),cvScalar(255,255,255),1,8,0);
	//******************************************************************************************************
	CurImg=Img_tmp;
	showImg();
}


void CPlateDialg::OnBnClickedResult()//车牌字符识别
{
	// TODO: 在此添加控件通知处理程序代码
	//字符识别模块，本模块主要有以下几个步骤
	//1.对已经分割的车牌字符进行归一化
	//2.对模板字符的学习（将模板字符的特征提取出来，并记录在文本中）
	//3.对归一化的字符进行特征提取
	//4.通过分类器，将车牌字符分属到不同的字符模板类中，从而识别出车牌字符
	//#################################################################################################################
	
	//1.对提取的车牌进行缩放，将车牌大小缩放到模板的大小
	IplImage *dst_chara1,*dst_chara2,*dst_chara3,*dst_chara4,*dst_chara5,*dst_chara6,*dst_chara7;

	int templat_height=40;
	int templat_width=20;

	CvSize dst_cvsize;
	dst_cvsize.height=templat_height;
	dst_cvsize.width=templat_width;
     //对每个车牌字符利用“临近插值法”进行放缩
	//***********************************************************************************
	dst_chara1=cvCreateImage(dst_cvsize,IPL_DEPTH_8U,1);
	cvResize(chara1,dst_chara1,CV_INTER_NN);       

	dst_chara2=cvCreateImage(dst_cvsize,IPL_DEPTH_8U,1);
	cvResize(chara2,dst_chara2,CV_INTER_NN);

	dst_chara3=cvCreateImage(dst_cvsize,IPL_DEPTH_8U,1);
	cvResize(chara3,dst_chara3,CV_INTER_NN);

	dst_chara4=cvCreateImage(dst_cvsize,IPL_DEPTH_8U,1);
	cvResize(chara4,dst_chara4,CV_INTER_NN);

	dst_chara5=cvCreateImage(dst_cvsize,IPL_DEPTH_8U,1);
	cvResize(chara5,dst_chara5,CV_INTER_NN);

	dst_chara6=cvCreateImage(dst_cvsize,IPL_DEPTH_8U,1);
	cvResize(chara6,dst_chara6,CV_INTER_NN);

	dst_chara7=cvCreateImage(dst_cvsize,IPL_DEPTH_8U,1);
	cvResize(chara7,dst_chara7,CV_INTER_NN);
	//***********************************************************************************


	//===============================================================================================================
	//2.提取模板特征
	//将模板特征提取出来存放在文件中，第一次需要提取，以后只需要从文件中直接读取便可,故而需要在文件夹中创建一个标志文件
	//以判别模板特征是否已经提取过
	FILE *fp;
	char filename[40]={0};
	sprintf(filename,".//Data//flags.txt");
	if((fp=fopen(filename,"r"))==NULL)
	{
		fp=fopen(filename,"w");
		fputs(filename,fp);
		getTemplateFeature();
	}

	//===============================================================================================================
	//3.提取车牌字符特征，并也存储在Data文件夹中
	getCharaFeature(dst_chara1,1);
	getCharaFeature(dst_chara2,2);
	getCharaFeature(dst_chara3,3);
	getCharaFeature(dst_chara4,4);
	getCharaFeature(dst_chara5,5);
	getCharaFeature(dst_chara6,6);
	getCharaFeature(dst_chara7,7);

	//4.分类器，将车牌特征从文件中读取出来，并与对应的模板特征一一比对
	//先比较第一个字符：汉字
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//从文件中读取汉子的十三个特征值
	int feature[13]={0};//用于存储汉子的十三个特征值的数组
	char featu[10]={0};
	int feaTemplate[13]={0};
	char FileName[30]={0};
	int c,count;
	long mindistance=99999999;                //用于记录每次计算后车牌字符和模板字符的最小距离
	long distance;
	int min=-1;                     //用于记录哪一个模板字符和车牌字符最近 
	char C;
	char result[10]={0};
	char str[10]={0};
	if((fp=fopen(".//Data//character_1.txt","r"))==NULL)
	{
		exit(0);
	}
	for(count=0;count<13;count++)//得到车牌字符的十三个特征值
	{
		fgets(featu,6,fp);
		feature[count]=atoi(featu);
	}

	for(count=34;count<65;count++)//从文本中读取30个模板字符的特征，并记录下和车牌字符最近的模板字符
	{
		sprintf(FileName,".//Data//%d.txt",count);
		if((fp=fopen(FileName,"r"))==NULL)
	    {
		    exit(0);
	    }
		for(c=0;c<13;c++)
	    {
		    fgets(featu,6,fp);
		    feaTemplate[c]=atoi(featu);
	    }
		fclose(fp);
		//通过欧式距离的平方计算，比较车牌字符更接近哪一个模板字符
		distance=0;
		for(c=0;c<13;c++)
		{
			distance+=(feaTemplate[c]-feature[c])*(feaTemplate[c]-feature[c]);
		}
		if(distance<mindistance)
		{
			mindistance=distance;
			min=count;
		}
		if(count==42)
			break;
	}
	char chinese[10]={0};
	switch(min)
	{
	case 34:
		sprintf(chinese,"藏");
		break;
	case 35:
		sprintf(chinese,"川");
		break;
	case 36:
		sprintf(chinese,"鄂");
		break;
	case 37:
		sprintf(chinese,"赣");
		break;
	case 38:
		sprintf(chinese,"蒙");
		break;
	case 39:
		sprintf(chinese,"新");
		break;
	case 40:
		sprintf(chinese,"豫");
		break;
	case 41:
		sprintf(chinese,"京");
		break;
	case 42:
		sprintf(chinese,"湘");
		break;
	case 43:
		sprintf(chinese,"冀");
		break;
	case 44:
		sprintf(chinese,"渝");
		break;
	case 45:
		sprintf(chinese,"闽");
		break;
	case 46:
		sprintf(chinese,"皖");
		break;
	case 47:
		sprintf(chinese,"沪");
		break;
	case 48:
		sprintf(chinese,"甘");
		break;
	case 49:
		sprintf(chinese,"津");
		break;
	case 50:
		sprintf(chinese,"辽");
		break;
	case 51:
		sprintf(chinese,"桂");
		break;
	case 52:
		sprintf(chinese,"琼");
		break;
	case 53:
		sprintf(chinese,"鲁");
		break;
	case 54:
		sprintf(chinese,"吉");
		break;
	case 55:
		sprintf(chinese,"浙");
		break;
	case 56:
		sprintf(chinese,"宁");
		break;
	case 57:
		sprintf(chinese,"贵");
		break;
	case 58:
		sprintf(chinese,"粤");
		break;	
	case 59:
		sprintf(chinese,"陕");
		break;
	case 60:
		sprintf(chinese,"云");
		break;
	case 61:
		sprintf(chinese,"黑");
		break;
	case 62:
		sprintf(chinese,"晋");
		break;
	case 63:
		sprintf(chinese,"青");
		break;
	case 64:
		sprintf(chinese,"苏");
		break;

	}
	fp=fopen(".//Result//result.txt","w");
	fputs(chinese,fp);
	fclose(fp);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//对车牌的第二个字符进行分类,第二个字符必然是字符
	if((fp=fopen(".//Data//character_2.txt","r"))==NULL)
	{
		exit(0);
	}
	for(count=0;count<13;count++)//得到车牌字符的十三个特征值
	{
		fgets(featu,6,fp);
		feature[count]=atoi(featu);
	}
	for(count=10;count<34;count++)//从文本中读取30个模板字符的特征，并记录下和车牌字符最近的模板字符
	{
		sprintf(FileName,".//Data//%d.txt",count);
		if((fp=fopen(FileName,"r"))==NULL)
	    {
		    exit(0);
	    }
		for(c=0;c<13;c++)
	    {
		    fgets(featu,6,fp);
		    feaTemplate[c]=atoi(featu);
	    }
		//通过欧式距离计算，比较车牌字符更接近哪一个模板字符
		distance=0;
		for(c=0;c<13;c++)
		{
			distance+=sqrt((float)(feaTemplate[c]-feature[c])*(feaTemplate[c]-feature[c]));
		}
		if(distance<mindistance)
		{
			mindistance=distance;
			min=count;
		}
		if(count==17)
			break;
	}
	fp=fopen(".//Result//result.txt","a");
	switch(min)
	{
	case 10: C='C';break;
	case 11: C='G';break;
	case 12: C='H';break;
	case 13: C='J';break;
	case 14: C='L';break;
	case 15: C='Q';break;
	case 16: C='R';break;
	case 17: C='X';break;
	case 18: C='Z';break;
	case 19: C='P';break; 
	case 20: C='K';break;
	case 21: C='I';break;
	case 22: C='S';break;
	case 23: C='W';break;
	case 24: C='V';break;
	case 25: C='U';break;
	case 26: C='N';break;
	case 27: C='D';break;
	case 28: C='F';break;
	case 29: C='A';break;
	case 30: C='B';break;
	case 31: C='Y';break;
	case 32: C='M';break;
	case 33: C='E';break;
	}
	fputc(C,fp);
	chara=C;
	fclose(fp);
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	//第三、四、五个字符是数字或者字符
	int i;
	char names[30];
	for(i=3;i<=5;i++)
	{
		sprintf(names,".//Data//character_%d.txt",i);
		if((fp=fopen(names,"r"))==NULL)
	    {
		    exit(0);
	    }
		for(count=0;count<13;count++)//得到车牌字符的十三个特征值
	    {
		    fgets(featu,6,fp);
		    feature[count]=atoi(featu);
	    }
		for(count=0;count<34;count++)//从文本中读取30个模板字符的特征，并记录下和车牌字符最近的模板字符
	   {
		   sprintf(FileName,".//Data//%d.txt",count);
		   if((fp=fopen(FileName,"r"))==NULL)
	       {
		       exit(0);
	       }
		   for(c=0;c<13;c++)
	       {
		       fgets(featu,6,fp);
		       feaTemplate[c]=atoi(featu);
	       }
		   //通过欧式距离计算，比较车牌字符更接近哪一个模板字符
		   distance=0;
		   for(c=0;c<13;c++)
		   {
			   distance+=sqrt((float)(feaTemplate[c]-feature[c])*(feaTemplate[c]-feature[c]));
		   }
		   if(distance<mindistance)
		   {
			   mindistance=distance;
			   min=count;
		   }
	   }
	   fp=fopen(".//Result//result.txt","a");
	   switch(min)
	   {
	   case 0: C='0';break;
	   case 1: C='1';break;
	   case 2: C='2';break;
	   case 3: C='3';break;
	   case 4: C='4';break;
	   case 5: C='5';break;
	   case 6: C='6';break;
	   case 7: C='7';break;
	   case 8: C='8';break;
	   case 9: C='9';break;
	   case 10: C='C';break;
	   case 11: C='G';break;
	   case 12: C='H';break;
	   case 13: C='J';break;
	   case 14: C='L';break;
	   case 15: C='Q';break;
	   case 16: C='R';break;
	   case 17: C='X';break;
	   case 18: C='Z';break;
	   case 19: C='P';break; 
	   case 20: C='K';break;
	   case 21: C='I';break;
	   case 22: C='S';break;
	   case 23: C='W';break;
	   case 24: C='V';break;
	   case 25: C='U';break;
	   case 26: C='N';break;
	   case 27: C='D';break;
	   case 28: C='F';break;
	   case 29: C='A';break;
	   case 30: C='B';break;
	   case 31: C='Y';break;
	   case 32: C='M';break;
	   case 33: C='E';break;
	  }
	   fclose(fp);
	}
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	//第六、七个字符必然是数字
	for(i=6;i<8;i++)
	{
		sprintf(names,".//Data//character_%d.txt",i);
		if((fp=fopen(names,"r"))==NULL)
	    {
		    exit(0);
	    }
		for(count=0;count<13;count++)//得到车牌字符的十三个特征值
	    {
		    fgets(featu,6,fp);
		    feature[count]=atoi(featu);
	    }
		for(count=0;count<9;count++)//从文本中读取30个模板字符的特征，并记录下和车牌字符最近的模板字符
	   {
		   sprintf(FileName,".//Data//%d.txt",count);
		   if((fp=fopen(FileName,"r"))==NULL)
	       {
		       exit(0);
	       }
		   for(c=0;c<13;c++)
	       {
		       fgets(featu,6,fp);
		       feaTemplate[c]=atoi(featu);
	       }
		   //通过欧式距离计算，比较车牌字符更接近哪一个模板字符
		   distance=0;
		   for(c=0;c<13;c++)
		   {
			   distance+=sqrt((float)(feaTemplate[c]-feature[c])*(feaTemplate[c]-feature[c]));
		   }
		   if(distance<mindistance)
		   {
			   mindistance=distance;
			   min=count;
		   }
	   }
	   fp=fopen(".//Result//result.txt","a");
	   switch(min)
	   {
	   case 0: C='0';break;
	   case 1: C='1';break;
	   case 2: C='2';break;
	   case 3: C='3';break;
	   case 4: C='4';break;
	   case 5: C='5';break;
	   case 6: C='6';break;
	   case 7: C='7';break;
	   case 8: C='8';break;
	   case 9: C='9';break;
	  }
	   fclose(fp);
	}
	findChar(chara,str);
	fp=fopen(".//Result//result.txt","a");
	fputs(str,fp);
	fclose(fp);
	//重新打开result.txt文件，将车牌信息从中读取出来，并显示在窗口中
	if((fp=fopen(".//Result//result.txt","r"))==NULL)
	{
		exit(0);
	}
	fgets(result,10,fp);
	SetDlgItemText(IDC_RESULTEDIT,result);
	fclose(fp);
}



void CPlateDialg::getTemplateFeature()
{
	int i,j;                                        //循环变量
	bool find=false;
	CvScalar pb;                                    //用于获取图像的像素的对象
	char szName[30]={0};
	int ii;
	char temper[10]={0};
	for(ii=0;ii<65;ii++)
	{
	    sprintf(szName,".//template//%d.bmp",ii);
		sprintf(temper,"%d",ii);
		IplImage *src=cvLoadImage(szName,0);

	    int height=src->height;
	    int width=src->width;

	    int feature[13]={0};//用于提取模板字符的十三个特征
	    int Hline1,Hline2,Hline3;
	    int Wline;
	    int WLine1,WLine2;
	    int HLine1,HLine2;

	    //赋值
	    Hline1=(int)height/4;
	    Hline2=(int)height/2;
	    Hline3=(int)height*3/4;
	    Wline=(int)width/2;

	    WLine1=(int)width/3;
	    WLine2=(int)width*2/3;
	    HLine1=(int)height/3;
	    HLine2=(int)height*2/3;
	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)          //统计该字符所有白色像素的总值
		    for(j=0;j<src->width;j++)
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[0]++;
		    }
        //**********************************************************************************************
    
        //**********************************************************************************************
	    for(i=0;i<Hline1;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第一块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[1]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第二块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[2]++;
		    }
	    }
        //**********************************************************************************************


        //**********************************************************************************************
	    for(i=Hline1;i<Hline2;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第三块区域的白色像素总值
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[3]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第四块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[4]++;
		    }
	    }
        //**********************************************************************************************


	    //**********************************************************************************************
	    for(i=Hline2;i<Hline3;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第五块区域的白色像素总值
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[5]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第六块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[6]++;
		    }
	    }
	    //**********************************************************************************************


	    //**********************************************************************************************
	    for(i=Hline3;i<src->height;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第七块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[7]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第八块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[8]++;
		    }    
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//统计第九块区域的白色像素总值
	    {
		    j=WLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
		    	feature[9]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//统计第十块区域的白色像素总值
	    {
		    j=WLine2;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[10]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//统计第十一块区域的白色像素总值
	    {
		    i=HLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[11]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//统计第十二块区域的白色像素总值
	    {
		    i=HLine2;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[12]++;
	    }
	    //**********************************************************************************************
		FILE *fp;
		char fileName[40]={0};
		sprintf(fileName,".//Data//%d.txt",ii);
		char temp[10]={0};
		if((fp=fopen(fileName,"w"))==NULL)
		{
			exit(0);
		}
		for(i=0;i<13;i++)
		{
			sprintf(temp,"%d",feature[i]);
			fputs(temp,fp);
			fputs("\n",fp);
		}
		fclose(fp);
    }
}
void CPlateDialg::getCharaFeature(IplImage *src,int n)
{
	int i,j;                                        //循环变量
	    CvScalar pb;                                    //用于获取图像的像素的对象
	    int height=src->height;
	    int width=src->width;
	    int feature[13]={0};//用于提取模板字符的十三个特征

	    int Hline1,Hline2,Hline3;
	    int Wline;
	    int WLine1,WLine2;
	    int HLine1,HLine2;

	    //赋值
	    Hline1=(int)height/4;
	    Hline2=(int)height/2;
    	Hline3=(int)height*3/4;
	    Wline=(int)width/2;

	    WLine1=(int)width/3;
	    WLine2=(int)width*2/3;
	    HLine1=(int)height/3;
	    HLine2=(int)height*2/3;
	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)          //统计该字符所有白色像素的总值
		    for(j=0;j<src->width;j++)
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[0]++;
		    }
        //**********************************************************************************************
    
        //**********************************************************************************************
	    for(i=0;i<Hline1;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第一块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[1]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第二块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[2]++;
		    }
	    }
        //**********************************************************************************************


        //**********************************************************************************************
	    for(i=Hline1;i<Hline2;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第三块区域的白色像素总值
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[3]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第四块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[4]++;
		    }
	    }
        //**********************************************************************************************


	    //**********************************************************************************************
	    for(i=Hline2;i<Hline3;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第五块区域的白色像素总值
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[5]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第六块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[6]++;
		    }
	    }
	    //**********************************************************************************************


	    //**********************************************************************************************
	    for(i=Hline3;i<src->height;i++)
	    {
		    for(j=0;j<Wline;j++)//统计第七块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[7]++;
		    }
		    for(j=Wline;j<src->width;j++)//统计第八块区域的白色像素总值
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[8]++;
		    }    
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//统计第九块区域的白色像素总值
	    {
		    j=WLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
		    	feature[9]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//统计第十块区域的白色像素总值
	    {
		    j=WLine2;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[10]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//统计第十一块区域的白色像素总值
	    {
		    i=HLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[11]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//统计第十二块区域的白色像素总值
	    {
		    i=HLine2;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[12]++;
	    }
	    //**********************************************************************************************
		FILE *fp;
		char fileName[40]={0};
		sprintf(fileName,".//Data//character_%d.txt",n);
		char temp[10]={0};
		if((fp=fopen(fileName,"w"))==NULL)
		{
			exit(0);
		}
		for(i=0;i<13;i++)
		{
			sprintf(temp,"%d",feature[i]);
			fputs(temp,fp);
			fputs("\n",fp);
		}
		fclose(fp);
}