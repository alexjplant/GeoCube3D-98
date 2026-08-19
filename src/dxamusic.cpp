#include <stdafx.h>
#include "dxamusic.h"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
CAMusic::CAMusic(HWND hwnd) : m_hwnd(hwnd)
{
}

CAMusic::~CAMusic()
{
  Stop();
}

BOOL CAMusic::Play(char *Filename)
{
	char buffer[256];

	sprintf(buffer, "open %s type sequencer alias MUSIC", Filename);

	if (mciSendString("close all", NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}

	if (mciSendString(buffer, NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}

	if (mciSendString("play MUSIC notify", NULL, 0, m_hwnd) != 0)
	{
		return(FALSE);
	}

	return TRUE;
}

CAMusic::Stop()
{
	if (mciSendString("close all", NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}   

	return TRUE;
}


BOOL CAMusic::Pause(void)
{
	// Pause if we're not already paused
	if (mciSendString("stop MUSIC", NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}

	return TRUE;
}


BOOL CAMusic::Resume(void)
{
	// Resume midi
	if (mciSendString("play MUSIC notify", NULL, 0, m_hwnd) != 0)
	{
		return(FALSE);
	}

	return TRUE;
}


BOOL CAMusic::Restart(void)
{
	// Replay midi
	if (mciSendString("play MUSIC from 0 notify", NULL, 0, m_hwnd) != 0)
	{
		return(FALSE);
	}

	return TRUE;
}
