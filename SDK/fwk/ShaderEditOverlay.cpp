#include <gfx/gfx.h>

#include "ShaderEditOverlay.h"

class LexState : public LexInterface {
    const LexerModule *lexCurrent;
public:
    int lexLanguage;

    LexState(Document *pdoc_) : LexInterface(pdoc_) {
        lexCurrent = 0;
        performingStyle = false;
        lexLanguage = SCLEX_CONTAINER;
    }

    ~LexState() {
        if (instance) {
            instance->Release();
            instance = 0;
        }
    }

    void SetLexerModule(const LexerModule *lex) {
        if (lex != lexCurrent) {
            if (instance) {
                instance->Release();
                instance = 0;
            }
            lexCurrent = lex;
            if (lexCurrent)
                instance = lexCurrent->Create();
            pdoc->LexerChanged();
        }
    }

    void SetLexer(uptr_t wParam) {
        lexLanguage = wParam;
        if (lexLanguage == SCLEX_CONTAINER) {
            SetLexerModule(0);
        } else {
            const LexerModule *lex = Catalogue::Find(lexLanguage);
            if (!lex)
                lex = Catalogue::Find(SCLEX_NULL);
            SetLexerModule(lex);
        }
    }

    void SetLexerLanguage(const char *languageName) {
        const LexerModule *lex = Catalogue::Find(languageName);
        if (!lex)
            lex = Catalogue::Find(SCLEX_NULL);
        if (lex)
            lexLanguage = lex->GetLanguage();
        SetLexerModule(lex);
    }

    void SetWordList(int n, const char *wl) {
        if (instance) {
            int firstModification = instance->WordListSet(n, wl);
            if (firstModification >= 0) {
                pdoc->ModifiedAt(firstModification);
            }
        }
    }

    void PropSet(const char *key, const char *val) {
        if (instance) {
            int firstModification = instance->PropertySet(key, val);
            if (firstModification >= 0) {
                pdoc->ModifiedAt(firstModification);
            }
        }
    }
};

ShaderEditOverlay::ShaderEditOverlay():mRequireReset(false)
{
    mNextTick = 0;
    mLexer = new LexState(mShaderEditor.GetDocument());
    mShaderEditor.SetLexer(mLexer);
}

ShaderEditOverlay::~ShaderEditOverlay()
{
    delete mLexer;
}

void ShaderEditOverlay::addPrograms(size_t count, GLuint* programs)
{
    mPrograms.insert(mPrograms.end(), programs, programs+count);
}

const char glslKeyword[] =
    "discard struct if else switch case default break goto return for while do continue";

const char glslType[] = 
    "attribute const in inout out uniform varying invariant "
    "centroid flat smooth noperspective layout patch sample "
    "subroutine lowp mediump highp precision "
    "void float vec2 vec3 vec4 bvec2 bvec3 bvec4 ivec2 ivec3 ivec4 "
    "uvec2 uvec3 uvec4 dvec2 dvec3 dvec4 "
    "sampler1D sampler2D sampler3D isampler2D isampler1D isampler3D "
    "usampler1D usampler2D usampler3D "
    "sampler1DShadow sampler2DShadow sampler1DArray sampler2DArray "
    "sampler1DArrayShadow sampler2DArrayShadow "
    "samplerCube samperCubeShadow samperCubeArrayShadow ";

