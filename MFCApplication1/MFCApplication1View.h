
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

class CMFCApplication1View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

// 特性
public:
	CMFCApplication1Doc* GetDocument() const;

// 操作
public:

	BYTE * image;
	int width, height;

	int findex, fnum;
	int bmpflag, rawflag, flag;
	char* fnames, directory[120];

	void readImg(int);

	BYTE* outImg;
	BYTE* rgbimg;

	
	// 计算图像直方图
	int histFlag;
	int hist[256]; //存储图像直方图，256灰度级
	void histCompute(BYTE*, int, int);//计算图像直方图函数


	//直方图均衡
	int eqlFlag;
	BYTE* eqlImg;  //存储直方图均衡后的图像
	void hisEqualiz(BYTE*, int, int, BYTE*);//直方图均衡函数
	
	//均值滤波
	int MeanFilterFlag;
	BYTE* meanFilterImg; // 存储均值滤波后的图像
	void meanFilter(BYTE*, int, int, BYTE*); // 均值滤波器
	int convolution(int* operatr, BYTE* block); //卷积函数
	
	//高斯滤波
 	int gaussianFlag;
	BYTE* gaussianImg; // 存储高斯滤波后的图像
	void gaussian(BYTE*, int, int, BYTE*);// 高斯滤波器

	//中值滤波
	int midFitlerFlag;
	BYTE* midValueImg;
	void midFindFiltering(BYTE*, int, int, BYTE*);
	int MidValueFind(int,BYTE*);

	//边缘提取
	int sobelFlag;
	int* sob_x, * sob_y;
	BYTE* outSobelx, * outSobely;
	void sobel(BYTE*, int, int, int*, int*);
	void tranByte(int*, BYTE*, int, int);

	//形态学处理
	int erosionFlag;
	BYTE* erosionImg;
	void erosion(BYTE*, int, int, BYTE*);

	int dilationFlag;
	BYTE* dilationImg;
	void dilation(BYTE*, int, int, BYTE*);

	int openFlag;
	BYTE* openImg;

	int closeFlag;
	BYTE* closeImg;

	//车牌识别相关
	//颜色分割
	int colorSegmentFlag;
	BYTE* colorSegmentImg;
	void ColorSegmentation(BYTE* rgbImg, int width, int height, BYTE* outImg);

	//车牌提取
	int plateExtractFlag;
	BYTE* plateExtractImg;
	int plateWidth, plateHeight; // 存储车牌的实际尺寸
	void PlateExtraction(BYTE* segmentImg, int width, int height, BYTE* outImg);
	int plateGrayFlag;
	BYTE* plateGrayImg;
	bool RunColorSegmentation(bool showMessage);
	bool RunPlateExtraction(bool showMessage);
	bool RunCharacterSegmentation(bool showMessage);

	//字符分割
	int charSegmentFlag;
	BYTE* charSegmentImg;
	int charWidth, charHeight; // 存储字符区域的实际尺寸
	void CharacterSegmentation(BYTE* plateImg, int width, int height, BYTE* outImg);
	int ComputeOtsuThreshold(const int histogram[256], int totalPixels);


	// ======================== 交通标志识别相关 ========================
	// 蓝色分割
	int blueSegmentFlag;
	BYTE* blueSegmentImg;
	void BlueSegmentation(BYTE* rgbImg, int width, int height, BYTE* outImg); // 蓝色区域分割
	void RGB2HSV(BYTE r, BYTE g, BYTE b, double& h, double& s, double& v); // RGB转HSV

	// 红色分割
	int redSegmentFlag;
	BYTE* redSegmentImg;
	void RedSegmentation(BYTE* rgbImg, int width, int height, BYTE* outImg); // 红色区域分割

	// 连通填充
	int connectedFillFlag;
	BYTE* connectedFillImg;
	void ConnectedComponentFill(BYTE* binImg, int width, int height, BYTE* outImg); // 连通区域填充
	int ConnectedComponentLabeling(BYTE* binImg, int width, int height, int* labels); // 8-连通区域标记

	// 形状检测
	int shapeDetectFlag;
	BYTE* shapeDetectImg;
	struct TrafficSignRegion {
		int label;          // 区域标签
		int minX, maxX;     // 边界框
		int minY, maxY;
		int area;           // 面积
		int perimeter;      // 周长
		double circularity; // 圆度
		double rectangularity; // 矩形度
		double huMoments[7]; // Hu不变矩
		CString shapeType;  // 形状类型：圆形、三角形、矩形等
	};
	std::vector<TrafficSignRegion> detectedSigns; // 检测到的标志
	void ShapeDetection(BYTE* binImg, int width, int height, BYTE* outImg); // 形状检测
	void ComputeRegionFeatures(int* labels, int labelNum, int width, int height, std::vector<TrafficSignRegion>& regions); // 计算区域特征
	void ClassifyShape(TrafficSignRegion& region); // 分类形状
	void ComputeHuMoments(int* labels, int label, int width, int height, double* huMoments); // 计算Hu不变矩
	void DrawShapeResults(CDC* pDC, int offsetX, int offsetY); // 绘制形状检测结果

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void HistCompute();
	afx_msg void HistEqualization();
	afx_msg void SmoothFiltering();
	afx_msg void GaussianFiltering();
	afx_msg void Sobel();
	afx_msg void Erosion();
	afx_msg void Dilation();
	afx_msg void Open();
	afx_msg void Close();
	afx_msg void OnBmp();
	afx_msg void MidValueFilter();

	//车牌识别相关消息处理函数
	afx_msg void OnColorSegmentation();
	afx_msg void OnPlateExtraction();
	afx_msg void OnCharacterSegmentation();

	//交通标志识别相关消息处理函数
	afx_msg void OnBlueSegmentation();      // 蓝色分割菜单响应
	afx_msg void OnRedSegmentation();       // 红色分割菜单响应
	afx_msg void OnConnectedFill();         // 连通填充菜单响应
	afx_msg void OnShapeDetection();        // 形状检测菜单响应
	afx_msg void OnAutoRecognize();
};

#ifndef _DEBUG  // MFCApplication1View.cpp 中的调试版本
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif
