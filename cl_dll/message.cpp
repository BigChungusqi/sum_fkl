/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// Message.cpp
//
// implementation of CHudMessage class
//

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#include "../utils/Nuklear-4.13.3/src/nuklear.h"

DECLARE_MESSAGE( m_Message, HudText )
DECLARE_MESSAGE( m_Message, GameTitle )

#define NK_UTF_INVALID 0xFFFD
#define NK_UTF_SIZE 4

NK_GLOBAL const unsigned char nk_utfbyte[NK_UTF_SIZE+1] = {0x80, 0, 0xC0, 0xE0, 0xF0};
NK_GLOBAL const unsigned char nk_utfmask[NK_UTF_SIZE+1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
NK_GLOBAL const unsigned int nk_utfmin[NK_UTF_SIZE+1] = {0, 0, 0x80, 0x800, 0x10000};
NK_GLOBAL const unsigned int nk_utfmax[NK_UTF_SIZE+1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

static nk_rune nk_utf_decode_byte_impl(char c, int *i)
{
	if (!i) return 0;
	for(*i = 0; *i < (int)(sizeof(nk_utfmask)/sizeof(nk_utfmask[0])); ++(*i)) {
		if (((unsigned char)c & nk_utfmask[*i]) == nk_utfbyte[*i])
			return (unsigned char)(c & ~nk_utfmask[*i]);
	}
	return 0;
}

static int nk_utf_validate_impl(nk_rune *u, int i)
{
	if (!u) return 0;
	if (!NK_BETWEEN(*u, nk_utfmin[i], nk_utfmax[i]) ||
		 NK_BETWEEN(*u, 0xD800, 0xDFFF))
			*u = NK_UTF_INVALID;
	for (i = 1; *u > nk_utfmax[i]; ++i);
	return i;
}

int nk_utf_decode(const char *c, nk_rune *u, int clen)
{
	int i, j, len, type=0;
	nk_rune udecoded;

	if (!c || !u) return 0;
	if (!clen) return 0;
	*u = NK_UTF_INVALID;

	udecoded = nk_utf_decode_byte_impl(c[0], &len);
	if (!NK_BETWEEN(len, 1, NK_UTF_SIZE+1))
		return 1;

	for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
		udecoded = (udecoded << 6) | nk_utf_decode_byte_impl(c[i], &type);
		if (type != 0)
			return j;
	}
	if (j < len)
		return 0;
	*u = udecoded;
	nk_utf_validate_impl(u, len);
	return len;
}

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#define STBTT_malloc(x,u) malloc(x)
#define STBTT_free(x,u) free(x)
#include "../utils/Nuklear-4.13.3/src/stb_truetype.h"

static struct stbtt_fontinfo g_fontInfo = {0};
static unsigned char* g_fontData = NULL;
static float g_fontScale = 0.0f;
static int g_fontAscent = 0;
static int g_fontDescent = 0;
static int g_fontLineGap = 0;
static float g_fontHeight = 0.0f;
static int g_fontLoaded = 0;
static float g_fontSize = 18.0f;

static int LoadChineseFont()
{
	if (g_fontLoaded) return 1;

	const char* fontPaths[] = {
		"msyh.ttf",
		"msyhbd.ttf",
		"msyhlight.ttf",
		"simhei.ttf",
		"simfang.ttf",
		"simkai.ttf",
		"simsun.ttc",
		"arialuni.ttf",
		"droidfallback.ttf",
		"DroidSans.ttf",
		NULL
	};

	char sysFontPath[MAX_PATH] = {0};
	char gameFontPath[MAX_PATH] = {0};

	for (int i = 0; fontPaths[i]; i++)
	{
		snprintf(sysFontPath, sizeof(sysFontPath), "C:/Windows/Fonts/%s", fontPaths[i]);
		FILE* fp = fopen(sysFontPath, "rb");
		if (!fp)
		{
			snprintf(sysFontPath, sizeof(sysFontPath), "D:/Windows/Fonts/%s", fontPaths[i]);
			fp = fopen(sysFontPath, "rb");
		}
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			long size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			g_fontData = (unsigned char*)malloc(size);
			if (g_fontData)
			{
				fread(g_fontData, 1, size, fp);
				if (stbtt_InitFont(&g_fontInfo, g_fontData, stbtt_GetFontOffsetForIndex(g_fontData, 0)))
				{
					g_fontScale = stbtt_ScaleForPixelHeight(&g_fontInfo, g_fontSize);
					stbtt_GetFontVMetrics(&g_fontInfo, &g_fontAscent, &g_fontDescent, &g_fontLineGap);
					g_fontHeight = (g_fontAscent - g_fontDescent + g_fontLineGap) * g_fontScale;
					g_fontLoaded = 1;
					fclose(fp);
					return 1;
				}
				free(g_fontData);
				g_fontData = NULL;
			}
			fclose(fp);
		}

		const char* gamedir = gEngfuncs.pfnGetGameDirectory();
		if (gamedir && gamedir[0])
		{
			snprintf(gameFontPath, sizeof(gameFontPath), "%s/%s", gamedir, fontPaths[i]);
			fp = fopen(gameFontPath, "rb");
			if (fp)
			{
				fseek(fp, 0, SEEK_END);
				long size = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				g_fontData = (unsigned char*)malloc(size);
				if (g_fontData)
				{
					fread(g_fontData, 1, size, fp);
					if (stbtt_InitFont(&g_fontInfo, g_fontData, stbtt_GetFontOffsetForIndex(g_fontData, 0)))
					{
						g_fontScale = stbtt_ScaleForPixelHeight(&g_fontInfo, g_fontSize);
						stbtt_GetFontVMetrics(&g_fontInfo, &g_fontAscent, &g_fontDescent, &g_fontLineGap);
						g_fontHeight = (g_fontAscent - g_fontDescent + g_fontLineGap) * g_fontScale;
						g_fontLoaded = 1;
						fclose(fp);
						return 1;
					}
					free(g_fontData);
					g_fontData = NULL;
				}
				fclose(fp);
			}

			snprintf(gameFontPath, sizeof(gameFontPath), "%s/gfx/%s", gamedir, fontPaths[i]);
			fp = fopen(gameFontPath, "rb");
			if (fp)
			{
				fseek(fp, 0, SEEK_END);
				long size = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				g_fontData = (unsigned char*)malloc(size);
				if (g_fontData)
				{
					fread(g_fontData, 1, size, fp);
					if (stbtt_InitFont(&g_fontInfo, g_fontData, stbtt_GetFontOffsetForIndex(g_fontData, 0)))
					{
						g_fontScale = stbtt_ScaleForPixelHeight(&g_fontInfo, g_fontSize);
						stbtt_GetFontVMetrics(&g_fontInfo, &g_fontAscent, &g_fontDescent, &g_fontLineGap);
						g_fontHeight = (g_fontAscent - g_fontDescent + g_fontLineGap) * g_fontScale;
						g_fontLoaded = 1;
						fclose(fp);
						return 1;
					}
					free(g_fontData);
					g_fontData = NULL;
				}
				fclose(fp);
			}
		}
	}

	return 0;
}

