# halflife-2_3 VGUI 系统分析文档

## 一、概述

**VGUI**（Valve Graphics User Interface）是 Valve 公司为其 GoldSrc 引擎开发的专有图形用户界面系统。`halflife-2_3` 项目使用的是 **VGUI 第一代**（非 Source 引擎的 VGUI2），通过 C++ 面向对象设计实现游戏中的所有 2D 界面元素，包括主菜单、HUD、对话框、记分板等。

### 核心设计理念

- **Panel 层级结构**：所有控件继承自基类 `Panel`，通过父子关系构建界面树
- **信号/槽机制**：通过 `ActionSignal`、`InputSignal` 等接口实现事件回调（观察者模式）
- **Scheme 外观方案**：通过 `Scheme` 系统统一管理颜色、字体、光标等视觉属性
- **Surface 渲染抽象**：通过 `SurfaceBase` → `Surface` → `SurfaceGL` 三层抽象支持不同渲染后端

---

## 二、目录结构

```
halflife-2_3-master/
├── utils/vgui/include/          # VGUI 核心库头文件（78个）
│   ├── VGUI.h                    # 命名空间基础定义
│   ├── VGUI_Panel.h              # 核心基类 Panel
│   ├── VGUI_App.h                # 应用核心类（单例）
│   ├── VGUI_Surface.h            # 渲染表面
│   ├── VGUI_SurfaceGL.h          # OpenGL 渲染实现
│   ├── VGUI_Font.h               # 字体类
│   ├── VGUI_Color.h              # RGBA 颜色类
│   ├── VGUI_Cursor.h             # 光标类
│   ├── VGUI_Scheme.h             # 外观方案
│   ├── VGUI_Layout.h             # 布局基类
│   ├── VGUI_BorderLayout.h       # 边界布局
│   ├── VGUI_FlowLayout.h         # 流式布局
│   ├── VGUI_StackLayout.h        # 堆叠布局
│   ├── VGUI_InputSignal.h        # 输入信号接口
│   ├── VGUI_ActionSignal.h       # 动作信号接口
│   ├── VGUI_Button.h             # 按钮控件
│   ├── VGUI_Label.h              # 标签控件
│   ├── VGUI_TextEntry.h          # 文本输入框
│   ├── VGUI_Frame.h              # 窗口框架
│   ├── VGUI_ScrollPanel.h        # 滚动面板
│   ├── VGUI_Menu.h               # 菜单
│   ├── VGUI_ProgressBar.h        # 进度条
│   ├── VGUI_ComboBox.h           # 下拉组合框
│   ├── VGUI_MessageBox.h          # 消息对话框
│   ├── VGUI_WizardPanel.h        # 向导面板
│   ├── ...                       # 其他控件和工具类
│   └── VGUI_TreeFolder.h         # 树形文件夹
├── cl_dll/                       # 客户端游戏代码
│   ├── vgui_int.cpp              # VGUI 与引擎接口层
│   ├── vgui_TeamFortressViewport.h  # 核心视口（30+ 类定义）
│   ├── vgui_TeamFortressViewport.cpp # 视口实现
│   ├── vgui_SchemeManager.h/cpp  # 方案管理器
│   ├── vgui_ScorePanel.h/cpp     # 记分板
│   ├── vgui_SpectatorPanel.h/cpp # 观察者面板
│   ├── vgui_ClassMenu.cpp        # 职业选择菜单
│   ├── vgui_TeamMenu.cpp         # 队伍选择菜单
│   ├── vgui_MOTDWindow.cpp       # MOTD 窗口
│   ├── vgui_CustomObjects.cpp    # 自定义控件实现
│   ├── vgui_ServerBrowser.h      # 服务器浏览器
│   ├── vgui_ControlConfigPanel.h # 控制配置面板
│   └── vgui_ConsolePanel.h       # 控制台面板
```

---

## 三、核心架构

### 3.1 Panel 基类（`VGUI_Panel.h`）

`Panel` 是所有 VGUI 可视组件的基类，提供约 100+ 个方法，是整个系统的根基。

#### 位置与尺寸

| 方法 | 说明 |
|------|------|
| `setPos(x, y)` | 设置位置 |
| `getPos(x, y)` | 获取位置 |
| `setSize(wide, tall)` | 设置大小 |
| `getSize(wide, tall)` | 获取大小 |
| `setBounds(x, y, wide, tall)` | 同时设置位置和大小 |
| `getBounds(x, y, wide, tall)` | 同时获取位置和大小 |

#### 父子层级关系

| 方法 | 说明 |
|------|------|
| `setParent(Panel*)` | 设置父面板 |
| `addChild(Panel*)` | 添加子面板 |
| `removeChild(Panel*)` | 移除子面板 |
| `getChildCount()` | 获取子面板数量 |
| `getChild(i)` | 获取第 i 个子面板 |

#### 可见性控制

