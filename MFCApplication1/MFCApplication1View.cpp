
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//


#include "pch.h"
#include "framework.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CMFCApplication1View::OnFileOpen)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_32771, &CMFCApplication1View::HistCompute)
	ON_COMMAND(ID_32772, &CMFCApplication1View::HistEqualization)
	ON_COMMAND(ID_32773, &CMFCApplication1View::SmoothFiltering)
	ON_COMMAND(ID_32774, &CMFCApplication1View::GaussianFiltering)
	ON_COMMAND(ID_32775, &CMFCApplication1View::Sobel)
	ON_COMMAND(ID_32776, &CMFCApplication1View::Erosion)
	ON_COMMAND(ID_32777, &CMFCApplication1View::Dilation)
	ON_COMMAND(ID_32778, &CMFCApplication1View::Open)
	ON_COMMAND(ID_32779, &CMFCApplication1View::Close)
	ON_COMMAND(ID_BMP_32780, &CMFCApplication1View::OnBmp)
	ON_COMMAND(ID_32781, &CMFCApplication1View::MidValueFilter)
	// 车牌识别相关命令
	ON_COMMAND(ID_32782, &CMFCApplication1View::OnColorSegmentation)
	ON_COMMAND(ID_32783, &CMFCApplication1View::OnPlateExtraction)
	ON_COMMAND(ID_32784, &CMFCApplication1View::OnCharacterSegmentation)
	// 交通标志识别相关命令
	ON_COMMAND(ID_32787, &CMFCApplication1View::OnBlueSegmentation)
	ON_COMMAND(ID_32788, &CMFCApplication1View::OnRedSegmentation)
	ON_COMMAND(ID_32789, &CMFCApplication1View::OnConnectedFill)
	ON_COMMAND(ID_32791, &CMFCApplication1View::OnShapeDetection)
END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码
	image = 0;
	outImg = 0;
	findex = 0;

	histFlag = 0;

	eqlFlag = 0;
	eqlImg = 0;

	MeanFilterFlag= 0;//均值
	meanFilterImg= 0;

	gaussianFlag = 0;//高斯低通
	gaussianImg = 0;

	midFitlerFlag =0; //中值
	midValueImg = 0;

	sobelFlag = 0; //边缘
	sob_x = 0;
	sob_y = 0;
	outSobelx = 0;
	outSobely = 0;

	erosionFlag = 0; //腐蚀
	erosionImg = 0;

	dilationFlag = 0; //膨胀
	dilationImg = 0;

	openFlag = 0; //开运算
	openImg = 0;

	closeFlag = 0;//闭运算
	closeImg = 0;

	//车牌识别相关初始化
	colorSegmentFlag = 0;
	colorSegmentImg = 0;

	plateExtractFlag = 0;
	plateExtractImg = 0;
	plateWidth = 0;
	plateHeight = 0;

	charSegmentFlag = 0;
	charSegmentImg = 0;
	charWidth = 0;
	charHeight = 0;

	//交通标志识别相关初始化
	blueSegmentFlag = 0;
	blueSegmentImg = 0;

	redSegmentFlag = 0;
	redSegmentImg = 0;

	connectedFillFlag = 0;
	connectedFillImg = 0;

	shapeDetectFlag = 0;
	shapeDetectImg = 0;
}

CMFCApplication1View::~CMFCApplication1View()
{
	if (image)
		delete image;
	if (outImg)
		delete outImg;
	if (meanFilterImg)
		delete meanFilterImg;
	if (gaussianImg)
		delete gaussianImg;
	if (midValueImg)
		delete midValueImg;
	if (outSobelx)
		delete outSobelx;
	if (outSobely)
		delete outSobely;
	if (erosionImg)
		delete erosionImg;
	if (dilationImg)
		delete dilationImg;
	
	//释放车牌识别相关内存
	if (colorSegmentImg)
		delete colorSegmentImg;
	if (plateExtractImg)
		delete plateExtractImg;
	if (charSegmentImg)
		delete charSegmentImg;

	//释放交通标志识别相关内存
	if (blueSegmentImg)
		delete blueSegmentImg;
	if (redSegmentImg)
		delete redSegmentImg;
	if (connectedFillImg)
		delete connectedFillImg;
	if (shapeDetectImg)
		delete shapeDetectImg;
}

// CMFCApplication1View 绘图

void CMFCApplication1View::OnDraw(CDC* pDC/*pDC*/)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (image == 0)
		return;

	int i, j;
	BYTE gray;
	if (rawflag == 1 || flag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = image[i * width + j];
				//显示原图像
				pDC->SetPixel(j, i, RGB(gray, gray, gray));
			}
		}
	}

	if (bmpflag == 1)
	{
		BYTE r, g, b;
		for (i = 0; i < height; i++)
			for (j = 0; j < 3 * width; j = j + 3) 
			{
				b = rgbimg[i * 3 * width + j];
				g = rgbimg[i * 3 * width + j + 1];
				r = rgbimg[i * 3 * width + j + 2];
				//显示bmp彩色图像
				pDC->SetPixelV(j / 3, i, RGB(r, g, b));
			}
	}
	
	if (histFlag == 1)
	{
		//画图像直方图
		for (i = 0; i < 256; i++)
		{
			pDC->MoveTo(width + 30 + 4 * i, height + 50);
			pDC->LineTo(width + 30 + 4 * i, height + 50 - hist[i] / 6.);
		}

		histFlag = 0;
	}

	if (eqlFlag == 1)
	{
		//显示直方图均衡后图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = eqlImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		eqlFlag = 0;
	}

	if (MeanFilterFlag == 1)
	{
		//显示均值滤波后的图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = meanFilterImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		MeanFilterFlag = 0;
	}
	
	if (gaussianFlag == 1)
	{
		//显示高斯滤波后的图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = gaussianImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		gaussianFlag = 0;
	}

	if (midFitlerFlag == 1)
	{
		//显示中值滤波后的图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = midValueImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		midFitlerFlag = 0;
	}



	if (sobelFlag == 1)
	{
		//显示边缘图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = outSobelx[i * width + j];
				pDC->SetPixel(j + (width + 10), i, RGB(gray, gray, gray));

				gray = outSobely[i * width + j];
				pDC->SetPixel(j + (width + 10) * 2, i, RGB(gray, gray, gray));

			}
		}
		sobelFlag = 0;
	}

	if (erosionFlag == 1)
	{
		//显示腐蚀图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = erosionImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		erosionFlag = 0;
	}

	if (dilationFlag == 1)
	{
		//显示膨胀图像
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = dilationImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		dilationFlag = 0;
	}

	if (openFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = openImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		openFlag = 0;
	}

	if (closeFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = closeImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		closeFlag = 0;
	}

	// 显示颜色分割结果
	if (colorSegmentFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = colorSegmentImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
		// 不要重置flag，保持显示
		// colorSegmentFlag = 0;
	}

	// 显示车牌提取结果
	if (plateExtractFlag == 1 && plateWidth > 0 && plateHeight > 0)
	{
		// 在原图下方显示提取的车牌
		for (i = 0; i < plateHeight; i++)
		{
			for (j = 0; j < plateWidth; j++)
			{
				int idx = (i * plateWidth + j) * 3;
				BYTE b = plateExtractImg[idx];
				BYTE g = plateExtractImg[idx + 1];
				BYTE r = plateExtractImg[idx + 2];
				pDC->SetPixel(j, i + height + 20, RGB(r, g, b));
			}
		}
		// 不要重置flag，保持显示
		// plateExtractFlag = 0;
	}

	// 显示字符分割结果（灰度）
	if (charSegmentFlag == 1 && charSegmentImg && charWidth > 0 && charHeight > 0)
	{
		int startX = width + 20;
		int startY = height + 20;

		for (int y = 0; y < charHeight; y++)
		{
			for (int x = 0; x < charWidth; x++)
			{
				BYTE v = charSegmentImg[y * charWidth + x];
				pDC->SetPixel(startX + x, startY + y, RGB(v, v, v));
			}
		}
	}

	// ======================== 交通标志识别结果显示 ========================
	
	// 显示蓝色分割结果
	if (blueSegmentFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = blueSegmentImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
	}

	// 显示红色分割结果
	if (redSegmentFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = redSegmentImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
	}

	// 显示连通填充结果
	if (connectedFillFlag == 1)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray = connectedFillImg[i * width + j];
				pDC->SetPixel(width + 10 + j, i, RGB(gray, gray, gray));
			}
		}
	}

	// 显示形状检测结果（在原图上绘制红色边框）
	if (shapeDetectFlag == 1)
	{
		// 绘制边界框（直接在原图上，偏移量为0）
		DrawShapeResults(pDC, 0, 0);
	}

}