static float GetCharWidth(unsigned int ch)
{
	if (!g_fontLoaded)
	{
		if (ch < 256)
			return gHUD.m_scrinfo.charWidths[ch];
		return g_fontSize;
	}

	int glyphIndex = stbtt_FindGlyphIndex(&g_fontInfo, ch);
	if (glyphIndex == 0)
	{
		// Character not found, return default width
		return g_fontSize * 0.6f;
	}

	int advanceWidth, leftSideBearing;
	stbtt_GetGlyphHMetrics(&g_fontInfo, glyphIndex, &advanceWidth, &leftSideBearing);
	return advanceWidth * g_fontScale;
}

static int DrawCharGlyph(int x, int y, unsigned int ch, int r, int g, int b)
{
	// If font not loaded, use engine default for ASCII
	if (!g_fontLoaded)
	{
		if (ch < 256)
			TextMessageDrawChar(x, y, (unsigned char)ch, r, g, b);
		return (int)GetCharWidth(ch);
	}

	// Check if glyph exists in font - skip if not found (no box rendering)
	int glyphIndex = stbtt_FindGlyphIndex(&g_fontInfo, ch);
	if (glyphIndex == 0)
	{
		// Character not in font, return width but don't draw anything
		return (int)GetCharWidth(ch);
	}

	if (r > 255) r = 255; if (r < 0) r = 0;
	if (g > 255) g = 255; if (g < 0) g = 0;
	if (b > 255) b = 255; if (b < 0) b = 0;

	int advanceWidth, leftSideBearing;
	stbtt_GetGlyphHMetrics(&g_fontInfo, glyphIndex, &advanceWidth, &leftSideBearing);

	int glyphWidth, glyphHeight, xOffset, yOffset;
	unsigned char* bitmap = stbtt_GetGlyphBitmap(&g_fontInfo, 0, g_fontScale, glyphIndex, &glyphWidth, &glyphHeight, &xOffset, &yOffset);

	if (bitmap && glyphWidth > 0 && glyphHeight > 0)
	{
		int drawX = x + (int)(leftSideBearing * g_fontScale) + xOffset;
		int drawY = y + (int)(g_fontAscent * g_fontScale) + yOffset;

		for (int by = 0; by < glyphHeight; by++)
		{
			for (int bx = 0; bx < glyphWidth; bx++)
			{
				int alpha = bitmap[by * glyphWidth + bx];
				if (alpha > 64)  // Threshold to avoid faint artifacts
				{
					int px = drawX + bx;
					int py = drawY + by;
					if (px >= 0 && px < ScreenWidth && py >= 0 && py < ScreenHeight)
					{
						FillRGBA(px, py, 1, 1, r, g, b, alpha);
					}
				}
			}
		}
		stbtt_FreeBitmap(bitmap, NULL);
	}

	return (int)(advanceWidth * g_fontScale);
}