| 方法 | 说明 |
|------|------|
| `setVisible(bool)` | 设置可见性 |
| `isVisible()` | 是否可见 |
| `isVisibleUp()` | 从自身到根是否全部可见 |

#### 绘制系统

| 方法 | 说明 |
|------|------|
| `paint()` | 绘制前景 |
| `paintBackground()` | 绘制背景 |
| `paintTraverse(bool pop)` | 递归绘制（遍历子面板） |
| `repaint()` | 请求重绘 |

#### 输入检测

| 方法 | 说明 |
|------|------|
| `wasMousePressed(MouseCode)` | 鼠标是否刚按下 |
| `isMouseDown(MouseCode)` | 鼠标是否持续按下 |
| `wasMouseReleased(MouseCode)` | 鼠标是否刚释放 |
| `wasKeyPressed(KeyCode)` | 键盘是否刚按下 |
| `isKeyDown(KeyCode)` | 键盘是否持续按下 |

#### 坐标转换

| 方法 | 说明 |
|------|------|
| `localToScreen(x, y)` | 本地坐标转屏幕坐标 |
| `screenToLocal(x, y)` | 屏幕坐标转本地坐标 |

#### 焦点管理

| 方法 | 说明 |
|------|------|
| `requestFocus()` | 请求焦点 |
| `hasFocus()` | 是否拥有焦点 |
| `setFocusNavGroup(FocusNavGroup*)` | 设置焦点导航组 |

#### 外观设置

| 方法 | 说明 |
|------|------|
| `setBorder(Border*)` | 设置边框 |
| `setFgColor(int r, g, b, a)` | 设置前景色 |
| `setBgColor(int r, g, b, a)` | 设置背景色 |
| `setCursor(Cursor*)` | 设置光标 |

### 3.2 App 应用核心（`VGUI_App.h`）

`App` 是 VGUI 应用的单例管理器，负责全局状态和主循环。

| 方法 | 说明 |
|------|------|
| `getInstance()` | 获取全局单例 |
| `start()` / `stop()` | 启动/停止应用 |
| `externalTick()` | 外部 tick 驱动 |
| `requestFocus(Panel*)` | 请求焦点 |
| `getFocus()` | 获取当前焦点面板 |
| `setMouseCapture(Panel*)` | 设置鼠标捕获 |
| `addTickSignal(TickSignal*)` | 添加定时回调 |
| `setClipboardText()` / `getClipboardText()` | 剪贴板操作 |
| `setRegistryString()` / `getRegistryInteger()` | 注册表操作 |

### 3.3 Surface 渲染表面（`VGUI_Surface.h`）

`Surface` 继承自 `SurfaceBase`，是 2D 绘制 API 的核心实现。

#### 窗口管理

| 方法 | 说明 |
|------|------|
| `setTitle(const char*)` | 设置窗口标题 |
| `setFullscreenMode(wide, tall, bpp)` | 设置全屏模式 |
| `setWindowedMode()` | 设置窗口模式 |
| `createPopup(Panel*)` | 创建弹出窗口 |
| `hasFocus()` | 是否拥有焦点 |

#### 2D 绘制 API

| 方法 | 说明 |
|------|------|
| `drawSetColor(r, g, b, a)` | 设置绘制颜色 |
| `drawFilledRect()` | 绘制填充矩形 |
| `drawOutlinedRect()` | 绘制边框矩形 |
| `drawSetTextFont(Font*)` | 设置文本字体 |
| `drawSetTextColor(r, g, b, a)` | 设置文本颜色 |
| `drawSetTextPos(x, y)` | 设置文本位置 |
| `drawPrintText(text, len)` | 绘制文本 |
| `drawSetTextureRGBA()` | 设置 RGBA 纹理 |
| `drawSetTexture()` | 设置纹理 ID |
| `drawTexturedRect()` | 绘制纹理矩形 |

#### 其他

| 方法 | 说明 |
|------|------|
| `createNewTextureID()` | 创建新纹理 ID |
| `invalidate(Panel*)` | 使面板无效（触发重绘） |
| `enableMouseCapture(bool)` | 启用/禁用鼠标捕获 |
| `setCursor(Cursor*)` | 设置光标 |
| `swapBuffers()` | 交换缓冲区 |

---

## 四、信号/回调系统

VGUI 通过纯虚接口实现观察者模式的事件回调。

### 4.1 InputSignal（输入信号）

处理所有鼠标和键盘输入事件：

