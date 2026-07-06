//========= Copyright  1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: New Scoreboard using GoldSrc HUD Drawing API
//
//=============================================================================

#include "NuklearScoreboard.h"
#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"
#include "keydefs.h"
#include "../game_shared/voice_status.h"
#include <stdio.h>

// External variables and function declarations
extern int g_iTeamNumber;
int EV_TFC_IsAllyTeam(int iTeam1, int iTeam2);

// Team colors
TeamColor_t g_TeamColors[MAX_TEAM_COLORS] = {
	{ 255, 0, 0 },
	{ 0, 0, 255 },
	{ 0, 255, 0 },
	{ 255, 255, 0 },
	{ 255, 128, 0 },
	{ 128, 0, 255 },
	{ 0, 255, 255 },
	{ 255, 0, 255 },
};

// Global instance
CNuklearScoreboard g_NuklearScoreboard;

//=============================================================================
// Global entry functions - called by HUD_DrawTransparentTriangles
//=============================================================================
void DrawNuklearScoreboard()
{
	g_NuklearScoreboard.Draw();
}

//=============================================================================
// Global entry functions - called by HUD_VidInit
//=============================================================================
void InitNuklearScoreboard()
{
	g_NuklearScoreboard.VidInit();
}

//=============================================================================
// Constructor and destructor
//=============================================================================

CNuklearScoreboard::CNuklearScoreboard()
{
	m_bVisible = false;
	m_bNeedsUpdate = false;
	m_bConfigLoaded = false;
	m_iX = 0;
	m_iY = 0;
	m_iWidth = 600;
	m_iHeight = 400;
	m_iContentY = 0;
	m_iScrollOffset = 0;
	m_iMaxVisibleRows = 0;
	m_iTotalContentHeight = 0;
	m_iHighlightRow = -1;
	m_iHighlightCol = -1;
	m_iNumTeams = 0;
	m_iNumSorted = 0;
	m_iLastKilledBy = 0;
	m_fLastKillTime = 0;
	m_iPlayerNum = 0;
	m_iCharWidth = 0;
	m_iCharHeight = 0;
	m_iTitleCharHeight = 0;
	m_bSquelchMode = false;

	memset(m_szServerName, 0, sizeof(m_szServerName));
	m_iCfgWidth = 0;
	m_iCfgHeight = 0;
	m_iCfgX = 0;
	m_iCfgY = 0;
	memset(m_szCfgTitle, 0, sizeof(m_szCfgTitle));
	memset(m_szCfgSubtitle, 0, sizeof(m_szCfgSubtitle));
	memset(m_szCfgColName, 0, sizeof(m_szCfgColName));
	memset(m_szCfgColTeam, 0, sizeof(m_szCfgColTeam));
	memset(m_szCfgColClass, 0, sizeof(m_szCfgColClass));
	memset(m_szCfgColScore, 0, sizeof(m_szCfgColScore));
	memset(m_szCfgColDeaths, 0, sizeof(m_szCfgColDeaths));
	memset(m_szCfgColPing, 0, sizeof(m_szCfgColPing));
	memset(m_szCfgColVoice, 0, sizeof(m_szCfgColVoice));
	memset(m_szCfgSpectators, 0, sizeof(m_szCfgSpectators));
	memset(m_szCfgPlayerCountFormat, 0, sizeof(m_szCfgPlayerCountFormat));
	memset(m_szCfgTeamFormat, 0, sizeof(m_szCfgTeamFormat));
	memset(m_szCfgIndexFormat, 0, sizeof(m_szCfgIndexFormat));
	memset(m_szCfgNameFormat, 0, sizeof(m_szCfgNameFormat));
	memset(m_szCfgScoreFormat, 0, sizeof(m_szCfgScoreFormat));
	memset(m_szCfgDeathFormat, 0, sizeof(m_szCfgDeathFormat));
	memset(m_szCfgPingFormat, 0, sizeof(m_szCfgPingFormat));
	memset(m_szCfgVoiceBanned, 0, sizeof(m_szCfgVoiceBanned));
	memset(m_szCfgVoiceSpeakingBlink, 0, sizeof(m_szCfgVoiceSpeakingBlink));
	memset(m_szCfgVoiceSpeaking, 0, sizeof(m_szCfgVoiceSpeaking));
	memset(m_szCfgVoiceNeverSpoke, 0, sizeof(m_szCfgVoiceNeverSpoke));
	memset(m_szCfgVoiceNotSpeaking, 0, sizeof(m_szCfgVoiceNotSpeaking));
	memset(m_szCfgSprVoiceBanned, 0, sizeof(m_szCfgSprVoiceBanned));
	memset(m_szCfgSprVoiceSpeaking, 0, sizeof(m_szCfgSprVoiceSpeaking));
	memset(m_szCfgSprVoiceSpeakingBlink, 0, sizeof(m_szCfgSprVoiceSpeakingBlink));
	memset(m_szCfgSprVoiceNeverSpoke, 0, sizeof(m_szCfgSprVoiceNeverSpoke));
	memset(m_szCfgSprVoiceNotSpeaking, 0, sizeof(m_szCfgSprVoiceNotSpeaking));
	memset(m_szCfgCmdHide, 0, sizeof(m_szCfgCmdHide));
	memset(m_szCfgCmdVoiceMute, 0, sizeof(m_szCfgCmdVoiceMute));
	memset(m_szCfgCmdVoiceUnmute, 0, sizeof(m_szCfgCmdVoiceUnmute));
	memset(m_iCfgBgColor, 0, sizeof(m_iCfgBgColor));
	memset(m_iCfgBorderColor, 0, sizeof(m_iCfgBorderColor));
	memset(m_iCfgHeaderBgColor, 0, sizeof(m_iCfgHeaderBgColor));
	memset(m_iCfgHeaderTextColor, 0, sizeof(m_iCfgHeaderTextColor));
	memset(m_iCfgRowEvenColor, 0, sizeof(m_iCfgRowEvenColor));
	memset(m_iCfgRowOddColor, 0, sizeof(m_iCfgRowOddColor));
	memset(m_iCfgSelfHighlightColor, 0, sizeof(m_iCfgSelfHighlightColor));
	memset(m_iCfgPlayerCountColor, 0, sizeof(m_iCfgPlayerCountColor));
	memset(m_iCfgSubtitleColor, 0, sizeof(m_iCfgSubtitleColor));
	memset(m_iCfgIndexColor, 0, sizeof(m_iCfgIndexColor));
	memset(m_iCfgDeathColor, 0, sizeof(m_iCfgDeathColor));
	memset(m_iCfgSelfPlayerColor, 0, sizeof(m_iCfgSelfPlayerColor));
	memset(m_iCfgVoiceBannedColor, 0, sizeof(m_iCfgVoiceBannedColor));
	memset(m_iCfgVoiceSpeakingColor, 0, sizeof(m_iCfgVoiceSpeakingColor));
	memset(m_iCfgVoiceNeverSpokeColor, 0, sizeof(m_iCfgVoiceNeverSpokeColor));
	memset(m_iCfgVoiceNotSpeakingColor, 0, sizeof(m_iCfgVoiceNotSpeakingColor));
	memset(m_iCfgPingGoodColor, 0, sizeof(m_iCfgPingGoodColor));
	memset(m_iCfgPingMediumColor, 0, sizeof(m_iCfgPingMediumColor));
	memset(m_iCfgPingBadColor, 0, sizeof(m_iCfgPingBadColor));
	m_iCfgPingGoodThreshold = 0;
	m_iCfgPingMediumThreshold = 0;
	memset(m_iCfgSpectatorBgColor, 0, sizeof(m_iCfgSpectatorBgColor));
	memset(m_iCfgSpectatorTextColor, 0, sizeof(m_iCfgSpectatorTextColor));
	memset(m_iCfgSpectatorBorderColor, 0, sizeof(m_iCfgSpectatorBorderColor));
	memset(m_iCfgSeparatorColor, 0, sizeof(m_iCfgSeparatorColor));
	memset(m_iCfgKillHighlightColor, 0, sizeof(m_iCfgKillHighlightColor));
	m_iCfgColNameWidth = 0;
	m_iCfgColTeamWidth = 0;
	m_iCfgColClassWidth = 0;
	m_iCfgColScoreWidth = 0;
	m_iCfgColDeathsWidth = 0;
	m_iCfgColPingWidth = 0;
	m_iCfgColVoiceWidth = 0;
	m_iCfgRowHeight = 0;
	m_iCfgTeamRowHeight = 0;
	m_iCfgHeaderHeight = 0;
	m_iCfgBorderWidth = 0;
	m_iCfgPadding = 0;
	memset(m_TeamInfo, 0, sizeof(m_TeamInfo));
	memset(m_PlayerInfo, 0, sizeof(m_PlayerInfo));
	memset(m_SortedPlayers, 0, sizeof(m_SortedPlayers));
	memset(m_bHasBeenSorted, 0, sizeof(m_bHasBeenSorted));
	memset(m_iMutedPlayers, 0, sizeof(m_iMutedPlayers));

	m_hSprVoiceBanned = 0;
	m_hSprVoiceSpeaking = 0;
	m_hSprVoiceSpeakingBlink = 0;
	m_hSprVoiceNeverSpoke = 0;
	m_hSprVoiceNotSpeaking = 0;
	m_bVoiceSpritesLoaded = false;

	SetDefaultConfig();
}

CNuklearScoreboard::~CNuklearScoreboard()
{
	Shutdown();
}

//=============================================================================
// Init and shutdown
//=============================================================================