static float GetTextWidth(const char* text)
{
	if (!text || !*text) return 0;

	float width = 0;
	unsigned int unicode;
	int len = strlen(text);
	int src_len = 0;
	int glyph_len = nk_utf_decode(text, &unicode, len);

	while (glyph_len && src_len < len)
	{
		width += GetCharWidth(unicode);
		src_len += glyph_len;
		glyph_len = nk_utf_decode(text + src_len, &unicode, len - src_len);
	}

	return width;
}

static int GetLineHeight()
{
	if (g_fontLoaded)
		return (int)g_fontHeight;
	return gHUD.m_scrinfo.iCharHeight;
}

// 1 Global client_textmessage_t for custom messages that aren't in the titles.txt
client_textmessage_t	g_pCustomMessage;
char *g_pCustomName = "Custom";
char g_pCustomText[1024];

int CHudMessage::Init(void)
{
	HOOK_MESSAGE( HudText );
	HOOK_MESSAGE( GameTitle );

	gHUD.AddHudElem(this);

	Reset();

	LoadChineseFont();

	return 1;
};

int CHudMessage::VidInit( void )
{
	m_HUD_title_half = gHUD.GetSpriteIndex( "title_half" );
	m_HUD_title_life = gHUD.GetSpriteIndex( "title_life" );

	LoadChineseFont();

	return 1;
};


void CHudMessage::Reset( void )
{
 	memset( m_pMessages, 0, sizeof( m_pMessages[0] ) * maxHUDMessages );
	memset( m_startTime, 0, sizeof( m_startTime[0] ) * maxHUDMessages );
	
	m_gameTitleTime = 0;
	m_pGameTitle = NULL;
}


float CHudMessage::FadeBlend( float fadein, float fadeout, float hold, float localTime )
{
	float fadeTime = fadein + hold;
	float fadeBlend;

	if ( localTime < 0 )
		return 0;

	if ( localTime < fadein )
	{
		fadeBlend = 1 - ((fadein - localTime) / fadein);
	}
	else if ( localTime > fadeTime )
	{
		if ( fadeout > 0 )
			fadeBlend = 1 - ((localTime - fadeTime) / fadeout);
		else
			fadeBlend = 0;
	}
	else
		fadeBlend = 1;

	return fadeBlend;
}


int	CHudMessage::XPosition( float x, int width, int totalWidth )
{
	int xPos;

	if ( x == -1 )
	{
		xPos = (ScreenWidth - width) / 2;
	}
	else
	{
		if ( x < 0 )
			xPos = (1.0 + x) * ScreenWidth - totalWidth;
		else
			xPos = x * ScreenWidth;
	}

	if ( xPos + width > ScreenWidth )
		xPos = ScreenWidth - width;
	else if ( xPos < 0 )
		xPos = 0;

	return xPos;
}


int CHudMessage::YPosition( float y, int height )
{
	int yPos;

	if ( y == -1 )
		yPos = (ScreenHeight - height) * 0.5;
	else
	{
		if ( y < 0 )
			yPos = (1.0 + y) * ScreenHeight - height;
		else
			yPos = y * ScreenHeight;
	}

	if ( yPos + height > ScreenHeight )
		yPos = ScreenHeight - height;
	else if ( yPos < 0 )
		yPos = 0;

	return yPos;
}