const char glslBuiltin[] = 
    "radians degrees sin cos tan asin acos atan sinh "
    "cosh tanh asinh acosh atanh pow exp log exp2 "
    "log2 sqrt inversesqrt abs sign floor trunc round "
    "roundEven ceil fract mod modf min max clamp mix "
    "step smoothstep isnan isinf floatBitsToInt floatBitsToUint "
    "intBitsToFloat uintBitsToFloat fma frexp ldexp packUnorm2x16 "
    "packUnorm4x8 packSnorm4x8 unpackUnorm2x16 unpackUnorm4x8 "
    "unpackSnorm4x8 packDouble2x32 unpackDouble2x32 length distance "
    "dot cross normalize ftransform faceforward reflect "
    "refract matrixCompMult outerProduct transpose determinant "
    "inverse lessThan lessThanEqual greaterThan greaterThanEqual "
    "equal notEqual any all not uaddCarry usubBorrow "
    "umulExtended imulExtended bitfieldExtract bitfildInsert "
    "bitfieldReverse bitCount findLSB findMSB textureSize "
    "textureQueryLOD texture textureProj textureLod textureOffset "
    "texelFetch texelFetchOffset textureProjOffset textureLodOffset "
    "textureProjLod textureProjLodOffset textureGrad textureGradOffset "
    "textureProjGrad textureProjGradOffset textureGather "
    "textureGatherOffset texture1D texture2D texture3D texture1DProj "
    "texture2DProj texture3DProj texture1DLod texture2DLod "
    "texture3DLod texture1DProjLod texture2DProjLod texture3DProjLod "
    "textureCube textureCubeLod shadow1D shadow2D shadow1DProj "
    "shadow2DProj shadow1DLod shadow2DLod shadow1DProjLod "
    "shadow2DProjLod dFdx dFdy fwidth interpolateAtCentroid "
    "interpolateAtSample interpolateAtOffset noise1 noise2 noise3 "
    "noise4 EmitStreamVertex EndStreamPrimitive EmitVertex "
    "EndPrimitive barrier "
    "gl_VertexID gl_InstanceID gl_Position gl_PointSize "
    "gl_ClipDistance gl_PrimitiveIDIn gl_InvocationID gl_PrimitiveID "
    "gl_Layer gl_PatchVerticesIn gl_TessLevelOuter gl_TessLevelInner "
    "gl_TessCoord gl_FragCoord gl_FrontFacing gl_PointCoord "
    "gl_SampleID gl_SamplePosition gl_FragColor gl_FragData "
    "gl_FragDepth gl_SampleMask gl_ClipVertex gl_FrontColor "
    "gl_BackColor gl_FrontSecondaryColor gl_BackSecondaryColor "
    "gl_TexCoord gl_FogFragCoord gl_Color gl_SecondaryColor "
    "gl_Normal gl_Vertex gl_MultiTexCoord0 gl_MultiTexCoord1 "
    "gl_MultiTexCoord2 gl_MultiTexCoord3 gl_MultiTexCoord4 "
    "gl_MultiTexCoord5 gl_MultiTexCoord6 gl_MultiTexCoord7 gl_FogCoord "
    "gl_MaxVertexAttribs gl_MaxVertexUniformComponents gl_MaxVaryingFloats "
    "gl_MaxVaryingComponents gl_MaxVertexOutputComponents "
    "gl_MaxGeometryInputComponents gl_MaxGeometryOutputComponents "
    "gl_MaxFragmentInputComponents gl_MaxVertexTextureImageUnits "
    "gl_MaxCombinedTextureImageUnits gl_MaxTextureImageUnits "
    "gl_MaxFragmentUniformComponents gl_MaxDrawBuffers gl_MaxClipDistances "
    "gl_MaxGeometryTextureImageUnits gl_MaxGeometryOutputVertices "
    "gl_MaxGeometryTotalOutputComponents gl_MaxGeometryUniformComponents "
    "gl_MaxGeometryVaryingComponents gl_MaxTessControlInputComponents "
    "gl_MaxTessControlOutputComponents gl_MaxTessControlTextureImageUnits "
    "gl_MaxTessControlUniformComponents "
    "gl_MaxTessControlTotalOutputComponents "
    "gl_MaxTessEvaluationInputComponents gl_MaxTessEvaluationOutputComponents "
    "gl_MaxTessEvaluationTextureImageUnits "
    "gl_MaxTessEvaluationUniformComponents gl_MaxTessPatchComponents "
    "gl_MaxPatchVertices gl_MaxTessGenLevel gl_MaxTextureUnits "
    "gl_MaxTextureCoords gl_MaxClipPlanes "
    "gl_DepthRange gl_ModelViewMatrix gl_ProjectionMatrix "
    "gl_ModelViewProjectionMatrix gl_TextureMatrix gl_NormalMatrix "
    "gl_ModelViewMatrixInverse gl_ProjectionMatrixInverse "
    "gl_ModelViewProjectionMatrixInverse gl_TextureMatrixInverse "
    "gl_ModelViewMatrixTranspose gl_ProjectionMatrixTranspose "
    "gl_ModelViewProjectionMatrixTranspose gl_TextureMatrixTranspose "
    "gl_ModelViewMatrixInverseTranspose gl_ProjectionMatrixInverseTranspose "
    "gl_ModelViewProjectionMatrixInverseTranspose "
    "gl_TextureMatrixInverseTranspose gl_NormalScale gl_ClipPlane "
    "gl_Point gl_FrontMaterial gl_BackMaterial gl_LightSource "
    "gl_LightModel gl_FrontLightModelProduct gl_BackLightModelProduct "
    "gl_FrontLightProduct gl_BackLightProduct gl_TextureEnvColor "
    "gl_EyePlaneS gl_EyePlaneT gl_EyePlaneR gl_EyePlaneQ "
    "gl_ObjectPlaneS gl_ObjectPlaneT gl_ObjectPlaneR gl_ObjectPlaneQ "
    "gl_Fog";

