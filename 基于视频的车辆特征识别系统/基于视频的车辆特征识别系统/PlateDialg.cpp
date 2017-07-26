// PlateDialg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlateDialg.h"
#include "afxdialogex.h"
#include "pxvatls.h"

// CPlateDialg �Ի���

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


// CPlateDialg ��Ϣ�������

void CPlateDialg::showImg()
{//�˺���������picture control����ʾͼƬ
	CDC* pDC=GetDlgItem(IDD_CAR_PLATE)->GetDC();				// �����ʾ�ؼ��� DC
    HDC hDC = pDC ->GetSafeHdc();									// ��ȡ HDC(�豸���) �����л�ͼ����

    CRect rect;
	GetDlgItem(IDD_CAR_PLATE)->GetClientRect(&rect);
    int rw = rect.right - rect.left;								// ���ͼƬ�ؼ��Ŀ�͸�
    int rh = rect.bottom - rect.top;
	IplImage *reSizeImg=cvCreateImage(cvSize(rw,rh),CurImg->depth,CurImg->nChannels);
	cvResize(CurImg,reSizeImg);
	SetRect(rect,0,0,rw,rh );
	CvvImage cvvImage;
	cvvImage.CopyOf(reSizeImg);										// ��ֵ������ͼ���Աָ��
    cvvImage.DrawToHDC(hDC,&rect );										// ���ø����ͼ��������ͼƬ
	cvReleaseImage(&reSizeImg);
    ReleaseDC(pDC);
}
void CPlateDialg::PostNcDestroy()
{
	delete this;
}
void CPlateDialg::OnBnClickedOpenimage()//��ͼ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilter; 
	CString FileName;
	CSimpleArray<GUID> aguidFileTypes; 
	HRESULT hResult;// ��ȡCImage֧�ֵ�ͼ���ļ��Ĺ����ַ���
	hResult = m_Image.GetExporterFilterString(strFilter,aguidFileTypes, _T( "All Image Files") );
	 if (FAILED(hResult)) 
	{ 
		MessageBox(_T("GetExporterFilter����ʧ�ܣ�"));
		return; 
	}
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if(IDOK != dlg.DoModal()) 
		return;
	m_Image.Destroy(); // ���ⲿͼ���ļ�װ�ص�CImage������
	hResult = m_Image.Load(dlg.GetPathName());
	if (FAILED(hResult)) 
	{
		MessageBox(_T("����ͼ���ļ�ʧ�ܣ�")); 
		return;
	}
	// ���������ڱ��������� 
	CString str; 
	int i;
	str.LoadString(AFX_IDS_APP_TITLE);
	AfxGetMainWnd()->SetWindowText(str + _T(" - ") +dlg.GetPathName());
	FileName=dlg.GetPathName();
	Model=0;
	//Invalidate(); // ǿ�Ƶ���OnDraw
	CurImg=cvLoadImage(FileName,1);
	showImg();
}


void CPlateDialg::OnBnClickedPlateFind()//���ƶ�λ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	//===========���ƶ�λģ��=============
	//1.  �ҶȻ�ͼƬ
	//2.  Sobel���ؼ��
	//3.  ��ֵ������
	//4.  ������������
	//5.  ˮƽͶӰ
	//6.  ��ֱͶӰ
	//#################################################################################################
	int i,j;                                        //ѭ������
	int pixel;                                  //���ر�������¼ͼ��ÿ�����ص�ֵ
	bool find=false;
	CvScalar pb,pa;                                    //���ڻ�ȡͼ������صĶ���
	IplImage *src=CurImg;//srcָ������ָ��ԭͼƬ
	IplImage *OprImg;
	//1.�ҶȻ�ͼƬ
	IplImage *gray=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	cvCvtColor(src,gray,CV_BGR2GRAY);
	OprImg=gray;
	//////////////////////////////////////////////////////////////////////////////

	//2.Sobel ��Ե���
	IplImage *sobel=NULL;
	sobel=cvCreateImage(cvGetSize(gray),IPL_DEPTH_16S,1);
    cvSobel(gray,sobel,1,0,3);
    IplImage *temper=cvCreateImage(cvGetSize(sobel),IPL_DEPTH_8U,1);
    cvConvertScaleAbs(sobel,temper,1,0);
	OprImg=temper;
	
	//*****************************************************************************************************************
    //3.��ֵ������
	IplImage *bi = cvCreateImage(cvGetSize(OprImg),IPL_DEPTH_8U,1);
    cvThreshold(OprImg,bi, 0,255,   CV_THRESH_OTSU | CV_THRESH_BINARY);
    OprImg=bi;
	//*****************************************************************************************************************

	////////////////////////////////////////////////////////////////
	//4.������������
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

	//======================���ƶ�λģ�� ,��ģ����Ҫʹ��ͶӰ�����в���
	//1.  ˮƽͶӰ
	//2.  ��ֱͶӰ
	//####################################################################################################
	//1.ˮƽͶӰ
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
	//2.��ֱͶӰ
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