void CHudMessage::MessageScanNextChar( unsigned int unicode )
{
	int srcRed, srcGreen, srcBlue, destRed, destGreen, destBlue;
	int blend;

	srcRed = m_parms.pMessage->r1;
	srcGreen = m_parms.pMessage->g1;
	srcBlue = m_parms.pMessage->b1;
	blend = 0;

	switch( m_parms.pMessage->effect )
	{
	case 0:
	case 1:
		destRed = destGreen = destBlue = 0;
		blend = m_parms.fadeBlend;
		break;

	case 2:
		m_parms.charTime += m_parms.pMessage->fadein;
		if ( m_parms.charTime > m_parms.time )
		{
			srcRed = srcGreen = srcBlue = 0;
			blend = 0;
		}
		else
		{
			float deltaTime = m_parms.time - m_parms.charTime;

			destRed = destGreen = destBlue = 0;
			if ( m_parms.time > m_parms.fadeTime )
			{
				blend = m_parms.fadeBlend;
			}
			else if ( deltaTime > m_parms.pMessage->fxtime )
				blend = 0;
			else
			{
				destRed = m_parms.pMessage->r2;
				destGreen = m_parms.pMessage->g2;
				destBlue = m_parms.pMessage->b2;
				blend = 255 - (deltaTime * (1.0/m_parms.pMessage->fxtime) * 255.0 + 0.5);
			}
		}
		break;
	}
	if ( blend > 255 )
		blend = 255;
	else if ( blend < 0 )
		blend = 0;

	m_parms.r = ((srcRed * (255-blend)) + (destRed * blend)) >> 8;
	m_parms.g = ((srcGreen * (255-blend)) + (destGreen * blend)) >> 8;
	m_parms.b = ((srcBlue * (255-blend)) + (destBlue * blend)) >> 8;

	if ( m_parms.pMessage->effect == 1 && m_parms.charTime != 0 )
	{
		if ( m_parms.x >= 0 && m_parms.y >= 0 && (m_parms.x + GetCharWidth(unicode)) <= ScreenWidth )
			DrawCharGlyph( m_parms.x, m_parms.y, unicode, m_parms.pMessage->r2, m_parms.pMessage->g2, m_parms.pMessage->b2 );
	}
}


void CHudMessage::MessageScanStart( void )
{
	switch( m_parms.pMessage->effect )
	{
	case 1:
	case 0:
		m_parms.fadeTime = m_parms.pMessage->fadein + m_parms.pMessage->holdtime;
		
		if ( m_parms.time < m_parms.pMessage->fadein )
		{
			m_parms.fadeBlend = ((m_parms.pMessage->fadein - m_parms.time) * (1.0/m_parms.pMessage->fadein) * 255);
		}
		else if ( m_parms.time > m_parms.fadeTime )
		{
			if ( m_parms.pMessage->fadeout > 0 )
				m_parms.fadeBlend = (((m_parms.time - m_parms.fadeTime) / m_parms.pMessage->fadeout) * 255);
			else
				m_parms.fadeBlend = 255;
		}
		else
			m_parms.fadeBlend = 0;
		m_parms.charTime = 0;

		if ( m_parms.pMessage->effect == 1 && (rand()%100) < 10 )
			m_parms.charTime = 1;
		break;

	case 2:
		m_parms.fadeTime = (m_parms.pMessage->fadein * m_parms.length) + m_parms.pMessage->holdtime;
		
		if ( m_parms.time > m_parms.fadeTime && m_parms.pMessage->fadeout > 0 )
			m_parms.fadeBlend = (((m_parms.time - m_parms.fadeTime) / m_parms.pMessage->fadeout) * 255);
		else
			m_parms.fadeBlend = 0;
		break;
	}
}