//点击打开文件的操作，调用下面的函数。获取图像路径，文件名
void CMFCApplication1View::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog MyFDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);

	MyFDlg.m_ofn.lpstrFilter = "(*.lst;*.raw;*.bmp)\0*.lst;*.raw;*.bmp\0";
	MyFDlg.m_ofn.lpstrInitialDir = "D:\\images";

	flag = 0;
	bmpflag = 0;
	rawflag = 0;

	// 重置车牌识别相关的标志和数据
	colorSegmentFlag = 0;
	plateExtractFlag = 0;
	charSegmentFlag = 0;
	plateWidth = 0;
	plateHeight = 0;
	charWidth = 0;
	charHeight = 0;
	
	if (colorSegmentImg)
	{
		delete colorSegmentImg;
		colorSegmentImg = 0;
	}
	if (plateExtractImg)
	{
		delete plateExtractImg;
		plateExtractImg = 0;
	}
	if (charSegmentImg)
	{
		delete charSegmentImg;
		charSegmentImg = 0;
	}

	// 重置交通标志识别相关的标志和数据
	blueSegmentFlag = 0;
	redSegmentFlag = 0;
	connectedFillFlag = 0;
	shapeDetectFlag = 0;
	detectedSigns.clear();

	if (blueSegmentImg)
	{
		delete blueSegmentImg;
		blueSegmentImg = 0;
	}
	if (redSegmentImg)
	{
		delete redSegmentImg;
		redSegmentImg = 0;
	}
	if (connectedFillImg)
	{
		delete connectedFillImg;
		connectedFillImg = 0;
	}
	if (shapeDetectImg)
	{
		delete shapeDetectImg;
		shapeDetectImg = 0;
	}

	if (MyFDlg.DoModal() == IDOK)
	{
		CString SFName;

		SFName = MyFDlg.GetPathName(); //full name with path

		char* fnstr;
		fnstr = SFName.GetBuffer(4); //read the name from string 

		FILE* fpLst;
		int n;

		int len;
		len = strlen(fnstr);
		if (!strcmp(fnstr + len - 3, "raw")) //single raw image显示单幅图像
		{
			rawflag = 1;
			fnum = 0;
			char* ptr;
			ptr = fnstr + len - 3;
			while (*ptr != '\\')
				ptr--;
			*ptr = 0;
			strcpy(directory, fnstr);//Directory

			fnames = new char[500];
			strcpy(fnames, ptr + 1); //image name
		}
		if (!strcmp(fnstr + len - 3, "lst")) //list file显示多幅图像
		{
			flag = 1;
			fpLst = fopen(fnstr, "rb");
			fscanf(fpLst, "%3d", &fnum);
			fscanf(fpLst, "%s", directory);//directory

			fnames = new char[fnum * 100];
			for (n = 0; n < fnum; n++)
				fscanf(fpLst, "%s", fnames + n * 100);// image names

			fclose(fpLst);
		}
		if (!strcmp(fnstr + len - 3, "bmp"))//显示bmp格式图像
		{
			bmpflag = 1;
			fnum = 0;
			char* ptr;
			ptr = fnstr + len - 3;
			while (*ptr != '\\')
				ptr--;
			*ptr = 0;
			strcpy(directory, fnstr);//Directory

			fnames = new char[500];
			strcpy(fnames, ptr + 1); //image name

			
		}

		//打开图像文件
		findex = 0;
		readImg(findex);
	}
}