void CPlateDialg::OnBnClickedPlateDiv()//�ַ��ָ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//===========================����ͼ��Ԥ����ģ��
	//1.  ��ͼƬ��С��Ϊ����ͼƬ��С
	//2.  �ҶȻ�����ֵ������
	//3.  Canny ���ؼ��
	//4.  Hough �任
	//5.  ����任
	//6.  ȥ�����Ʊ߿�
	//7.  ͨ����ֱͶӰ���ҵ�ÿ�������ַ������ұ���
	//8.  ����ÿ���ַ������ұ��أ����ַ�����ָ����ÿһ���ַ����������´���һ��ͼƬ����
	//####################################################################################################
	int i,j;                                        //ѭ������
	int pixel;                                  //���ر�������¼ͼ��ÿ�����ص�ֵ
	bool find=false;
	CvScalar pb,pa;                                    //���ڻ�ȡͼ������صĶ���
	IplImage *src=CurImg;//srcָ������ָ��ԭͼƬ
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
	//================================�ҶȻ�����==================================
	IplImage *linshi=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	cvCvtColor(image,linshi,CV_RGB2GRAY);
	//================================��ֵ������===================================
    cvThreshold(linshi,linshi, 0,255,   CV_THRESH_OTSU | CV_THRESH_BINARY);
	src=linshi;
	//==================================Canny ���==================================
	IplImage *canny=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	cvCanny(src,canny,50,150,3);
	//==================================Hough �任==================================
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
	//=========================================����任==========================
	IplImage * Img_tmp=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	float m[6]; 
	theta=4;
	CvMat M = cvMat( 2, 3, CV_32F, m );//һ��2*3�ľ���
	CvPoint2D32f center;//�������
	center.x=float (src->width/2.0+0.5);//Ѱ�ҷ�ת������
	center.y=float (src->height/2.0+0.5);   
	cv2DRotationMatrix( center, theta,1, &M);
	cvWarpAffine(src,Img_tmp, &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );
	src=Img_tmp;
	//===========================================ȥ�����±߿�=====================
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
	//==================����ȥ��===============
	cvSmooth(src,src,CV_MEDIAN);

	//###################################################################################################
	int l[8]={0};             //��¼ÿһ���ַ�����߽�
	int r[8]={0};             //��¼ÿһ���ַ����ұ߽�
	int pixelSum[2]={0};
	int Count[8]={0};
	a=0,b=0;
	int count=0;
	int charaWidth;         //ÿһ���ַ��Ŀ�ȱ���һ��
	int width;             //ÿ���ַ��Ŀ�Ⱥ�����ȵĲ�ֵ
	for(j=0;j<src->width;j++)//ͨ��ˮƽͶӰ����ÿһ���ַ��ָ��
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

	//ͳһ�涨ÿ���ַ��Ŀ��Ϊ�����ַ��п�������ַ����
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
	//�ڶ�ֵ����ͼƬ��������һ���ַ���λ��
	//��һ���ַ����ĸ��������Ϊ��A(l[0],img_bottom),B(r[0],img_bottom),C(l[0],img_top),D(r[0],img_top)
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
	//�ڶ����ַ����ĸ��������Ϊ��A(l[1]-width,img_bottom),B(r[1]+width,img_bottom),C(l[1]-width,img_top),D(r[1]+width,img_top)
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
	//�������ַ����ĸ��������Ϊ��A(l[2]-width,img_bottom),B(r[2]+width,img_bottom),C(l[2]-width,img_top),D(r[2]+width,img_top)
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
	//���ĸ��ַ����ĸ��������Ϊ��A(l[3]-width,img_bottom),B(r[3]+width,img_bottom),C(l[3]-width,img_top),D(r[3]+width,img_top)
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
	//������ַ����ĸ��������Ϊ��A(l[4]-width,img_bottom),B(r[4]+width,img_bottom),C(l[4]-width,img_top),D(r[4]+width,img_top)
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
	//�������ַ����ĸ��������Ϊ��A(l[5]-width,img_bottom),B(r[5]+width,img_bottom),C(l[5]-width,img_top),D(r[5]+width,img_top)
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
	//���߸��ַ����ĸ��������Ϊ��A(l[6]-width,img_bottom),B(r[6]+width,img_bottom),C(l[6]-width,img_top),D(r[6]+width,img_top)
	cvLine(Img_tmp,cvPoint(l[6]-width,img_bottom),cvPoint(r[6]+width,img_bottom),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[6]+width,img_bottom),cvPoint(r[6]+width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(r[6]+width,img_top),cvPoint(l[6]-width,img_top),cvScalar(255,255,255),1,8,0);
	cvLine(Img_tmp,cvPoint(l[6]-width,img_top),cvPoint(l[6]-width,img_bottom),cvScalar(255,255,255),1,8,0);
	//******************************************************************************************************
	CurImg=Img_tmp;
	showImg();
	OnBnClickedResult();
}


