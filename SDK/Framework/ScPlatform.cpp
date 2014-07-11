// ScintillaGL source code edit control
// PlatGTK.cxx - implementation of platform facilities on GTK+/Linux
// Copyright 2011 by Mykhailo Parfeniuk
// The License.txt file describes the conditions under which this software may be distributed.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>

#include <scintilla/Platform.h>
#include <scintilla/Scintilla.h>
#include <scintilla/UniConversion.h>
#include <scintilla/XPM.h>
#include <opengl.h>
#include <vg/vg.h>

#include "DefaultFontData.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

Colour Platform::Chrome() {
	return MakeRGBA(0xe0, 0xe0, 0xe0);
}

Colour Platform::ChromeHighlight() {
	return MakeRGBA(0xff, 0xff, 0xff);
}

const char *Platform::DefaultFont() {
	return "Lucida Console";
}

int Platform::DefaultFontSize() {
	return 10;
}

unsigned int Platform::DoubleClickTime() {
	return 500; 	// Half a second
}

bool Platform::MouseButtonBounce() {
	return true;
}

void Platform::Assert(const char *c, const char *file, int line) {
	char buffer[2000];
	sprintf(buffer, "Assertion [%s] failed at %s %d", c, file, line);
	strcat(buffer, "\r\n");
	assert(false);
}

#include <windows.h>
#include <winuser.h>
#include <wtypes.h>

CLIPFORMAT cfColumnSelect;
CLIPFORMAT cfLineSelect;

void Platform_Initialise()
{
	// There does not seem to be a real standard for indicating that the clipboard
	// contains a rectangular selection, so copy Developer Studio.
	cfColumnSelect = static_cast<CLIPFORMAT>(
		::RegisterClipboardFormat(TEXT("MSDEVColumnSelect")));

	// Likewise for line-copy (copies a full line when no text is selected)
	cfLineSelect = static_cast<CLIPFORMAT>(
		::RegisterClipboardFormat(TEXT("MSDEVLineSelect")));
}

void Platform_Finalise()
{
}

class GlobalMemory {
	HGLOBAL hand;
public:
	void *ptr;
	GlobalMemory() : hand(0), ptr(0) {
	}
	GlobalMemory(HGLOBAL hand_) : hand(hand_), ptr(0) {
		if (hand) {
			ptr = ::GlobalLock(hand);
		}
	}
	~GlobalMemory() {
		PLATFORM_ASSERT(!ptr);
	}
	void Allocate(size_t bytes) {
		hand = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, bytes);
		if (hand) {
			ptr = ::GlobalLock(hand);
		}
	}
	HGLOBAL Unlock() {
		PLATFORM_ASSERT(ptr);
		HGLOBAL handCopy = hand;
		::GlobalUnlock(hand);
		ptr = 0;
		hand = 0;
		return handCopy;
	}
	void SetClip(UINT uFormat) {
		::SetClipboardData(uFormat, Unlock());
	}
	operator bool() const {
		return ptr != 0;
	}
	SIZE_T Size() {
		return ::GlobalSize(hand);
	}
};

int IsClipboardTextAvailable(AdditionalTextFormat fmt)
{
	if (fmt == TEXT_FORMAT_UTF8_LINE) return ::IsClipboardFormatAvailable(cfLineSelect) != 0;
	if (fmt == TEXT_FORMAT_UTF8_RECT) return ::IsClipboardFormatAvailable(cfColumnSelect) != 0;
	if (::IsClipboardFormatAvailable(CF_TEXT))
		return true;
	return ::IsClipboardFormatAvailable(CF_UNICODETEXT) != 0;
}

void SetClipboardTextUTF8(const char* text, size_t len, int additionalFormat)
{
	if (!::OpenClipboard(NULL))
		return;
	::EmptyClipboard();

	GlobalMemory uniText;

	int uchars = UTF16Length(text, len);
	uniText.Allocate(2 * uchars);
	if (uniText) {
		UTF16FromUTF8(text, len, static_cast<wchar_t *>(uniText.ptr), uchars);
	}

	if (uniText) {
		uniText.SetClip(CF_UNICODETEXT);
	}

	if (additionalFormat==TEXT_FORMAT_UTF8_RECT) {
		::SetClipboardData(cfColumnSelect, 0);
	} else if (additionalFormat==TEXT_FORMAT_UTF8_LINE) {
		::SetClipboardData(cfLineSelect, 0);
	}

	::CloseClipboard();
}

