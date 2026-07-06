//========================================
// Sound Test Window - Interactive in-game window
// Uses Nuklear 4.13.3 for type definitions
// GoldSrc API for rendering and input
//========================================

#include "SoundTestWindow.h"
#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"
#include "keydefs.h"
#include "winsani_in.h"
#include "windows.h"
#include "winsani_out.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <direct.h>

#undef PlaySound

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

// External
extern int g_iVisibleMouse;

CSoundTestWindow g_SoundTestWindow;

// Helper: draw filled rectangle
static void DrawRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	gEngfuncs.pfnFillRGBA(x, y, w, h, r, g, b, a);
}

// Helper: draw string
static void DrawStr(int x, int y, const char* text, int r, int g, int b)
{
	gEngfuncs.pfnDrawSetTextColor(r / 255.0f, g / 255.0f, b / 255.0f);
	gEngfuncs.pfnDrawConsoleString(x, y, (char*)text);
}

// Helper: get string width
static int GetStrWidth(const char* text)
{
	return ConsoleStringLen((char*)text);
}

//=============================================================================
// Constructor / Destructor
//=============================================================================

CSoundTestWindow::CSoundTestWindow()
{
	m_bVisible = false;
	m_bInitialized = false;
	m_iX = 0;
	m_iY = 0;
	m_iWidth = 400;
	m_iHeight = 180;
	m_iMouseX = 0;
	m_iMouseY = 0;
	m_bMousePressed = false;
	m_bBtnPlayHover = false;
	m_bBtnPlayPressed = false;
	m_bBtnCloseHover = false;
	m_bBtnClosePressed = false;
	m_iNumSoundFiles = 0;
	memset(m_szSoundFiles, 0, sizeof(m_szSoundFiles));
	memset(m_szLastPlayed, 0, sizeof(m_szLastPlayed));
}

CSoundTestWindow::~CSoundTestWindow()
{
}

//=============================================================================
// Init / Shutdown
//=============================================================================

void CSoundTestWindow::Init()
{
	srand((unsigned)time(NULL));
	m_bInitialized = true;
}

void CSoundTestWindow::VidInit()
{
	// Center window on screen
	int screenW = ScreenWidth;
	int screenH = ScreenHeight;
	m_iX = (screenW - m_iWidth) / 2;
	m_iY = (screenH - m_iHeight) / 2;

	ScanSoundFiles();
}

void CSoundTestWindow::Shutdown()
{
	m_iNumSoundFiles = 0;
}

//=============================================================================
// Show / Hide
//=============================================================================

void CSoundTestWindow::Show(bool show)
{
	if (m_bVisible == show)
		return;

	m_bVisible = show;

	if (show)
	{
		// Show mouse cursor, disable player movement
		g_iVisibleMouse = 1;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ShowCursor(TRUE);
		VidInit();
	}
	else
	{
		// Hide mouse cursor, re-enable player movement
		g_iVisibleMouse = 0;
		SetCursor(NULL);
		ShowCursor(FALSE);
	}
}

//=============================================================================
// Sound file scanning
//=============================================================================