void CMFCApplication1View::readImg(int findex)
{

	char fullName[120];
	sprintf(fullName, "%s\\%s", directory, fnames + findex * 100);

	FILE* fpImg;
	fpImg = fopen(fullName, "rb");
	if (fpImg == 0)
	{
		AfxMessageBox("Cannot open the list file", MB_OK, 0);
		return;
	}

	if (bmpflag == 1)//读取bmp格式图像
	{
		int bicount;
		BITMAPFILEHEADER bmpFHeader;
		BITMAPINFOHEADER bmiHeader;

		fread(&bmpFHeader, sizeof(BITMAPFILEHEADER), 1, fpImg);
		fread(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fpImg);

		width = bmiHeader.biWidth;
		height = bmiHeader.biHeight;
		bicount = bmiHeader.biBitCount;

		BYTE* rowBuff;
		int rowLen;

		rowLen = ((width * bicount) / 8 + 3) / 4 * 4;
		rowBuff = new BYTE[rowLen];

		if (bicount == 8)
		{
			RGBQUAD bmiColors[256];
			fread(bmiColors, sizeof(RGBQUAD), 256, fpImg);
			image = new BYTE[width * height];
			rgbimg = new BYTE[3 * width * height];
			for (int i = height - 1; i >= 0; i--)
			{
				fread(rowBuff, 1, rowLen, fpImg);
				memcpy(image + i * width, rowBuff, width);
			}
			for (int i = 0; i < height; i++)
				for (int j = 0; j < 3 * width; j = j + 3)
				{
					rgbimg[i * 3 * width + j] = bmiColors[image[i * width + j / 3]].rgbBlue;
					rgbimg[i * 3 * width + j + 1] = bmiColors[image[i * width + j / 3]].rgbGreen;
					rgbimg[i * 3 * width + j + 2] = bmiColors[image[i * width + j / 3]].rgbRed;
				}
		}
		if (bicount == 24)
		{
			image = new BYTE[width * height];
			rgbimg = new BYTE[rowLen * height];
			for (int i = height - 1; i >= 0; i--)
			{
				fread(rowBuff, 1, rowLen, fpImg);
				memcpy(rgbimg + i * 3 * width, rowBuff, 3 * width);
			}
		}
		delete rowBuff;
	}

	if (rawflag == 1 || flag == 1)
	{
		fread(&width, sizeof(int), 1, fpImg);
		fread(&height, sizeof(int), 1, fpImg);
		if (image)
			delete image;

		image = new BYTE[width * height];

		fread(image, sizeof(BYTE), width * height, fpImg);
	}

	fclose(fpImg);

	////////////////////////////////////////////////////////////
	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//计算图像直方图
void CMFCApplication1View::HistCompute()
{
	// TODO: 在此添加命令处理程序代码

	histCompute(image, width, height);
	histFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//直方图均衡
void CMFCApplication1View::HistEqualization()
{
	// TODO: 在此添加命令处理程序代码
	if (eqlImg)
		delete eqlImg;
	eqlImg = new BYTE[width * height]; //存储直方图均衡之后的图像
	hisEqualiz(image, width, height, eqlImg);//直方图均衡函数
	eqlFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//均值滤波
void CMFCApplication1View::SmoothFiltering()
{
	if (meanFilterImg)//均值滤波
		delete meanFilterImg;
	meanFilterImg = new BYTE[width * height];
	meanFilter(image, width, height, meanFilterImg);//均值滤波函数
	MeanFilterFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}

//高斯低通滤波
void CMFCApplication1View::GaussianFiltering()
{
	if (gaussianImg)
		delete gaussianImg;
	gaussianImg = new BYTE[width * height];
	gaussian(image, width, height, gaussianImg); //高斯低通滤波函数
	gaussianFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);

}

//中值滤波
void CMFCApplication1View::MidValueFilter()
{
	if (midValueImg)
		delete midValueImg;
	midValueImg = new BYTE[width * height];

	midFindFiltering(image, width, height, midValueImg);
	midFitlerFlag=1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Sobel()
{
	// TODO: 在此添加命令处理程序代码
	if (sob_x)
		delete sob_x;
	sob_x = new int[width * height];

	if (sob_y)
		delete sob_y;
	sob_y = new int[width * height];

	if (outSobelx)
		delete outSobelx;
	outSobelx = new BYTE[width * height];//存储图像水平边缘

	if (outSobely)
		delete outSobely;
	outSobely = new BYTE[width * height];//存储图像垂直边缘

	sobel(image, width, height, sob_x, sob_y);
	tranByte(sob_x, outSobelx, width, height);//将图像水平边缘值映射至0-255，用于显示
	tranByte(sob_y, outSobely, width, height);
	sobelFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Erosion()
{
	// TODO: 在此添加命令处理程序代码
	if (erosionImg)
		delete erosionImg;
	erosionImg = new BYTE[width * height];

	BYTE* temp;
	temp = new BYTE[width * height];

	memcpy(temp, image, sizeof(BYTE) * width * height);
	erosion(temp, width, height, erosionImg);

	delete temp;
	erosionFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Dilation()
{
	// TODO: 在此添加命令处理程序代码
	if (dilationImg)
		delete dilationImg;
	dilationImg = new BYTE[width * height];

	BYTE* temp;
	temp = new BYTE[width * height];

	memcpy(temp, image, sizeof(BYTE) * width * height);
	dilation(temp, width, height, dilationImg);

	delete temp;
	dilationFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Open()
{
	// TODO: 在此添加命令处理程序代码
	if (openImg)
		delete openImg;
	openImg = new BYTE[width * height];

	BYTE* tempI;
	tempI = new BYTE[width * height];
	BYTE* tempO;
	tempO = new BYTE[width * height];

	memcpy(tempI, image, sizeof(BYTE) * width * height);
	erosion(tempI, width, height, tempO);
	dilation(tempO, width, height, openImg);

	delete tempI;
	delete tempO;
	openFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


void CMFCApplication1View::Close()
{
	// TODO: 在此添加命令处理程序代码
	if (closeImg)
		delete closeImg;
	closeImg = new BYTE[width * height];

	BYTE* tempI;
	tempI = new BYTE[width * height];
	BYTE* tempO;
	tempO = new BYTE[width * height];

	memcpy(tempI, image, sizeof(BYTE) * width * height);
	dilation(tempI, width, height, tempO);
	erosion(tempO, width, height, closeImg);

	delete tempI;
	delete tempO;
	closeFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);
}


//////////////////////////////////   图像处理函数   ///////////////////////////////

void CMFCApplication1View::histCompute(BYTE*image, int width, int height)
{
	// 初始化直方图数组
	memset(hist, 0, sizeof(hist));

	// 统计每个灰度级的像素数量
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			hist[image[i * width + j]]++;
		}
	}
}

void CMFCApplication1View::hisEqualiz(BYTE* image, int w, int h, BYTE* outImg)
{
	// 计算直方图
	histCompute(image, w, h);

	// 计算累积直方图
	int cdf[256] = {0};
	cdf[0] = hist[0];
	for(int i = 1; i < 256; i++) {
		cdf[i] = cdf[i-1] + hist[i];
	}

	// 归一化
	int total_pixels = w * h;
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			outImg[i * w + j] = (BYTE)(cdf[image[i * w + j]] * 255.0 / total_pixels);
		}
	}
}


void CMFCApplication1View::meanFilter(BYTE* image, int width, int heigth, BYTE* outImg)
{
	// 3x3均值滤波
	for(int i = 1; i < heigth-1; i++) {
		for(int j = 1; j < width-1; j++) {
			int sum = 0;
			for(int m = -1; m <= 1; m++) {
				for(int n = -1; n <= 1; n++) {
					sum += image[(i+m)*width + (j+n)];
				}
			}
			outImg[i*width + j] = (BYTE)(sum / 9);
		}
	}

	// 处理边界
	for(int i = 0; i < heigth; i++) {
		outImg[i*width] = image[i*width];
		outImg[i*width + width-1] = image[i*width + width-1];
	}
	for(int j = 0; j < width; j++) {
		outImg[j] = image[j];
		outImg[(heigth-1)*width + j] = image[(heigth-1)*width + j];
	}
}


int CMFCApplication1View::convolution(int* operatr, BYTE* block)
{
	int sum = 0;
	for(int i = 0; i < 9; i++) {
		sum += operatr[i] * block[i];
	}
	return sum;

}

void CMFCApplication1View::gaussian(BYTE* image, int width, int heigth, BYTE* outImg)
{
	// 高斯滤波核 3x3
	double kernel[9] = {
		1.0/16, 2.0/16, 1.0/16,
		2.0/16, 4.0/16, 2.0/16,
		1.0/16, 2.0/16, 1.0/16
	};

	// 应用高斯滤波
	for(int i = 1; i < heigth-1; i++) {
		for(int j = 1; j < width-1; j++) {
			double sum = 0;
			for(int m = -1; m <= 1; m++) {
				for(int n = -1; n <= 1; n++) {
					sum += image[(i+m)*width + (j+n)] * kernel[(m+1)*3 + (n+1)];
				}
			}
			outImg[i*width + j] = (BYTE)sum;
		}
	}

	// 处理边界
	for(int i = 0; i < heigth; i++) {
		outImg[i*width] = image[i*width];
		outImg[i*width + width-1] = image[i*width + width-1];
	}
	for(int j = 0; j < width; j++) {
		outImg[j] = image[j];
		outImg[(heigth-1)*width + j] = image[(heigth-1)*width + j];
	}
}


void CMFCApplication1View::midFindFiltering(BYTE* image, int width, int heigth, BYTE* outImg)
{
	// 3x3中值滤波
	BYTE window[9];
	for(int i = 1; i < heigth-1; i++) {
		for(int j = 1; j < width-1; j++) {
			// 收集3x3窗口中的像素
			int k = 0;
			for(int m = -1; m <= 1; m++) {
				for(int n = -1; n <= 1; n++) {
					window[k++] = image[(i+m)*width + (j+n)];
				}
			}
			// 找到中值
			outImg[i*width + j] = MidValueFind(9, window);
		}
	}

	// 处理边界
	for(int i = 0; i < heigth; i++) {
		outImg[i*width] = image[i*width];
		outImg[i*width + width-1] = image[i*width + width-1];
	}
	for(int j = 0; j < width; j++) {
		outImg[j] = image[j];
		outImg[(heigth-1)*width + j] = image[(heigth-1)*width + j];
	}
}



int CMFCApplication1View::MidValueFind(int num, BYTE* d)
{
	int value;

	int i, j;
	int temp;
	for (i = 0; i < num - 1; i++)
		for (j = i + 1; j < num; j++)
		{
			if (d[i] < d[j])
			{
				temp = d[i];
				d[i] = d[j];
				d[j] = temp;
			}
		}

	return d[num / 2];

}


void CMFCApplication1View::sobel(BYTE* window, int wid, int hei, int* sob_x, int* sob_y)
{
    int so_x[9];//horizontal
    so_x[0] = -1;
    so_x[1] = 0;
    so_x[2] = 1;
    so_x[3] = -2;
    so_x[4] = 0;
    so_x[5] = 2;
    so_x[6] = -1;
    so_x[7] = 0;
    so_x[8] = 1;

    int so_y[9];
    so_y[0] = -1;
    so_y[1] = -2;
    so_y[2] = -1;
    so_y[3] = 0;
    so_y[4] = 0;
    so_y[5] = 0;
    so_y[6] = 1;
    so_y[7] = 2;
    so_y[8] = 1;

    // 计算边缘
    for(int i = 1; i < hei-1; i++)
    {
        for(int j = 1; j < wid-1; j++)
        {
            int gx = 0, gy = 0;
            for(int m = -1; m <= 1; m++)
            {
                for(int n = -1; n <= 1; n++)
                {
                    int pos = (i+m)*wid + (j+n);
                    int maskPos = (m+1)*3 + (n+1);
                    gx += window[pos] * so_x[maskPos];
                    gy += window[pos] * so_y[maskPos];
                }
            }
            sob_x[i*wid + j] = gx;
            sob_y[i*wid + j] = gy;
        }
    }
}

void CMFCApplication1View::erosion(BYTE* image, int w, int h, BYTE* outImg)
{
    // 3x3结构元素的腐蚀操作
    for(int i = 1; i < h-1; i++)
    {
        for(int j = 1; j < w-1; j++)
        {
            BYTE minVal = 255;
            for(int m = -1; m <= 1; m++)
            {
                for(int n = -1; n <= 1; n++)
                {
                    BYTE val = image[(i+m)*w + (j+n)];
                    if(val < minVal)
                        minVal = val;
                }
            }
            outImg[i*w + j] = minVal;
        }
    }
}

void CMFCApplication1View::dilation(BYTE* image, int w, int h, BYTE* outImg)
{
    // 3x3结构元素的膨胀操作
    for(int i = 1; i < h-1; i++)
    {
        for(int j = 1; j < w-1; j++)
        {
            BYTE maxVal = 0;
            for(int m = -1; m <= 1; m++)
            {
                for(int n = -1; n <= 1; n++)
                {
                    BYTE val = image[(i+m)*w + (j+n)];
                    if(val > maxVal)
                        maxVal = val;
                }
            }
            outImg[i*w + j] = maxVal;
        }
    }
}


/////////////// 功能函数 ///////////////////////////////////////////////////////
void CMFCApplication1View::tranByte(int* temp, BYTE* array, int w, int h)
{
	int dim;
	dim = w * h;

	double stm, stvar;
	stm = 120;
	stvar = 60;
	int i;

	double mean = 0;
	for (i = 0; i < dim; i++)
		mean += temp[i];
	mean /= dim;//mean value

	double varia = 0;
	double diff;
	for (i = 0; i < dim; i++)
	{
		diff = temp[i] - mean;
		varia += diff * diff;
	}
	varia /= dim;//varianve

	if (varia < 0.0001)
		return;
	double ratio;
	ratio = stvar / sqrt(varia);

	BYTE value;
	for (i = 0; i < dim; i++)
	{
		value = (BYTE)((temp[i] - mean) * ratio + stm);

		if (value >= 200)
			array[i] = 200;
		else if (value < 80)
			array[i] = 80;
		else
			array[i] = value;
	}
}


void CMFCApplication1View::OnBmp()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog MyFDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);

	MyFDlg.m_ofn.lpstrFilter = "(*.bmp)\0*.bmp\0";
	MyFDlg.m_ofn.lpstrInitialDir = "D:\\";

	bmpflag = 0;

	// 重置车牌识别相关的标志和数据
	colorSegmentFlag = 0;
	plateExtractFlag = 0;
	charSegmentFlag = 0;
	plateWidth = 0;
	plateHeight = 0;
	charWidth = 0;
	charHeight = 0;
	
	if (colorSegmentImg)
	{
		delete colorSegmentImg;
		colorSegmentImg = 0;
	}
	if (plateExtractImg)
	{
		delete plateExtractImg;
		plateExtractImg = 0;
	}
	if (charSegmentImg)
	{
		delete charSegmentImg;
		charSegmentImg = 0;
	}

	// 重置交通标志识别相关的标志和数据
	blueSegmentFlag = 0;
	redSegmentFlag = 0;
	connectedFillFlag = 0;
	shapeDetectFlag = 0;
	detectedSigns.clear();

	if (blueSegmentImg)
	{
		delete blueSegmentImg;
		blueSegmentImg = 0;
	}
	if (redSegmentImg)
	{
		delete redSegmentImg;
		redSegmentImg = 0;
	}
	if (connectedFillImg)
	{
		delete connectedFillImg;
		connectedFillImg = 0;
	}
	if (shapeDetectImg)
	{
		delete shapeDetectImg;
		shapeDetectImg = 0;
	}

	if (MyFDlg.DoModal() == IDOK)
	{
		CString SFName;

		SFName = MyFDlg.GetPathName(); //full name with path

		char* fnstr;
		fnstr = SFName.GetBuffer(4); //read the name from string 

		int len;
		len = strlen(fnstr);

		if (!strcmp(fnstr + len - 3, "bmp"))
		{
			bmpflag = 1;
			fnum = 0;
			char* ptr;
			ptr = fnstr + len - 3;
			while (*ptr != '\\')
				ptr--;
			*ptr = 0;
			strcpy(directory, fnstr);//Directory

			fnames = new char[500];
			strcpy(fnames, ptr + 1); //image name


		}
		findex = 0;
		readImg(findex);
	}
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 打印

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG



// 颜色分割 - 分离车身和车牌区域
void CMFCApplication1View::ColorSegmentation(BYTE* rgbImg, int width, int height, BYTE* outImg)
{
	// ============ 请在此实现颜色分割功能 ============
	// 功能说明：将RGB图像转换到HSV颜色空间，提取蓝色车牌区域，
	//          使用形态学操作去除噪声，输出二值化的车牌候选区域
	int imgSize = width * height;
	std::vector<float> H(imgSize), S(imgSize), V(imgSize);

	// ================= 1. RGB → HSV =================
	for (int i = 0; i < imgSize; i++)
	{
		float r = rgbImg[3 * i + 0] / 255.0f;
		float g = rgbImg[3 * i + 1] / 255.0f;
		float b = rgbImg[3 * i + 2] / 255.0f;

		float maxv = max(r, max(g, b));
		float minv = min(r, min(g, b));
		float delta = maxv - minv;

		float h = 0;
		if (delta > 0)
		{
			if (maxv == r)      h = 60 * fmod((g - b) / delta, 6);
			else if (maxv == g) h = 60 * (((b - r) / delta) + 2);
			else                h = 60 * (((r - g) / delta) + 4);
		}
		if (h < 0) h += 360;

		float s = (maxv == 0) ? 0 : delta / maxv;
		float v = maxv;

		H[i] = h;
		S[i] = s;
		V[i] = v;
	}

	// ================= 2. 蓝色车牌颜色判定（增强版） =================
	for (int i = 0; i < imgSize; i++)
	{
		BYTE r = rgbImg[3 * i + 0];
		BYTE g = rgbImg[3 * i + 1];
		BYTE b = rgbImg[3 * i + 2];

		// 蓝色 HSV 范围
		bool hsvBlue =
			(H[i] >= 190 && H[i] <= 250) &&    // Hue 200°附近
			S[i] >= 0.30f &&
			V[i] >= 0.25f;

		// RGB 强约束
		bool rgbBlue =
			(b > r + 10) &&
			(b > g + 10) &&
			(b > 80);

		outImg[i] = (hsvBlue && rgbBlue) ? 255 : 0;
	}

	// ================= 3. 形态学：使用你自己的腐蚀/膨胀 =================

	// 创建临时图像缓存
	std::vector<BYTE> temp(imgSize);

	// --- 腐蚀（使用你已有的函数） ---
	erosion(outImg, width, height, temp.data());

	// --- 膨胀（使用你已有的函数） ---
	dilation(temp.data(), width, height, outImg);

}

void CMFCApplication1View::OnColorSegmentation()
{
	// ============ 请在此实现颜色分割菜单响应功能 ============
	// 功能说明：检查是否已加载图像，调用ColorSegmentation函数处理，更新显示
	// 检查是否有彩色图像数据（程序中 bmpflag==1 时会填充 rgbimg）
	if (bmpflag != 1 || rgbimg == 0 || width <= 0 || height <= 0)
	{
		AfxMessageBox(_T("请先打开一幅彩色 BMP 图像 (bmpflag==1)。"));
		return;
	}

	// 释放旧的分割结果（如果存在）
	if (colorSegmentImg)
	{
		delete[] colorSegmentImg;
		colorSegmentImg = 0;
	}

	// 分配输出二值图像缓冲区（每像素一个字节，0 or 255）
	colorSegmentImg = new BYTE[width * height];
	memset(colorSegmentImg, 0, width * height);

	// 有些 bmp 的 rgbimg 存储为 B G R 顺序，ColorSegmentation 期望 R G B。
	// 因此先把 rgbimg 转成标准的 R G B 缓冲区（连续 3*width*height，RGB 顺序）。
	BYTE* tmpRGB = new BYTE[3 * width * height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// 源数组在读取 bmp 时是按行存放，索引为 i*3*width + j (j 为 0,1,2,3...)
			int srcIdx = y * 3 * width + x * 3;
			int dstIdx = (y * width + x) * 3;

			// 源：B G R  -> 目标：R G B
			BYTE b = rgbimg[srcIdx + 0];
			BYTE g = rgbimg[srcIdx + 1];
			BYTE r = rgbimg[srcIdx + 2];

			tmpRGB[dstIdx + 0] = r;
			tmpRGB[dstIdx + 1] = g;
			tmpRGB[dstIdx + 2] = b;
		}
	}

	// 调用颜色分割函数（将会在 colorSegmentImg 中写入 0/255 二值掩膜）
	ColorSegmentation(tmpRGB, width, height, colorSegmentImg);

	// 释放临时 RGB 缓冲区
	delete[] tmpRGB;

	// 设置标志以便 OnDraw 显示分割结果，并刷新窗口
	colorSegmentFlag = 1;

	OnInitialUpdate();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	InvalidateRect(&ClientRect);

	AfxMessageBox(_T("颜色分割完成。"));
	
}