int GetClipboardTextUTF8(char* text, size_t len)
{
	if (!::OpenClipboard(NULL))
		return 0;

	unsigned int clipLen = 0;

	GlobalMemory memUSelection(::GetClipboardData(CF_UNICODETEXT));
	if (memUSelection) {
		wchar_t *uptr = static_cast<wchar_t *>(memUSelection.ptr);
		unsigned int bytes = memUSelection.Size();
		clipLen = UTF8Length(uptr, bytes / 2);
		if (uptr && text) {
			UTF8FromUTF16(uptr, bytes / 2, text, len);
		}
		memUSelection.Unlock();
	} else {
		// CF_UNICODETEXT not available, paste ANSI text
		GlobalMemory memSelection(::GetClipboardData(CF_TEXT));
		if (memSelection) {
			char *ptr = static_cast<char *>(memSelection.ptr);
			if (ptr) {
				unsigned int bytes = memSelection.Size();
				unsigned int len = bytes;
				for (unsigned int i = 0; i < bytes; i++) {
					if ((len == bytes) && (0 == ptr[i]))
						len = i;
				}

				// convert clipboard text to UTF-8
				wchar_t *uptr = new wchar_t[len+1];

				unsigned int ulen = ::MultiByteToWideChar(CP_ACP, 0,
				                    ptr, len, uptr, len+1);

				clipLen = UTF8Length(uptr, ulen);
				if (text) {
					// CP_UTF8 not available on Windows 95, so use UTF8FromUTF16()
					UTF8FromUTF16(uptr, ulen, text, len);
				}

				delete []uptr;
			}
			memSelection.Unlock();
		}
	}
	::CloseClipboard();

	return clipLen;
}

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif
class SurfaceImpl : public Surface {
	Colour penColour;
	float x;
	float y;
public:
	SurfaceImpl();
	virtual ~SurfaceImpl();

	void Release();
	void PenColour(Colour fore);
	int LogPixelsY();
	float DeviceHeightFont(int points);
	void MoveTo(float x_, float y_);
	void LineTo(float x_, float y_);
	void Polygon(Point *pts, int npts, Colour fore, Colour back);
	void RectangleDraw(PRectangle rc, Colour fore, Colour back);
	void FillRectangle(PRectangle rc, Colour back);
	void FillRectangle(PRectangle rc, Surface &surfacePattern);
	void RoundedRectangle(PRectangle rc, Colour fore, Colour back);
	void AlphaRectangle(PRectangle rc, int cornerSize, Colour fill, int alphaFill,
		Colour outline, int alphaOutline, int flags);
	void Ellipse(PRectangle rc, Colour fore, Colour back);

	virtual void DrawPixmap(PRectangle rc, Point from, Pixmap pixmap);

	void DrawTextBase(PRectangle rc, Font &font_, float ybase, const char *s, int len, Colour fore);
	void DrawTextNoClip(PRectangle rc, Font &font_, float ybase, const char *s, int len, Colour fore, Colour back);
	void DrawTextClipped(PRectangle rc, Font &font_, float ybase, const char *s, int len, Colour fore, Colour back);
	void DrawTextTransparent(PRectangle rc, Font &font_, float ybase, const char *s, int len, Colour fore);
	void MeasureWidths(Font &font_, const char *s, int len, float *positions);
	float WidthText(Font &font_, const char *s, int len);
	float WidthChar(Font &font_, char ch);
	float Ascent(Font &font_);
	float Descent(Font &font_);
	float InternalLeading(Font &font_);
	float ExternalLeading(Font &font_);
	float Height(Font &font_);
	float AverageCharWidth(Font &font_);

	void SetClip(PRectangle rc);
	void FlushCachedState();
};
#ifdef SCI_NAMESPACE
}
#endif

SurfaceImpl::SurfaceImpl() : x(0), y(0) {
}

SurfaceImpl::~SurfaceImpl() {
}

void SurfaceImpl::Release() {
}

void SurfaceImpl::PenColour(Colour fore) {
	penColour = fore;
}

int SurfaceImpl::LogPixelsY() {
	return 72;
}

float SurfaceImpl::DeviceHeightFont(int points) {
	int logPix = LogPixelsY();
	return (points * logPix + logPix / 2) / 72.0f;
}

void SurfaceImpl::MoveTo(float x_, float y_) {
	x = x_;
	y = y_;
}

void SurfaceImpl::LineTo(float x_, float y_) {
	glColor4ubv((GLubyte*)&penColour);
	glBegin(GL_LINES);
	glVertex2f(x+0.5f,  y+0.5f);
	glVertex2f(x_+0.5f, y_+0.5f);
	glEnd();
	x = x_;
	y = y_;
}

void SurfaceImpl::Polygon(Point* /*pts*/, int /*npts*/, Colour /*fore*/,
                          Colour /*back*/) {
	assert(0);
}

void SurfaceImpl::RectangleDraw(PRectangle rc, Colour fore, Colour back) {
	FillRectangle(rc, back);
	glColor4ubv((GLubyte*)&fore);
	glBegin(GL_LINE_STRIP);
	glVertex2f(rc.left+0.5f,  rc.top+0.5f);
	glVertex2f(rc.right-0.5f, rc.top+0.5f);
	glVertex2f(rc.right-0.5f, rc.bottom-0.5f);
	glVertex2f(rc.left+0.5f,  rc.bottom-0.5f);
	glVertex2f(rc.left+0.5f,  rc.top+0.5f);
	glEnd();
}

struct pixmap_t
{
	GLuint tex;
	float scalex, scaley;
	bool initialised;
};

Pixmap	CreatePixmap()
{
	Pixmap pm = new pixmap_t;
	pm->scalex = 0;
	pm->scaley = 0;
	pm->initialised = false;

	return pm;
}

bool	IsPixmapInitialised(Pixmap pixmap)
{
	return pixmap->initialised;
}

void	DestroyPixmap(Pixmap pixmap)
{
	glDeleteTextures(1, &pixmap->tex);
	delete pixmap;
}

void	UpdatePixmap(Pixmap pixmap, int w, int h, int* data)
{
	if (!pixmap->initialised)
	{
		glGenTextures(1, &pixmap->tex);
		glBindTexture(GL_TEXTURE_2D, pixmap->tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, pixmap->tex);
	}

	pixmap->initialised = true;
	pixmap->scalex = 1.0f/w;
	pixmap->scaley = 1.0f/h;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SurfaceImpl::DrawPixmap(PRectangle rc, Point offset, Pixmap pixmap)
{
	float w = (rc.right-rc.left)*pixmap->scalex, h=(rc.bottom-rc.top)*pixmap->scaley;
	float u1 = offset.x*pixmap->scalex, v1 = offset.y*pixmap->scaley, u2 = u1+w, v2 = v1+h;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pixmap->tex);
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glBegin(GL_QUADS);
	glTexCoord2f(u1, v1);
	glVertex2f(rc.left,  rc.top);
	glTexCoord2f(u2, v1);
	glVertex2f(rc.right, rc.top);
	glTexCoord2f(u2, v2);
	glVertex2f(rc.right, rc.bottom);
	glTexCoord2f(u1, v2);
	glVertex2f(rc.left,  rc.bottom);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void SurfaceImpl::FillRectangle(PRectangle rc, Colour back) {
	glColor4ubv((GLubyte*)&back);
	glBegin(GL_QUADS);
	glVertex2f(rc.left,  rc.top);
	glVertex2f(rc.right, rc.top);
	glVertex2f(rc.right, rc.bottom);
	glVertex2f(rc.left,  rc.bottom);
	glEnd();
}

void SurfaceImpl::FillRectangle(PRectangle /*rc*/, Surface &/*surfacePattern*/) {
	assert(0);
}

void SurfaceImpl::RoundedRectangle(PRectangle /*rc*/, Colour /*fore*/, Colour /*back*/) {
	assert(0);
}

void SurfaceImpl::AlphaRectangle(PRectangle rc, int /*cornerSize*/, Colour fill, int alphaFill,
		Colour /*outline*/, int /*alphaOutline*/, int /*flags*/) {
	unsigned int back = fill&0xFFFFFF | ((alphaFill&0xFF)<<24);
	glColor4ubv((GLubyte*)&back);
	glBegin(GL_QUADS);
	glVertex2f(rc.left,  rc.top);
	glVertex2f(rc.right, rc.top);
	glVertex2f(rc.right, rc.bottom);
	glVertex2f(rc.left,  rc.bottom);
	glEnd();
}

void SurfaceImpl::Ellipse(PRectangle /*rc*/, Colour /*fore*/, Colour /*back*/) {
	assert(0);
}

const int maxLengthTextRun = 10000;

Font::Font() : fid(0)
{
}

Font::~Font()
{
}

void Font::Create(const char *faceName, int /*characterSet*/, int size,	bool /*bold*/, bool /*italic*/, int)
{
	vg::Font font = vg::createFont(anonymousProRTTF, sizeof(anonymousProRTTF), size);
	fid = font;
}

void Font::Release()
{
	if (fid)
	{
		vg::Font font = (vg::Font)fid;
		vg::destroyFont(font);
		fid=0;
	}
}

void SurfaceImpl::DrawTextBase(PRectangle rc, Font &font_, float ybase, const char *s, int len,
                                 Colour fore)
{
	glColor4ubv((GLubyte*)&fore);
	vg::drawString((vg::Font)font_.fid, rc.left, ybase, s, len);
}

void SurfaceImpl::DrawTextNoClip(PRectangle rc, Font &font_, float ybase, const char *s, int len,
                                 Colour fore, Colour /*back*/) {
	DrawTextBase(rc, font_, ybase, s, len, fore);
}

void SurfaceImpl::DrawTextClipped(PRectangle rc, Font &font_, float ybase, const char *s, int len,
                                  Colour fore, Colour /*back*/) {
	DrawTextBase(rc, font_, ybase, s, len, fore);
}

void SurfaceImpl::DrawTextTransparent(PRectangle rc, Font &font_, float ybase, const char *s, int len,
                                  Colour fore) {
	DrawTextBase(rc, font_, ybase, s, len, fore);
}

void SurfaceImpl::MeasureWidths(Font &font_, const char *s, int len, float *positions) {
	char* tmp = (char*)malloc(len+1);
	strncpy(tmp, s, len+1);
	char* c = tmp+1;
	while (len--) {
		char t = *c;
		*c = 0;
		*positions++  = vg::getTextHExtent((vg::Font)font_.fid, tmp);
		*c++ = t;
	}
}

float SurfaceImpl::WidthText(Font &font_, const char *s, int len) {
	assert(len==strlen(s));
	return vg::getTextHExtent((vg::Font)font_.fid, s);
}

float SurfaceImpl::WidthChar(Font &font_, char ch) {
	const char s[2] = {ch, 0};
	return vg::getTextHExtent((vg::Font)font_.fid, s);
}

float SurfaceImpl::Ascent(Font &font_) {
	return vg::getTextAscender((vg::Font)font_.fid);
}

float SurfaceImpl::Descent(Font &font_) {
	return -vg::getTextDescender((vg::Font)font_.fid);
}

float SurfaceImpl::InternalLeading(Font &) {
	//WTF is this?????
	return 0;
}

float SurfaceImpl::ExternalLeading(Font& font_) {
	//WTF is this?????
	return 4;
}

float SurfaceImpl::Height(Font &font_) {
	return Ascent(font_) + Descent(font_);
}

float SurfaceImpl::AverageCharWidth(Font &font_) {
	return WidthChar(font_, 'n');
}

void SurfaceImpl::SetClip(PRectangle rc) {
	double plane[][4] = {
		{ 1,  0, 0, -rc.left  },
		{-1,  0, 0,  rc.right },
		{ 0,  1, 0, -rc.top   },
		{ 0, -1, 0,  rc.bottom},
	};
	glClipPlane(GL_CLIP_PLANE0, plane[0]);
	glClipPlane(GL_CLIP_PLANE1, plane[1]);
	glClipPlane(GL_CLIP_PLANE2, plane[2]);
	glClipPlane(GL_CLIP_PLANE3, plane[3]);
	//assert(0);
}

void SurfaceImpl::FlushCachedState() {}

Surface *Surface::Allocate() {
	return new SurfaceImpl;
}
