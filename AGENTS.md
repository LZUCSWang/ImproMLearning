# Repository Guidelines

## Project Structure & Module Organization
The repository root contains `MFCApplication1.sln` plus Visual Studio metadata. All code lives in `MFCApplication1/`, which follows the Doc/View split: `MFCApplication1Doc.*` owns document state, `MFCApplication1View.*` hosts image-processing logic, and frame helpers (`MainFrm.*`, `ClassView.*`, `PropertiesWnd.*`, `OutputWnd.*`) wire up the panes. UI resources sit in `res/` alongside `resource.h`; edit the `.rc` file and header together so identifiers stay aligned. Build outputs populate the `Debug/` folders and can be regenerated at any time. Sample imagery for license-plate and traffic-sign flows sits in `车牌定位实验图像/`; store future datasets in sibling folders to keep source directories clean.

## Build, Test, and Development Commands
Run these from the repository root:
```
devenv MFCApplication1.sln /Build "Debug|x64"   # IDE build + deploy to Debug/MFCApplication1.exe
msbuild MFCApplication1.sln /p:Configuration=Release /p:Platform="x64"
msbuild MFCApplication1.sln /t:Clean
```
Run `Debug/MFCApplication1.exe` for local validation and rebuild after any `MFCApplication1.rc` or `resource.h` change to keep identifiers synchronized.

## Coding Style & Naming Conventions
Stick to the existing MFC conventions: Allman braces, tabs (Visual Studio default, roughly four spaces), and `C`-prefixed class names (`CMFCApplication1View`). Member fields use camelCase (`colorSegmentImg`), Win32 IDs remain all caps (`ID_32782`). Keep include order as `pch.h`, `framework.h`, standard headers, then project headers to preserve precompiled-header benefits. Declare every new command in the header, add it to `BEGIN_MESSAGE_MAP`, and pair UI handlers with matching string resources plus short comments.

## Testing Guidelines
There is no automated harness, so manual testing is mandatory. Exercise each affected menu path (Histogram Equalization, Morphology, Color/Plate/Character segmentation, Traffic Sign detection) against at least one sample in `车牌定位实验图像/`. Capture before/after screenshots or histogram summaries and mention any parameter tweaks (kernel size, thresholds, ROI coordinates).

## Commit & Pull Request Guidelines
Write commits in the form `type(scope): imperative summary`, e.g., `feat(view): add Hu-moment classifier`, and avoid mixing code, resources, and datasets when possible. Pull requests must include a concise scenario description, reproduction steps (`Debug/MFCApplication1.exe` → “文件” → “打开”), screenshots or GIFs when pixel output changes, the exact samples used (e.g., `车牌定位实验图像/plate_03.bmp`), and any performance or memory callouts so reviewers know what to double-check.

## Data & Configuration Tips
Do not commit regenerated binaries (`Debug/`), `.aps` resource caches, or bulky experimental datasets. Because paths include Chinese characters, run scripts in UTF-8/GBK-aware shells before batch-processing assets. Keep configuration constants (kernel sizes, HSV thresholds, struct definitions) centralized in the view class and document any new knobs inline so other contributors can reproduce your setup.
