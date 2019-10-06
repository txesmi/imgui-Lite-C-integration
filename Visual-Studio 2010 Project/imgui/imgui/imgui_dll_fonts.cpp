#include "dllmain.h"

namespace imgui_fonts {
	enum GLYPH_RANGE {
		GLYPH_RANGE_Default,
		GLYPH_RANGE_ChineseFull,
		GLYPH_RANGE_ChineseSimplified,
		GLYPH_RANGE_Cyrillic,
		GLYPH_RANGE_Japanese,
		GLYPH_RANGE_Korean,
		GLYPH_RANGE_Thai,
		GLYPH_RANGE_COUNT
	};
};

using namespace imgui_fonts;

DLLFUNC ImFont *imgui_add_ttf_from_file(char *_chrFont, float _size, var _glyphRangeID) {
	const ImWchar *_glyphRange = NULL;

	ImGuiIO& io = ImGui::GetIO();

	switch (_INT(_glyphRangeID)) {
		case GLYPH_RANGE_ChineseFull:       _glyphRange = io.Fonts->GetGlyphRangesChineseFull(); break;
		case GLYPH_RANGE_ChineseSimplified: _glyphRange = io.Fonts->GetGlyphRangesChineseSimplifiedCommon(); break;
		case GLYPH_RANGE_Cyrillic:          _glyphRange = io.Fonts->GetGlyphRangesCyrillic(); break;
		case GLYPH_RANGE_Japanese:          _glyphRange = io.Fonts->GetGlyphRangesJapanese(); break;
		case GLYPH_RANGE_Korean:            _glyphRange = io.Fonts->GetGlyphRangesKorean(); break;
		case GLYPH_RANGE_Thai:              _glyphRange = io.Fonts->GetGlyphRangesThai(); break;
		default:                            _glyphRange = io.Fonts->GetGlyphRangesDefault();
	}

	return io.Fonts->AddFontFromFileTTF(_chrFont, _size, NULL, _glyphRange);
}

DLLFUNC void imgui_push_font(ImFont* _font) {
	ImGui::PushFont(_font);
}

DLLFUNC void imgui_pop_font() {
	ImGui::PopFont();
}


