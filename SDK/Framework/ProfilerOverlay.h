#pragma once

#include <new>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

#include <string>
#include <vector>

#include <SDL.h>
#include <opengl.h>

#include <scintilla/Platform.h>

#include <scintilla/ILexer.h>
#include <scintilla/Scintilla.h>
#include <scintilla/SVector.h>
#include <scintilla/SplitVector.h>
#include <scintilla/Partitioning.h>
#include <scintilla/RunStyles.h>
#include <scintilla/ContractionState.h>
#include <scintilla/CellBuffer.h>
#include <scintilla/KeyMap.h>
#include <scintilla/Indicator.h>
#include <scintilla/XPM.h>
#include <scintilla/LineMarker.h>
#include <scintilla/Style.h>
#include <scintilla/ViewStyle.h>
#include <scintilla/Decoration.h>
#include <scintilla/CharClassify.h>
#include <scintilla/Document.h>
#include <scintilla/Selection.h>
#include <scintilla/PositionCache.h>
#include <scintilla/Editor.h>
#include <scintilla/UniConversion.h>

struct Vertex
{
    float x, y;
};

class ProfilerOverlay
{
public:
    void initialise(int w, int h);
    void update();

    void handleKeyDown(SDL_KeyboardEvent& event);
    void renderFullscreen();

private:
    void initialiseView();

private:
    Editor  mProfilerView;
    
    std::vector<Vertex> drawData;
    float mWidth;
    float mHeight;
};
