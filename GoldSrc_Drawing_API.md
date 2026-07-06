# GoldSrc 引擎绘制 API 文档

## 目录

1. [概述](#概述)
2. [SPR（精灵）纹理 API](#spr精灵纹理-api)
3. [屏幕绘制 API](#屏幕绘制-api)
4. [文本绘制 API](#文本绘制-api)
5. [三角形 API (pTriAPI)](#三角形-api-ptriapi)
6. [坐标系统](#坐标系统)
7. [渲染时机](#渲染时机)
8. [实用示例](#实用示例)

---

## 概述

GoldSrc 引擎提供了多套绘制 API，用于客户端 DLL 进行 2D 界面渲染。所有 API 都通过全局结构体 `gEngfuncs`（类型为 `cl_enginefunc_t`）访问。

**头文件位置**：
- 引擎函数表：`engine/cdll_int.h`
- 三角形 API：`common/triangleapi.h`
- 精灵矩形：`cl_dll/wrect.h`

---

## SPR（精灵）纹理 API

精灵（Sprite）是 GoldSrc 引擎中最常用的纹理绘制方式。精灵使用 `.spr` 格式文件，可以包含多帧动画。

### 类型定义

```cpp
// 精灵句柄（本质是 int）
typedef int HSPRITE;

// 精灵矩形区域
typedef struct rect_s
{
    int left, right, top, bottom;
} wrect_t;

// 精灵信息
typedef struct client_sprite_s
{
    char szName[64];      // 精灵名称
    char szSprite[64];    // 精灵文件名
    int hspr;             // 精灵句柄
    int iRes;             // 分辨率
    wrect_t rc;           // 矩形区域
} client_sprite_t;
```

### API 函数列表

| 函数 | 声明 | 说明 |
|------|------|------|
| `pfnSPR_Load` | `HSPRITE (*pfnSPR_Load)(const char *szPicName)` | 加载精灵文件，返回精灵句柄 |
| `pfnSPR_Frames` | `int (*pfnSPR_Frames)(HSPRITE hPic)` | 获取精灵的总帧数 |
| `pfnSPR_Height` | `int (*pfnSPR_Height)(HSPRITE hPic, int frame)` | 获取指定帧的高度 |
| `pfnSPR_Width` | `int (*pfnSPR_Width)(HSPRITE hPic, int frame)` | 获取指定帧的宽度 |
| `pfnSPR_Set` | `void (*pfnSPR_Set)(HSPRITE hPic, int r, int g, int b)` | 设置当前要绘制的精灵及颜色 |
| `pfnSPR_Draw` | `void (*pfnSPR_Draw)(int frame, int x, int y, const wrect_t *prc)` | 正常绘制精灵 |
| `pfnSPR_DrawHoles` | `void (*pfnSPR_DrawHoles)(int frame, int x, int y, const wrect_t *prc)` | 带孔洞（Alpha 测试）绘制精灵 |
| `pfnSPR_DrawAdditive` | `void (*pfnSPR_DrawAdditive)(int frame, int x, int y, const wrect_t *prc)` | 加法混合绘制精灵 |
| `pfnSPR_EnableScissor` | `void (*pfnSPR_EnableScissor)(int x, int y, int width, int height)` | 启用裁剪矩形 |
| `pfnSPR_DisableScissor` | `void (*pfnSPR_DisableScissor)(void)` | 禁用裁剪矩形 |
| `pfnSPR_GetList` | `client_sprite_t* (*pfnSPR_GetList)(char *psz, int *piCount)` | 获取精灵列表 |

### 宏定义（cl_util.h）

为了方便使用，`cl_util.h` 中定义了宏：

```cpp
#define SPR_Load         (*gEngfuncs.pfnSPR_Load)
#define SPR_Set          (*gEngfuncs.pfnSPR_Set)
#define SPR_Frames       (*gEngfuncs.pfnSPR_Frames)
#define SPR_GetList      (*gEngfuncs.pfnSPR_GetList)
#define SPR_Draw         (*gEngfuncs.pfnSPR_Draw)
#define SPR_DrawHoles    (*gEngfuncs.pfnSPR_DrawHoles)
#define SPR_DrawAdditive (*gEngfuncs.pfnSPR_DrawAdditive)
#define SPR_EnableScissor  (*gEngfuncs.pfnSPR_EnableScissor)
#define SPR_DisableScissor (*gEngfuncs.pfnSPR_DisableScissor)
```

### SPR 绘制示例

```cpp
// 1. 加载精灵
HSPRITE hSprite = SPR_Load("sprites/crosshair.spr");

// 2. 设置精灵和颜色
SPR_Set(hSprite, 255, 0, 0);  // 红色

// 3. 绘制（加法混合模式，推荐用于 HUD）
SPR_DrawAdditive(0, 100, 100, NULL);  // 第 0 帧，位置 (100, 100)，完整精灵

// 4. 绘制精灵的一部分
wrect_t rc = { 10, 30, 20, 40 };  // left, right, top, bottom
SPR_DrawAdditive(0, 100, 100, &rc);

// 5. 获取精灵尺寸
int width = SPR_Width(hSprite, 0);
int height = SPR_Height(hSprite, 0);
```

### SPR 支持的图片格式

- **.spr**：GoldSrc 引擎原生精灵格式（推荐）
- **.tga**：TGA 图片（引擎也支持直接加载 TGA 作为精灵）

---

## 屏幕绘制 API

### FillRGBA（矩形填充）

```cpp
void (*pfnFillRGBA)(int x, int y, int width, int height, int r, int g, int b, int a);
```

**参数**：
- `x, y`：矩形左上角坐标（屏幕坐标，像素）
- `width, height`：矩形尺寸
- `r, g, b, a`：颜色和透明度（0-255）

**宏定义**：
```cpp
#define FillRGBA  (*gEngfuncs.pfnFillRGBA)
```

**示例**：
```cpp
// 绘制半透明黑色背景
FillRGBA(100, 100, 200, 150, 0, 0, 0, 200);

// 绘制不透明红色边框
FillRGBA(95, 95, 210, 3, 255, 0, 0, 255);   // 上
FillRGBA(95, 242, 210, 3, 255, 0, 0, 255);  // 下
FillRGBA(95, 95, 3, 153, 255, 0, 0, 255);   // 左
FillRGBA(302, 95, 3, 153, 255, 0, 0, 255);  // 右
```

### GetScreenInfo（获取屏幕信息）

```cpp
int (*pfnGetScreenInfo)(SCREENINFO *pscrinfo);
```

**SCREENINFO 结构**：
```cpp
typedef struct SCREENINFO_s
{
    int   iSize;
    int   iWidth;         // 屏幕宽度
    int   iHeight;        // 屏幕高度
    int   iFlags;
    int   iCharHeight;    // 字符高度
    short charWidths[256]; // 每个字符的宽度
} SCREENINFO;
```

**示例**：
```cpp
SCREENINFO scrinfo;
scrinfo.iSize = sizeof(SCREENINFO);
gEngfuncs.pfnGetScreenInfo(&scrinfo);

int screenW = scrinfo.iWidth;
int screenH = scrinfo.iHeight;
```

**注意**：也可以直接使用全局变量 `ScreenWidth` 和 `ScreenHeight`。

---

## 文本绘制 API

### pfnDrawCharacter（绘制单个字符）

```cpp
int (*pfnDrawCharacter)(int x, int y, int number, int r, int g, int b);
```

**参数**：
- `x, y`：字符左上角坐标
- `number`：ASCII 字符码（0-255）
- `r, g, b`：文字颜色（0-255）

**返回值**：字符宽度

**示例**：
```cpp
// 绘制 "Hello"
const char* text = "Hello";
int x = 100, y = 100;
while (*text)
{
    gEngfuncs.pfnDrawCharacter(x, y, *text, 255, 200, 0);
    x += 8;  // 每个字符约 8 像素宽
    text++;
}
```

### pfnDrawConsoleString（绘制控制台字符串）

```cpp
int (*pfnDrawConsoleString)(int x, int y, char *string);
```

### pfnDrawSetTextColor（设置文本颜色）

```cpp
void (*pfnDrawSetTextColor)(float r, float g, float b);
```

**参数**：颜色值范围 0.0 - 1.0

### pfnDrawConsoleStringLen（获取字符串尺寸）

```cpp
void (*pfnDrawConsoleStringLen)(const char *string, int *length, int *height);
```

### 字体信息

使用 `GetScreenInfo` 获取字体信息：

```cpp
SCREENINFO scrinfo;
scrinfo.iSize = sizeof(SCREENINFO);
gEngfuncs.pfnGetScreenInfo(&scrinfo);

int charHeight = scrinfo.iCharHeight;         // 字符高度
int charWidthA = scrinfo.charWidths['A'];     // 'A' 字符的宽度
```

---

## 三角形 API (pTriAPI)

三角形 API 提供了类 OpenGL 的低级绘制接口，支持绘制点、线、三角形等基本图元。通过 `gEngfuncs.pTriAPI` 访问。

**头文件**：`common/triangleapi.h`

### 类型定义

```cpp
// 图元类型
typedef enum 
{
    TRI_TRIANGLES = 0,      // 独立三角形
    TRI_TRIANGLE_STRIP,     // 三角形条带
    TRI_TRIANGLE_FAN,       // 三角形扇形
    TRI_POLYGON,            // 多边形
    TRI_LINES,              // 独立线段
    TRI_LINE_STRIP,         // 线段条带
    TRI_LINE_LOOP,          // 闭合线段
    TRI_POINTS,             // 点
} tritype_t;
```

### 核心 API 函数

#### 1. 渲染模式控制

| 函数 | 说明 |
|------|------|
| `RenderMode(int mode)` | 设置渲染模式 |
| `CullFace(int mode)` | 设置背面剔除模式（TRI_NONE = 禁用） |

**常用渲染模式**：
- `kRenderNormal` - 正常渲染
- `kRenderTransColor` - 颜色透明（无纹理）
- `kRenderTransTexture` - 纹理透明
- `kRenderTransAdd` - 加法混合
- `kRenderTransAlpha` - Alpha 混合
- `kRenderGlow` - 发光效果

#### 2. 图元绘制

| 函数 | 说明 |
|------|------|
| `Begin(int primitiveCode)` | 开始绘制指定图元 |
| `End()` | 结束绘制 |

#### 3. 顶点属性

| 函数 | 说明 |
|------|------|
| `Vertex3f(float x, float y, float z)` | 指定顶点位置 |
| `Vertex3fv(float *v)` | 指定顶点位置（数组） |
| `Color4f(float r, float g, float b, float a)` | 指定顶点颜色（浮点） |
| `Color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)` | 指定顶点颜色（字节） |
| `TexCoord2f(float u, float v)` | 指定纹理坐标 |
| `TexCoord2fv(float *v)` | 指定纹理坐标（数组） |

#### 4. 点和线属性

| 函数 | 说明 |
|------|------|
| `PointSize(float size)` | 设置点大小 |
| `LineWidth(float width)` | 设置线宽 |

#### 5. 状态开关

| 函数 | 说明 |
|------|------|
| `Enable(int state)` | 启用状态 |
| `Disable(int state)` | 禁用状态 |

#### 6. 纹理管理

| 函数 | 说明 |
|------|------|
| `BindTexture(int target, int texture)` | 绑定纹理 |
| `GenTextures(int n, int *textures)` | 生成纹理 |
| `DeleteTextures(int n, int *textures)` | 删除纹理 |
| `IsTexture(int texture)` | 检查是否为有效纹理 |
| `TexImage2D(...)` | 上传 2D 纹理数据 |
| `TexSubImage2D(...)` | 更新 2D 纹理子区域 |
| `TexParameterf(...)` | 设置纹理参数（浮点） |
| `TexParameteri(...)` | 设置纹理参数（整数） |
| `TexEnvf(...)` | 设置纹理环境 |
| `CopyTexImage2D(...)` | 从帧缓冲复制到纹理 |
| `CopyTexSubImage2D(...)` | 从帧缓冲复制到纹理子区域 |

#### 7. 矩阵操作

| 函数 | 说明 |
|------|------|
| `PushMatrix()` | 压入矩阵栈 |
| `PopMatrix()` | 弹出矩阵栈 |
| `MatrixMode(int mode)` | 设置矩阵模式 |
| `LoadIdentity()` | 加载单位矩阵 |
| `LoadMatrixf(float *m)` | 加载矩阵 |
| `MultMatrixf(float *m)` | 乘矩阵 |
| `Rotatef(float angle, float x, float y, float z)` | 旋转 |
| `Translatef(float x, float y, float z)` | 平移 |
| `Scalef(float x, float y, float z)` | 缩放 |
| `Viewport(int x, int y, int width, int height)` | 设置视口 |
| `Ortho(...)` | 设置正交投影 |
| `Frustum(...)` | 设置透视投影 |

#### 8. 显示列表

| 函数 | 说明 |
|------|------|
| `NewList(int list, int mode)` | 创建显示列表 |
| `EndList()` | 结束显示列表 |
| `CallList(int list)` | 调用显示列表 |
| `GenLists(int num)` | 生成显示列表 ID |
| `DeleteLists(int list, int num)` | 删除显示列表 |

#### 9. 其他

| 函数 | 说明 |
|------|------|
| `GetError()` | 获取错误代码 |
| `GetString(unsigned long name)` | 获取状态字符串 |
| `GetBooleanv(unsigned long cap, int *buf)` | 获取布尔状态 |
| `GetIntegerv(unsigned long cap, int *buf)` | 获取整数状态 |
| `GetFloatv(unsigned long cap, float *buf)` | 获取浮点状态 |
| `GetDoublev(unsigned long cap, double *buf)` | 获取双精度状态 |
| `AlphaFunc(int func, float ref)` | 设置 Alpha 测试函数 |
| `BlendFunc(int sfactor, int dfactor)` | 设置混合函数 |
| `DrawBuffer(int mode)` | 设置绘制缓冲 |
| `ReadBuffer(int mode)` | 设置读取缓冲 |
| `ReadPixels(...)` | 读取像素数据 |
| `RasterPos2f(float x, float y)` | 设置光栅位置 |

### 三角形 API 使用示例

#### 示例 1：绘制纯色三角形（屏幕空间）

```cpp
triangleapi_t* tri = gEngfuncs.pTriAPI;

// 设置渲染模式（无纹理透明颜色）
tri->RenderMode(kRenderTransColor);
tri->CullFace(TRI_NONE);

// 开始绘制三角形
tri->Begin(TRI_TRIANGLES);

// 注意：坐标需要转换为标准化设备坐标（-1 到 1）
// 屏幕坐标 (x, y) -> NDC (2*x/width-1, 1-2*y/height, 0)
float w = (float)ScreenWidth;
float h = (float)ScreenHeight;

// 顶点 1 - 红色
tri->Color4f(1.0f, 0.0f, 0.0f, 1.0f);
tri->Vertex3f(100 * 2.0f / w - 1.0f, 1.0f - 100 * 2.0f / h, 0.0f);

// 顶点 2 - 绿色
tri->Color4f(0.0f, 1.0f, 0.0f, 1.0f);
tri->Vertex3f(200 * 2.0f / w - 1.0f, 1.0f - 100 * 2.0f / h, 0.0f);

// 顶点 3 - 蓝色
tri->Color4f(0.0f, 0.0f, 1.0f, 1.0f);
tri->Vertex3f(150 * 2.0f / w - 1.0f, 1.0f - 200 * 2.0f / h, 0.0f);

tri->End();
```

#### 示例 2：绘制带纹理的矩形

```cpp
triangleapi_t* tri = gEngfuncs.pTriAPI;

tri->RenderMode(kRenderTransTexture);
tri->CullFace(TRI_NONE);
tri->BindTexture(0, textureId);  // 绑定纹理

tri->Begin(TRI_TRIANGLE_STRIP);

tri->Color4f(1.0f, 1.0f, 1.0f, 1.0f);

// 左下
tri->TexCoord2f(0.0f, 1.0f);
tri->Vertex3f(x1, y1, 0.0f);

// 左上
tri->TexCoord2f(0.0f, 0.0f);
tri->Vertex3f(x1, y2, 0.0f);

// 右下
tri->TexCoord2f(1.0f, 1.0f);
tri->Vertex3f(x2, y1, 0.0f);

// 右上
tri->TexCoord2f(1.0f, 0.0f);
tri->Vertex3f(x2, y2, 0.0f);

tri->End();
```

---

## 坐标系统

### 屏幕坐标

SPR 绘制、FillRGBA、文本绘制都使用**屏幕坐标**：
- **原点**：左上角 (0, 0)
- **X 轴**：向右为正
- **Y 轴**：向下为正
- **单位**：像素

### 三角形 API 坐标

三角形 API 默认使用**标准化设备坐标 (NDC)**：
- **原点**：屏幕中心 (0, 0, 0)
- **X 轴**：向右为正（范围 -1 到 1）
- **Y 轴**：向上为正（范围 -1 到 1）
- **Z 轴**：深度（范围 -1 到 1，0 为近裁剪面）

**屏幕坐标转 NDC**：
```cpp
float ndc_x = screen_x * 2.0f / ScreenWidth - 1.0f;
float ndc_y = 1.0f - screen_y * 2.0f / ScreenHeight;
float ndc_z = 0.0f;  // 2D 绘制时 z 设为 0
```

也可以使用 `Ortho()` 设置正交投影来直接使用屏幕坐标。

---

## 渲染时机

### 客户端 DLL 渲染回调

在 `cdll_int.h` 中定义了两个关键的渲染回调函数：

```cpp
// 绘制不透明三角形（在 3D 场景渲染中调用）
extern void ClientDLL_DrawNormalTriangles(void);

// 绘制透明三角形（在 3D 场景渲染后调用，用于 HUD 等）
extern void ClientDLL_DrawTransparentTriangles(void);
```

### 实际使用位置

在项目中，这两个函数实现在 `cl_dll/tri.cpp`：

| 函数 | 用途 |
|------|------|
| `HUD_DrawNormalTriangles` | 绘制 3D 世界中的实体三角形 |
| `HUD_DrawTransparentTriangles` | 绘制 HUD、UI 等 2D 覆盖物 |

**注意**：2D UI 绘制（计分板、菜单等）应该在 `HUD_DrawTransparentTriangles` 中进行。

---

## 实用示例

### 示例 1：绘制带边框的窗口

```cpp
void DrawWindow(int x, int y, int w, int h, int border)
{
    // 背景（半透明黑色）
    FillRGBA(x, y, w, h, 0, 0, 0, 200);
    
    // 外边框（橙色）
    FillRGBA(x - border, y - border, w + border * 2, border, 255, 100, 0, 255);
    FillRGBA(x - border, y + h, w + border * 2, border, 255, 100, 0, 255);
    FillRGBA(x - border, y, border, h, 255, 100, 0, 255);
    FillRGBA(x + w, y, border, h, 255, 100, 0, 255);
}
```

### 示例 2：绘制文字字符串

```cpp
void DrawText(int x, int y, const char* text, int r, int g, int b)
{
    while (*text)
    {
        gEngfuncs.pfnDrawCharacter(x, y, *text, r, g, b);
        x += 8;  // 标准字符宽度约 8 像素
        text++;
    }
}
```

### 示例 3：绘制计分板行

```cpp
void DrawScoreboardRow(int x, int y, int w, const char* name, int score, int deaths, int ping)
{
    char buf[64];
    
    // 行背景
    FillRGBA(x, y, w, 25, 20, 20, 20, 100);
    
    // 玩家名（橙色）
    int cx = x + 10;
    const char* p = name;
    while (*p)
    {
        gEngfuncs.pfnDrawCharacter(cx, y + 5, *p, 255, 100, 0);
        cx += 8;
        p++;
    }
    
    // 分数
    sprintf(buf, "%d", score);
    cx = x + 200;
    p = buf;
    while (*p)
    {
        gEngfuncs.pfnDrawCharacter(cx, y + 5, *p, 255, 255, 255);
        cx += 8;
        p++;
    }
}
```

### 示例 4：使用三角形 API 绘制渐变矩形

```cpp
void DrawGradientRect(int x, int y, int w, int h, 
                      int r1, int g1, int b1, int a1,
                      int r2, int g2, int b2, int a2)
{
    triangleapi_t* tri = gEngfuncs.pTriAPI;
    float fw = (float)ScreenWidth;
    float fh = (float)ScreenHeight;
    
    tri->RenderMode(kRenderTransColor);
    tri->CullFace(TRI_NONE);
    tri->Begin(TRI_TRIANGLE_STRIP);
    
    // 左上 - 颜色 1
    tri->Color4f(r1/255.0f, g1/255.0f, b1/255.0f, a1/255.0f);
    tri->Vertex3f(x * 2.0f/fw - 1.0f, 1.0f - y * 2.0f/fh, 0.0f);
    
    // 左下 - 颜色 2
    tri->Color4f(r2/255.0f, g2/255.0f, b2/255.0f, a2/255.0f);
    tri->Vertex3f(x * 2.0f/fw - 1.0f, 1.0f - (y+h) * 2.0f/fh, 0.0f);
    
    // 右上 - 颜色 1
    tri->Color4f(r1/255.0f, g1/255.0f, b1/255.0f, a1/255.0f);
    tri->Vertex3f((x+w) * 2.0f/fw - 1.0f, 1.0f - y * 2.0f/fh, 0.0f);
    
    // 右下 - 颜色 2
    tri->Color4f(r2/255.0f, g2/255.0f, b2/255.0f, a2/255.0f);
    tri->Vertex3f((x+w) * 2.0f/fw - 1.0f, 1.0f - (y+h) * 2.0f/fh, 0.0f);
    
    tri->End();
}
```

---

## 性能注意事项

1. **优先使用 SPR**：精灵绘制由引擎优化，性能最好
2. **FillRGBA 简单高效**：纯色矩形用 FillRGBA 比三角形 API 更简单
3. **减少绘制调用**：尽量合并多个 FillRGBA 调用
4. **三角形 API 用于复杂图形**：渐变、多边形等复杂图形才使用 pTriAPI
5. **纹理上传**：避免每帧上传纹理，应该在初始化时完成

---

## 兼容性

所有这些 API 都是 GoldSrc 引擎的标准接口，在以下渲染模式下均可用：
- OpenGL (opengl32.dll)
- Direct3D (d3d.dll / d3d8.dll)
- Software (sw.dll)

无需为不同渲染模式编写不同代码。