```cpp
class InputSignal {
    virtual void cursorMoved(int x, int y, Panel* panel) = 0;       // 鼠标移动
    virtual void cursorEntered(Panel* panel) = 0;                    // 鼠标进入
    virtual void cursorExited(Panel* panel) = 0;                     // 鼠标离开
    virtual void mousePressed(MouseCode code, Panel* panel) = 0;   // 鼠标按下
    virtual void mouseDoublePressed(MouseCode code, Panel* panel) = 0;// 鼠标双击
    virtual void mouseReleased(MouseCode code, Panel* panel) = 0;   // 鼠标释放
    virtual void mouseWheeled(int delta, Panel* panel) = 0;          // 鼠标滚轮
    virtual void keyPressed(KeyCode code, Panel* panel) = 0;       // 键盘按下
    virtual void keyTyped(KeyCode code, Panel* panel) = 0;          // 键盘输入字符
    virtual void keyReleased(KeyCode code, Panel* panel) = 0;      // 键盘释放
    virtual void keyFocusTicked(Panel* panel) = 0;                   // 焦点 tick
};
```

### 4.2 ActionSignal（动作信号）

处理 UI 控件激活事件：

```cpp
class ActionSignal {
    virtual void actionPerformed(Panel* panel) = 0;  // 动作触发回调
};
```

### 4.3 其他信号接口

| 信号类 | 纯虚方法 | 用途 |
|--------|---------|------|
| `ChangeSignal` | `valueChanged(Panel*)` | 值变更通知 |
| `FocusChangeSignal` | `focusChanged(bool lost, Panel*)` | 焦点变更通知 |
| `IntChangeSignal` | `intChanged(int value, Panel*)` | 整数值变更通知 |
| `FrameSignal` | `closing(Frame*)`, `minimizing(Frame*, bool)` | 窗口事件通知 |
| `RepaintSignal` | `panelRepainted(Panel*)` | 重绘完成通知 |
| `TickSignal` | `ticked()` | 定时 tick 通知 |

### 4.4 项目中的信号处理器

| 处理器类 | 继承 | 功能 |
|---------|------|------|
| `CMenuHandler_StringCommand` | `ActionSignal` | 执行字符串控制台命令 |
| `CMenuHandler_StringCommandWatch` | `CMenuHandler_StringCommand` | 执行命令并更新队伍状态 |
| `CMenuHandler_StringCommandClassSelect` | `CMenuHandler_StringCommand` | 职业选择（支持自动击杀） |
| `CMenuHandler_PopupSubMenuInput` | `InputSignal` | 弹出子菜单 |
| `CMenuHandler_LabelInput` | `InputSignal` | 标签点击处理 |
| `CMenuHandler_TextWindow` | `ActionSignal` | 文本窗口显示/隐藏 |
| `CMenuHandler_ToggleCvar` | `ActionSignal` | 切换控制台变量 |
| `CDragNDropHandler` | `InputSignal` | 拖拽处理 |
| `CHandler_MenuButtonOver` | `InputSignal` | 菜单按钮悬停 |
| `CHandler_ButtonHighlight` | `InputSignal` | 按钮高亮 |
| `CHandler_CommandButtonHighlight` | `CHandler_ButtonHighlight` | 命令按钮高亮 |
| `CSpectatorHandler_Command` | `ActionSignal` | 观察者命令执行 |

---

## 五、内置控件库

### 5.1 VGUI 核心库控件（`utils/vgui/include/`）

#### 基础控件

| 控件类 | 继承自 | 功能 |
|--------|--------|------|
| `Label` | `Panel` | 文本/图像标签，支持 9 种对齐方式 |
| `Button` | `Label` | 按钮，支持选中/激活状态、ActionSignal |
| `ToggleButton` | `Button` | 切换按钮（保持按下状态） |
| `CheckButton` | `ToggleButton` | 复选框 |
| `RadioButton` | `ToggleButton` | 单选按钮 |
| `TextEntry` | `Panel`, `InputSignal` | 文本输入框，支持光标、选择、复制粘贴、密码模式 |
| `ImagePanel` | `Panel` | 图像显示面板 |
| `Slider` | `Panel` | 滑块，支持拖拽、范围窗口、垂直/水平方向 |
| `ScrollBar` | `Panel` | 滚动条，包含两个按钮和一个滑块 |
| `ScrollPanel` | `Panel` | 滚动面板，包含水平和垂直滚动条 |
| `ListPanel` | `Panel` | 列表面板，支持像素级滚动 |
| `ProgressBar` | `Panel` | 进度条，支持分段显示 |
| `TabPanel` | `Panel` | 标签面板，支持上/下/左/右放置 |

#### 容器控件

| 控件类 | 继承自 | 功能 |
|--------|--------|------|
| `Frame` | `Panel` | 窗口框架，支持标题、移动/缩放、最小化/最大化/关闭 |
| `MessageBox` | `Frame` | 消息对话框，含文本和 OK 按钮 |
| `WizardPanel` | `Panel` | 向导面板，支持多页导航 |
| `ConfigWizard` | `Panel` | 配置向导，含 TreeFolder |

#### 菜单控件