const size_t NB_FOLDER_STATE = 7;
const size_t FOLDER_TYPE = 0;
const int markersArray[][NB_FOLDER_STATE] = {
    {SC_MARKNUM_FOLDEROPEN, SC_MARKNUM_FOLDER, SC_MARKNUM_FOLDERSUB, SC_MARKNUM_FOLDERTAIL, SC_MARKNUM_FOLDEREND,        SC_MARKNUM_FOLDEROPENMID,     SC_MARKNUM_FOLDERMIDTAIL},
    {SC_MARK_MINUS,         SC_MARK_PLUS,      SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
    {SC_MARK_ARROWDOWN,     SC_MARK_ARROW,     SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
    {SC_MARK_CIRCLEMINUS,   SC_MARK_CIRCLEPLUS,SC_MARK_VLINE,        SC_MARK_LCORNERCURVE,  SC_MARK_CIRCLEPLUSCONNECTED, SC_MARK_CIRCLEMINUSCONNECTED, SC_MARK_TCORNERCURVE},
    {SC_MARK_BOXMINUS,      SC_MARK_BOXPLUS,   SC_MARK_VLINE,        SC_MARK_LCORNER,       SC_MARK_BOXPLUSCONNECTED,    SC_MARK_BOXMINUSCONNECTED,    SC_MARK_TCORNER}
};

void ShaderEditOverlay::initialiseShaderEditor()
{
    mShaderEditor.Command(SCI_SETSTYLEBITS, 7);

    mLexer->SetLexer(SCLEX_CPP);
    mLexer->SetWordList(0, glslKeyword);
    mLexer->SetWordList(1, glslType);
    mLexer->SetWordList(4, glslBuiltin);
    mLexer->PropSet("fold", "1");

    vg::font_t font = vg::findFont("Courier New");

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    mShaderEditor.setStyle(STYLE_DEFAULT, 0xFFFFFFFF, 0xD0000000, font, 16);
    mShaderEditor.Command(SCI_STYLECLEARALL);	// Copies global style to all others
    mShaderEditor.setStyle(STYLE_INDENTGUIDE, 0xFFC0C0C0, 0xD0000000, font, 16);
    mShaderEditor.setStyle(STYLE_BRACELIGHT,  0xFF00FF00, 0xD0000000, font, 16);
    mShaderEditor.setStyle(STYLE_BRACEBAD,    0xFF0000FF, 0xD0000000, font, 16);
    mShaderEditor.setStyle(STYLE_LINENUMBER,  0xFFC0C0C0, 0xD0333333, font, 16);
    mShaderEditor.Command(SCI_SETFOLDMARGINCOLOUR,   1, 0xD01A1A1A);
    mShaderEditor.Command(SCI_SETFOLDMARGINHICOLOUR, 1, 0xD01A1A1A);
    mShaderEditor.Command(SCI_SETSELBACK,            1, 0xD0CC9966);
    mShaderEditor.Command(SCI_SETCARETFORE,          0xFFFFFFFF, 0);
    mShaderEditor.Command(SCI_SETCARETLINEVISIBLE,   1);
    mShaderEditor.Command(SCI_SETCARETLINEBACK,      0xFFFFFFFF);
    mShaderEditor.Command(SCI_SETCARETLINEBACKALPHA, 0x20);

    mShaderEditor.Command(SCI_SETMARGINWIDTHN, 0, 44);//Calculate correct width
    mShaderEditor.Command(SCI_SETMARGINWIDTHN, 1, 20);//Calculate correct width
    mShaderEditor.Command(SCI_SETMARGINMASKN, 1, SC_MASK_FOLDERS);//Calculate correct width

    for (int i = 0 ; i < NB_FOLDER_STATE ; i++)
    {
        mShaderEditor.Command(SCI_MARKERDEFINE, markersArray[FOLDER_TYPE][i], markersArray[4][i]);
        mShaderEditor.Command(SCI_MARKERSETBACK, markersArray[FOLDER_TYPE][i], 0xFF6A6A6A);
        mShaderEditor.Command(SCI_MARKERSETFORE, markersArray[FOLDER_TYPE][i], 0xFF333333);
    }

    mShaderEditor.Command(SCI_SETUSETABS, 1);
    mShaderEditor.Command(SCI_SETTABWIDTH, 4);
    mShaderEditor.Command(SCI_SETINDENTATIONGUIDES, SC_IV_REAL);

    mShaderEditor.setStyle(SCE_C_DEFAULT,      0xFFFFFFFF, 0xD0000000, font, 16);
    mShaderEditor.setStyle(SCE_C_WORD,         0xFF0066FF, 0xD0000000, font, 16);
    mShaderEditor.setStyle(SCE_C_WORD2,        0xFFFFFF00, 0xD0000000, font, 16);
    mShaderEditor.setStyle(SCE_C_PREPROCESSOR, 0xFFC0C0C0, 0xD0000000, font, 16);
    mShaderEditor.setStyle(SCE_C_NUMBER,       0xFF0080FF, 0xD0000000, font, 16);
    mShaderEditor.setStyle(SCE_C_OPERATOR,     0xFF00CCFF, 0xD0000000, font, 16);
    mShaderEditor.setStyle(SCE_C_COMMENT,      0xFF00FF00, 0xD0000000, font, 16);
    mShaderEditor.setStyle(SCE_C_COMMENTLINE,  0xFF00FF00, 0xD0000000, font, 16);
}

void ShaderEditOverlay::initialiseDebugOutputView() {
    mDebugOutputView.Command(SCI_SETSTYLEBITS, 7);

    vg::font_t font = vg::findFont("Courier New");

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    mDebugOutputView.setStyle(STYLE_DEFAULT, 0xFFFFFFFF, 0xD0000000, font, 16);
    mDebugOutputView.Command(SCI_STYLECLEARALL);	// Copies global style to all others
    mDebugOutputView.Command(SCI_SETMARGINWIDTHN, 1, 0);//Calculate correct width

    mDebugOutputView.Command(SCI_SETUSETABS, 1);
    mDebugOutputView.Command(SCI_SETTABWIDTH, 4);

    mDebugOutputView.Command(SCI_SETSELBACK,            1, 0xD0CC9966);
    mDebugOutputView.Command(SCI_SETCARETFORE,          0xFFFFFFFF, 0);
    mDebugOutputView.Command(SCI_SETCARETLINEVISIBLE,   1);
    mDebugOutputView.Command(SCI_SETCARETLINEBACK,      0xFFFFFFFF);
    mDebugOutputView.Command(SCI_SETCARETLINEBACKALPHA, 0x20);
}

void ShaderEditOverlay::initialiseSelectionList() {
    mSelectionList.Command(SCI_SETSTYLEBITS, 7);

    vg::font_t font = vg::findFont("Courier New");

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    mSelectionList.setStyle(STYLE_DEFAULT, 0xFFFFFFFF, 0xD0000000, font, 16);
    mSelectionList.Command(SCI_STYLECLEARALL);	// Copies global style to all others
    mSelectionList.Command(SCI_SETMARGINWIDTHN, 1, 0);//Calculate correct width

    mSelectionList.Command(SCI_SETUSETABS, 1);
    mSelectionList.Command(SCI_SETTABWIDTH, 4);

    mSelectionList.Command(SCI_SETSELBACK,            1, 0xD0CC9966);
    mSelectionList.Command(SCI_SETCARETFORE,          0x00FFFFFF, 0);
    mSelectionList.Command(SCI_SETCARETLINEVISIBLE,   1);
    mSelectionList.Command(SCI_SETCARETLINEBACK,      0xFFFFFFFF);
    mSelectionList.Command(SCI_SETCARETLINEBACKALPHA, 0x20);
}

void ShaderEditOverlay::initialise(int w, int h)
{
    mNextTick = 0;

    initialiseShaderEditor();
    initialiseDebugOutputView();
    initialiseSelectionList();

    mSelectedProgram = 0;
    mSelectedShader  = 0;
    mSelectionMode   = SELMODE_PROGRAM_LIST;

    mWidth = w; mHeight=h;

    float w1=mWidth-80.0f, h1=mHeight-80.0f;/*80=30+20+30*/

    mShaderEditor.SetSize(w1*0.7f, h1*0.7f);
    mDebugOutputView.SetSize(w1*0.7f, h1*0.3f);
    mSelectionList.SetSize(w1*0.3f, h1+20.0f);

    mActiveEditor = &mSelectionList;
}

void ShaderEditOverlay::reset()
{
    mSelectionMode=SELMODE_PROGRAM_LIST;
    fillListWithPrograms();

    mSelectedShader=0;
    mSelectedProgram=0;

    mShaderEditor.Command(SCI_CANCEL);
    mShaderEditor.Command(SCI_CLEARALL);
    mShaderEditor.Command(SCI_EMPTYUNDOBUFFER);
    mShaderEditor.Command(SCI_SETFOCUS, false);

    mDebugOutputView.Command(SCI_CANCEL);
    mDebugOutputView.Command(SCI_CLEARALL);
    mDebugOutputView.Command(SCI_EMPTYUNDOBUFFER);
    mDebugOutputView.Command(SCI_SETFOCUS, false);

    mSelectionList.Command(SCI_SETFOCUS, true);

    mActiveEditor = &mSelectionList;
}

void ShaderEditOverlay::saveShaderSource()
{
    if (mSelectedProgram&&mSelectedShader)
    {
        GLint lengthDoc;
        TextRange tr;

        lengthDoc = mShaderEditor.Command(SCI_GETLENGTH);
        tr.chrg.cpMin = 0;
        tr.chrg.cpMax = lengthDoc;
        tr.lpstrText  = (char*)malloc(lengthDoc+1);
        mShaderEditor.Command(SCI_GETTEXTRANGE, 0, reinterpret_cast<sptr_t>(&tr));

        glShaderSource(mSelectedShader, 1, (const char**)&tr.lpstrText, &lengthDoc);
        free(tr.lpstrText);
    }
}

void ShaderEditOverlay::compileProgram()
{
    char	errbuf[65536];
    GLint	type;
    GLint	size;
    GLint	res;
    GLuint	sh;
    GLuint	prg;

    saveShaderSource();

    prg = glCreateProgram();

    //Some string manipulation black magic to avoid ATI GL errors.
    //It seems ATI GL runtime, when destroys programs, deletes shaders,
    //which are marked for deletion, anyway without checking whether
    //they are attached to other programs.
    //If this bug is fixed, probably this code should be rewritten...
    for (size_t i=0; i<mAttachedShaders.size(); ++i)
    {
        GLint len;
        char* buf;

        glGetShaderiv(mAttachedShaders[i], GL_SHADER_TYPE, &type);
        sh = glCreateShader(type);

        glGetShaderiv(mAttachedShaders[i], GL_SHADER_SOURCE_LENGTH, &len);
        buf = (char*)malloc(len);
        glGetShaderSource(mAttachedShaders[i], len, &len, buf);
        glShaderSource(sh, 1, (const char**)&buf, &len);
        free(buf);

        glCompileShader(sh);

        if (mAttachedShaders[i]==mSelectedShader)
            glGetShaderInfoLog(sh, sizeof(errbuf), &size, errbuf);

        glGetShaderiv(sh, GL_COMPILE_STATUS, &res);

        if (!res)
        {
            glDeleteShader(sh);
            goto error;
        }

        glAttachShader(prg, sh);

        glDeleteShader(sh);
    }

    glLinkProgram(prg);
    glGetProgramInfoLog(prg, sizeof(errbuf)-size, &size, errbuf+size);
    glGetProgramiv(prg, GL_LINK_STATUS, &res);
    if (res)
    {
        glCompileShader(mSelectedShader);
        glGetShaderiv(mSelectedShader, GL_COMPILE_STATUS, &res);
        assert(res);

        glLinkProgram(mSelectedProgram);
        glGetProgramiv(mSelectedProgram, GL_LINK_STATUS, &res);
        assert(res);
    }
    glDeleteProgram(prg);

error:
    //update result window
    mDebugOutputView.Command(SCI_CANCEL);
    mDebugOutputView.Command(SCI_SETREADONLY, false);
    mDebugOutputView.Command(SCI_CLEARALL);
    mDebugOutputView.Command(SCI_ADDTEXT, strlen(errbuf), reinterpret_cast<sptr_t>((char*)errbuf));
    mDebugOutputView.Command(SCI_SETREADONLY, true);
    mDebugOutputView.Command(SCI_EMPTYUNDOBUFFER);
    mDebugOutputView.Command(SCI_GOTOPOS, 0);
}

void ShaderEditOverlay::fillListWithShaders()
{
    int pos = (int)mSelectionList.Command(SCI_GETCURRENTPOS);
    int line = mSelectionList.Command(SCI_LINEFROMPOSITION, pos);
    mSelectedProgram = mPrograms[line];

    GLint attachedCount;
    glGetProgramiv(mSelectedProgram, GL_ATTACHED_SHADERS, &attachedCount);
    mAttachedShaders.resize(attachedCount);
    glGetAttachedShaders(mSelectedProgram, attachedCount, &attachedCount, &mAttachedShaders[0]);

    mSelectionList.Command(SCI_CLEARALL);
    for (int i=0; i<attachedCount; ++i)
    {
        char str[128];
        GLint type;
        glGetShaderiv(mAttachedShaders[i], GL_SHADER_TYPE, &type);
        cstr_printf(str, "%s%s #%d",
            (i==0)?"":"\n",
            type==GL_VERTEX_SHADER?"GL_VERTEX_SHADER":"GL_FRAGMENT_SHADER",
            mAttachedShaders[i]);
        mSelectionList.Command(SCI_ADDTEXT, strlen(str), reinterpret_cast<sptr_t>(str));
    }
    mSelectionList.Command(SCI_GOTOLINE, 0);
}

void ShaderEditOverlay::fillListWithPrograms()
{
    mSelectionList.Command(SCI_CLEARALL);
    for (size_t i=0; i<mPrograms.size(); ++i)
    {
        char str[128];
        cstr_printf(str, "%sProgram #%d", (i==0)?"":"\n", mPrograms[i]);
        mSelectionList.Command(SCI_ADDTEXT, strlen(str), reinterpret_cast<sptr_t>(str));
    }
    mSelectionList.Command(SCI_GOTOLINE, 0);
}

void ShaderEditOverlay::loadShaderSource()
{
    int pos = (int)mSelectionList.Command(SCI_GETCURRENTPOS);
    int line = mSelectionList.Command(SCI_LINEFROMPOSITION, pos);

    GLint len;
    char* buf;

    mSelectedShader = mAttachedShaders[line];
    glGetShaderiv(mSelectedShader, GL_SHADER_SOURCE_LENGTH, &len);
    buf = (char*)malloc(len);
    glGetShaderSource(mSelectedShader, len, &len, buf);

    mShaderEditor.Command(SCI_CANCEL);
    mShaderEditor.Command(SCI_CLEARALL);
    mShaderEditor.Command(SCI_SETUNDOCOLLECTION, 0);
    mShaderEditor.Command(SCI_ADDTEXT, len-1, reinterpret_cast<sptr_t>(buf));
    mShaderEditor.Command(SCI_SETUNDOCOLLECTION, 1);
    mShaderEditor.Command(SCI_EMPTYUNDOBUFFER);
    mShaderEditor.Command(SCI_SETSAVEPOINT);
    mShaderEditor.Command(SCI_GOTOPOS, 0);

    free(buf);
}

void BraceMatch(Editor& ed) 
{
    int braceAtCaret = -1;
    int braceOpposite = -1;
    int caretPos = int(ed.Command(SCI_GETCURRENTPOS));
    char charBefore = '\0';

    int lengthDoc = int(ed.Command(SCI_GETLENGTH));

    if ((lengthDoc > 0) && (caretPos > 0)) 
    {
        charBefore = ed.Command(SCI_GETCHARAT, caretPos - 1, 0);
    }
    // Priority goes to character before caret
    if (charBefore && strchr("[](){}", charBefore))
    {
        braceAtCaret = caretPos - 1;
    }

    if (lengthDoc > 0  && (braceAtCaret < 0)) 
    {
        // No brace found so check other side
        char charAfter = ed.Command(SCI_GETCHARAT, caretPos, 0);
        if (charAfter && strchr("[](){}", charAfter))
        {
            braceAtCaret = caretPos;
        }
    }
    if (braceAtCaret >= 0) 
        braceOpposite = int(ed.Command(SCI_BRACEMATCH, braceAtCaret, 0));

    if ((braceAtCaret != -1) && (braceOpposite == -1))
    {
        ed.Command(SCI_BRACEBADLIGHT, braceAtCaret);
        ed.Command(SCI_SETHIGHLIGHTGUIDE, 0);
    } 
    else 
    {
        ed.Command(SCI_BRACEHIGHLIGHT, braceAtCaret, braceOpposite);

        //if (_pEditView->isShownIndentGuide())
        {
            int columnAtCaret = int(ed.Command(SCI_GETCOLUMN, braceAtCaret));
            int columnOpposite = int(ed.Command(SCI_GETCOLUMN, braceOpposite));
            ed.Command(SCI_SETHIGHLIGHTGUIDE, (columnAtCaret < columnOpposite)?columnAtCaret:columnOpposite);
        }
    }
}

void ShaderEditOverlay::renderFullscreen()
{
    //update logic
    if (timerAbsoluteTime()>mNextTick)
    {
        mShaderEditor.Tick();
        mDebugOutputView.Tick();
        mNextTick = timerAbsoluteTime()+TICK_INTERVAL;
    }

    BraceMatch(mShaderEditor);

    glEnable(GL_SCISSOR_TEST);

    float w1=mWidth-80.0f, h1=mHeight-80.0f;

    v128 mv[4];
    ml::make_identity_mat4(mv);

    {
        PROFILER_CPU_TIMESLICE("mSelectionList.Paint");
        mv[3] = vi_set(30.0f, 30.0f, 0.0f, 1.0f);
        gfx::setModelViewMatrix(mv);
        mSelectionList.Paint();
    }

    {
        PROFILER_CPU_TIMESLICE("mShaderEditor.Paint");
        mv[3] = vi_set(w1*0.3f+50.0f, 30.0f, 0.0f, 1.0f);
        gfx::setModelViewMatrix(mv);
        mShaderEditor.Paint();
    }

    {
        PROFILER_CPU_TIMESLICE("mDebugOutputView.Paint");
        mv[3] = vi_set(w1*0.3f+50.0f, h1*0.7f+50.0f, 0.0f, 1.0f);
        gfx::setModelViewMatrix(mv);
        mDebugOutputView.Paint();
    }

    glDisable(GL_SCISSOR_TEST);
}

bool isModEnabled(int mod, int modState)
{
    int mask = ~(KMOD_NUM|KMOD_CAPS|KMOD_MODE|mod);
    return (modState&mask)==0 && ((modState&mod)!=0 || mod==0); 
}

void ShaderEditOverlay::inputText(char* text)
{
    if (mActiveEditor==&mShaderEditor)
        mActiveEditor->AddCharUTF(text, strlen(text));
};

void ShaderEditOverlay::handleKeyDown(SDL_KeyboardEvent& event)
{
    if (event.keysym.sym=='s' && isModEnabled(KMOD_CTRL, event.keysym.mod))
    {
        saveShaderSource();
    }
    if ('1'<=event.keysym.sym && event.keysym.sym<='3' && isModEnabled(KMOD_ALT, event.keysym.mod))
    {
        mActiveEditor->Command(SCI_SETFOCUS, false);
        switch(event.keysym.sym)
        {
            case '1': mActiveEditor=&mSelectionList; break;
            case '2': mActiveEditor=&mShaderEditor; break;
            case '3': mActiveEditor=&mDebugOutputView; break;
        }
        mActiveEditor->Command(SCI_SETFOCUS, true);
    }
    if (event.keysym.sym==SDLK_F7 && isModEnabled(0, event.keysym.mod)&&mSelectedShader&&mSelectedProgram)
    {
        compileProgram();
        mRequireReset = true;
    }
    else if (mActiveEditor==&mSelectionList)
    {
        switch (event.keysym.sym)
        {
            case SDLK_UP:
                mSelectionList.Command(SCI_LINEUP);
                break;
            case SDLK_DOWN:
                mSelectionList.Command(SCI_LINEDOWN);
                break;
            case SDLK_RETURN:
                if (mSelectionMode==SELMODE_PROGRAM_LIST)
                {
                    fillListWithShaders();
                    mSelectionMode=SELMODE_SHADER_LIST;
                }
                else
                {
                    loadShaderSource();
                }
                break;
            case SDLK_BACKSPACE:
                if (mSelectionMode==SELMODE_SHADER_LIST)
                {
                    fillListWithPrograms();
                    mSelectionMode=SELMODE_PROGRAM_LIST;
                }
                break;
        }

    }
    else
    {
        int sciKey;
        switch(event.keysym.sym)
        {
            case SDLK_DOWN:             sciKey = SCK_DOWN;          break;
            case SDLK_UP:               sciKey = SCK_UP;            break;
            case SDLK_LEFT:             sciKey = SCK_LEFT;          break;
            case SDLK_RIGHT:            sciKey = SCK_RIGHT;         break;
            case SDLK_HOME:             sciKey = SCK_HOME;          break;
            case SDLK_END:              sciKey = SCK_END;           break;
            case SDLK_PAGEUP:           sciKey = SCK_PRIOR;         break;
            case SDLK_PAGEDOWN:         sciKey = SCK_NEXT;	        break;
            case SDLK_DELETE:           sciKey = SCK_DELETE;        break;
            case SDLK_INSERT:           sciKey = SCK_INSERT;        break;
            case SDLK_ESCAPE:           sciKey = SCK_ESCAPE;        break;
            case SDLK_BACKSPACE:        sciKey = SCK_BACK;          break;
            case SDLK_TAB:              sciKey = SCK_TAB;           break;
            case SDLK_RETURN:           sciKey = SCK_RETURN;        break;
            case SDLK_KP_PLUS:          sciKey = SCK_ADD;           break;
            case SDLK_KP_MINUS:         sciKey = SCK_SUBTRACT;      break;
            case SDLK_KP_DIVIDE:        sciKey = SCK_DIVIDE;        break;
            case SDLK_LGUI:             sciKey = SCK_WIN;           break;
            case SDLK_RGUI:             sciKey = SCK_RWIN;          break;
            case SDLK_MENU:             sciKey = SCK_MENU;          break;
            case SDLK_SLASH:            sciKey = '/';               break;
            case SDLK_ASTERISK:         sciKey = '`';               break;
            case SDLK_LEFTBRACKET:      sciKey = '[';               break;
            case SDLK_BACKSLASH:        sciKey = '\\';              break;
            case SDLK_RIGHTBRACKET:     sciKey = ']';               break;
            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
            case SDLK_LALT:
            case SDLK_RALT:
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                sciKey = 0;
                break;
            default:
                sciKey = event.keysym.sym;
        }

        if (sciKey)
        {
            bool consumed;
            bool ctrlPressed  = event.keysym.mod&KMOD_LCTRL  || event.keysym.mod&KMOD_RCTRL;
            bool altPressed   = event.keysym.mod&KMOD_LALT   || event.keysym.mod&KMOD_RALT;
            bool shiftPressed = event.keysym.mod&KMOD_LSHIFT || event.keysym.mod&KMOD_RSHIFT;
            mActiveEditor->KeyDown((SDLK_a<=sciKey && sciKey<=SDLK_z)?sciKey-'a'+'A':sciKey,
                shiftPressed, ctrlPressed, altPressed,
                &consumed
                );
        }
    }
}