void CNuklearScoreboard::Init()
{
	// Initialize font info
	SCREENINFO scrinfo;
	scrinfo.iSize = sizeof(SCREENINFO);
	GetScreenInfo(&scrinfo);
	m_iCharWidth = scrinfo.charWidths['A'] ? scrinfo.charWidths['A'] : 8;
	m_iCharHeight = scrinfo.iCharHeight ? scrinfo.iCharHeight : 14;
	m_iTitleCharHeight = m_iCharHeight + 4;

	// Initialize team colors
	g_TeamColors[0].r = 255; g_TeamColors[0].g = 60; g_TeamColors[0].b = 60;
	g_TeamColors[1].r = 60; g_TeamColors[1].g = 60; g_TeamColors[1].b = 255;
	g_TeamColors[2].r = 60; g_TeamColors[2].g = 180; g_TeamColors[2].b = 60;
	g_TeamColors[3].r = 255; g_TeamColors[3].g = 220; g_TeamColors[3].b = 60;
	g_TeamColors[4].r = 255; g_TeamColors[4].g = 140; g_TeamColors[4].b = 40;
	g_TeamColors[5].r = 180; g_TeamColors[5].g = 60; g_TeamColors[5].b = 255;
	g_TeamColors[6].r = 60; g_TeamColors[6].g = 220; g_TeamColors[6].b = 220;
	g_TeamColors[7].r = 255; g_TeamColors[7].g = 100; g_TeamColors[7].b = 180;

	// Load config
	LoadConfig();

	CalculateLayout();

	// Load voice SPR icons
	LoadVoiceSprites();
}

void CNuklearScoreboard::VidInit()
{
	LoadConfig();
	CalculateLayout();
	LoadVoiceSprites();
}

void CNuklearScoreboard::Shutdown()
{
	FreeVoiceSprites();
}

// Voice state enum (used internally for icon selection)
#define VOICE_STATE_BANNED			0
#define VOICE_STATE_SPEAKING		1
#define VOICE_STATE_SPEAKING_BLINK	2
#define VOICE_STATE_NEVER_SPOKEN	3
#define VOICE_STATE_NOT_SPEAKING	4

void CNuklearScoreboard::LoadVoiceSprites()
{
	FreeVoiceSprites();

	if (m_szCfgSprVoiceBanned[0])
		m_hSprVoiceBanned = SPR_Load(m_szCfgSprVoiceBanned);
	if (m_szCfgSprVoiceSpeaking[0])
		m_hSprVoiceSpeaking = SPR_Load(m_szCfgSprVoiceSpeaking);
	if (m_szCfgSprVoiceSpeakingBlink[0])
		m_hSprVoiceSpeakingBlink = SPR_Load(m_szCfgSprVoiceSpeakingBlink);
	if (m_szCfgSprVoiceNeverSpoke[0])
		m_hSprVoiceNeverSpoke = SPR_Load(m_szCfgSprVoiceNeverSpoke);
	if (m_szCfgSprVoiceNotSpeaking[0])
		m_hSprVoiceNotSpeaking = SPR_Load(m_szCfgSprVoiceNotSpeaking);

	m_bVoiceSpritesLoaded = (m_hSprVoiceBanned != 0 ||
		m_hSprVoiceSpeaking != 0 ||
		m_hSprVoiceSpeakingBlink != 0 ||
		m_hSprVoiceNeverSpoke != 0 ||
		m_hSprVoiceNotSpeaking != 0);
}

void CNuklearScoreboard::FreeVoiceSprites()
{
	// SPR handles are reference-counted by the engine;
	// we just clear our references.
	m_hSprVoiceBanned = 0;
	m_hSprVoiceSpeaking = 0;
	m_hSprVoiceSpeakingBlink = 0;
	m_hSprVoiceNeverSpoke = 0;
	m_hSprVoiceNotSpeaking = 0;
	m_bVoiceSpritesLoaded = false;
}

void CNuklearScoreboard::DrawVoiceIcon(int x, int y, int voiceState, int r, int g, int b)
{
	HSPRITE hSpr = 0;

	switch (voiceState)
	{
	case VOICE_STATE_BANNED:
		hSpr = m_hSprVoiceBanned;
		break;
	case VOICE_STATE_SPEAKING:
		hSpr = m_hSprVoiceSpeaking;
		break;
	case VOICE_STATE_SPEAKING_BLINK:
		hSpr = m_hSprVoiceSpeakingBlink;
		break;
	case VOICE_STATE_NEVER_SPOKEN:
		hSpr = m_hSprVoiceNeverSpoke;
		break;
	case VOICE_STATE_NOT_SPEAKING:
	default:
		hSpr = m_hSprVoiceNotSpeaking;
		break;
	}

	if (hSpr != 0)
	{
		SPR_Set(hSpr, r, g, b);
		SPR_DrawAdditive(0, x, y, NULL);
	}
}

//=============================================================================
// Config loading
//=============================================================================

void CNuklearScoreboard::SetDefaultConfig()
{
	// All text strings MUST come from Scoreboard.txt - no hardcoded strings.
	// Strings default to empty; only numeric/color fallbacks are provided so
	// the layout still renders if the config file is missing.

	// Default dimensions
	m_iCfgWidth = 600;
	m_iCfgHeight = 400;
	m_iCfgX = -1;
	m_iCfgY = -1;

	// Empty all text fields (must be loaded from Scoreboard.txt)
	m_szCfgTitle[0] = '\0';
	m_szCfgSubtitle[0] = '\0';
	m_szCfgColName[0] = '\0';
	m_szCfgColTeam[0] = '\0';
	m_szCfgColClass[0] = '\0';
	m_szCfgColScore[0] = '\0';
	m_szCfgColDeaths[0] = '\0';
	m_szCfgColPing[0] = '\0';
	m_szCfgColVoice[0] = '\0';
	m_szCfgSpectators[0] = '\0';
	m_szCfgPlayerCountFormat[0] = '\0';
	m_szCfgTeamFormat[0] = '\0';
	m_szCfgIndexFormat[0] = '\0';
	m_szCfgNameFormat[0] = '\0';
	m_szCfgScoreFormat[0] = '\0';
	m_szCfgDeathFormat[0] = '\0';
	m_szCfgPingFormat[0] = '\0';
	m_szCfgVoiceBanned[0] = '\0';
	m_szCfgVoiceSpeakingBlink[0] = '\0';
	m_szCfgVoiceSpeaking[0] = '\0';
	m_szCfgVoiceNeverSpoke[0] = '\0';
	m_szCfgVoiceNotSpeaking[0] = '\0';
	m_szCfgSprVoiceBanned[0] = '\0';
	m_szCfgSprVoiceSpeaking[0] = '\0';
	m_szCfgSprVoiceSpeakingBlink[0] = '\0';
	m_szCfgSprVoiceNeverSpoke[0] = '\0';
	m_szCfgSprVoiceNotSpeaking[0] = '\0';
	m_szCfgCmdHide[0] = '\0';
	m_szCfgCmdVoiceMute[0] = '\0';
	m_szCfgCmdVoiceUnmute[0] = '\0';

	// Default colors
	m_iCfgBgColor[0] = 0; m_iCfgBgColor[1] = 0; m_iCfgBgColor[2] = 0; m_iCfgBgColor[3] = 220;
	m_iCfgBorderColor[0] = 60; m_iCfgBorderColor[1] = 60; m_iCfgBorderColor[2] = 60; m_iCfgBorderColor[3] = 255;
	m_iCfgHeaderBgColor[0] = 30; m_iCfgHeaderBgColor[1] = 30; m_iCfgHeaderBgColor[2] = 30; m_iCfgHeaderBgColor[3] = 200;
	m_iCfgHeaderTextColor[0] = 255; m_iCfgHeaderTextColor[1] = 170; m_iCfgHeaderTextColor[2] = 0; m_iCfgHeaderTextColor[3] = 255;
	m_iCfgRowEvenColor[0] = 25; m_iCfgRowEvenColor[1] = 25; m_iCfgRowEvenColor[2] = 25; m_iCfgRowEvenColor[3] = 180;
	m_iCfgRowOddColor[0] = 20; m_iCfgRowOddColor[1] = 20; m_iCfgRowOddColor[2] = 20; m_iCfgRowOddColor[3] = 180;
	m_iCfgSelfHighlightColor[0] = 100; m_iCfgSelfHighlightColor[1] = 100; m_iCfgSelfHighlightColor[2] = 255; m_iCfgSelfHighlightColor[3] = 60;

	// Default text colors
	m_iCfgPlayerCountColor[0] = 180; m_iCfgPlayerCountColor[1] = 180; m_iCfgPlayerCountColor[2] = 180; m_iCfgPlayerCountColor[3] = 255;
	m_iCfgSubtitleColor[0] = 100; m_iCfgSubtitleColor[1] = 100; m_iCfgSubtitleColor[2] = 100; m_iCfgSubtitleColor[3] = 255;
	m_iCfgIndexColor[0] = 100; m_iCfgIndexColor[1] = 100; m_iCfgIndexColor[2] = 100; m_iCfgIndexColor[3] = 255;
	m_iCfgDeathColor[0] = 200; m_iCfgDeathColor[1] = 200; m_iCfgDeathColor[2] = 200; m_iCfgDeathColor[3] = 255;
	m_iCfgSelfPlayerColor[0] = 255; m_iCfgSelfPlayerColor[1] = 255; m_iCfgSelfPlayerColor[2] = 255; m_iCfgSelfPlayerColor[3] = 255;

	// Default voice status colors
	m_iCfgVoiceBannedColor[0] = 255; m_iCfgVoiceBannedColor[1] = 50; m_iCfgVoiceBannedColor[2] = 50; m_iCfgVoiceBannedColor[3] = 255;
	m_iCfgVoiceSpeakingColor[0] = 255; m_iCfgVoiceSpeakingColor[1] = 170; m_iCfgVoiceSpeakingColor[2] = 0; m_iCfgVoiceSpeakingColor[3] = 255;
	m_iCfgVoiceNeverSpokeColor[0] = 100; m_iCfgVoiceNeverSpokeColor[1] = 100; m_iCfgVoiceNeverSpokeColor[2] = 100; m_iCfgVoiceNeverSpokeColor[3] = 255;
	m_iCfgVoiceNotSpeakingColor[0] = 150; m_iCfgVoiceNotSpeakingColor[1] = 150; m_iCfgVoiceNotSpeakingColor[2] = 150; m_iCfgVoiceNotSpeakingColor[3] = 255;

	// Default ping colors and thresholds
	m_iCfgPingGoodColor[0] = 100; m_iCfgPingGoodColor[1] = 200; m_iCfgPingGoodColor[2] = 100; m_iCfgPingGoodColor[3] = 255;
	m_iCfgPingMediumColor[0] = 255; m_iCfgPingMediumColor[1] = 200; m_iCfgPingMediumColor[2] = 100; m_iCfgPingMediumColor[3] = 255;
	m_iCfgPingBadColor[0] = 255; m_iCfgPingBadColor[1] = 100; m_iCfgPingBadColor[2] = 100; m_iCfgPingBadColor[3] = 255;
	m_iCfgPingGoodThreshold = 80;
	m_iCfgPingMediumThreshold = 150;

	// Default spectator row colors
	m_iCfgSpectatorBgColor[0] = 40; m_iCfgSpectatorBgColor[1] = 40; m_iCfgSpectatorBgColor[2] = 40; m_iCfgSpectatorBgColor[3] = 200;
	m_iCfgSpectatorTextColor[0] = 100; m_iCfgSpectatorTextColor[1] = 100; m_iCfgSpectatorTextColor[2] = 100; m_iCfgSpectatorTextColor[3] = 255;
	m_iCfgSpectatorBorderColor[0] = 100; m_iCfgSpectatorBorderColor[1] = 100; m_iCfgSpectatorBorderColor[2] = 100; m_iCfgSpectatorBorderColor[3] = 255;

	// Default separator and kill highlight
	m_iCfgSeparatorColor[0] = 50; m_iCfgSeparatorColor[1] = 50; m_iCfgSeparatorColor[2] = 50; m_iCfgSeparatorColor[3] = 255;
	m_iCfgKillHighlightColor[0] = 255; m_iCfgKillHighlightColor[1] = 0; m_iCfgKillHighlightColor[2] = 0; m_iCfgKillHighlightColor[3] = 255;

	// Default scrollbar colors
	m_iCfgScrollbarColor[0] = 40; m_iCfgScrollbarColor[1] = 40; m_iCfgScrollbarColor[2] = 40; m_iCfgScrollbarColor[3] = 200;
	m_iCfgScrollbarThumbColor[0] = 120; m_iCfgScrollbarThumbColor[1] = 120; m_iCfgScrollbarThumbColor[2] = 120; m_iCfgScrollbarThumbColor[3] = 220;

	// Default column widths
	m_iCfgColNameWidth = 160;
	m_iCfgColTeamWidth = 70;
	m_iCfgColClassWidth = 80;
	m_iCfgColScoreWidth = 50;
	m_iCfgColDeathsWidth = 50;
	m_iCfgColPingWidth = 50;
	m_iCfgColVoiceWidth = 40;

	// Default row heights
	m_iCfgRowHeight = 20;
	m_iCfgTeamRowHeight = 24;
	m_iCfgHeaderHeight = 36;

	// Default border and padding
	m_iCfgBorderWidth = 2;
	m_iCfgPadding = 6;
}