void CHudMessage::MessageDrawScan( client_textmessage_t *pMessage, float time )
{
	int i, length;
	const char *pText;

	pText = pMessage->pMessage;
	m_parms.lines = 1;
	m_parms.time = time;
	m_parms.pMessage = pMessage;
	length = 0;
	m_parms.totalWidth = 0;

	const char* lineStart = pText;
	while ( *pText )
	{
		if ( *pText == '\n' )
		{
			m_parms.lines++;
			float lineWidth = GetTextWidth(lineStart);
			if ( lineWidth > m_parms.totalWidth )
				m_parms.totalWidth = lineWidth;
			lineStart = pText + 1;
		}
		pText++;
		length++;
	}
	float lastLineWidth = GetTextWidth(lineStart);
	if ( lastLineWidth > m_parms.totalWidth )
		m_parms.totalWidth = lastLineWidth;

	m_parms.length = length;
	m_parms.totalHeight = (m_parms.lines * GetLineHeight());

	m_parms.y = YPosition( pMessage->y, m_parms.totalHeight );
	pText = pMessage->pMessage;

	m_parms.charTime = 0;

	MessageScanStart();

	for ( i = 0; i < m_parms.lines; i++ )
	{
		m_parms.width = 0;
		const char* lineStart = pText;
		unsigned int unicode;
		int lineLen = 0;
		int textLen = strlen(pText);
		int srcLen = 0;
		int glyphLen = nk_utf_decode(pText, &unicode, textLen);

		while ( glyphLen && srcLen < textLen )
		{
			if ( unicode == '\n' )
				break;
			m_parms.width += GetCharWidth(unicode);
			lineLen += glyphLen;
			srcLen += glyphLen;
			glyphLen = nk_utf_decode(pText + srcLen, &unicode, textLen - srcLen);
		}

		m_parms.x = XPosition( pMessage->x, m_parms.width, (int)m_parms.totalWidth );

		srcLen = 0;
		glyphLen = nk_utf_decode(lineStart, &unicode, lineLen);
		int drawX = m_parms.x;

		while ( glyphLen && srcLen < lineLen )
		{
			if ( unicode == '\n' )
				break;

			MessageScanNextChar(unicode);
			
			if ( drawX >= 0 && m_parms.y >= 0 && (drawX + GetCharWidth(unicode)) <= ScreenWidth )
				drawX += DrawCharGlyph( drawX, m_parms.y, unicode, m_parms.r, m_parms.g, m_parms.b );
			else
				drawX += GetCharWidth(unicode);

			srcLen += glyphLen;
			glyphLen = nk_utf_decode(lineStart + srcLen, &unicode, lineLen - srcLen);
		}

		if ( *pText == '\n' )
			pText++;
		m_parms.y += GetLineHeight();
	}
}


int CHudMessage::Draw( float fTime )
{
	int i, drawn;
	client_textmessage_t *pMessage;
	float endTime;

	drawn = 0;

	if ( m_gameTitleTime > 0 )
	{
		float localTime = gHUD.m_flTime - m_gameTitleTime;
		float brightness;

		if ( m_gameTitleTime > gHUD.m_flTime )
			m_gameTitleTime = gHUD.m_flTime;

		if ( localTime > (m_pGameTitle->fadein + m_pGameTitle->holdtime + m_pGameTitle->fadeout) )
			m_gameTitleTime = 0;
		else
		{
			brightness = FadeBlend( m_pGameTitle->fadein, m_pGameTitle->fadeout, m_pGameTitle->holdtime, localTime );

			int halfWidth = gHUD.GetSpriteRect(m_HUD_title_half).right - gHUD.GetSpriteRect(m_HUD_title_half).left;
			int fullWidth = halfWidth + gHUD.GetSpriteRect(m_HUD_title_life).right - gHUD.GetSpriteRect(m_HUD_title_life).left;
			int fullHeight = gHUD.GetSpriteRect(m_HUD_title_half).bottom - gHUD.GetSpriteRect(m_HUD_title_half).top;

			int x = XPosition( m_pGameTitle->x, fullWidth, fullWidth );
			int y = YPosition( m_pGameTitle->y, fullHeight );


			SPR_Set( gHUD.GetSprite(m_HUD_title_half), brightness * m_pGameTitle->r1, brightness * m_pGameTitle->g1, brightness * m_pGameTitle->b1 );
			SPR_DrawAdditive( 0, x, y, &gHUD.GetSpriteRect(m_HUD_title_half) );

			SPR_Set( gHUD.GetSprite(m_HUD_title_life), brightness * m_pGameTitle->r1, brightness * m_pGameTitle->g1, brightness * m_pGameTitle->b1 );
			SPR_DrawAdditive( 0, x + halfWidth, y, &gHUD.GetSpriteRect(m_HUD_title_life) );

			drawn = 1;
		}
	}
	for ( i = 0; i < maxHUDMessages; i++ )
	{
		if ( m_pMessages[i] )
		{
			pMessage = m_pMessages[i];
			if ( m_startTime[i] > gHUD.m_flTime )
				m_startTime[i] = gHUD.m_flTime + m_parms.time - m_startTime[i] + 0.2;
		}
	}

	for ( i = 0; i < maxHUDMessages; i++ )
	{
		if ( m_pMessages[i] )
		{
			pMessage = m_pMessages[i];

			switch( pMessage->effect )
			{
			case 0:
			case 1:
				endTime = m_startTime[i] + pMessage->fadein + pMessage->fadeout + pMessage->holdtime;
				break;
			
			case 2:
				endTime = m_startTime[i] + (pMessage->fadein * strlen( pMessage->pMessage )) + pMessage->fadeout + pMessage->holdtime;
				break;
			}

			if ( fTime <= endTime )
			{
				float messageTime = fTime - m_startTime[i];

				MessageDrawScan( pMessage, messageTime );

				drawn++;
			}
			else
			{
				m_pMessages[i] = NULL;
			}
		}
	}

	m_parms.time = gHUD.m_flTime;
	if ( !drawn )
		m_iFlags &= ~HUD_ACTIVE;

	return 1;
}


