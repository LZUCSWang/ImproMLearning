# ImproMLearning

一个基于 MFC 的车牌识别与图像处理实验项目，包含：
- 车牌定位与字符分割流程
- 图像处理可视化界面（MFCApplication1）
- 实验用车牌数据样例（`车牌定位实验图像/`）

## 开发环境
- Windows + Visual Studio（建议 2019/2022）
- MFC 框架开启

## 构建与运行
1. 用 Visual Studio 打开 `MFCApplication1.sln`。
2. 选择 Debug/Release 目标，确保已安装 MFC 组件。
3. 编译并运行，界面内可加载示例图片进行车牌检测与字符分割。

## 仓库清理与忽略
- `.gitignore` 已屏蔽 `.vs/`、`Debug/` 等 IDE/编译产物，请勿手动添加这些目录。
- 如果出现新的构建目录，请在提交前确认未加入版本控制。

## 目录结构
- `MFCApplication1/`：核心 MFC 工程代码。
- `车牌定位实验图像/`：示例数据集（体积较大，已保留必要样例）。
- `AGENTS.md`：实验环境或依赖说明。