// 车牌提取 - 定位和提取车牌区域
void CMFCApplication1View::PlateExtraction(BYTE* segmentImg, int width, int height, BYTE* outImg)
{
	// ============ 请在此实现车牌提取功能 ============
	// 功能说明：对颜色分割后的二值图像进行形态学处理，
	//          通过连通区域分析定位车牌位置，提取车牌区域并输出RGB图像
	// ===================================================
	if (!segmentImg || !rgbimg || !outImg || width <= 0 || height <= 0)
		return;

	int size = width * height;

	// 1) 形态学闭运算（使用已有的 erosion/dilation）
	BYTE* tmp1 = new BYTE[size];
	BYTE* tmp2 = new BYTE[size];
	for (int i = 0; i < size; ++i)
		tmp1[i] = (segmentImg[i] > 128) ? 255 : 0;

	// 做两次开闭（先膨胀再腐蚀）增强连通性
	dilation(tmp1, width, height, tmp2);
	erosion(tmp2, width, height, tmp1);
	dilation(tmp1, width, height, tmp2);
	erosion(tmp2, width, height, tmp1);
	// 结果在 tmp1

	// 2) 连通域标记（BFS）
	int* labels = new int[size];
	memset(labels, 0, sizeof(int) * size);
	int* queue = new int[size];
	int qh, qt;
	int curLabel = 1;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int idx = y * width + x;
			if (tmp1[idx] == 255 && labels[idx] == 0) {
				qh = 0; qt = 0;
				queue[qt++] = idx;
				labels[idx] = curLabel;
				while (qh < qt) {
					int cur = queue[qh++];
					int cy = cur / width;
					int cx = cur % width;
					for (int yy = -1; yy <= 1; ++yy) {
						int ny = cy + yy;
						if (ny < 0 || ny >= height) continue;
						for (int xx = -1; xx <= 1; ++xx) {
							int nx = cx + xx;
							if (nx < 0 || nx >= width) continue;
							int nidx = ny * width + nx;
							if (tmp1[nidx] == 255 && labels[nidx] == 0) {
								labels[nidx] = curLabel;
								queue[qt++] = nidx;
							}
						}
					}
				}
				curLabel++;
			}
		}
	}

	int numLabels = curLabel - 1;
	if (numLabels <= 0) {
		delete[] tmp1; delete[] tmp2; delete[] labels; delete[] queue;
		return;
	}

	// 3) 计算每个连通区域边界框与面积
	int* minX = new int[numLabels + 1];
	int* maxX = new int[numLabels + 1];
	int* minY = new int[numLabels + 1];
	int* maxY = new int[numLabels + 1];
	int* area = new int[numLabels + 1];
	for (int i = 1; i <= numLabels; ++i) {
		minX[i] = width; maxX[i] = 0;
		minY[i] = height; maxY[i] = 0;
		area[i] = 0;
	}
	for (int i = 0; i < size; ++i) {
		int lb = labels[i];
		if (lb > 0) {
			int y = i / width;
			int x = i % width;
			if (x < minX[lb]) minX[lb] = x;
			if (x > maxX[lb]) maxX[lb] = x;
			if (y < minY[lb]) minY[lb] = y;
			if (y > maxY[lb]) maxY[lb] = y;
			area[lb]++;
		}
	}

	// 4) 区域评分，选择最佳车牌候选
	int bestLabel = -1;
	double bestScore = 0.0;
	for (int i = 1; i <= numLabels; ++i) {
		int w = maxX[i] - minX[i] + 1;
		int h = maxY[i] - minY[i] + 1;
		if (w <= 4 || h <= 4) continue;
		double aspect = (double)w / (double)h;
		if (aspect < 2.0 || aspect > 7.0) continue;

		double fillRate = (double)area[i] / (double)(w * h);
		if (area[i] < (width * height) / 2000) continue;

		double ratioScore = 1.0 - fabs(aspect - 4.0) / 4.0;
		if (ratioScore < 0.0) ratioScore = 0.0;

		double fillScore = fillRate;
		double score = (double)area[i] * (0.6 * ratioScore + 0.4 * fillScore);

		if (score > bestScore) {
			bestScore = score;
			bestLabel = i;
		}
	}

	// 5) 提取车牌区域
	if (bestLabel > 0) {
		int bx1 = minX[bestLabel];
		int by1 = minY[bestLabel];
		int bx2 = maxX[bestLabel];
		int by2 = maxY[bestLabel];
		int plateW = bx2 - bx1 + 1;
		int plateH = by2 - by1 + 1;

		int pad = 4;
		int sx = max(bx1 - pad, 0);
		int sy = max(by1 - pad, 0);
		int ex = min(bx2 + pad, width - 1);
		int ey = min(by2 + pad, height - 1);
		int outW = ex - sx + 1;
		int outH = ey - sy + 1;

		for (int y = 0; y < outH; ++y) {
			for (int x = 0; x < outW; ++x) {
				int srcX = sx + x;
				int srcY = sy + y;
				int srcIdx = (srcY * width + srcX) * 3;
				int dstIdx = (y * outW + x) * 3;
				outImg[dstIdx + 0] = rgbimg[srcIdx + 0];
				outImg[dstIdx + 1] = rgbimg[srcIdx + 1];
				outImg[dstIdx + 2] = rgbimg[srcIdx + 2];
			}
		}

		plateWidth = outW;
		plateHeight = outH;
		plateExtractFlag = 1;
	}
	else {
		plateWidth = 0;
		plateHeight = 0;
		plateExtractFlag = 0;
	}

	// 释放内存
	delete[] tmp1;
	delete[] tmp2;
	delete[] labels;
	delete[] queue;
	delete[] minX; delete[] maxX; delete[] minY; delete[] maxY; delete[] area;

	

}

void CMFCApplication1View::OnPlateExtraction()
{
	// ============ 请在此实现车牌提取菜单响应功能 ============
	// 功能说明：检查是否已完成颜色分割，调用PlateExtraction函数处理，更新显示
	// 1) 检查是否已有颜色分割结果
	if (colorSegmentImg == 0 || width <= 0 || height <= 0)
	{
		AfxMessageBox(_T("请先进行颜色分割！"));
		return;
	}

	// 2) 删除旧结果
	if (plateExtractImg)
	{
		delete[] plateExtractImg;
		plateExtractImg = 0;
	}

	// 3) 分配 RGB 缓冲区（与原图一样大）
	int imgSize = width * height * 3;
	plateExtractImg = new BYTE[imgSize];
	if (!plateExtractImg)
	{
		AfxMessageBox(_T("内存分配失败！"));
		return;
	}
	memset(plateExtractImg, 0, imgSize);

	// 4) 调用车牌提取函数
	PlateExtraction(colorSegmentImg, width, height, plateExtractImg);

	// 5) 判断是否检测到车牌
	if (plateExtractFlag == 0 || plateWidth <= 0 || plateHeight <= 0)
	{
		delete[] plateExtractImg;
		plateExtractImg = 0;
		plateWidth = 0;
		plateHeight = 0;

		AfxMessageBox(_T("未检测到车牌区域，请调整参数或检查颜色分割效果。"));
		return;
	}

	// 6) 缩减缓冲区只保留车牌尺寸，避免后续处理读取无效数据
	int packedSize = plateWidth * plateHeight * 3;
	BYTE* packed = new BYTE[packedSize];
	memcpy(packed, plateExtractImg, packedSize);
	delete[] plateExtractImg;
	plateExtractImg = packed;

	// 8) 刷新显示
	OnInitialUpdate();
	CRect rc;
	GetClientRect(&rc);
	InvalidateRect(&rc);

	AfxMessageBox(_T("车牌提取完成。"));
	
	
	
}