| 控件类 | 继承自 | 功能 |
|--------|--------|------|
| `Menu` | `Panel` | 菜单基类 |
| `MenuItem` | `Button` | 菜单项，可关联子菜单 |
| `MenuSeparator` | `Label` | 菜单分隔线 |
| `PopupMenu` | `Menu` | 弹出菜单，支持模态显示 |

#### 表格控件

| 控件类 | 继承自 | 功能 |
|--------|--------|------|
| `TablePanel` | `Panel` | 表格面板，支持列/行/单元格选择和编辑 |
| `HeaderPanel` | `Panel` | 表头面板，支持可拖拽调整的分节 |
| `TextGrid` | `Panel` | 文本网格，支持格式化输出，每个字符可独立着色 |
| `TextPanel` | `Panel` | 文本面板，使用 TextImage 显示 |

#### 桌面控件

| 控件类 | 继承自 | 功能 |
|--------|--------|------|
| `Desktop` | `Panel` | 桌面面板，管理图标和任务栏 |
| `DesktopIcon` | `Panel` | 桌面图标 |
| `TaskBar` | `Panel` | 任务栏 |
| `MiniApp` | - | 小应用程序接口 |
| `TreeFolder` | `Panel` | 树形文件夹，支持展开/折叠 |

### 5.2 项目自定义控件（`cl_dll/`）

| 控件类 | 继承自 | 用途 |
|--------|--------|------|
| `CommandButton` | `Button` | 基础命令按钮，支持子菜单、快捷键绑定、悬停高亮 |
| `ColorButton` | `CommandButton` | 可自定义颜色的命令按钮 |
| `SpectButton` | `CommandButton` | 观察者模式专用按钮 |
| `ClassButton` | `CommandButton` | 职业选择按钮（带有效性检查） |
| `TeamButton` | `CommandButton` | 队伍选择按钮 |
| `BuildButton` | `CommandButton` | 工程师建造按钮 |
| `FeignButton` | `CommandButton` | 间谍假死按钮 |
| `DisguiseButton` | `CommandButton` | 间谍伪装按钮 |
| `DetpackButton` | `CommandButton` | 爆破手炸药包按钮 |
| `SpectateButton` | `CommandButton` | 观战按钮 |
| `MapButton` | `CommandButton` | 地图按钮（仅当前地图时可见） |
| `TeamOnlyCommandButton` | `CommandButton` | 仅特定队伍可见的命令按钮 |
| `ToggleCommandButton` | `CommandButton`, `InputSignal` | 带开关状态的命令按钮 |
| `SpectToggleButton` | `CommandButton`, `InputSignal` | 观察者模式切换按钮 |
| `CImageLabel` | `Label` | 带 TGA 图片的标签（背景透明） |
| `CommandLabel` | `Label` | 命令标签（可暗化显示子菜单状态） |
| `CTransparentPanel` | `Panel` | 半透明黑色背景面板 |
| `DragNDropPanel` | `Panel` | 可拖拽面板 |
| `CMenuPanel` | `CTransparentPanel` | 菜单面板基类，支持菜单缓冲队列 |
| `CHealthPanel` | `DragNDropPanel` | 生命值 HUD 面板 |
| `CTFScrollPanel` | `ScrollPanel` | 自定义滚动面板（TGA 箭头按钮） |
| `CTFScrollButton` | `CommandButton` | 自定义滚动条按钮 |
| `CTFSlider` | `Slider` | 自定义滑块（橙色风格） |
| `CTextImage2` | `Image` | 双文本图像（两种颜色文本叠加） |
| `CLabelHeader` | `Label` | 记分板表头标签（支持排序指示） |

---

## 六、布局系统

| 布局类 | 功能描述 |
|--------|---------|
| `Layout` | 布局基类，纯虚 `performLayout(Panel*)` |
| `BorderLayout` | 边界布局（北/南/东/西/中五个区域） |
| `FlowLayout` | 流式布局（水平排列，自动换行） |
| `StackLayout` | 堆叠布局（垂直排列） |
| `GridLayout` | 网格布局（代码中已注释，未实现） |

---

## 七、边框系统

边框类继承自 `Image`，通过重写绘制方法实现不同视觉效果。

| 边框类 | 功能 |
|--------|------|
| `Border` | 边框基类，管理四边内边距（inset） |
| `BorderPair` | 双边框组合，依次绘制两个边框 |
| `LineBorder` | 线条边框，可设置颜色和粗细 |
| `EtchedBorder` | 蚀刻风格边框 |
| `RaisedBorder` | 凸起风格边框 |
| `LoweredBorder` | 凹陷风格边框 |

---

## 八、输入系统

### 8.1 鼠标代码（`VGUI_MouseCode.h`）

| 枚举值 | 说明 |
|--------|------|
| `MOUSE_LEFT` | 鼠标左键 |
| `MOUSE_RIGHT` | 鼠标右键 |
| `MOUSE_MIDDLE` | 鼠标中键 |