static void ParseColor(const char* str, int color[4])
{
	int r, g, b, a;
	if (sscanf(str, "%d %d %d %d", &r, &g, &b, &a) == 4)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}
}

void CNuklearScoreboard::LoadConfig()
{
	SetDefaultConfig();

	// Load the config file. Priority:
	// 1. Language-specific folder (e.g., gasi_schinese)
	// 2. Main mod folder (e.g., gasi)
	// 3. COM_LoadFile as fallback for VFS/packed files
	char* pfile = NULL;
	bool bMalloced = false;

	const char* gamedir = gEngfuncs.pfnGetGameDirectory();
	if (gamedir && gamedir[0])
	{
		// List of language suffixes to try (in priority order)
		const char* langSuffixes[] = { "_schinese", "_tchinese", "_english", "" };
		char fullPath[512];

		for (int i = 0; i < sizeof(langSuffixes) / sizeof(langSuffixes[0]) && !pfile; i++)
		{
			if (langSuffixes[i][0] == '\0')
			{
				// Empty suffix = main mod directory
				snprintf(fullPath, sizeof(fullPath), "%s/Scoreboard.txt", gamedir);
			}
			else
			{
				snprintf(fullPath, sizeof(fullPath), "%s%s/Scoreboard.txt", gamedir, langSuffixes[i]);
			}

			FILE* fp = fopen(fullPath, "rb");
			if (fp)
			{
				fseek(fp, 0, SEEK_END);
				long size = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if (size > 0)
				{
					pfile = (char*)malloc(size + 1);
					if (pfile)
					{
						size_t rd = fread(pfile, 1, size, fp);
						pfile[rd] = '\0';
						bMalloced = true;
					}
				}
				fclose(fp);
			}
		}
	}

	if (!pfile)
	{
		pfile = (char*)gEngfuncs.COM_LoadFile("Scoreboard.txt", 5, NULL);
	}

	if (!pfile)
	{
		m_bConfigLoaded = true;
		return;
	}

	char token[1024];
	char value[1024];
	char* p = pfile;

	while (p && *p)
	{
		// Skip whitespace and comments
		while (*p && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r'))
			p++;

		if (*p == '/' && *(p + 1) == '/')
		{
			while (*p && *p != '\n')
				p++;
			continue;
		}

		if (!*p)
			break;

		// Read key name
		int i = 0;
		while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r' && i < (int)sizeof(token) - 1)
		{
			token[i++] = *p++;
		}
		token[i] = '\0';

		// Skip whitespace
		while (*p && (*p == ' ' || *p == '\t'))
			p++;

		// Read value (quoted)
		if (*p == '"')
		{
			p++;
			i = 0;
			while (*p && *p != '"' && i < (int)sizeof(value) - 1)
			{
				value[i++] = *p++;
			}
			value[i] = '\0';
			if (*p == '"')
				p++;
		}
		else
		{
			// Read unquoted value
			i = 0;
			while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r' && i < (int)sizeof(value) - 1)
			{
				value[i++] = *p++;
			}
			value[i] = '\0';
		}

		// Parse config entries
		if (strcmp(token, "title") == 0)
			snprintf(m_szCfgTitle, sizeof(m_szCfgTitle), "%s", value);
		else if (strcmp(token, "subtitle") == 0)
			snprintf(m_szCfgSubtitle, sizeof(m_szCfgSubtitle), "%s", value);
		else if (strcmp(token, "colName") == 0)
			snprintf(m_szCfgColName, sizeof(m_szCfgColName), "%s", value);
		else if (strcmp(token, "colTeam") == 0)
			snprintf(m_szCfgColTeam, sizeof(m_szCfgColTeam), "%s", value);
		else if (strcmp(token, "colClass") == 0)
			snprintf(m_szCfgColClass, sizeof(m_szCfgColClass), "%s", value);
		else if (strcmp(token, "colScore") == 0)
			snprintf(m_szCfgColScore, sizeof(m_szCfgColScore), "%s", value);
		else if (strcmp(token, "colDeaths") == 0)
			snprintf(m_szCfgColDeaths, sizeof(m_szCfgColDeaths), "%s", value);
		else if (strcmp(token, "colPing") == 0)
			snprintf(m_szCfgColPing, sizeof(m_szCfgColPing), "%s", value);
		else if (strcmp(token, "colVoice") == 0)
			snprintf(m_szCfgColVoice, sizeof(m_szCfgColVoice), "%s", value);
		else if (strcmp(token, "spectators") == 0)
			snprintf(m_szCfgSpectators, sizeof(m_szCfgSpectators), "%s", value);
		else if (strcmp(token, "playerCountFormat") == 0)
			snprintf(m_szCfgPlayerCountFormat, sizeof(m_szCfgPlayerCountFormat), "%s", value);
		else if (strcmp(token, "teamFormat") == 0)
			snprintf(m_szCfgTeamFormat, sizeof(m_szCfgTeamFormat), "%s", value);
		else if (strcmp(token, "indexFormat") == 0)
			snprintf(m_szCfgIndexFormat, sizeof(m_szCfgIndexFormat), "%s", value);
		else if (strcmp(token, "nameFormat") == 0)
			snprintf(m_szCfgNameFormat, sizeof(m_szCfgNameFormat), "%s", value);
		else if (strcmp(token, "scoreFormat") == 0)
			snprintf(m_szCfgScoreFormat, sizeof(m_szCfgScoreFormat), "%s", value);
		else if (strcmp(token, "deathFormat") == 0)
			snprintf(m_szCfgDeathFormat, sizeof(m_szCfgDeathFormat), "%s", value);
		else if (strcmp(token, "pingFormat") == 0)
			snprintf(m_szCfgPingFormat, sizeof(m_szCfgPingFormat), "%s", value);
		else if (strcmp(token, "voiceBanned") == 0)
			snprintf(m_szCfgVoiceBanned, sizeof(m_szCfgVoiceBanned), "%s", value);
		else if (strcmp(token, "voiceSpeakingBlink") == 0)
			snprintf(m_szCfgVoiceSpeakingBlink, sizeof(m_szCfgVoiceSpeakingBlink), "%s", value);
		else if (strcmp(token, "voiceSpeaking") == 0)
			snprintf(m_szCfgVoiceSpeaking, sizeof(m_szCfgVoiceSpeaking), "%s", value);
		else if (strcmp(token, "voiceNeverSpoke") == 0)
			snprintf(m_szCfgVoiceNeverSpoke, sizeof(m_szCfgVoiceNeverSpoke), "%s", value);
		else if (strcmp(token, "voiceNotSpeaking") == 0)
			snprintf(m_szCfgVoiceNotSpeaking, sizeof(m_szCfgVoiceNotSpeaking), "%s", value);
		else if (strcmp(token, "sprVoiceBanned") == 0)
			snprintf(m_szCfgSprVoiceBanned, sizeof(m_szCfgSprVoiceBanned), "%s", value);
		else if (strcmp(token, "sprVoiceSpeaking") == 0)
			snprintf(m_szCfgSprVoiceSpeaking, sizeof(m_szCfgSprVoiceSpeaking), "%s", value);
		else if (strcmp(token, "sprVoiceSpeakingBlink") == 0)
			snprintf(m_szCfgSprVoiceSpeakingBlink, sizeof(m_szCfgSprVoiceSpeakingBlink), "%s", value);
		else if (strcmp(token, "sprVoiceNeverSpoke") == 0)
			snprintf(m_szCfgSprVoiceNeverSpoke, sizeof(m_szCfgSprVoiceNeverSpoke), "%s", value);
		else if (strcmp(token, "sprVoiceNotSpeaking") == 0)
			snprintf(m_szCfgSprVoiceNotSpeaking, sizeof(m_szCfgSprVoiceNotSpeaking), "%s", value);
		else if (strcmp(token, "cmdHide") == 0)
			snprintf(m_szCfgCmdHide, sizeof(m_szCfgCmdHide), "%s", value);
		else if (strcmp(token, "cmdVoiceMute") == 0)
			snprintf(m_szCfgCmdVoiceMute, sizeof(m_szCfgCmdVoiceMute), "%s", value);
		else if (strcmp(token, "cmdVoiceUnmute") == 0)
			snprintf(m_szCfgCmdVoiceUnmute, sizeof(m_szCfgCmdVoiceUnmute), "%s", value);
		else if (strcmp(token, "width") == 0)
			m_iCfgWidth = atoi(value);
		else if (strcmp(token, "height") == 0)
			m_iCfgHeight = atoi(value);
		else if (strcmp(token, "x") == 0)
			m_iCfgX = atoi(value);
		else if (strcmp(token, "y") == 0)
			m_iCfgY = atoi(value);
		else if (strcmp(token, "bgColor") == 0)
			ParseColor(value, m_iCfgBgColor);
		else if (strcmp(token, "borderColor") == 0)
			ParseColor(value, m_iCfgBorderColor);
		else if (strcmp(token, "headerBgColor") == 0)
			ParseColor(value, m_iCfgHeaderBgColor);
		else if (strcmp(token, "headerTextColor") == 0)
			ParseColor(value, m_iCfgHeaderTextColor);
		else if (strcmp(token, "rowEvenColor") == 0)
			ParseColor(value, m_iCfgRowEvenColor);
		else if (strcmp(token, "rowOddColor") == 0)
			ParseColor(value, m_iCfgRowOddColor);
		else if (strcmp(token, "selfHighlightColor") == 0)
			ParseColor(value, m_iCfgSelfHighlightColor);
		else if (strcmp(token, "playerCountColor") == 0)
			ParseColor(value, m_iCfgPlayerCountColor);
		else if (strcmp(token, "subtitleColor") == 0)
			ParseColor(value, m_iCfgSubtitleColor);
		else if (strcmp(token, "indexColor") == 0)
			ParseColor(value, m_iCfgIndexColor);
		else if (strcmp(token, "deathColor") == 0)
			ParseColor(value, m_iCfgDeathColor);
		else if (strcmp(token, "selfPlayerColor") == 0)
			ParseColor(value, m_iCfgSelfPlayerColor);
		else if (strcmp(token, "voiceBannedColor") == 0)
			ParseColor(value, m_iCfgVoiceBannedColor);
		else if (strcmp(token, "voiceSpeakingColor") == 0)
			ParseColor(value, m_iCfgVoiceSpeakingColor);
		else if (strcmp(token, "voiceNeverSpokeColor") == 0)
			ParseColor(value, m_iCfgVoiceNeverSpokeColor);
		else if (strcmp(token, "voiceNotSpeakingColor") == 0)
			ParseColor(value, m_iCfgVoiceNotSpeakingColor);
		else if (strcmp(token, "pingGoodColor") == 0)
			ParseColor(value, m_iCfgPingGoodColor);
		else if (strcmp(token, "pingMediumColor") == 0)
			ParseColor(value, m_iCfgPingMediumColor);
		else if (strcmp(token, "pingBadColor") == 0)
			ParseColor(value, m_iCfgPingBadColor);
		else if (strcmp(token, "pingGoodThreshold") == 0)
			m_iCfgPingGoodThreshold = atoi(value);
		else if (strcmp(token, "pingMediumThreshold") == 0)
			m_iCfgPingMediumThreshold = atoi(value);
		else if (strcmp(token, "spectatorBgColor") == 0)
			ParseColor(value, m_iCfgSpectatorBgColor);
		else if (strcmp(token, "spectatorTextColor") == 0)
			ParseColor(value, m_iCfgSpectatorTextColor);
		else if (strcmp(token, "spectatorBorderColor") == 0)
			ParseColor(value, m_iCfgSpectatorBorderColor);
		else if (strcmp(token, "separatorColor") == 0)
			ParseColor(value, m_iCfgSeparatorColor);
		else if (strcmp(token, "killHighlightColor") == 0)
			ParseColor(value, m_iCfgKillHighlightColor);
		else if (strcmp(token, "scrollbarColor") == 0)
			ParseColor(value, m_iCfgScrollbarColor);
		else if (strcmp(token, "scrollbarThumbColor") == 0)
			ParseColor(value, m_iCfgScrollbarThumbColor);
		else if (strcmp(token, "colNameWidth") == 0)
			m_iCfgColNameWidth = atoi(value);
		else if (strcmp(token, "colTeamWidth") == 0)
			m_iCfgColTeamWidth = atoi(value);
		else if (strcmp(token, "colClassWidth") == 0)
			m_iCfgColClassWidth = atoi(value);
		else if (strcmp(token, "colScoreWidth") == 0)
			m_iCfgColScoreWidth = atoi(value);
		else if (strcmp(token, "colDeathsWidth") == 0)
			m_iCfgColDeathsWidth = atoi(value);
		else if (strcmp(token, "colPingWidth") == 0)
			m_iCfgColPingWidth = atoi(value);
		else if (strcmp(token, "colVoiceWidth") == 0)
			m_iCfgColVoiceWidth = atoi(value);
		else if (strcmp(token, "rowHeight") == 0)
			m_iCfgRowHeight = atoi(value);
		else if (strcmp(token, "teamRowHeight") == 0)
			m_iCfgTeamRowHeight = atoi(value);
		else if (strcmp(token, "headerHeight") == 0)
			m_iCfgHeaderHeight = atoi(value);
		else if (strcmp(token, "borderWidth") == 0)
			m_iCfgBorderWidth = atoi(value);
		else if (strcmp(token, "padding") == 0)
			m_iCfgPadding = atoi(value);
	}

	if (bMalloced)
		free(pfile);
	else
		gEngfuncs.COM_FreeFile(pfile);
	m_bConfigLoaded = true;
}

//=============================================================================
// Multiplayer check
//=============================================================================

bool CNuklearScoreboard::IsMultiplayer() const
{
	return gEngfuncs.GetMaxClients() > 1;
}

//=============================================================================
// Show/hide
//=============================================================================

void CNuklearScoreboard::Show(bool show)
{
	// Force hide in singleplayer mode
	if (show && !IsMultiplayer())
	{
		return;
	}

	if (m_bVisible != show)
	{
		m_bVisible = show;
		if (show)
		{
			m_iScrollOffset = 0;
			Update();
		}
	}
}

bool CNuklearScoreboard::IsVisible() const
{
	return m_bVisible;
}

int CNuklearScoreboard::GetHighlightRow() const
{
	return m_iHighlightRow;
}

//=============================================================================
// Layout calculations
//=============================================================================

void CNuklearScoreboard::CalculateLayout()
{
	// Use fixed size from config
	m_iWidth = m_iCfgWidth;
	m_iHeight = m_iCfgHeight;

	// Calculate position (-1 means center)
	if (m_iCfgX == -1)
		m_iX = (ScreenWidth - m_iWidth) / 2;
	else
		m_iX = m_iCfgX;

	if (m_iCfgY == -1)
		m_iY = (ScreenHeight - m_iHeight) / 2;
	else
		m_iY = m_iCfgY;

	// Content start Y position (below header)
	m_iContentY = m_iY + m_iCfgHeaderHeight + m_iCfgRowHeight + m_iCfgPadding;

	// Calculate visible row count
	int availableHeight = m_iHeight - m_iCfgHeaderHeight - m_iCfgRowHeight - m_iCfgPadding * 2;
	m_iMaxVisibleRows = availableHeight / m_iCfgRowHeight;
}

//=============================================================================
// Data collection and sorting
//=============================================================================

void CNuklearScoreboard::CollectPlayerData()
{
	// Get all player info
	if (gViewPort)
	{
		gViewPort->GetAllPlayersInfo();

		// Copy player info
		for (int i = 1; i <= MAX_PLAYERS; i++)
		{
			hud_player_info_t* pl_info = &g_PlayerInfoList[i];
			extra_player_info_t* extra_info = &g_PlayerExtraInfo[i];

			if (pl_info->name && pl_info->name[0])
			{
				m_PlayerInfo[i].connected = true;
				strncpy(m_PlayerInfo[i].name, pl_info->name, MAX_PLAYER_NAME_LENGTH - 1);
				m_PlayerInfo[i].name[MAX_PLAYER_NAME_LENGTH - 1] = '\0';
				m_PlayerInfo[i].score = extra_info->frags;
				m_PlayerInfo[i].deaths = extra_info->deaths;
				m_PlayerInfo[i].ping = pl_info->ping;
				m_PlayerInfo[i].packetloss = pl_info->packetloss;
				m_PlayerInfo[i].teamnumber = extra_info->teamnumber;
				m_PlayerInfo[i].playerclass = extra_info->playerclass;
				m_PlayerInfo[i].thisplayer = pl_info->thisplayer;
				m_PlayerInfo[i].alive = true;

				if (pl_info->thisplayer)
				{
					m_iPlayerNum = i;
				}
			}
			else
			{
				m_PlayerInfo[i].connected = false;
			}
		}
	}

	// Get server name
	if (gViewPort && gViewPort->m_szServerName)
	{
		strncpy(m_szServerName, gViewPort->m_szServerName, 255);
		m_szServerName[255] = '\0';
	}
}

void CNuklearScoreboard::RebuildTeams()
{
	// Clear team player counts
	for (int i = 1; i <= m_iNumTeams; i++)
	{
		m_TeamInfo[i].players = 0;
	}

	// Rebuild team list
	CollectPlayerData();
	m_iNumTeams = 0;

	for (int i = 1; i <= MAX_PLAYERS; i++)
	{
		if (!m_PlayerInfo[i].connected)
			continue;

		if (m_PlayerInfo[i].teamnumber == 0)
			continue;

		// Check if team already exists
		int j;
		for (j = 1; j <= m_iNumTeams; j++)
		{
			if (m_TeamInfo[j].name[0] == '\0')
				break;

			if (m_TeamInfo[j].teamnumber == m_PlayerInfo[i].teamnumber)
				break;
		}

		if (j > m_iNumTeams)
		{
			// Create new team
			for (int k = 1; k <= m_iNumTeams; k++)
			{
				if (m_TeamInfo[k].name[0] == '\0')
				{
					j = k;
					break;
				}
			}
			m_iNumTeams = max(j, m_iNumTeams);

			// Get team name
			if (gViewPort)
			{
				const char* teamName = gViewPort->GetTeamName(m_PlayerInfo[i].teamnumber);
				if (teamName)
				{
					strncpy(m_TeamInfo[j].name, teamName, MAX_TEAM_NAME - 1);
				}
			}
			m_TeamInfo[j].teamnumber = m_PlayerInfo[i].teamnumber;
		}

		m_TeamInfo[j].players++;
	}

	// Clear empty teams
	for (int i = 1; i <= m_iNumTeams; i++)
	{
		if (m_TeamInfo[i].players < 1)
		{
			memset(&m_TeamInfo[i], 0, sizeof(TeamInfo_t));
		}
	}
}

void CNuklearScoreboard::SortPlayers()
{
	m_iNumSorted = 0;

	// Clear sorted flags
	memset(m_bHasBeenSorted, 0, sizeof(m_bHasBeenSorted));

	// If teamplay mode, sort teams first
	if (gHUD.m_Teamplay)
	{
		// Calculate team scores
		for (int i = 1; i <= m_iNumTeams; i++)
		{
			if (!m_TeamInfo[i].scores_overriden)
			{
				m_TeamInfo[i].frags = 0;
				m_TeamInfo[i].deaths = 0;
			}
			m_TeamInfo[i].ping = 0;
			m_TeamInfo[i].packetloss = 0;
		}

		// Accumulate player scores to teams
		for (int i = 1; i <= MAX_PLAYERS; i++)
		{
			if (!m_PlayerInfo[i].connected)
				continue;

			// Find player's team
			for (int j = 1; j <= m_iNumTeams; j++)
			{
				if (m_TeamInfo[j].teamnumber == m_PlayerInfo[i].teamnumber)
				{
					if (!m_TeamInfo[j].scores_overriden)
					{
						m_TeamInfo[j].frags += m_PlayerInfo[i].score;
						m_TeamInfo[j].deaths += m_PlayerInfo[i].deaths;
					}
					m_TeamInfo[j].ping += m_PlayerInfo[i].ping;
					m_TeamInfo[j].packetloss += m_PlayerInfo[i].packetloss;

					if (m_PlayerInfo[i].thisplayer)
						m_TeamInfo[j].ownteam = true;

					break;
				}
			}
		}

		// Calculate average team ping
		for (int i = 1; i <= m_iNumTeams; i++)
		{
			m_TeamInfo[i].already_drawn = false;
			if (m_TeamInfo[i].players > 0)
			{
				m_TeamInfo[i].ping /= m_TeamInfo[i].players;
				m_TeamInfo[i].packetloss /= m_TeamInfo[i].players;
			}
		}

		// Sort teams by score
		while (true)
		{
			int best_team = 0;
			int highest_frags = -99999;
			int lowest_deaths = 99999;

			for (int i = 1; i <= m_iNumTeams; i++)
			{
				if (m_TeamInfo[i].players < 1)
					continue;

				if (!m_TeamInfo[i].already_drawn && m_TeamInfo[i].frags >= highest_frags)
				{
					if (m_TeamInfo[i].frags > highest_frags || m_TeamInfo[i].deaths < lowest_deaths)
					{
						best_team = i;
						lowest_deaths = m_TeamInfo[i].deaths;
						highest_frags = m_TeamInfo[i].frags;
					}
				}
			}

			if (!best_team)
				break;

			// Add team row
			m_SortedPlayers[m_iNumSorted].playerIndex = best_team;
			m_SortedPlayers[m_iNumSorted].isTeam = true;
			m_SortedPlayers[m_iNumSorted].teamIndex = best_team;
			m_SortedPlayers[m_iNumSorted].teamType = TEAM_YES;
			m_iNumSorted++;

			m_TeamInfo[best_team].already_drawn = true;

			// Add players for this team
			SortTeamPlayers(best_team);
		}

		// Add spectators
		SortTeamPlayers(TEAM_SPECTATORS);
	}
	else
	{
		// Non-teamplay mode, sort all players directly
		SortTeamPlayers(0);
	}
}

void CNuklearScoreboard::SortTeamPlayers(int teamIndex)
{
	bool teamCreated = false;
	bool bHasTeamPlayers = false;

	bool bIsSpecificTeam = (teamIndex >= 1 && teamIndex <= m_iNumTeams);

	while (true)
	{
		int best_player = 0;
		int highest_frags = -99999;
		int lowest_deaths = 99999;

		for (int i = 1; i <= MAX_PLAYERS; i++)
		{
			if (!m_PlayerInfo[i].connected)
				continue;

			if (m_bHasBeenSorted[i])
				continue;

			// Check if player matches filter
			bool bMatch = false;

			if (teamIndex == 0)
			{
				// teamIndex == 0: sort all players (non-teamplay mode)
				bMatch = true;
			}
			else if (teamIndex == TEAM_SPECTATORS)
			{
				// TEAM_SPECTATORS: sort spectators (players not in any team)
				bool bInTeam = false;
				for (int j = 1; j <= m_iNumTeams; j++)
				{
					if (m_TeamInfo[j].teamnumber == m_PlayerInfo[i].teamnumber)
					{
						bInTeam = true;
						break;
					}
				}
				bMatch = !bInTeam;
			}
			else if (bIsSpecificTeam)
			{
				// Specific team
				bMatch = (m_PlayerInfo[i].teamnumber == m_TeamInfo[teamIndex].teamnumber);
			}

			if (!bMatch)
				continue;

			// Find highest scoring player
			if (m_PlayerInfo[i].score >= highest_frags)
			{
				if (m_PlayerInfo[i].score > highest_frags || m_PlayerInfo[i].deaths < lowest_deaths)
				{
					best_player = i;
					lowest_deaths = m_PlayerInfo[i].deaths;
					highest_frags = m_PlayerInfo[i].score;
				}
			}
		}

		if (!best_player)
			break;

		// If team row not created yet and needs to be created
		if (!teamCreated && (teamIndex == TEAM_SPECTATORS))
		{
			// Add spectator team row
			m_SortedPlayers[m_iNumSorted].playerIndex = 0;
			m_SortedPlayers[m_iNumSorted].isTeam = true;
			m_SortedPlayers[m_iNumSorted].teamIndex = 0;
			m_SortedPlayers[m_iNumSorted].teamType = TEAM_SPECTATORS;
			m_iNumSorted++;

			teamCreated = true;
		}

		// Mark that we found players
		bHasTeamPlayers = true;

		// Add player
		m_SortedPlayers[m_iNumSorted].playerIndex = best_player;
		m_SortedPlayers[m_iNumSorted].isTeam = false;
		m_SortedPlayers[m_iNumSorted].teamIndex = teamIndex;
		m_SortedPlayers[m_iNumSorted].teamType = TEAM_NO;
		m_bHasBeenSorted[best_player] = true;
		m_iNumSorted++;
	}

	// Add separator row (only for specific teams with players)
	if (bIsSpecificTeam && bHasTeamPlayers)
	{
		m_SortedPlayers[m_iNumSorted].playerIndex = 0;
		m_SortedPlayers[m_iNumSorted].isTeam = false;
		m_SortedPlayers[m_iNumSorted].teamIndex = 0;
		m_SortedPlayers[m_iNumSorted].teamType = TEAM_BLANK;
		m_iNumSorted++;
	}
}

void CNuklearScoreboard::Update()
{
	CollectPlayerData();
	RebuildTeams();
	SortPlayers();
	m_bNeedsUpdate = false;
}

//=============================================================================
// Draw functions
//=============================================================================

void CNuklearScoreboard::Draw()
{
	// Return immediately in singleplayer mode
	if (!IsMultiplayer())
		return;

	if (!m_bVisible)
		return;

	// Draw background
	DrawBackground();

	// Draw header
	DrawHeader();

	// Draw column headers
	DrawColumnHeaders();

	// Draw content
	DrawPlayerList();

	// Draw highlight effect
	if (m_iHighlightRow >= 0 && m_iHighlightRow < m_iNumSorted)
	{
		DrawHighlight(m_iHighlightRow);
	}
}

void CNuklearScoreboard::DrawBackground()
{
	int x = m_iX;
	int y = m_iY;
	int w = m_iWidth;
	int h = m_iHeight;

	DrawRect(x, y, w, h, m_iCfgBgColor[0], m_iCfgBgColor[1], m_iCfgBgColor[2], m_iCfgBgColor[3]);

	DrawBorder(x, y, w, h, m_iCfgBorderWidth, m_iCfgBorderColor[0], m_iCfgBorderColor[1], m_iCfgBorderColor[2], m_iCfgBorderColor[3]);
}

void CNuklearScoreboard::DrawHeader()
{
	int x = m_iX + m_iCfgPadding;
	int y = m_iY + m_iCfgPadding;
	int w = m_iWidth - m_iCfgPadding * 2;
	int h = m_iCfgHeaderHeight;

	// Title: prefer server name, fall back to configured title
	const char* titleText = (m_szServerName[0] != '\0') ? m_szServerName : m_szCfgTitle;
	DrawString(x, y, titleText, m_iCfgHeaderTextColor[0], m_iCfgHeaderTextColor[1], m_iCfgHeaderTextColor[2]);

	// Show player count
	char playerCount[64];
	int totalPlayers = 0;
	for (int i = 1; i <= MAX_PLAYERS; i++)
	{
		if (m_PlayerInfo[i].connected)
			totalPlayers++;
	}

	snprintf(playerCount, sizeof(playerCount), m_szCfgPlayerCountFormat, totalPlayers);
	int textWidth = ConsoleStringLen(playerCount);
	DrawString(m_iX + m_iWidth - m_iCfgPadding - textWidth, y, playerCount,
		m_iCfgPlayerCountColor[0], m_iCfgPlayerCountColor[1], m_iCfgPlayerCountColor[2]);

	// Draw subtitle
	DrawStringCenter(m_iX + m_iWidth / 2, y + m_iCfgHeaderHeight - m_iCfgPadding - m_iCharHeight, m_szCfgSubtitle,
		m_iCfgSubtitleColor[0], m_iCfgSubtitleColor[1], m_iCfgSubtitleColor[2]);
}

void CNuklearScoreboard::DrawColumnHeaders()
{
	int y = m_iY + m_iCfgHeaderHeight + 4;
	int colY = m_iCfgRowHeight;

	DrawRect(m_iX + m_iCfgBorderWidth, y, m_iWidth - m_iCfgBorderWidth * 2, colY,
		m_iCfgHeaderBgColor[0], m_iCfgHeaderBgColor[1], m_iCfgHeaderBgColor[2], m_iCfgHeaderBgColor[3]);

	int colX = m_iX + m_iCfgBorderWidth + m_iCfgPadding;

	colX += 30;

	DrawString(colX, y + 6, m_szCfgColName, m_iCfgHeaderTextColor[0], m_iCfgHeaderTextColor[1], m_iCfgHeaderTextColor[2]);
	colX += m_iCfgColNameWidth;

	if (gHUD.m_Teamplay)
	{
		DrawString(colX, y + 6, m_szCfgColTeam, m_iCfgHeaderTextColor[0], m_iCfgHeaderTextColor[1], m_iCfgHeaderTextColor[2]);
		colX += m_iCfgColTeamWidth;
	}

	colX += m_iCfgColClassWidth;

	int scoreW = ConsoleStringLen(m_szCfgColScore);
	DrawString(colX + m_iCfgColScoreWidth - scoreW, y + 6, m_szCfgColScore, m_iCfgHeaderTextColor[0], m_iCfgHeaderTextColor[1], m_iCfgHeaderTextColor[2]);
	colX += m_iCfgColScoreWidth;

	int deathsW = ConsoleStringLen(m_szCfgColDeaths);
	DrawString(colX + m_iCfgColDeathsWidth - deathsW, y + 6, m_szCfgColDeaths, m_iCfgHeaderTextColor[0], m_iCfgHeaderTextColor[1], m_iCfgHeaderTextColor[2]);
	colX += m_iCfgColDeathsWidth;

	int latencyW = ConsoleStringLen(m_szCfgColPing);
	DrawString(colX + m_iCfgColPingWidth - latencyW, y + 6, m_szCfgColPing, m_iCfgHeaderTextColor[0], m_iCfgHeaderTextColor[1], m_iCfgHeaderTextColor[2]);
	colX += m_iCfgColPingWidth;

	int voiceW = ConsoleStringLen(m_szCfgColVoice);
	DrawString(colX + m_iCfgColVoiceWidth - voiceW, y + 6, m_szCfgColVoice, m_iCfgHeaderTextColor[0], m_iCfgHeaderTextColor[1], m_iCfgHeaderTextColor[2]);
}

void CNuklearScoreboard::DrawPlayerList()
{
	int startY = m_iY + m_iCfgHeaderHeight + m_iCfgRowHeight + m_iCfgPadding;
	int visibleHeight = m_iY + m_iHeight - 10 - startY;

	// First pass: calculate total content height
	int totalHeight = 0;
	for (int i = 0; i < m_iNumSorted; i++)
	{
		if (m_SortedPlayers[i].isTeam)
			totalHeight += m_iCfgTeamRowHeight;
		else if (m_SortedPlayers[i].teamType == TEAM_BLANK)
			totalHeight += 6;
		else
			totalHeight += m_iCfgRowHeight;
	}
	m_iTotalContentHeight = totalHeight;

	// Clamp scroll offset
	int maxScroll = totalHeight - visibleHeight;
	if (maxScroll < 0) maxScroll = 0;
	if (m_iScrollOffset < 0) m_iScrollOffset = 0;
	if (m_iScrollOffset > maxScroll) m_iScrollOffset = maxScroll;

	// Draw player rows with scroll offset
	int y = startY - m_iScrollOffset;
	int rowHeight = m_iCfgRowHeight;

	for (int i = 0; i < m_iNumSorted; i++)
	{
		int currentRowHeight;
		if (m_SortedPlayers[i].isTeam)
			currentRowHeight = m_iCfgTeamRowHeight;
		else if (m_SortedPlayers[i].teamType == TEAM_BLANK)
			currentRowHeight = 6;
		else
			currentRowHeight = rowHeight;

		// Skip rows above visible area
		if (y + currentRowHeight < startY)
		{
			y += currentRowHeight;
			continue;
		}

		// Stop if past visible area
		if (y >= startY + visibleHeight)
			break;

		if (m_SortedPlayers[i].isTeam)
		{
			DrawTeamRow(m_iX + m_iCfgBorderWidth, y,
				i, m_iCfgTeamRowHeight,
				m_SortedPlayers[i].teamIndex, m_SortedPlayers[i].teamType);
			y += m_iCfgTeamRowHeight;
		}
		else if (m_SortedPlayers[i].teamType == TEAM_BLANK)
		{
			DrawRect(m_iX + m_iCfgBorderWidth, y + 2,
				m_iWidth - m_iCfgBorderWidth * 2, 1,
				m_iCfgSeparatorColor[0], m_iCfgSeparatorColor[1], m_iCfgSeparatorColor[2], m_iCfgSeparatorColor[3]);
			y += 6;
		}
		else
		{
			DrawPlayerRow(m_iX + m_iCfgBorderWidth, y, i, rowHeight);
			y += rowHeight;
		}
	}

	// Draw scrollbar if content exceeds visible area
	if (totalHeight > visibleHeight)
	{
		DrawScrollbar(totalHeight, visibleHeight);
	}
}

void CNuklearScoreboard::DrawScrollbar(int contentHeight, int visibleHeight)
{
	int barX = m_iX + m_iWidth - m_iCfgBorderWidth - 6;
	int barY = m_iY + m_iCfgHeaderHeight + m_iCfgRowHeight + m_iCfgPadding;
	int barW = 4;
	int barH = visibleHeight;

	// Track background
	DrawRect(barX, barY, barW, barH,
		m_iCfgScrollbarColor[0], m_iCfgScrollbarColor[1], m_iCfgScrollbarColor[2], m_iCfgScrollbarColor[3]);

	// Thumb
	int maxScroll = contentHeight - visibleHeight;
	if (maxScroll <= 0) return;

	float ratio = (float)m_iScrollOffset / (float)maxScroll;
	int thumbH = max(20, (int)((float)barH * (float)visibleHeight / (float)contentHeight));
	int thumbY = barY + (int)((float)(barH - thumbH) * ratio);

	DrawRect(barX, thumbY, barW, thumbH,
		m_iCfgScrollbarThumbColor[0], m_iCfgScrollbarThumbColor[1], m_iCfgScrollbarThumbColor[2], m_iCfgScrollbarThumbColor[3]);
}

void CNuklearScoreboard::DrawPlayerRow(int x, int y, int row, int height)
{
	int sortedIndex = m_SortedPlayers[row].playerIndex;
	PlayerInfo_t* player = &m_PlayerInfo[sortedIndex];

	if (!player->connected)
		return;

	bool isEven = (row % 2 == 0);
	if (isEven)
	{
		DrawRect(x, y, m_iWidth - m_iCfgBorderWidth * 2, height,
			m_iCfgRowEvenColor[0], m_iCfgRowEvenColor[1], m_iCfgRowEvenColor[2], m_iCfgRowEvenColor[3]);
	}
	else
	{
		DrawRect(x, y, m_iWidth - m_iCfgBorderWidth * 2, height,
			m_iCfgRowOddColor[0], m_iCfgRowOddColor[1], m_iCfgRowOddColor[2], m_iCfgRowOddColor[3]);
	}

	if (player->thisplayer)
	{
		DrawRect(x, y, m_iWidth - m_iCfgBorderWidth * 2, height,
			m_iCfgSelfHighlightColor[0],
			m_iCfgSelfHighlightColor[1],
			m_iCfgSelfHighlightColor[2],
			m_iCfgSelfHighlightColor[3]);
	}

	if (sortedIndex == m_iLastKilledBy && m_fLastKillTime > gHUD.m_flTime && m_iLastKilledBy != 0)
	{
		int alpha = (int)(255.0f * min(1.0f, (m_fLastKillTime - gHUD.m_flTime) / 2.0f));
		DrawRect(x, y, m_iWidth - m_iCfgBorderWidth * 2, height,
			m_iCfgKillHighlightColor[0], m_iCfgKillHighlightColor[1], m_iCfgKillHighlightColor[2], alpha);
	}

	DrawPlayerInfo(row);
}

void CNuklearScoreboard::DrawTeamRow(int x, int y, int row, int height, int teamIndex, int teamType)
{
	if (teamType == TEAM_SPECTATORS)
	{
		DrawRect(x, y, m_iWidth - m_iCfgBorderWidth * 2, height,
			m_iCfgSpectatorBgColor[0], m_iCfgSpectatorBgColor[1], m_iCfgSpectatorBgColor[2], m_iCfgSpectatorBgColor[3]);
		DrawString(x + m_iCfgPadding, y + 5, m_szCfgSpectators,
			m_iCfgSpectatorTextColor[0], m_iCfgSpectatorTextColor[1], m_iCfgSpectatorTextColor[2]);

		DrawRect(x, y + height - 2, m_iWidth - m_iCfgBorderWidth * 2, 2,
			m_iCfgSpectatorBorderColor[0], m_iCfgSpectatorBorderColor[1], m_iCfgSpectatorBorderColor[2], m_iCfgSpectatorBorderColor[3]);
	}
	else if (teamType == TEAM_YES && teamIndex > 0 && teamIndex <= m_iNumTeams)
	{
		TeamInfo_t* team = &m_TeamInfo[teamIndex];

		int teamColorIndex = team->teamnumber % MAX_TEAM_COLORS;

		DrawRect(x, y, m_iWidth - m_iCfgBorderWidth * 2, height,
			g_TeamColors[teamColorIndex].r,
			g_TeamColors[teamColorIndex].g,
			g_TeamColors[teamColorIndex].b,
			40);

		char teamText[128];
		snprintf(teamText, sizeof(teamText), m_szCfgTeamFormat, team->name, team->players);
		DrawString(x + m_iCfgPadding, y + 5, teamText,
			g_TeamColors[teamColorIndex].r,
			g_TeamColors[teamColorIndex].g,
			g_TeamColors[teamColorIndex].b);

		char scoreText[32];
		snprintf(scoreText, sizeof(scoreText), m_szCfgScoreFormat, team->frags);
		int scoreX = x + m_iCfgColNameWidth + m_iCfgColTeamWidth + m_iCfgColClassWidth + m_iCfgColScoreWidth - ConsoleStringLen(scoreText);
		DrawString(scoreX, y + 5, scoreText,
			g_TeamColors[teamColorIndex].r,
			g_TeamColors[teamColorIndex].g,
			g_TeamColors[teamColorIndex].b);

		snprintf(scoreText, sizeof(scoreText), m_szCfgDeathFormat, team->deaths);
		scoreX = x + m_iCfgColNameWidth + m_iCfgColTeamWidth + m_iCfgColClassWidth + m_iCfgColScoreWidth + m_iCfgColDeathsWidth - ConsoleStringLen(scoreText);
		DrawString(scoreX, y + 5, scoreText,
			g_TeamColors[teamColorIndex].r,
			g_TeamColors[teamColorIndex].g,
			g_TeamColors[teamColorIndex].b);

		snprintf(scoreText, sizeof(scoreText), m_szCfgPingFormat, team->ping);
		scoreX = x + m_iCfgColNameWidth + m_iCfgColTeamWidth + m_iCfgColClassWidth + m_iCfgColScoreWidth + m_iCfgColDeathsWidth + m_iCfgColPingWidth - ConsoleStringLen(scoreText);
		DrawString(scoreX, y + 5, scoreText,
			g_TeamColors[teamColorIndex].r,
			g_TeamColors[teamColorIndex].g,
			g_TeamColors[teamColorIndex].b);

		DrawRect(x, y + height - 2, m_iWidth - m_iCfgBorderWidth * 2, 2,
			g_TeamColors[teamColorIndex].r,
			g_TeamColors[teamColorIndex].g,
			g_TeamColors[teamColorIndex].b,
			255);
	}
}

void CNuklearScoreboard::DrawPlayerInfo(int row)
{
	int sortedIndex = m_SortedPlayers[row].playerIndex;
	PlayerInfo_t* player = &m_PlayerInfo[sortedIndex];

	int x = m_iX + m_iCfgBorderWidth + m_iCfgPadding;
	int y = GetRowY(row);
	int colX = x;

	int teamColorIndex = player->teamnumber % MAX_TEAM_COLORS;
	int r = g_TeamColors[teamColorIndex].r;
	int g = g_TeamColors[teamColorIndex].g;
	int b = g_TeamColors[teamColorIndex].b;

	if (player->thisplayer)
	{
		r = m_iCfgSelfPlayerColor[0];
		g = m_iCfgSelfPlayerColor[1];
		b = m_iCfgSelfPlayerColor[2];
	}

	char indexStr[16];
	snprintf(indexStr, sizeof(indexStr), m_szCfgIndexFormat, sortedIndex);
	DrawString(colX, y + 3, indexStr,
		m_iCfgIndexColor[0], m_iCfgIndexColor[1], m_iCfgIndexColor[2]);
	colX += 30;

	char nameText[MAX_PLAYER_NAME_LENGTH + 4];
	snprintf(nameText, sizeof(nameText), m_szCfgNameFormat, player->name);
	DrawString(colX, y + 3, nameText, r, g, b);
	colX += m_iCfgColNameWidth;

	if (gHUD.m_Teamplay)
	{
		if (gViewPort)
		{
			const char* teamName = gViewPort->GetTeamName(player->teamnumber);
			if (teamName)
			{
				DrawString(colX, y + 3, teamName, r, g, b);
			}
		}
		colX += m_iCfgColTeamWidth;
	}

	colX += m_iCfgColClassWidth;

	char scoreText[32];
	snprintf(scoreText, sizeof(scoreText), m_szCfgScoreFormat, player->score);
	int textWidth = ConsoleStringLen(scoreText);
	DrawString(colX + m_iCfgColScoreWidth - textWidth - m_iCfgPadding, y + 3, scoreText, r, g, b);
	colX += m_iCfgColScoreWidth;

	snprintf(scoreText, sizeof(scoreText), m_szCfgDeathFormat, player->deaths);
	textWidth = ConsoleStringLen(scoreText);
	DrawString(colX + m_iCfgColDeathsWidth - textWidth - m_iCfgPadding, y + 3, scoreText,
		m_iCfgDeathColor[0], m_iCfgDeathColor[1], m_iCfgDeathColor[2]);
	colX += m_iCfgColDeathsWidth;

	int pingR = m_iCfgPingGoodColor[0];
	int pingG = m_iCfgPingGoodColor[1];
	int pingB = m_iCfgPingGoodColor[2];
	if (player->ping > m_iCfgPingMediumThreshold)
	{
		pingR = m_iCfgPingBadColor[0];
		pingG = m_iCfgPingBadColor[1];
		pingB = m_iCfgPingBadColor[2];
	}
	else if (player->ping > m_iCfgPingGoodThreshold)
	{
		pingR = m_iCfgPingMediumColor[0];
		pingG = m_iCfgPingMediumColor[1];
		pingB = m_iCfgPingMediumColor[2];
	}

	snprintf(scoreText, sizeof(scoreText), m_szCfgPingFormat, player->ping);
	textWidth = ConsoleStringLen(scoreText);
	DrawString(colX + m_iCfgColPingWidth - textWidth - m_iCfgPadding, y + 3, scoreText, pingR, pingG, pingB);
	colX += m_iCfgColPingWidth;

	CVoiceStatus *pVoiceMgr = GetClientVoiceMgr();
	if (pVoiceMgr && sortedIndex > 0)
	{
		bool bBanned = pVoiceMgr->IsPlayerBlocked(sortedIndex);
		bool bTalking = false;
		bool bNeverSpoken = false;

		if (!bBanned)
		{
			bTalking = pVoiceMgr->m_VoicePlayers[sortedIndex - 1];
			bNeverSpoken = !pVoiceMgr->m_VoiceEnabledPlayers[sortedIndex - 1];
		}

		int voiceState = VOICE_STATE_NOT_SPEAKING;
		int iconR = 255, iconG = 255, iconB = 255;
		const char* voiceChar = "";

		if (bBanned)
		{
			voiceState = VOICE_STATE_BANNED;
			voiceChar = m_szCfgVoiceBanned;
			iconR = m_iCfgVoiceBannedColor[0];
			iconG = m_iCfgVoiceBannedColor[1];
			iconB = m_iCfgVoiceBannedColor[2];
		}
		else if (bTalking)
		{
			bool bBlink = (fmod(gHUD.m_flTime, 0.6f) < 0.3f);
			voiceState = bBlink ? VOICE_STATE_SPEAKING_BLINK : VOICE_STATE_SPEAKING;
			voiceChar = bBlink ? m_szCfgVoiceSpeakingBlink : m_szCfgVoiceSpeaking;
			iconR = m_iCfgVoiceSpeakingColor[0];
			iconG = m_iCfgVoiceSpeakingColor[1];
			iconB = m_iCfgVoiceSpeakingColor[2];
		}
		else if (bNeverSpoken)
		{
			voiceState = VOICE_STATE_NEVER_SPOKEN;
			voiceChar = m_szCfgVoiceNeverSpoke;
			iconR = m_iCfgVoiceNeverSpokeColor[0];
			iconG = m_iCfgVoiceNeverSpokeColor[1];
			iconB = m_iCfgVoiceNeverSpokeColor[2];
		}
		else
		{
			voiceState = VOICE_STATE_NOT_SPEAKING;
			voiceChar = m_szCfgVoiceNotSpeaking;
			iconR = m_iCfgVoiceNotSpeakingColor[0];
			iconG = m_iCfgVoiceNotSpeakingColor[1];
			iconB = m_iCfgVoiceNotSpeakingColor[2];
		}

		bool bDrawn = false;
		if (m_bVoiceSpritesLoaded)
		{
			HSPRITE hSpr = 0;
			switch (voiceState)
			{
			case VOICE_STATE_BANNED:			hSpr = m_hSprVoiceBanned; break;
			case VOICE_STATE_SPEAKING:			hSpr = m_hSprVoiceSpeaking; break;
			case VOICE_STATE_SPEAKING_BLINK:	hSpr = m_hSprVoiceSpeakingBlink; break;
			case VOICE_STATE_NEVER_SPOKEN:		hSpr = m_hSprVoiceNeverSpoke; break;
			case VOICE_STATE_NOT_SPEAKING:		hSpr = m_hSprVoiceNotSpeaking; break;
			}
			if (hSpr != 0)
			{
				int sprW = SPR_Width(hSpr, 0);
				int sprH = SPR_Height(hSpr, 0);
				int voiceX = colX + m_iCfgColVoiceWidth - sprW - m_iCfgPadding;
				int voiceY = y + (m_iCfgRowHeight - sprH) / 2;
				SPR_Set(hSpr, iconR, iconG, iconB);
				SPR_DrawAdditive(0, voiceX, voiceY, NULL);
				bDrawn = true;
			}
		}

		if (!bDrawn && voiceChar[0])
		{
			int voiceX = colX + m_iCfgColVoiceWidth - ConsoleStringLen(voiceChar) - m_iCfgPadding;
			DrawString(voiceX, y + 3, voiceChar, iconR, iconG, iconB);
		}
	}
}

void CNuklearScoreboard::DrawHighlight(int row)
{
	int y = GetRowY(row);
	int height = GetRowHeight(row);

	DrawRect(m_iX + m_iCfgBorderWidth, y, m_iWidth - m_iCfgBorderWidth * 2, height,
		m_iCfgSelfHighlightColor[0], m_iCfgSelfHighlightColor[1], m_iCfgSelfHighlightColor[2], m_iCfgSelfHighlightColor[3]);
}

int CNuklearScoreboard::GetRowY(int row)
{
	int y = m_iY + m_iCfgHeaderHeight + m_iCfgRowHeight + m_iCfgPadding;

	for (int i = 0; i < row && i < m_iNumSorted; i++)
	{
		if (m_SortedPlayers[i].isTeam)
		{
			y += m_iCfgTeamRowHeight;
		}
		else if (m_SortedPlayers[i].teamType == TEAM_BLANK)
		{
			y += 6;
		}
		else
		{
			y += m_iCfgRowHeight;
		}
	}

	return y;
}

int CNuklearScoreboard::GetRowHeight(int row)
{
	if (m_SortedPlayers[row].isTeam)
	{
		return m_iCfgTeamRowHeight;
	}
	else if (m_SortedPlayers[row].teamType == TEAM_BLANK)
	{
		return 6;
	}
	return m_iCfgRowHeight;
}

//=============================================================================
// Utility functions
//=============================================================================

int CNuklearScoreboard::DrawString(int x, int y, const char* text, int r, int g, int b)
{
	if (!text)
		return 0;

	gEngfuncs.pfnDrawSetTextColor(r / 255.0f, g / 255.0f, b / 255.0f);
	return DrawConsoleString(x, y, (char*)text);
}

int CNuklearScoreboard::DrawStringCenter(int x, int y, const char* text, int r, int g, int b)
{
	if (!text)
		return 0;

	int width = ConsoleStringLen(text);
	return DrawString(x - width / 2, y, text, r, g, b);
}

void CNuklearScoreboard::DrawRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	FillRGBA(x, y, w, h, r, g, b, a);
}