// 字符分割 - 提取蓝底白字的车牌字符
void CMFCApplication1View::CharacterSegmentation(BYTE* plateImg, int width, int height, BYTE* outImg)
{
	// ============ 优化后的字符分割功能 ============
	// 功能说明：对车牌图像进行垂直投影分析，分割出单个字符，
	//          每个字符用黑色背景间隔显示，便于后续字符识别
	// 清空输出
	memset(outImg, 0, width * height);

	// 计算垂直投影
	std::vector<int> projection(width, 0);
	int maxProjection = 0;
	int sumProjection = 0;
	for (int x = 0; x < width; x++)
	{
		int sum = 0;
		for (int y = 0; y < height; y++)
		{
			if (plateImg[y * width + x] > 128)  // 白色像素计数
				sum++;
		}
		projection[x] = sum;
		if (sum > maxProjection)
			maxProjection = sum;
		sumProjection += sum;
	}

	// 自适应阈值计算 - 更加准确地确定字符区域
	int threshold;
	// 如果最大投影值较大，使用相对阈值
	if (maxProjection > height / 4)
	{
		threshold = max(maxProjection / 4, height / 10); // 使用最大值的1/4或高度的1/10
	}
	else
	{
		// 否则使用较低的阈值，避免漏检
		threshold = max(height / 12, 3); // 至少3个像素
	}
	
	// 平滑投影曲线，减少噪声影响
	std::vector<int> smoothedProjection = projection;
	for (int x = 1; x < width - 1; x++)
	{
		smoothedProjection[x] = (projection[x-1] + projection[x] + projection[x+1]) / 3;
	}

	// 开始扫描字符区域
	std::vector<std::pair<int, int>> regions;  // (start, end)
	bool inRegion = false;
	int startX = 0;

	for (int x = 0; x < width; x++)
	{
		if (!inRegion && smoothedProjection[x] > threshold)
		{
			inRegion = true;
			startX = x;
		}
		else if (inRegion)
		{
			// 改进：允许中间有短暂的低投影值，避免字符被分割
			bool isRegionEnd = true;
			// 检查接下来的几个像素是否仍有字符
			int checkLength = min(3, width - x - 1);
			for (int i = 0; i <= checkLength; i++)
			{
				if (x + i < width && smoothedProjection[x + i] > threshold / 2)
				{
					isRegionEnd = false;
					break;
				}
			}
			
			if (isRegionEnd || x == width - 1)
			{
				inRegion = false;
				int endX = (x == width - 1) ? x : x - 1;
				regions.push_back(std::make_pair(startX, endX));
			}
		}
	}

	if (regions.empty())
	{
		// 如果没有检测到字符，尝试使用更低的阈值重新检测
		threshold = max(height / 15, 2);
		regions.clear();
		inRegion = false;
		for (int x = 0; x < width; x++)
		{
			if (!inRegion && projection[x] > threshold)
			{
				inRegion = true;
				startX = x;
			}
			else if (inRegion && projection[x] <= threshold)
			{
				inRegion = false;
				regions.push_back(std::make_pair(startX, x - 1));
			}
		}
		if (inRegion)
		{
			regions.push_back(std::make_pair(startX, width - 1));
		}
	}

	// 过滤小区域噪声
	std::vector<std::pair<int, int>> validRegions;
	for (const auto& region : regions)
	{
		int charW = region.second - region.first + 1;
		// 根据车牌字符特点，过滤过窄或过宽的区域
		if (charW >= 3 && charW < width / 3) // 宽度至少3像素，不超过图像1/3
		{
			validRegions.push_back(region);
		}
	}

	// 如果有效区域过少，可能是阈值问题，尝试恢复一些被过滤的区域
	if (validRegions.size() < 4) // 车牌至少应有4个字符
	{
		for (const auto& region : regions)
		{
			bool alreadyAdded = false;
			for (const auto& valid : validRegions)
			{
				if (region.first == valid.first && region.second == valid.second)
				{
					alreadyAdded = true;
					break;
				}
			}
			if (!alreadyAdded && (region.second - region.first + 1) >= 2)
			{
				validRegions.push_back(region);
			}
		}
	}

	// 将有效区域按x坐标排序
	std::sort(validRegions.begin(), validRegions.end());

	if (validRegions.empty())
	{
		// 最后尝试：如果还是没有检测到字符，直接使用整个图像作为一个区域
		// 这样至少能显示一些内容，方便调试
		validRegions.push_back(std::make_pair(0, width - 1));
	}

	// 将切割出的字符绘制到 outImg
	int writeX = 0; // 写入位置
	int maxCharWidth = (width - (validRegions.size() - 1) * 2) / validRegions.size();

	for (size_t k = 0; k < validRegions.size(); k++)
	{
		int sx = validRegions[k].first;
		int ex = validRegions[k].second;

		int charW = ex - sx + 1;
		// 限制字符宽度，避免单个字符占用过多空间
		if (charW > maxCharWidth)
		{
			charW = maxCharWidth;
			sx = (validRegions[k].first + validRegions[k].second - charW + 1) / 2; // 居中裁剪
		}

		// 将字符拷贝到输出
		for (int x = 0; x < charW && (writeX + x < width); x++)
		{
			int srcX = sx + x;
			if (srcX < width)
			{
				for (int y = 0; y < height; y++)
				{
					outImg[y * width + (writeX + x)] = plateImg[y * width + srcX];
				}
			}
		}

		writeX += charW;

		// 字符间留2-3像素黑间隔
		int gapWidth = 2;
		if (writeX + gapWidth < width && k < validRegions.size() - 1)
		{
			// 填充间隔为黑色
			for (int x = 0; x < gapWidth && (writeX + x < width); x++)
			{
				for (int y = 0; y < height; y++)
				{
					outImg[y * width + (writeX + x)] = 0;
				}
			}
			writeX += gapWidth;
		}
		else
		{
			break;
		}
	}
	
	// 如果outImg仍然全黑，手动复制原始图像的一部分，确保有输出
	bool hasContent = false;
	for (int i = 0; i < width * height && !hasContent; i++)
	{
		if (outImg[i] > 0)
		{
			hasContent = true;
			break;
		}
	}
	
	if (!hasContent)
	{
		// 复制原始图像的中心部分
		int centerStart = (width - width / 2) / 2;
		int centerEnd = centerStart + width / 2;
		for (int x = centerStart; x < centerEnd && x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int outX = x - centerStart;
				if (outX < width)
				{
					outImg[y * width + outX] = plateImg[y * width + x];
				}
			}
		}
	}
}

