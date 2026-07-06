//========= Copyright  1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: New Scoreboard using GoldSrc HUD Drawing API
//
//=============================================================================

#ifndef NUKLEAR_SCOREBOARD_H
#define NUKLEAR_SCOREBOARD_H

#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"

// Team info
struct TeamInfo_t
{
	char name[MAX_TEAM_NAME];
	int players;
	int frags;
	int deaths;
	int ping;
	int packetloss;
	int teamnumber;
	bool scores_overriden;
	bool already_drawn;
	bool ownteam;
};

// Player info
struct PlayerInfo_t
{
	char name[MAX_PLAYER_NAME_LENGTH];
	int score;
	int deaths;
	int ping;
	int packetloss;
	int teamnumber;
	int playerclass;
	bool thisplayer;
	bool connected;
	bool alive;
};

#define MAX_DISPLAY_PLAYERS 32
#define MAX_SCOREBOARD_TEAMS 5
#define NUM_ROWS (MAX_PLAYERS + (MAX_SCOREBOARD_TEAMS * 2))

// Team type constants
#define TEAM_NO         0
#define TEAM_YES        1
#define TEAM_SPECTATORS 2
#define TEAM_BLANK      3

// Team color definition
struct TeamColor_t
{
	int r, g, b;
};

#define MAX_TEAM_COLORS 8
extern TeamColor_t g_TeamColors[MAX_TEAM_COLORS];

// Sorted player entry
struct SortedPlayer_t
{
	int playerIndex;
	bool isTeam;
	int teamIndex;
	int teamType;
};

//=============================================================================
// CNuklearScoreboard - New scoreboard using GoldSrc drawing API
//=============================================================================
class CNuklearScoreboard
{
public:
	CNuklearScoreboard();
	~CNuklearScoreboard();

	// Init and shutdown
	void Init();
	void VidInit();
	void Shutdown();

	// Config loading
	void LoadConfig();
	void SetDefaultConfig();

	// Show/hide
	void Show(bool show);
	bool IsVisible() const;

	// Multiplayer check
	bool IsMultiplayer() const;

	// Update data
	void Update();

	// Draw
	void Draw();

	// Input handling
	void MousePressed(int x, int y);
	void MouseMoved(int x, int y);
	void KeyPressed(int key);
	void MouseWheel(int delta);

	// Death message
	void DeathMsg(int killer, int victim);

	// Get highlight row
	int GetHighlightRow() const;

private:
	// Data processing
	void CollectPlayerData();
	void SortPlayers();
	void SortTeamPlayers(int teamIndex);
	void RebuildTeams();

	// Draw functions
	void DrawBackground();
	void DrawHeader();
	void DrawColumnHeaders();
	void DrawPlayerList();
	void DrawScrollbar(int contentHeight, int visibleHeight);
	void DrawPlayerRow(int x, int y, int row, int height);
	void DrawTeamRow(int x, int y, int row, int height, int teamIndex, int teamType);
	void DrawPlayerInfo(int row);
	void DrawHighlight(int row);

	// Utility functions
	int DrawString(int x, int y, const char* text, int r, int g, int b);
	int DrawStringCenter(int x, int y, const char* text, int r, int g, int b);
	void DrawRect(int x, int y, int w, int h, int r, int g, int b, int a);
	void DrawBorder(int x, int y, int w, int h, int border, int r, int g, int b, int a);
	void DrawVoiceIcon(int x, int y, int voiceState, int r, int g, int b);

	// SPR loading
	void LoadVoiceSprites();
	void FreeVoiceSprites();

	// Layout calculations
	void CalculateLayout();
	int GetRowY(int row);
	int GetRowHeight(int row);

	// State
	bool m_bVisible;
	bool m_bNeedsUpdate;

	// Layout
	int m_iX, m_iY;
	int m_iWidth, m_iHeight;
	int m_iContentY;
	int m_iScrollOffset;
	int m_iMaxVisibleRows;
	int m_iTotalContentHeight;

	// Highlight
	int m_iHighlightRow;
	int m_iHighlightCol;

	// Data
	TeamInfo_t m_TeamInfo[MAX_TEAMS];
	int m_iNumTeams;
	PlayerInfo_t m_PlayerInfo[MAX_PLAYERS + 1];

	// Sorted list
	SortedPlayer_t m_SortedPlayers[NUM_ROWS];
	int m_iNumSorted;

	// Sorted flag
	bool m_bHasBeenSorted[MAX_PLAYERS + 1];

	// Kill tracking
	int m_iLastKilledBy;
	float m_fLastKillTime;
	int m_iPlayerNum;

	// Font size
	int m_iCharWidth;
	int m_iCharHeight;
	int m_iTitleCharHeight;

	// Squelch management
	bool m_bSquelchMode;
	int m_iMutedPlayers[MAX_PLAYERS];

