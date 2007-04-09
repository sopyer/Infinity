#pragma once

#include "glm\glmext.h"

class Cursor
{
	HWND m_hWnd;
	HCURSOR cursor;
public:
	Cursor(HWND hWnd)
	{
		m_hWnd = hWnd;
		cursor = 0;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	glm::vec2 get()
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd,&pt);
		return glm::vec2(pt.x, pt.y);
	}
	int set(int x,int y) 
	{
		POINT pt;
		pt.x = x;
		pt.y = y;
		ClientToScreen(m_hWnd,&pt);
		SetCursorPos(pt.x,pt.y);
		return 0;
	}

	/*
	 */
	int show(bool show) 
	{
		if(show && cursor)
		{
			SetCursor(cursor);
			//SetCapture(m_hWnd);
			cursor = 0;
		}
		else {
			if(!cursor) cursor = GetCursor();
			SetCursor(NULL);
			//ReleaseCapture();
		}
		return 0;
	}

};