#ifndef     __FTGLTextureFont__
#define     __FTGLTextureFont__

struct GLFont;
typedef GLFont* Font;

Font createFont(const char* fontPath, size_t faceSize);
Font createFont(const unsigned char* fontData, size_t dataSize, size_t faceSize);
void destroyFont(Font font);

template<typename T> void drawString(Font font, const T* str);

#endif // __FTGLTextureFont__