void CSoundTestWindow::ScanSoundFiles()
{
	m_iNumSoundFiles = 0;

	const char* gameDir = gEngfuncs.pfnGetGameDirectory();
	if (!gameDir || !gameDir[0])
		return;

	char soundPath[MAX_PATH];
	snprintf(soundPath, sizeof(soundPath), "%s/sound", gameDir);

	struct _finddata_t fileinfo;
	char searchPath[MAX_PATH];

	// Search root sound directory for .wav files
	snprintf(searchPath, sizeof(searchPath), "%s/*.wav", soundPath);
	long handle = _findfirst(searchPath, &fileinfo);
	if (handle != -1)
	{
		do
		{
			if (m_iNumSoundFiles >= MAX_SOUND_FILES)
				break;
			snprintf(m_szSoundFiles[m_iNumSoundFiles], MAX_SOUND_PATH,
				"%s", fileinfo.name);
			m_iNumSoundFiles++;
		} while (_findnext(handle, &fileinfo) == 0);
		_findclose(handle);
	}

	// Search subdirectories (one level deep)
	snprintf(searchPath, sizeof(searchPath), "%s/*", soundPath);
	handle = _findfirst(searchPath, &fileinfo);
	if (handle != -1)
	{
		do
		{
			if (m_iNumSoundFiles >= MAX_SOUND_FILES)
				break;

			if (!(fileinfo.attrib & _A_SUBDIR))
				continue;

			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;

			// Save subdirectory name before inner _finddata_t overwrites it
			char dirName[MAX_PATH];
			strncpy(dirName, fileinfo.name, sizeof(dirName) - 1);
			dirName[sizeof(dirName) - 1] = '\0';

			char subSearch[MAX_PATH];
			snprintf(subSearch, sizeof(subSearch), "%s/%s/*.wav", soundPath, dirName);

			struct _finddata_t subfileinfo;
			long subHandle = _findfirst(subSearch, &subfileinfo);
			if (subHandle != -1)
			{
				do
				{
					if (m_iNumSoundFiles >= MAX_SOUND_FILES)
						break;
					snprintf(m_szSoundFiles[m_iNumSoundFiles], MAX_SOUND_PATH,
						"%s/%s", dirName, subfileinfo.name);
					m_iNumSoundFiles++;
				} while (_findnext(subHandle, &subfileinfo) == 0);
				_findclose(subHandle);
			}
		} while (_findnext(handle, &fileinfo) == 0);
		_findclose(handle);
	}
}

//=============================================================================
// Play random sound
//=============================================================================

void CSoundTestWindow::PlayRandomSound()
{
	if (m_iNumSoundFiles <= 0)
		return;

	int idx = rand() % m_iNumSoundFiles;
	char soundPath[MAX_SOUND_PATH];
	snprintf(soundPath, sizeof(soundPath), "%s", m_szSoundFiles[idx]);

	// Play the sound (volume 1.0)
	PlaySound(soundPath, 1.0f);

	// Record last played
	strncpy(m_szLastPlayed, soundPath, MAX_SOUND_PATH - 1);
	m_szLastPlayed[MAX_SOUND_PATH - 1] = '\0';
}

//=============================================================================
// Draw
//=============================================================================

void CSoundTestWindow::Draw()
{
	if (!m_bVisible || !m_bInitialized)
		return;

	// Keep the Windows cursor visible; GoldSrc may reset it every frame
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	DrawWindow();
}

void CSoundTestWindow::DrawWindow()
{
	int x = m_iX;
	int y = m_iY;
	int w = m_iWidth;
	int h = m_iHeight;

	// Window background
	DrawRect(x, y, w, h, 30, 30, 35, 240);

	// Window border
	DrawRect(x, y, w, 2, 80, 80, 90, 255);                         // top
	DrawRect(x, y + h - 2, w, 2, 80, 80, 90, 255);                 // bottom
	DrawRect(x, y, 2, h, 80, 80, 90, 255);                         // left
	DrawRect(x + w - 2, y, 2, h, 80, 80, 90, 255);                 // right

	// Title bar
	int titleH = 28;
	DrawRect(x + 2, y + 2, w - 4, titleH, 50, 50, 60, 255);
	DrawStr(x + 10, y + 8, "Sound Test Window", 255, 170, 0);

	// Close button (top-right)
	int closeBtnSize = 20;
	int closeX = x + w - closeBtnSize - 6;
	int closeY = y + 4;
	DrawButton(closeX, closeY, closeBtnSize, closeBtnSize, "X",
		m_bBtnCloseHover, m_bBtnClosePressed);

	// Play button (center)
	int btnW = 200;
	int btnH = 40;
	int btnX = x + (w - btnW) / 2;
	int btnY = y + titleH + 20;
	DrawButton(btnX, btnY, btnW, btnH, "Play Random Sound",
		m_bBtnPlayHover, m_bBtnPlayPressed);

	// Info text
	int infoY = btnY + btnH + 15;
	char infoText[256];
	if (m_iNumSoundFiles > 0)
	{
		snprintf(infoText, sizeof(infoText), "Found %d sound files", m_iNumSoundFiles);
		DrawStr(x + 10, infoY, infoText, 180, 180, 180);
	}
	else
	{
		DrawStr(x + 10, infoY, "No sound files found!", 255, 80, 80);
	}

	// Last played
	if (m_szLastPlayed[0])
	{
		char lastText[300];
		snprintf(lastText, sizeof(lastText), "Last: %s", m_szLastPlayed);
		DrawStr(x + 10, infoY + 16, lastText, 100, 255, 100);
	}

	// Hint text
	DrawStr(x + 10, y + h - 18, "Press ESC to close", 100, 100, 100);
}