### 8.2 键盘代码（`VGUI_KeyCode.h`）

| 类别 | 枚举值 |
|------|--------|
| 数字键 | `KEY_0` ~ `KEY_9` |
| 字母键 | `KEY_A` ~ `KEY_Z` |
| 小键盘 | `KEY_PAD_0` ~ `KEY_PAD_9`, `KEY_PAD_DIVIDE`, `KEY_PAD_MULTIPLY`, `KEY_PAD_MINUS`, `KEY_PAD_PLUS`, `KEY_PAD_ENTER`, `KEY_PAD_DECIMAL` |
| 标点符号 | `KEY_LBRACKET`, `KEY_RBRACKET`, `KEY_SEMICOLON`, `KEY_APOSTROPHE`, `KEY_BACKQUOTE`, `KEY_COMMA`, `KEY_PERIOD`, `KEY_SLASH`, `KEY_BACKSLASH`, `KEY_MINUS`, `KEY_EQUAL` |
| 控制键 | `KEY_ENTER`, `KEY_SPACE`, `KEY_BACKSPACE`, `KEY_TAB`, `KEY_CAPSLOCK`, `KEY_NUMLOCK`, `KEY_ESCAPE`, `KEY_SCROLLLOCK` |
| 编辑键 | `KEY_INSERT`, `KEY_DELETE`, `KEY_HOME`, `KEY_END`, `KEY_PAGEUP`, `KEY_PAGEDOWN`, `KEY_BREAK` |
| 修饰键 | `KEY_LSHIFT`, `KEY_RSHIFT`, `KEY_LALT`, `KEY_RALT`, `KEY_LCONTROL`, `KEY_RCONTROL`, `KEY_LWIN`, `KEY_RWIN`, `KEY_APP` |
| 方向键 | `KEY_UP`, `KEY_LEFT`, `KEY_DOWN`, `KEY_RIGHT` |
| 功能键 | `KEY_F1` ~ `KEY_F12` |

### 8.3 光标样式（`VGUI_Cursor.h`）

| 枚举值 | 说明 |
|--------|------|
| `dc_user` | 用户自定义 |
| `dc_none` | 无光标 |
| `dc_arrow` | 箭头 |
| `dc_ibeam` | I 型文本光标 |
| `dc_hourglass` | 沙漏（等待） |
| `dc_crosshair` | 十字准星 |
| `dc_up` | 向上箭头 |
| `dc_sizenwse` | 左上-右下调整大小 |
| `dc_sizenesw` | 右上-左下调整大小 |
| `dc_sizewe` | 水平调整大小 |
| `dc_sizens` | 垂直调整大小 |
| `dc_sizeall` | 全方向调整大小 |
| `dc_no` | 禁止 |
| `dc_hand` | 手型 |

---

## 九、颜色与方案系统

### 9.1 Color 类（`VGUI_Color.h`）

RGBA 颜色封装，支持两种模式：
- **直接 RGBA**：`Color(r, g, b, a)` 指定数值
- **Scheme 引用**：`Color(Scheme::SchemeColor sc)` 引用方案颜色

### 9.2 Scheme 颜色枚举

| 枚举值 | 说明 |
|--------|------|
| `sc_primary1` / `sc_primary2` / `sc_primary3` | 主色调 1/2/3 |
| `sc_secondary1` / `sc_secondary2` / `sc_secondary3` | 次色调 1/2/3 |

### 9.3 CSchemeManager（`vgui_SchemeManager.h/cpp`）

管理字体方案，根据分辨率加载对应的文本方案文件。

**加载流程**：
1. 根据当前分辨率查找 `{xRes}_textscheme.txt` 文件
2. 解析方案定义（SchemeName、FontName、FontSize、FontWeight、各颜色）
3. 创建字体对象，相同参数的方案共享字体指针
4. 支持位图字体（`gfx\vgui\fonts\{xRes}_{schemeName}.tga`）

**方案属性**：

| 属性 | 说明 |
|------|------|
| `SchemeName` | 方案名称 |
| `FontName` | 字体名称 |
| `FontSize` | 字体大小 |
| `FontWeight` | 字体粗细 |
| `FgColor` | 前景色 |
| `BgColor` | 背景色 |
| `FgColorArmed` | 激活状态前景色 |
| `BgColorArmed` | 激活状态背景色 |
| `FgColorMousedown` | 按下状态前景色 |
| `BgColorMousedown` | 按下状态背景色 |
| `BorderColor` | 边框颜色 |

**分辨率适配**：支持 320、400、512、640、800、1024、1152、1280、1600 等分辨率。

---

## 十、字体系统（`VGUI_Font.h`）