void CNuklearScoreboard::DrawBorder(int x, int y, int w, int h, int border, int r, int g, int b, int a)
{
	// Top border
	FillRGBA(x, y, w, border, r, g, b, a);
	// Bottom border
	FillRGBA(x, y + h - border, w, border, r, g, b, a);
	// Left border
	FillRGBA(x, y, border, h, r, g, b, a);
	// Right border
	FillRGBA(x + w - border, y, border, h, r, g, b, a);
}

//=============================================================================
// Input handling
//=============================================================================

void CNuklearScoreboard::MousePressed(int x, int y)
{
	// Check if within scoreboard area
	if (x < m_iX || x > m_iX + m_iWidth || y < m_iY || y > m_iY + m_iHeight)
	{
		return;
	}

	// Find clicked row (account for scroll offset)
	int currentY = m_iY + m_iCfgHeaderHeight + m_iCfgRowHeight + m_iCfgPadding - m_iScrollOffset;

	for (int i = 0; i < m_iNumSorted; i++)
	{
		int rowHeight = GetRowHeight(i);

		if (y >= currentY && y < currentY + rowHeight)
		{
			m_iHighlightRow = i;

			// If player row, click to mute/unmute
			if (!m_SortedPlayers[i].isTeam && m_SortedPlayers[i].teamType != TEAM_BLANK)
			{
				int playerIndex = m_SortedPlayers[i].playerIndex;
				if (playerIndex > 0 && !m_PlayerInfo[playerIndex].thisplayer)
				{
					hud_player_info_t* pl_info = &g_PlayerInfoList[playerIndex];
					if (pl_info && pl_info->name && pl_info->name[0])
					{
						// Toggle mute state
						m_iMutedPlayers[playerIndex] = !m_iMutedPlayers[playerIndex];

						// Send command
						char cmd[128];
						if (m_iMutedPlayers[playerIndex])
						{
							snprintf(cmd, sizeof(cmd), m_szCfgCmdVoiceMute, playerIndex);
						}
						else
						{
							snprintf(cmd, sizeof(cmd), m_szCfgCmdVoiceUnmute, playerIndex);
						}
						ClientCmd(cmd);

						// Print localized feedback message
						char string[256];
						char string1[1024];
						if (m_iMutedPlayers[playerIndex])
						{
							char string2[1024];
							sprintf(string1, CHudTextMessage::BufferedLocaliseTextString("#Muted"), pl_info->name);
							sprintf(string2, CHudTextMessage::BufferedLocaliseTextString("#No_longer_hear_that_player"));
							sprintf(string, "%c** %s %s\n", HUD_PRINTTALK, string1, string2);
						}
						else
						{
							sprintf(string1, CHudTextMessage::BufferedLocaliseTextString("#Unmuted"), pl_info->name);
							sprintf(string, "%c** %s\n", HUD_PRINTTALK, string1);
						}
						gHUD.m_TextMessage.MsgFunc_TextMsg(NULL, strlen(string) + 1, string);
					}
				}
			}
			return;
		}

		currentY += rowHeight;
	}
}

