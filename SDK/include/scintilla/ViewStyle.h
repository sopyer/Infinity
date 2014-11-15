// Scintilla source code edit control
/** @file ViewStyle.h
** Store information on how the document is to be viewed.
**/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef VIEWSTYLE_H
#define VIEWSTYLE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

enum IndentView     {ivNone, ivReal, ivLookForward, ivLookBoth};
enum WhiteSpaceView {wsInvisible=0, wsVisibleAlways=1, wsVisibleAfterIndent=2};

class MarginStyle {
public:
    int   style;
    int   width;
    int   mask;
    int   cursor;
    bool  sensitive;
};

struct TextStyle
{
    Colour  fore;
    Colour  back;
    bool    eolFilled  : 1;
    bool    underline  : 1;
    bool    visible    : 1;
    bool    changeable : 1;
    bool    hotspot    : 1;

    vg::font_t  font;
    float       size;
    uint32_t    flags;

};

class ViewStyle {
public:
    size_t stylesSize;
    vg::face_t*  faces;
    TextStyle*   styles;
    LineMarker markers[MARKER_MAX + 1];
    int largestMarkerHeight;
    Indicator indicators[INDIC_MAX + 1];
    float lineHeight;
    float maxAscent;
    float maxDescent;
    float aveCharWidth;
    float spaceWidth;
    bool selforeset;
    Colour selforeground;
    Colour selAdditionalForeground;
    bool selbackset;
    Colour selbackground;
    Colour selAdditionalBackground;
    Colour selbackground2;
    int selAlpha;
    int selAdditionalAlpha;
    bool selEOLFilled;
    bool whitespaceForegroundSet;
    Colour whitespaceForeground;
    bool whitespaceBackgroundSet;
    Colour whitespaceBackground;
    Colour selbar;
    Colour selbarlight;
    bool foldmarginColourSet;
    Colour foldmarginColour;
    bool foldmarginHighlightColourSet;
    Colour foldmarginHighlightColour;
    bool hotspotForegroundSet;
    Colour hotspotForeground;
    bool hotspotBackgroundSet;
    Colour hotspotBackground;
    bool hotspotUnderline;
    bool hotspotSingleLine;
    /// Margins are ordered: Line Numbers, Selection Margin, Spacing Margin
    enum { margins=5 };
    int leftMarginWidth;	///< Spacing margin on left of text
    int rightMarginWidth;	///< Spacing margin on left of text
    int maskInLine;	///< Mask for markers to be put into text because there is nowhere for them to go in margin
    MarginStyle ms[margins];
    int fixedColumnWidth;
    int zoomLevel;
    WhiteSpaceView viewWhitespace;
    int whitespaceSize;
    IndentView viewIndentationGuides;
    bool viewEOL;
    Colour caretcolour;
    Colour additionalCaretColour;
    bool showCaretLineBackground;
    Colour caretLineBackground;
    int caretLineAlpha;
    Colour edgecolour;
    int edgeState;
    int caretStyle;
    int caretWidth;
    bool someStylesProtected;
    int extraFontFlag;
    int extraAscent;
    int extraDescent;
    int marginStyleOffset;
    int annotationVisible;
    int annotationStyleOffset;
    bool braceHighlightIndicatorSet;
    int braceHighlightIndicator;
    bool braceBadLightIndicatorSet;
    int braceBadLightIndicator;

    ViewStyle();
    ViewStyle(const ViewStyle &source);
    ~ViewStyle();
    void Init(size_t stylesSize_=64);
    void Refresh();
    void AllocStyles(size_t sizeNew);
    void EnsureStyle(size_t index);
    void ResetDefaultStyle();
    void ClearStyles();
    bool ProtectionActive() const;
    bool ValidStyle(size_t styleIndex) const;
    void CalcLargestMarkerHeight();
};

void marginStyleSetDefault(MarginStyle& mstyle);

void textStyleSetDefault(TextStyle& style);
void textStyleSet(TextStyle& style, Colour fore, Colour back,
            vg::font_t font, int size, uint32_t flags, bool eolFilled,
            bool underline, bool visible, bool changeable, bool hotspot);
bool textStyleIsProtected(TextStyle& style);

bool viewStyleIsStyleProtected(ViewStyle& vs, size_t style);
void viewStyleSetStyle(ViewStyle& vs, size_t style, Colour fore, Colour back,
            vg::font_t font, int size, uint32_t flags, bool eolFilled,
            bool underline, bool visible, bool changeable, bool hotspot);

#ifdef SCI_NAMESPACE
}
#endif

#endif