| 方法 | 说明 |
|------|------|
| `Font(name, tall, wide, rotation, weight, italic, underline, strikeout, symbol)` | 通过参数创建字体 |
| `Font(name, pFileData, fileDataLen, ...)` | 通过 TGA 文件数据创建字体 |
| `getCharRGBA(ch, x, y, w, h, rgba)` | 获取字符 RGBA 像素数据 |
| `getCharABCwide(ch, a, b, c)` | 获取字符 A/B/C 宽度 |
| `getTextSize(text, wide, tall)` | 获取文本尺寸 |
| `getTall()` | 获取字体高度 |
| `getId()` | 获取字体 ID |

---

## 十一、游戏 UI 面板

### 11.1 TeamFortressViewport（核心视口）

`TeamFortressViewport` 是游戏 HUD 的总入口，继承自 `Panel`，管理所有 VGUI 子系统。

#### 初始化流程

```
VGui_Startup()
  └── TeamFortressViewport() 构造
       ├── 初始化 SchemeManager
       ├── 设置颜色方案（sc_primary1~sc_secondary2）
       ├── 创建 TeamMenu
       ├── 创建 ClassMenu
       ├── 创建 SpectatorMenu
       ├── 创建 ScoreBoard
       ├── 解析 commandmenu.txt
       ├── 解析 spectatormenu.txt
       ├── 解析 spectcammenu.txt
       └── 创建 ServerBrowser
```

#### 关键方法

| 方法 | 说明 |
|------|------|
| `Initialize()` | 新关卡重置：清空玩家信息、隐藏所有菜单 |
| `CreateCommandMenu()` | 从 .txt 文件解析命令菜单树 |
| `ShowVGUIMenu(menuID)` | 分发显示 VGUI 菜单（Team/Class/MapBriefing/Intro/ClassHelp/SpecHelp） |
| `KeyInput(down, keynum)` | 统一按键处理（菜单数字键、Escape、命令菜单） |
| `paintBackground()` | 每帧更新：检测队伍/职业变化、更新观察者和记分板 |
| `UpdateCursorState()` | 根据可见面板决定是否显示鼠标光标 |
| `ShowCommandMenu()` / `HideCommandMenu()` | 显示/隐藏命令菜单 |
| `ShowScoreBoard()` / `HideScoreBoard()` | 显示/隐藏记分板 |
| `DeathMsg()` | 死亡消息处理 |

#### 网络消息处理

| 消息 | 处理函数 | 说明 |
|------|---------|------|
| `VGUIMenu` | `MsgFunc_VGUIMenu()` | 服务器请求显示 VGUI 菜单 |
| `MOTD` | `MsgFunc_MOTD()` | 拼接 MOTD 文本 |
| `ScoreInfo` | `MsgFunc_ScoreInfo()` | 更新玩家分数/死亡/职业/队伍 |
| `TeamInfo` | `MsgFunc_TeamInfo()` | 更新玩家队伍名 |
| `TeamScore` | `MsgFunc_TeamScore()` | 接收队伍分数 |
| `TeamNames` | `MsgFunc_TeamNames()` | 接收队伍名称列表 |
| `ValClass` | `MsgFunc_ValClass()` | 接收有效职业位掩码 |
| `Feign` | `MsgFunc_Feign()` | 间谍装死状态 |
| `Detpack` | `MsgFunc_Detpack()` | 炸药包状态 |
| `BuildSt` | `MsgFunc_BuildSt()` | 工程师建造状态 |
| `RandomPC` | `MsgFunc_RandomPC()` | 随机职业标志 |
| `ServerName` | `MsgFunc_ServerName()` | 服务器名称 |
| `Spectator` | `MsgFunc_Spectator()` | 观察者状态 |
| `AllowSpec` | `MsgFunc_AllowSpec()` | 是否允许观察 |

### 11.2 队伍选择菜单（`CTeamMenuPanel`）

继承自 `CMenuPanel`，显示队伍按钮、自动分配、观察者按钮和地图简介。

| 功能 | 说明 |
|------|------|
| 队伍按钮 | 最多 4 个队伍 + 1 个自动分配 |
| 观察者按钮 | 进入观察者模式 |
| 地图简介 | 从 `{mapname}.txt` 加载简介文本 |
| 数字键 | 1~4 选择队伍，5 自动分配，6 观察者 |

### 11.3 职业选择菜单（`CClassMenuPanel`）

继承自 `CMenuPanel`，显示职业按钮和职业描述信息。

| 功能 | 说明 |
|------|------|
| 职业按钮 | 11 个（Civilian + Scout~Engineer + RandomPC） |
| 职业描述 | 从 `classes/short_{class}.txt` 加载 |
| 职业图片 | 红/蓝队伍职业 TGA 图片 |
| 玩家计数 | 显示每个职业的队友数量 |
| 有效性检查 | 根据 `GetValidClasses()` 控制按钮可见性 |
| 自动击杀 | `hud_classautokill` CVar 控制切换职业时是否自动死亡 |

### 11.4 记分板（`ScorePanel`）