void CNuklearScoreboard::MouseMoved(int x, int y)
{
	// Check if within scoreboard area
	if (x < m_iX || x > m_iX + m_iWidth || y < m_iY || y > m_iY + m_iHeight)
	{
		m_iHighlightRow = -1;
		return;
	}

	// Find hovered row (account for scroll offset)
	int currentY = m_iY + m_iCfgHeaderHeight + m_iCfgRowHeight + m_iCfgPadding - m_iScrollOffset;

	for (int i = 0; i < m_iNumSorted; i++)
	{
		int rowHeight = GetRowHeight(i);

		if (y >= currentY && y < currentY + rowHeight)
		{
			m_iHighlightRow = i;
			return;
		}

		currentY += rowHeight;
	}

	m_iHighlightRow = -1;
}

void CNuklearScoreboard::KeyPressed(int key)
{
	// ESC or TAB key closes scoreboard
	if (key == K_ESCAPE || key == K_TAB)
	{
		Show(false);
		ClientCmd(m_szCfgCmdHide);
	}
}

void CNuklearScoreboard::MouseWheel(int delta)
{
	if (!m_bVisible)
		return;

	int visibleHeight = m_iY + m_iHeight - 10 - (m_iY + m_iCfgHeaderHeight + m_iCfgRowHeight + m_iCfgPadding);
	int maxScroll = m_iTotalContentHeight - visibleHeight;
	if (maxScroll <= 0) return;

	// Scroll by one row per wheel notch
	int scrollStep = m_iCfgRowHeight;
	m_iScrollOffset -= delta * scrollStep;

	// Clamp
	if (m_iScrollOffset < 0) m_iScrollOffset = 0;
	if (m_iScrollOffset > maxScroll) m_iScrollOffset = maxScroll;
}

//=============================================================================
// Death message
//=============================================================================

void CNuklearScoreboard::DeathMsg(int killer, int victim)
{
	if (victim == m_iPlayerNum || killer == 0)
	{
		m_iLastKilledBy = killer ? killer : m_iPlayerNum;
		m_fLastKillTime = gHUD.m_flTime + 10.0f;

		if (killer == m_iPlayerNum)
		{
			m_iLastKilledBy = m_iPlayerNum;
		}
	}
}
