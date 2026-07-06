//========================================
// Sound Test Window - Interactive in-game window
// Uses Nuklear 4.13.3 header for definitions
//========================================

#ifndef SOUND_TEST_WINDOW_H
#define SOUND_TEST_WINDOW_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_STANDARD_IO
#include "../utils/Nuklear-4.13.3/src/nuklear.h"

class CSoundTestWindow
{
public:
	CSoundTestWindow();
	~CSoundTestWindow();

	void Init();
	void VidInit();
	void Shutdown();

	void Show(bool show);
	bool IsVisible() const { return m_bVisible; }

	void Draw();
	void MousePressed(int x, int y);
	void MouseMoved(int x, int y);
	void KeyPressed(int key);

private:
	void DrawWindow();
	void DrawButton(int x, int y, int w, int h, const char* text, bool hover, bool pressed);
	void PlayRandomSound();
	void ScanSoundFiles();

	bool m_bVisible;
	bool m_bInitialized;

	// Window position and size
	int m_iX, m_iY;
	int m_iWidth, m_iHeight;

	// Mouse state
	int m_iMouseX, m_iMouseY;
	bool m_bMousePressed;

	// Button hover/press states
	bool m_bBtnPlayHover;
	bool m_bBtnPlayPressed;
	bool m_bBtnCloseHover;
	bool m_bBtnClosePressed;

	// Sound file list
	static const int MAX_SOUND_FILES = 512;
	static const int MAX_SOUND_PATH = 256;
	char m_szSoundFiles[MAX_SOUND_FILES][MAX_SOUND_PATH];
	int m_iNumSoundFiles;

	// Last played sound
	char m_szLastPlayed[MAX_SOUND_PATH];
};

extern CSoundTestWindow g_SoundTestWindow;

#endif // SOUND_TEST_WINDOW_H