void CSoundTestWindow::DrawButton(int x, int y, int w, int h, const char* text,
	bool hover, bool pressed)
{
	int r, g, b, a;

	if (pressed)
	{
		r = 60; g = 90; b = 140; a = 255;
	}
	else if (hover)
	{
		r = 50; g = 70; b = 110; a = 255;
	}
	else
	{
		r = 40; g = 55; b = 85; a = 255;
	}

	DrawRect(x, y, w, h, r, g, b, a);

	// Button border
	DrawRect(x, y, w, 1, r + 40, g + 40, b + 40, 255);
	DrawRect(x, y + h - 1, w, 1, r + 20, g + 20, b + 20, 255);
	DrawRect(x, y, 1, h, r + 40, g + 40, b + 40, 255);
	DrawRect(x + w - 1, y, 1, h, r + 20, g + 20, b + 20, 255);

	// Button text (centered)
	int textW = GetStrWidth((char*)text);
	int textX = x + (w - textW) / 2;
	int textY = y + (h - 12) / 2 + 2;
	DrawStr(textX, textY, text, 255, 255, 255);
}

//=============================================================================
// Input handling
//=============================================================================

void CSoundTestWindow::MousePressed(int x, int y)
{
	if (!m_bVisible)
		return;

	m_iMouseX = x;
	m_iMouseY = y;
	m_bMousePressed = true;

	// Check close button
	int closeBtnSize = 20;
	int closeX = m_iX + m_iWidth - closeBtnSize - 6;
	int closeY = m_iY + 4;

	if (x >= closeX && x < closeX + closeBtnSize &&
		y >= closeY && y < closeY + closeBtnSize)
	{
		m_bBtnClosePressed = true;
		Show(false);
		return;
	}

	// Check play button
	int btnW = 200;
	int btnH = 40;
	int btnX = m_iX + (m_iWidth - btnW) / 2;
	int btnY = m_iY + 28 + 20;

	if (x >= btnX && x < btnX + btnW &&
		y >= btnY && y < btnY + btnH)
	{
		m_bBtnPlayPressed = true;
		PlayRandomSound();
		return;
	}
}

void CSoundTestWindow::MouseMoved(int x, int y)
{
	if (!m_bVisible)
		return;

	m_iMouseX = x;
	m_iMouseY = y;
	m_bMousePressed = false;

	// Check close button hover
	int closeBtnSize = 20;
	int closeX = m_iX + m_iWidth - closeBtnSize - 6;
	int closeY = m_iY + 4;
	m_bBtnCloseHover = (x >= closeX && x < closeX + closeBtnSize &&
		y >= closeY && y < closeY + closeBtnSize);

	// Check play button hover
	int btnW = 200;
	int btnH = 40;
	int btnX = m_iX + (m_iWidth - btnW) / 2;
	int btnY = m_iY + 28 + 20;
	m_bBtnPlayHover = (x >= btnX && x < btnX + btnW &&
		y >= btnY && y < btnY + btnH);

	// Reset pressed states when not clicking
	m_bBtnPlayPressed = false;
	m_bBtnClosePressed = false;
}

void CSoundTestWindow::KeyPressed(int key)
{
	if (!m_bVisible)
		return;

	if (key == K_ESCAPE)
	{
		Show(false);
	}
}