继承自 `Panel`，显示玩家/队伍的分数、死亡、延迟、语音状态。

| 功能 | 说明 |
|------|------|
| 表头 | 8 列：头像、名字、职业、分数、死亡、延迟、语音、空白 |
| 排序 | Teamplay 按队伍分组排序，否则按 frags 降序 |
| 高亮 | 自己高亮显示，被击杀者红色渐隐 10 秒 |
| 语音屏蔽 | 点击玩家切换静音状态 |
| 滚动 | 支持大量玩家时的滚动显示 |

### 11.5 观察者面板（`SpectatorPanel`）

继承自 `Panel`，显示观战模式 UI。

| 功能 | 说明 |
|------|------|
| 顶部信息栏 | 地图名、时间、队伍分数 |
| 底部控制栏 | 选项/摄像机/上一位/下一位/画中画按钮 |
| 画中画 | 支持小窗口视图切换 |
| 菜单显隐 | 可隐藏底部按钮，居中显示帮助文字 |

### 11.6 服务器浏览器（`ServerBrowser`）

继承自 `CTransparentPanel`，提供服务器浏览和连接功能。

| 组件 | 说明 |
|------|------|
| `_headerPanel` | 表头面板 |
| `_tablePanel` | 表格面板 |
| `_connectButton` | 连接按钮 |
| `_refreshButton` | 刷新按钮 |
| `_broadcastRefreshButton` | 广播刷新按钮 |
| `_stopButton` | 停止按钮 |
| `_sortButton` | 排序按钮 |
| `_cancelButton` | 取消按钮 |
| `_pingButton` | Ping 按钮 |

### 11.7 MOTD 窗口（`CMessageWindowPanel`）

用于显示服务器 MOTD、地图简介、职业描述、观察者帮助等文本。

| 功能 | 说明 |
|------|------|
| 半透明背景 | 支持全屏或窗口化透明度 |
| 滚动文本 | 使用 ScrollPanel 显示长文本 |
| 自动换行 | `getTextSizeWrapped()` 自动计算换行 |
| OK 按钮 | 关闭窗口 |

### 11.8 控制台面板（`ConsolePanel`）

继承自 `Panel`，提供文本输出和命令输入。

| 功能 | 说明 |
|------|------|
| 文本网格 | `_textGrid` 显示历史输出 |
| 文本输入 | `_textEntry` 命令输入框 |
| 格式化输出 | `printf()` / `vprintf()` |
| 命令执行 | `doExecCommand()` |

### 11.9 控制配置面板（`ControlConfigPanel`）

继承自 `Panel`，管理按键绑定。

| 功能 | 说明 |
|------|------|
| CVar 管理 | `AddCVar()` 添加控制台变量 |
| 按键绑定 | `GetCVarBind()` / `SetCVarBind()` |
| 表格显示 | 使用 TablePanel 显示绑定列表 |

---

## 十二、命令菜单系统

### 12.1 菜单文件

| 文件 | 用途 |
|------|------|
| `commandmenu.txt` | 标准命令菜单 |
| `spectatormenu.txt` | 观察者选项菜单 |
| `spectcammenu.txt` | 观察者相机菜单 |

### 12.2 解析规则

命令菜单从文本文件解析，支持以下标记：

| 标记 | 说明 |
|------|------|
| `CUSTOM` | 自定义按钮（如 `!CHANGETEAM`） |
| `MAP` | 仅当前地图时可见 |
| `TEAMn` | 仅特定队伍可见 |
| `TOGGLE` | 切换型按钮（绑定 CVar） |
| 职业名 | 仅特定职业时可见 |
| `{` | 子菜单开始 |
| `}` | 子菜单结束 |

### 12.3 自定义按钮

| 标记 | 创建的按钮类型 | 功能 |
|------|---------------|------|
| `!CHANGETEAM` | `CommandButton` | 打开队伍选择菜单 |
| `!CHANGECLASS` | `CommandButton` | 打开职业选择菜单 |
| `!MAPBRIEFING` | `CommandButton` | 显示地图简介 |
| `!CLASSDESC` | `CommandButton` | 显示职业描述 |
| `!SPY` | `CommandButton` | 间谍菜单 |
| `!DISGUISE` | `DisguiseButton` | 伪装子菜单 |
| `!DETPACKSTART` | `DetpackButton` | 启动炸药包 |
| `!DETPACKSTOP` | `DetpackButton` | 停止炸药包 |
| `!BUILD` | `BuildButton` | 工程师建造菜单 |

### 12.4 快捷键

数字键 0-9 直接绑定到命令菜单按钮，Escape 关闭菜单。

---

## 十三、VGUI 与引擎接口

### 13.1 接口函数（`vgui_int.cpp`）