void CMFCApplication1View::OnCharacterSegmentation()
{
	// ============ 请在此实现字符分割菜单响应功能 ============
	// 功能说明：检查是否已提取车牌，调用CharacterSegmentation函数处理，更新显示
	// 1) 检查是否已经完成车牌提取
	if (plateExtractFlag == 0 ||
		plateExtractImg == nullptr ||
		plateWidth <= 0 || plateHeight <= 0)
	{
		AfxMessageBox(_T("请先提取车牌！"));
		return;
	}

	// 2) 删除旧的字符分割结果
	if (charSegmentImg)
	{
		delete[] charSegmentImg;
		charSegmentImg = nullptr;
	}

	// 3) 分配输出图内存（保持与车牌区域大小一致）
	charSegmentImg = new BYTE[plateWidth * plateHeight];
	if (!charSegmentImg)
	{
		AfxMessageBox(_T("字符分割内存分配失败！"));
		return;
	}
	memset(charSegmentImg, 0, plateWidth * plateHeight);

	// 4) 将 plateExtractImg 的 RGB 转成灰度，用于 CharacterSegmentation
	BYTE* plateGray = new BYTE[plateWidth * plateHeight];
	for (int i = 0; i < plateWidth * plateHeight; i++)
	{
		BYTE r = plateExtractImg[i * 3 + 0];
		BYTE g = plateExtractImg[i * 3 + 1];
		BYTE b = plateExtractImg[i * 3 + 2];
		plateGray[i] = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b);
	}

	// 5) 对灰度图进行二值化预处理，提高字符分割效果
	BYTE* plateBinary = new BYTE[plateWidth * plateHeight];
	int threshold = 128; // 初始阈值
	// 自动计算阈值（Otsu's方法简化版）
	int histogram[256] = {0};
	for (int i = 0; i < plateWidth * plateHeight; i++)
	{
		histogram[plateGray[i]]++;
	}
	// 简单计算中间值作为阈值
	int totalPixels = plateWidth * plateHeight;
	int sumPixels = 0;
	for (int i = 0; i < 256; i++)
	{
		sumPixels += histogram[i];
		if (sumPixels > totalPixels / 2)
		{
			threshold = i;
			break;
		}
	}
	// 二值化
	for (int i = 0; i < plateWidth * plateHeight; i++)
	{
		plateBinary[i] = (plateGray[i] > threshold) ? 255 : 0;
	}

	// 6) 调用字符分割主功能
	CharacterSegmentation(
		plateBinary,
		plateWidth,
		plateHeight,
		charSegmentImg
	);

	delete[] plateGray;
	delete[] plateBinary;

	// 7) 设置显示标志和尺寸
	charSegmentFlag = 1; // 设置标志以便OnDraw显示
	charWidth = plateWidth; // 使用车牌宽度作为字符显示宽度
	charHeight = plateHeight; // 使用车牌高度作为字符显示高度

	// 8) 刷新窗口显示
	OnInitialUpdate();
	CRect rc;
	GetClientRect(&rc);
	InvalidateRect(&rc);

	AfxMessageBox(_T("字符分割完成！"));
}

