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
#include <map>

#include <SDL2/SDL.h>
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

#include <scintilla/SciLexer.h>
#include <scintilla/lexlib/LexerModule.h>
#include <scintilla/SciLexer.h>
#include <scintilla/lexlib/LexerModule.h>
#include <scintilla/Catalogue.h>

class LexState;

class ShaderEditOverlay
{
public:
    ShaderEditOverlay();
    ~ShaderEditOverlay();

    void initialise(int w, int h);
    void reset();

    void inputText(char* text);
    void handleKeyDown(SDL_KeyboardEvent& event);
    void renderFullscreen();

    bool requireReset() {bool prevValue=mRequireReset; mRequireReset = false; return prevValue;}

    void addPrograms(size_t count, GLuint* programs);

private:
    void initialiseShaderEditor();
    void initialiseDebugOutputView();
    void initialiseSelectionList();

    void saveShaderSource();
    void compileProgram();
    void fillListWithShaders();
    void fillListWithPrograms();
    void loadShaderSource();

private:
    enum ModesEnum
    {
        SELMODE_PROGRAM_LIST,
        SELMODE_SHADER_LIST
    };

private:
    static const size_t TICK_INTERVAL = 100;

    bool   mRequireReset;

    size_t mNextTick;

    ModesEnum mSelectionMode;
    GLuint    mSelectedProgram;
    GLuint    mSelectedShader;

    std::vector<GLuint> mPrograms;
    std::vector<GLuint> mAttachedShaders;

    LexState* mLexer;

    Editor  mShaderEditor;
    Editor  mDebugOutputView;
    Editor  mSelectionList;
    Editor* mActiveEditor;

    float mWidth;
    float mHeight;
};