void CHudMessage::MessageAdd( const char *pName, float time )
{
	int i,j;
	client_textmessage_t *tempMessage;

	for ( i = 0; i < maxHUDMessages; i++ )
	{
		if ( !m_pMessages[i] )
		{
			if ( pName[0] == '#' ) 
				tempMessage = TextMessageGet( pName+1 );
			else
				tempMessage = TextMessageGet( pName );
			if ( !tempMessage )
			{
				g_pCustomMessage.effect = 2;
				g_pCustomMessage.r1 = g_pCustomMessage.g1 = g_pCustomMessage.b1 = g_pCustomMessage.a1 = 100;
				g_pCustomMessage.r2 = 240;
				g_pCustomMessage.g2 = 110;
				g_pCustomMessage.b2 = 0;
				g_pCustomMessage.a2 = 0;
				g_pCustomMessage.x = -1;
				g_pCustomMessage.y = 0.7;
				g_pCustomMessage.fadein = 0.01;
				g_pCustomMessage.fadeout = 1.5;
				g_pCustomMessage.fxtime = 0.25;
				g_pCustomMessage.holdtime = 5;
				g_pCustomMessage.pName = g_pCustomName;
				strcpy( g_pCustomText, pName );
				g_pCustomMessage.pMessage = g_pCustomText;

				tempMessage = &g_pCustomMessage;
			}

			for ( j = 0; j < maxHUDMessages; j++ )
			{
				if ( m_pMessages[j] )
				{
					if ( !strcmp( tempMessage->pMessage, m_pMessages[j]->pMessage ) )
					{
						return;
					}

					if ( fabs( tempMessage->y - m_pMessages[j]->y ) < 0.0001 )
					{
						if ( fabs( tempMessage->x - m_pMessages[j]->x ) < 0.0001 )
						{
							m_pMessages[j] = NULL;
						}
					}
				}
			}

			m_pMessages[i] = tempMessage;
			m_startTime[i] = time;
			return;
		}
	}
}


int CHudMessage::MsgFunc_HudText( const char *pszName,  int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	char *pString = READ_STRING();

	MessageAdd( pString, gHUD.m_flTime );
	m_parms.time = gHUD.m_flTime;

	if ( !(m_iFlags & HUD_ACTIVE) )
		m_iFlags |= HUD_ACTIVE;

	return 1;
}


int CHudMessage::MsgFunc_GameTitle( const char *pszName,  int iSize, void *pbuf )
{
	m_pGameTitle = TextMessageGet( "GAMETITLE" );
	if ( m_pGameTitle != NULL )
	{
		m_gameTitleTime = gHUD.m_flTime;

		if ( !(m_iFlags & HUD_ACTIVE) )
			m_iFlags |= HUD_ACTIVE;
	}

	return 1;
}

void CHudMessage::MessageAdd(client_textmessage_t * newMessage )
{
	m_parms.time = gHUD.m_flTime;

	if ( !(m_iFlags & HUD_ACTIVE) )
		m_iFlags |= HUD_ACTIVE;

	for ( int i = 0; i < maxHUDMessages; i++ )
	{
		if ( !m_pMessages[i] )
		{
			m_pMessages[i] = newMessage;
			m_startTime[i] = gHUD.m_flTime;
			return;
		}
	}

}