	// Server name
	char m_szServerName[256];

	// Config - dimensions and position
	int m_iCfgWidth;
	int m_iCfgHeight;
	int m_iCfgX;
	int m_iCfgY;

	// Config - title text
	char m_szCfgTitle[128];
	char m_szCfgSubtitle[128];

	// Config - column headers
	char m_szCfgColName[64];
	char m_szCfgColTeam[64];
	char m_szCfgColClass[64];
	char m_szCfgColScore[64];
	char m_szCfgColDeaths[64];
	char m_szCfgColPing[64];
	char m_szCfgColVoice[64];

	// Config - spectators title
	char m_szCfgSpectators[64];

	// Config - player count format
	char m_szCfgPlayerCountFormat[64];

	// Config - team format
	char m_szCfgTeamFormat[128];

	// Config - index format
	char m_szCfgIndexFormat[16];

	// Config - name format
	char m_szCfgNameFormat[64];

	// Config - score/death/ping format
	char m_szCfgScoreFormat[16];
	char m_szCfgDeathFormat[16];
	char m_szCfgPingFormat[16];

	// Config - voice status symbols
	char m_szCfgVoiceBanned[16];
	char m_szCfgVoiceSpeakingBlink[16];
	char m_szCfgVoiceSpeaking[16];
	char m_szCfgVoiceNeverSpoke[16];
	char m_szCfgVoiceNotSpeaking[16];

	// Config - voice status SPR icon paths
	char m_szCfgSprVoiceBanned[128];
	char m_szCfgSprVoiceSpeaking[128];
	char m_szCfgSprVoiceSpeakingBlink[128];
	char m_szCfgSprVoiceNeverSpoke[128];
	char m_szCfgSprVoiceNotSpeaking[128];

	// Config - console commands
	char m_szCfgCmdHide[64];
	char m_szCfgCmdVoiceMute[64];
	char m_szCfgCmdVoiceUnmute[64];

	// Config - colors (r,g,b,a)
	int m_iCfgBgColor[4];
	int m_iCfgBorderColor[4];
	int m_iCfgHeaderBgColor[4];
	int m_iCfgHeaderTextColor[4];
	int m_iCfgRowEvenColor[4];
	int m_iCfgRowOddColor[4];
	int m_iCfgSelfHighlightColor[4];

	// Config - text colors
	int m_iCfgPlayerCountColor[4];
	int m_iCfgSubtitleColor[4];
	int m_iCfgIndexColor[4];
	int m_iCfgDeathColor[4];
	int m_iCfgSelfPlayerColor[4];

	// Config - voice status colors
	int m_iCfgVoiceBannedColor[4];
	int m_iCfgVoiceSpeakingColor[4];
	int m_iCfgVoiceNeverSpokeColor[4];
	int m_iCfgVoiceNotSpeakingColor[4];

	// Config - ping colors and thresholds
	int m_iCfgPingGoodColor[4];
	int m_iCfgPingMediumColor[4];
	int m_iCfgPingBadColor[4];
	int m_iCfgPingGoodThreshold;
	int m_iCfgPingMediumThreshold;

	// Config - spectator row colors
	int m_iCfgSpectatorBgColor[4];
	int m_iCfgSpectatorTextColor[4];
	int m_iCfgSpectatorBorderColor[4];

	// Config - separator and kill highlight
	int m_iCfgSeparatorColor[4];
	int m_iCfgKillHighlightColor[4];

	// Config - scrollbar colors
	int m_iCfgScrollbarColor[4];
	int m_iCfgScrollbarThumbColor[4];

	// Config - column widths
	int m_iCfgColNameWidth;
	int m_iCfgColTeamWidth;
	int m_iCfgColClassWidth;
	int m_iCfgColScoreWidth;
	int m_iCfgColDeathsWidth;
	int m_iCfgColPingWidth;
	int m_iCfgColVoiceWidth;

	// Config - row heights
	int m_iCfgRowHeight;
	int m_iCfgTeamRowHeight;
	int m_iCfgHeaderHeight;

	// Config - border and padding
	int m_iCfgBorderWidth;
	int m_iCfgPadding;

	bool m_bConfigLoaded;

	// Voice status SPR icons
	HSPRITE m_hSprVoiceBanned;
	HSPRITE m_hSprVoiceSpeaking;
	HSPRITE m_hSprVoiceSpeakingBlink;
	HSPRITE m_hSprVoiceNeverSpoke;
	HSPRITE m_hSprVoiceNotSpeaking;
	bool m_bVoiceSpritesLoaded;
};

// Global instance
extern CNuklearScoreboard g_NuklearScoreboard;

// Global entry functions
extern void DrawNuklearScoreboard();
extern void InitNuklearScoreboard();

#endif // NUKLEAR_SCOREBOARD_H