| 函数 | 说明 |
|------|------|
| `VGui_Startup()` | 获取引擎根面板，创建 `TeamFortressViewport` |
| `VGui_Shutdown()` | 删除视口并清理 |
| `VGui_ViewportPaintBackground()` | 委托引擎绘制视口背景 |
| `VGui_GetPanel()` | 从引擎获取根 Panel 指针 |

### 13.2 控制台命令

| 命令 | 功能 |
|------|------|
| `+commandmenu` / `-commandmenu` | 打开/关闭命令菜单 |
| `ForceCloseCommandMenu` | 强制关闭命令菜单 |
| `special` | 玩家特殊能力 |
| `togglebrowser` | 切换服务器浏览器 |

---

## 十四、全局数据结构

| 数据 | 类型 | 说明 |
|------|------|------|
| `g_PlayerInfoList[]` | `hud_player_info_t` | 引擎提供的玩家基础信息 |
| `g_PlayerExtraInfo[]` | `player_info_t` | 客户端额外玩家信息（frags/deaths/team/class） |
| `g_TeamInfo[]` | `team_info_t` | 队伍信息（名称、分数、玩家数） |
| `g_IsSpectator[]` | `bool` | 玩家是否为观察者 |
| `iTeamColors[][]` | `int` | 5 种队伍颜色（橙/蓝/红/黄/绿） |
| `sTFClasses[]` | `char*` | 职业名称映射表 |
| `sLocalisedClasses[]` | `char*` | 本地化职业名称 |
| `sTFClassSelection[]` | `char*` | 职业选择名称 |
| `sTFValidClassInts[]` | `int` | 职业有效性位掩码 |

---

## 十五、功能启用状态总结

### 已启用的功能

| 模块 | 状态 | 说明 |
|------|------|------|
| Panel 基础系统 | ✅ 启用 | 位置/尺寸/父子关系/绘制/输入 |
| App 应用框架 | ✅ 启用 | 单例、输入状态、焦点、剪贴板 |
| 输入信号系统 | ✅ 启用 | InputSignal / ActionSignal 事件处理 |
| 绘制系统 | ✅ 启用 | paint / paintBackground / paintTraverse |
| 焦点导航 | ✅ 启用 | FocusNavGroup 键盘 Tab 切换 |
| Scheme 外观方案 | ✅ 启用 | 颜色、字体、分辨率自适应 |
| 队伍选择菜单 | ✅ 启用 | CTeamMenuPanel |
| 职业选择菜单 | ✅ 启用 | CClassMenuPanel |
| 记分板 | ✅ 启用 | ScorePanel |
| 观察者面板 | ✅ 启用 | SpectatorPanel |
| 服务器浏览器 | ✅ 启用 | ServerBrowser |
| MOTD 窗口 | ✅ 启用 | CMessageWindowPanel |
| 命令菜单 | ✅ 启用 | CCommandMenu（从 .txt 加载） |
| 自定义按钮体系 | ✅ 启用 | 14 种专用按钮 |
| 网络消息处理 | ✅ 启用 | 14 种服务器消息 |
| 控制台面板 | ✅ 启用 | ConsolePanel |
| 控制配置面板 | ✅ 启用 | ControlConfigPanel |

### 未使用的 VGUI 核心库控件

| 控件 | 说明 |
|------|------|
| `WizardPanel` | 向导面板 |
| `ConfigWizard` | 配置向导 |
| `Desktop` / `DesktopIcon` | 桌面图标 |
| `MiniApp` | 迷你应用 |
| `TaskBar` | 任务栏 |
| `TreeFolder` | 树形文件夹 |
| `PopupMenu` | 弹出菜单（项目使用自定义实现） |
| `CheckButton` / `RadioButton` | 复选框/单选按钮 |
| `ProgressBar` | 进度条 |
| `ComboBox` | 下拉组合框 |
| `MessageBox` | 消息对话框 |
| `EditPanel` | 多行编辑面板 |
| `TabPanel` | 标签面板 |
| `GridLayout` | 网格布局（代码中已注释） |

---

## 十六、条件编译与功能开关

| 条件 | 影响 |
|------|------|
| `gEngfuncs.pDemoAPI->IsPlayingback()` | Demo 回放时不打开菜单 |
| `gHUD.m_iIntermission` | 中场休息时只允许 MOTD |
| `gEngfuncs.IsSpectateOnly()` | HLTV 纯观察者不显示 MOTD |
| `ScreenWidth >= 640` | 分辨率缩放适配 |
| `ScreenWidth < 640` | 不显示职业图片 |
| `gHUD.m_Teamplay` | 决定记分板按队伍分组还是全部排序 |
| `hud_classautokill` | 切换职业时是否自动死亡 |
| `bitmapfonts` CVar | 是否加载位图字体 |
| `m_drawstatus` CVar | 观察者信息栏可见性 |
| `EV_TFC_IsAllyTeam()` | 是否显示敌方职业信息 |
| `GetClientVoiceMgr()->IsInSquelchMode()` | 语音屏蔽模式 |
