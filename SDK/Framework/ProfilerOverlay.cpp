#include <windows.h>
#include <opengl.h>
#include <vg/vg.h>

#include "ProfilerOverlay.h"
#include "profiler.h"

//HACK: already defined in shader overlay!!!!
void SetAStyle(Editor& ed, int style, Colour fore, Colour back=0xFFFFFFFF, int size=-1, const char *face=0);
//{
//	ed.Command(SCI_STYLESETFORE, style, fore);
//	ed.Command(SCI_STYLESETBACK, style, back);
//	if (size >= 1)
//		ed.Command(SCI_STYLESETSIZE, style, size);
//	if (face) 
//		ed.Command(SCI_STYLESETFONT, style, reinterpret_cast<sptr_t>(face));
//}

void ProfilerOverlay::initialiseView() {
	mProfilerView.Command(SCI_SETSTYLEBITS, 7);

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle(mProfilerView, STYLE_DEFAULT,     0xFFFFFFFF, 0xD0000000, 16, "c:/windows/fonts/cour.ttf");
	mProfilerView.Command(SCI_STYLECLEARALL);	// Copies global style to all others
	mProfilerView.Command(SCI_SETMARGINWIDTHN, 1, 0);//Calculate correct width

	mProfilerView.Command(SCI_SETUSETABS, 1);
	mProfilerView.Command(SCI_SETTABWIDTH, 4);
	
	mProfilerView.Command(SCI_SETSELBACK,            1, 0xD0CC9966);
	mProfilerView.Command(SCI_SETCARETFORE,          0x00FFFFFF, 0);
	mProfilerView.Command(SCI_SETCARETLINEVISIBLE,   1);
	mProfilerView.Command(SCI_SETCARETLINEBACK,      0xFFFFFFFF);
	mProfilerView.Command(SCI_SETCARETLINEBACKALPHA, 0x20);
	mProfilerView.Command(SCI_SETUNDOCOLLECTION, 0);
}

void ProfilerOverlay::initialise(int w, int h)
{
	initialiseView();

	mWidth = (float)w; mHeight=(float)h;

	float w1=mWidth-60.0f, h1=mHeight-60.0f;/*60=30+30*/

	mProfilerView.SetSize(w1, h1);
}

void ProfilerOverlay::update()
{
	char* buf = (char*) malloc(profilerData.numBlocks*100);

	memset(buf, ' ', profilerData.numBlocks*100);

	char*	line=buf;
	size_t	st[10];
	size_t	nestLvl=0;

	st[nestLvl] = 0;

	for (size_t i=0; i<profilerData.numBlocks; ++i)
	{
		size_t           prevID = st[nestLvl];
		profiler_block_t prev   = profilerData.blocks[prevID];
		profiler_block_t block  = profilerData.blocks[i];
		
		while (nestLvl>0 && block.start>profilerData.blocks[st[nestLvl]].end)
			--nestLvl;

		if (block.end<prev.end)
			++nestLvl;

		st[nestLvl] = i;

		assert(nestLvl<10);

		size_t offset = 44-nestLvl*4;
		memcpy(line+nestLvl*4, block.name, min(offset, strlen(block.name)));
		line += 44;
		line +=_snprintf(line, 15, "%10.3f\n", block.duration);
		*line = ' ';
	}
	*line = 0;

	GLint len = strlen(buf);
	mProfilerView.Command(SCI_CLEARALL);
	mProfilerView.Command(SCI_ADDTEXT, len, reinterpret_cast<LPARAM>(buf));
	mProfilerView.Command(SCI_GOTOPOS, 0);
}

void ProfilerOverlay::renderFullscreen()
{
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, mWidth, 0, mHeight, 0, 500);
	glTranslatef(0, mHeight, 0);
	glScalef(1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);

	float w1=mWidth-80.0f, h1=mHeight-80.0f;

	glTranslatef(30, 30, 0);
	mProfilerView.Paint();

	glPopAttrib();
}

void ProfilerOverlay::handleKeyDown(SDL_KeyboardEvent& event)
{
	switch (event.keysym.sym)
	{
		case SDLK_UP:
			mProfilerView.Command(SCI_LINEUP);
			break;
		case SDLK_DOWN:
			mProfilerView.Command(SCI_LINEDOWN);
			break;
	}
}
