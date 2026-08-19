#ifndef _DXAMUSIC_H
#define _DXAMUSIC_H

class CAMusic
{
public:
	CAMusic( HWND hwnd ); 
	~CAMusic(); 

	BOOL Play(char *Filename);
	BOOL Stop(void);
	BOOL Pause(void);
	BOOL Resume(void);
	BOOL Restart(void);

  HWND m_hwnd;
};

#endif