void CPlateDialg::OnBnClickedResult()//�����ַ�ʶ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ַ�ʶ��ģ�飬��ģ����Ҫ�����¼�������
	//1.���Ѿ��ָ�ĳ����ַ����й�һ��
	//2.��ģ���ַ���ѧϰ����ģ���ַ���������ȡ����������¼���ı��У�
	//3.�Թ�һ�����ַ�����������ȡ
	//4.ͨ�����������������ַ���������ͬ���ַ�ģ�����У��Ӷ�ʶ��������ַ�
	//#################################################################################################################
	
	//1.����ȡ�ĳ��ƽ������ţ������ƴ�С���ŵ�ģ��Ĵ�С
	IplImage *dst_chara1,*dst_chara2,*dst_chara3,*dst_chara4,*dst_chara5,*dst_chara6,*dst_chara7;

	int templat_height=40;
	int templat_width=20;

	CvSize dst_cvsize;
	dst_cvsize.height=templat_height;
	dst_cvsize.width=templat_width;
     //��ÿ�������ַ����á��ٽ���ֵ�������з���
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
	//2.��ȡģ������
	//��ģ��������ȡ����������ļ��У���һ����Ҫ��ȡ���Ժ�ֻ��Ҫ���ļ���ֱ�Ӷ�ȡ���,�ʶ���Ҫ���ļ����д���һ����־�ļ�
	//���б�ģ�������Ƿ��Ѿ���ȡ��
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
	//3.��ȡ�����ַ���������Ҳ�洢��Data�ļ�����
	getCharaFeature(dst_chara1,1);
	getCharaFeature(dst_chara2,2);
	getCharaFeature(dst_chara3,3);
	getCharaFeature(dst_chara4,4);
	getCharaFeature(dst_chara5,5);
	getCharaFeature(dst_chara6,6);
	getCharaFeature(dst_chara7,7);

	//4.���������������������ļ��ж�ȡ�����������Ӧ��ģ������һһ�ȶ�
	//�ȱȽϵ�һ���ַ�������
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���ļ��ж�ȡ���ӵ�ʮ��������ֵ
	int feature[13]={0};//���ڴ洢���ӵ�ʮ��������ֵ������
	char featu[10]={0};
	int feaTemplate[13]={0};
	char FileName[30]={0};
	int c,count;
	long mindistance=99999999;                //���ڼ�¼ÿ�μ�������ַ���ģ���ַ�����С����
	long distance;
	int min=-1;                     //���ڼ�¼��һ��ģ���ַ��ͳ����ַ���� 
	char C;
	char result[10]={0};
	char str[10]={0};
	if((fp=fopen(".//Data//character_1.txt","r"))==NULL)
	{
		exit(0);
	}
	for(count=0;count<13;count++)//�õ������ַ���ʮ��������ֵ
	{
		fgets(featu,6,fp);
		feature[count]=atoi(featu);
	}

	for(count=34;count<65;count++)//���ı��ж�ȡ30��ģ���ַ�������������¼�ºͳ����ַ������ģ���ַ�
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
		//ͨ��ŷʽ�����ƽ�����㣬�Ƚϳ����ַ����ӽ���һ��ģ���ַ�
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
		sprintf(chinese,"��");
		break;
	case 35:
		sprintf(chinese,"��");
		break;
	case 36:
		sprintf(chinese,"��");
		break;
	case 37:
		sprintf(chinese,"��");
		break;
	case 38:
		sprintf(chinese,"��");
		break;
	case 39:
		sprintf(chinese,"��");
		break;
	case 40:
		sprintf(chinese,"ԥ");
		break;
	case 41:
		sprintf(chinese,"��");
		break;
	case 42:
		sprintf(chinese,"��");
		break;
	case 43:
		sprintf(chinese,"��");
		break;
	case 44:
		sprintf(chinese,"��");
		break;
	case 45:
		sprintf(chinese,"��");
		break;
	case 46:
		sprintf(chinese,"��");
		break;
	case 47:
		sprintf(chinese,"��");
		break;
	case 48:
		sprintf(chinese,"��");
		break;
	case 49:
		sprintf(chinese,"��");
		break;
	case 50:
		sprintf(chinese,"��");
		break;
	case 51:
		sprintf(chinese,"��");
		break;
	case 52:
		sprintf(chinese,"��");
		break;
	case 53:
		sprintf(chinese,"³");
		break;
	case 54:
		sprintf(chinese,"��");
		break;
	case 55:
		sprintf(chinese,"��");
		break;
	case 56:
		sprintf(chinese,"��");
		break;
	case 57:
		sprintf(chinese,"��");
		break;
	case 58:
		sprintf(chinese,"��");
		break;	
	case 59:
		sprintf(chinese,"��");
		break;
	case 60:
		sprintf(chinese,"��");
		break;
	case 61:
		sprintf(chinese,"��");
		break;
	case 62:
		sprintf(chinese,"��");
		break;
	case 63:
		sprintf(chinese,"��");
		break;
	case 64:
		sprintf(chinese,"��");
		break;

	}
	fp=fopen(".//Result//result.txt","w");
	fputs(chinese,fp);
	fclose(fp);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�Գ��Ƶĵڶ����ַ����з���,�ڶ����ַ���Ȼ���ַ�
	if((fp=fopen(".//Data//character_2.txt","r"))==NULL)
	{
		exit(0);
	}
	for(count=0;count<13;count++)//�õ������ַ���ʮ��������ֵ
	{
		fgets(featu,6,fp);
		feature[count]=atoi(featu);
	}
	for(count=10;count<34;count++)//���ı��ж�ȡ30��ģ���ַ�������������¼�ºͳ����ַ������ģ���ַ�
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
		//ͨ��ŷʽ������㣬�Ƚϳ����ַ����ӽ���һ��ģ���ַ�
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
	fclose(fp);
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
	//�������ġ�����ַ������ֻ����ַ�
	int i;
	char names[30];
	for(i=3;i<=5;i++)
	{
		sprintf(names,".//Data//character_%d.txt",i);
		if((fp=fopen(names,"r"))==NULL)
	    {
		    exit(0);
	    }
		for(count=0;count<13;count++)//�õ������ַ���ʮ��������ֵ
	    {
		    fgets(featu,6,fp);
		    feature[count]=atoi(featu);
	    }
		for(count=0;count<34;count++)//���ı��ж�ȡ30��ģ���ַ�������������¼�ºͳ����ַ������ģ���ַ�
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
		   //ͨ��ŷʽ������㣬�Ƚϳ����ַ����ӽ���һ��ģ���ַ�
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
	//�������߸��ַ���Ȼ������
	for(i=6;i<8;i++)
	{
		sprintf(names,".//Data//character_%d.txt",i);
		if((fp=fopen(names,"r"))==NULL)
	    {
		    exit(0);
	    }
		for(count=0;count<13;count++)//�õ������ַ���ʮ��������ֵ
	    {
		    fgets(featu,6,fp);
		    feature[count]=atoi(featu);
	    }
		for(count=0;count<9;count++)//���ı��ж�ȡ30��ģ���ַ�������������¼�ºͳ����ַ������ģ���ַ�
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
		   //ͨ��ŷʽ������㣬�Ƚϳ����ַ����ӽ���һ��ģ���ַ�
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
	//���´�result.txt�ļ�����������Ϣ���ж�ȡ����������ʾ�ڴ�����
	if((fp=fopen(".//Result//result.txt","r"))==NULL)
	{
		exit(0);
	}
	fgets(result,10,fp);
	SetDlgItemText(IDC_RESULTEDIT,result);
	fclose(fp);
	/*
	IplImage *dst_chara1,*dst_chara2,*dst_chara3,*dst_chara4,*dst_chara5,*dst_chara6,*dst_chara7;
	*/
	cvReleaseImage(&dst_chara1);
	cvReleaseImage(&dst_chara2);
	cvReleaseImage(&dst_chara3);
	cvReleaseImage(&dst_chara4);
	cvReleaseImage(&dst_chara5);
	cvReleaseImage(&dst_chara6);
	cvReleaseImage(&dst_chara7);

}



void CPlateDialg::getTemplateFeature()
{
	int i,j;                                        //ѭ������
	bool find=false;
	CvScalar pb;                                    //���ڻ�ȡͼ������صĶ���
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

	    int feature[13]={0};//������ȡģ���ַ���ʮ��������
	    int Hline1,Hline2,Hline3;
	    int Wline;
	    int WLine1,WLine2;
	    int HLine1,HLine2;

	    //��ֵ
	    Hline1=(int)height/4;
	    Hline2=(int)height/2;
	    Hline3=(int)height*3/4;
	    Wline=(int)width/2;

	    WLine1=(int)width/3;
	    WLine2=(int)width*2/3;
	    HLine1=(int)height/3;
	    HLine2=(int)height*2/3;
	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)          //ͳ�Ƹ��ַ����а�ɫ���ص���ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ�һ������İ�ɫ������ֵ
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[1]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶڶ�������İ�ɫ������ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ���������İ�ɫ������ֵ
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[3]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶ��Ŀ�����İ�ɫ������ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ��������İ�ɫ������ֵ
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[5]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶ���������İ�ɫ������ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ��߿�����İ�ɫ������ֵ
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[7]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶڰ˿�����İ�ɫ������ֵ
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[8]++;
		    }    
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//ͳ�Ƶھſ�����İ�ɫ������ֵ
	    {
		    j=WLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
		    	feature[9]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//ͳ�Ƶ�ʮ������İ�ɫ������ֵ
	    {
		    j=WLine2;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[10]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//ͳ�Ƶ�ʮһ������İ�ɫ������ֵ
	    {
		    i=HLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[11]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//ͳ�Ƶ�ʮ��������İ�ɫ������ֵ
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
	int i,j;                                        //ѭ������
	    CvScalar pb;                                    //���ڻ�ȡͼ������صĶ���
	    int height=src->height;
	    int width=src->width;
	    int feature[13]={0};//������ȡģ���ַ���ʮ��������

	    int Hline1,Hline2,Hline3;
	    int Wline;
	    int WLine1,WLine2;
	    int HLine1,HLine2;

	    //��ֵ
	    Hline1=(int)height/4;
	    Hline2=(int)height/2;
    	Hline3=(int)height*3/4;
	    Wline=(int)width/2;

	    WLine1=(int)width/3;
	    WLine2=(int)width*2/3;
	    HLine1=(int)height/3;
	    HLine2=(int)height*2/3;
	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)          //ͳ�Ƹ��ַ����а�ɫ���ص���ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ�һ������İ�ɫ������ֵ
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[1]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶڶ�������İ�ɫ������ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ���������İ�ɫ������ֵ
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[3]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶ��Ŀ�����İ�ɫ������ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ��������İ�ɫ������ֵ
		    {
		    	pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
			    	feature[5]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶ���������İ�ɫ������ֵ
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
		    for(j=0;j<Wline;j++)//ͳ�Ƶ��߿�����İ�ɫ������ֵ
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[7]++;
		    }
		    for(j=Wline;j<src->width;j++)//ͳ�Ƶڰ˿�����İ�ɫ������ֵ
		    {
			    pb=cvGet2D(src,i,j);
			    if(pb.val[0]>0)
				    feature[8]++;
		    }    
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//ͳ�Ƶھſ�����İ�ɫ������ֵ
	    {
		    j=WLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
		    	feature[9]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(i=0;i<src->height;i++)//ͳ�Ƶ�ʮ������İ�ɫ������ֵ
	    {
		    j=WLine2;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[10]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//ͳ�Ƶ�ʮһ������İ�ɫ������ֵ
	    {
		    i=HLine1;
		    pb=cvGet2D(src,i,j);
		    if(pb.val[0]>0)
			    feature[11]++;
	    }
	    //**********************************************************************************************

	    //**********************************************************************************************
	    for(j=0;j<src->width;j++)//ͳ�Ƶ�ʮ��������İ�ɫ������ֵ
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