void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_NEXT)
	{
		if (findex < fnum)
		{
			findex++;
			readImg(findex);
		}
	}
	if (nChar == VK_PRIOR)
	{
		if (findex > 0)
		{
			findex--;
			readImg(findex);
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// ======================== 交通标志识别功能实现 ========================

// RGB转HSV辅助函数
void CMFCApplication1View::RGB2HSV(BYTE r, BYTE g, BYTE b, double& h, double& s, double& v)
{
	double dr = r / 255.0;
	double dg = g / 255.0;
	double db = b / 255.0;

	double maxVal = max(max(dr, dg), db);
	double minVal = min(min(dr, dg), db);
	double delta = maxVal - minVal;

	// 计算V (明度)
	v = maxVal;

	// 计算S (饱和度)
	if (maxVal == 0)
		s = 0;
	else
		s = delta / maxVal;

	// 计算H (色调)
	if (delta == 0)
		h = 0;
	else if (maxVal == dr)
		h = 60 * fmod((dg - db) / delta, 6.0);
	else if (maxVal == dg)
		h = 60 * ((db - dr) / delta + 2);
	else
		h = 60 * ((dr - dg) / delta + 4);

	if (h < 0)
		h += 360;
}

//  蓝色分割功能（参考车牌识别的颜色分割实现）
void CMFCApplication1View::BlueSegmentation(BYTE* rgbImg, int width, int height, BYTE* outImg)
{
	// ============ 请在此实现蓝色分割功能 ============
	// 功能说明：将RGB图像转换到HSV颜色空间，提取蓝色交通标志区域(H: 200-260°)，
	//          使用形态学开闭运算去除噪声，输出二值化的蓝色区域掩膜
	
	
}

// 菜单响应 - 蓝色分割
void CMFCApplication1View::OnBlueSegmentation()
{
	// ============ 请在此实现蓝色分割菜单响应功能 ============
	// 功能说明：检查是否已加载彩色图像，调用BlueSegmentation函数处理，更新显示
	
	
}

//  红色分割功能（参考车牌识别的颜色分割实现）
void CMFCApplication1View::RedSegmentation(BYTE* rgbImg, int width, int height, BYTE* outImg)
{
	// ============ 请在此实现红色分割功能 ============
	// 功能说明：将RGB图像转换到HSV颜色空间，提取红色交通标志区域(H: [0°,20°]∪[330°,360°])，
	//          使用形态学开闭运算去除噪声，输出二值化的红色区域掩膜
	
	
}

// 菜单响应 - 红色分割
void CMFCApplication1View::OnRedSegmentation()
{
	// ============ 请在此实现红色分割菜单响应功能 ============
	// 功能说明：检查是否已加载彩色图像，调用RedSegmentation函数处理，更新显示
	
	
}

//  连通填充功能 - 8-连通区域标记算法
int CMFCApplication1View::ConnectedComponentLabeling(BYTE* binImg, int width, int height, int* labels)
{
	// 初始化标签矩阵为0（背景）
	memset(labels, 0, width * height * sizeof(int));
	
	int currentLabel = 0;
	
	// 第一遍扫描：为每个前景像素分配临时标签
	std::vector<int> equivalenceTable;
	equivalenceTable.push_back(0); // 标签0对应背景，不参与等价类
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// 如果当前像素是前景（白色）
			if (binImg[y * width + x] == 255) {
				// 获取8邻域像素的标签
				std::vector<int> neighborLabels;
				
				// 检查上方4个邻域
				if (y > 0) {
					if (labels[(y - 1) * width + x] > 0) // 正上方
						neighborLabels.push_back(labels[(y - 1) * width + x]);
					if (x > 0 && labels[(y - 1) * width + (x - 1)] > 0) // 左上角
						neighborLabels.push_back(labels[(y - 1) * width + (x - 1)]);
					if (x < width - 1 && labels[(y - 1) * width + (x + 1)] > 0) // 右上角
						neighborLabels.push_back(labels[(y - 1) * width + (x + 1)]);
				}
				
				// 检查左侧邻域
				if (x > 0 && labels[y * width + (x - 1)] > 0) // 正左方
					neighborLabels.push_back(labels[y * width + (x - 1)]);
				
				// 检查左下方邻域
				if (y < height - 1 && x > 0 && labels[(y + 1) * width + (x - 1)] > 0)
					neighborLabels.push_back(labels[(y + 1) * width + (x - 1)]);
				
				// 去除重复标签
				sort(neighborLabels.begin(), neighborLabels.end());
				neighborLabels.erase(unique(neighborLabels.begin(), neighborLabels.end()), neighborLabels.end());
				
				if (neighborLabels.empty()) {
					// 没有相邻标签，创建新标签
					currentLabel++;
					labels[y * width + x] = currentLabel;
					equivalenceTable.push_back(currentLabel); // 初始化等价类为自己
				} else {
					// 有相邻标签，取最小的标签
					int minLabel = neighborLabels[0];
					labels[y * width + x] = minLabel;
					
					// 记录等价关系
					for (size_t i = 1; i < neighborLabels.size(); i++) {
						int currLabel = neighborLabels[i];
						// 路径压缩的等价类合并
						while (equivalenceTable[currLabel] != currLabel && equivalenceTable[currLabel] > minLabel) {
							int nextLabel = equivalenceTable[currLabel];
							equivalenceTable[currLabel] = minLabel;
							currLabel = nextLabel;
						}
						// 更新等价表，确保所有标签都指向最小等价标签
						if (equivalenceTable[currLabel] > minLabel) {
							equivalenceTable[currLabel] = minLabel;
						}
					}
				}
			}
		}
	}
	
	// 路径压缩 - 确保所有等价类都指向其最小代表
	for (int i = 1; i <= currentLabel; i++) {
		int root = i;
		while (equivalenceTable[root] != root) {
			root = equivalenceTable[root];
		}
		// 压缩路径
		int j = i;
		while (equivalenceTable[j] != root) {
			int next = equivalenceTable[j];
			equivalenceTable[j] = root;
			j = next;
		}
	}
	
	// 第二遍扫描：重新编号标签，去除等价类，使用连续编号
	int newLabel = 0;
	std::vector<int> labelMap(currentLabel + 1, 0);
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (labels[y * width + x] > 0) {
				int root = equivalenceTable[labels[y * width + x]];
				if (labelMap[root] == 0) {
					newLabel++;
					labelMap[root] = newLabel;
				}
				labels[y * width + x] = labelMap[root];
			}
		}
	}
	
	return newLabel; // 返回连通区域数量
}

// 连通区域填充与去噪
void CMFCApplication1View::ConnectedComponentFill(BYTE* binImg, int width, int height, BYTE* outImg)
{
	// ============ 请在此实现连通填充功能 ============
	// 功能说明：对二值图像进行8-连通区域标记，统计各区域面积，
	//          删除小面积噪声区域(面积<200像素)，使用形态学闭运算填充内部空洞
	
	
}

// 菜单响应 - 连通填充
void CMFCApplication1View::OnConnectedFill()
{
	// ============ 请在此实现连通填充菜单响应功能 ============
	// 功能说明：检查是否已完成蓝色或红色分割，调用ConnectedComponentFill函数处理，更新显示
	
	
}

//  形状检测功能

// 计算Hu不变矩
void CMFCApplication1View::ComputeHuMoments(int* labels, int label, int width, int height, double* huMoments)
{
	// ============ 请在此实现Hu不变矩计算功能 ============
	// 功能说明：计算指定标签区域的7个Hu不变矩，用于形状识别
	//          Hu不变矩对平移、旋转、缩放具有不变性
	
	// TODO: 在此实现Hu不变矩计算
	// 提示：计算图像矩(m00, m10, m01, m20, m11, m02等)
	//      计算中心矩，归一化后计算7个Hu不变矩
}

// 计算区域特征（改进版：增加严格的筛选条件）
void CMFCApplication1View::ComputeRegionFeatures(int* labels, int labelNum, int width, int height, std::vector<TrafficSignRegion>& regions)
{
	// ============ 请在此实现区域特征计算功能 ============
	// 功能说明：计算每个连通区域的几何特征，包括面积、周长、边界框、圆度、矩形度等
	//          筛选出符合条件的候选区域，调用ComputeHuMoments计算Hu不变矩
	
	// TODO: 在此实现区域特征计算
	// 提示：遍历所有标签，计算每个区域的几何特征
	//      根据面积、宽高比等条件进行初步筛选
	//      调用ClassifyShape进行形状分类
}

// 分类形状（去掉占空比限制，主要依靠圆度和宽高比）
void CMFCApplication1View::ClassifyShape(TrafficSignRegion& region)
{
	// ============ 请在此实现形状分类功能 ============
	// 功能说明：根据区域的几何特征(圆度、矩形度、宽高比)和Hu不变矩，
	//          判断区域形状类型(圆形、正方形、矩形、三角形、不规则)
	
	// TODO: 在此实现形状分类算法
	// 提示：根据圆度判断圆形，根据矩形度和宽高比判断正方形和矩形
	//      根据占空比和圆度判断三角形
	//      设置region.shapeType为对应的形状类型
}

// 形状检测主函数
void CMFCApplication1View::ShapeDetection(BYTE* binImg, int width, int height, BYTE* outImg)
{
	// ============ 请在此实现形状检测功能 ============
	// 功能说明：对二值图像进行连通区域标记，计算每个区域的几何特征(面积、周长、圆度、矩形度)，
	//          根据特征参数分类形状(圆形、正方形、矩形、三角形)，输出检测结果
	
	// TODO: 在此实现形状检测算法
	// 提示：调用ConnectedComponentLabeling进行区域标记
	//      调用ComputeRegionFeatures计算特征
	//      调用ClassifyShape进行形状分类
}

// 绘制形状检测结果（绘制绿色边框，内部透明无填充）
void CMFCApplication1View::DrawShapeResults(CDC* pDC, int offsetX, int offsetY)
{
	// ============ 请在此实现形状检测结果绘制功能 ============
	// 功能说明：在原图上绘制检测到的交通标志的绿色边界框（3像素宽度，内部透明）
	
	// TODO: 在此实现绘制功能
	// 提示：遍历detectedSigns向量，为每个检测到的标志绘制绿色矩形框
	//      使用CPen创建绿色画笔，使用NULL_BRUSH设置透明填充
}

// 菜单响应 - 形状检测
void CMFCApplication1View::OnShapeDetection()
{
	// ============ 请在此实现形状检测菜单响应功能 ============
	// 功能说明：检查是否已完成颜色分割或连通填充，调用ShapeDetection函数处理，
	//          在原图上绘制绿色边界框标识检测到的交通标志
	
	// TODO: 在此实现菜单响应
	// 提示：检查connectedFillImg、blueSegmentImg或redSegmentImg是否存在
	//      调用ShapeDetection进行形状检测
	//      设置shapeDetectFlag标志并刷新显示
}
