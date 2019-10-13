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

DLLFUNC ImFont *imgui_add_ttf_from_file(char *_chrFont, var _size, var _glyphRangeID) {
	const ImWchar *_glyphRange = NULL;
	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };

	ImGuiIO& io = ImGui::GetIO();

	switch (_INT(_glyphRangeID)) {
		case imgui_fonts::GLYPH_RANGE_ChineseFull:       _glyphRange = io.Fonts->GetGlyphRangesChineseFull(); break;
		case imgui_fonts::GLYPH_RANGE_ChineseSimplified: _glyphRange = io.Fonts->GetGlyphRangesChineseSimplifiedCommon(); break;
		case imgui_fonts::GLYPH_RANGE_Cyrillic:          _glyphRange = io.Fonts->GetGlyphRangesCyrillic(); break;
		case imgui_fonts::GLYPH_RANGE_Japanese:          _glyphRange = io.Fonts->GetGlyphRangesJapanese(); break;
		case imgui_fonts::GLYPH_RANGE_Korean:            _glyphRange = io.Fonts->GetGlyphRangesKorean(); break;
		case imgui_fonts::GLYPH_RANGE_Thai:              _glyphRange = io.Fonts->GetGlyphRangesThai(); break;
		default:                            _glyphRange = io.Fonts->GetGlyphRangesDefault();
	}

	return io.Fonts->AddFontFromFileTTF(_chrFont, _FLOAT(_size), NULL, _glyphRange);
}

DLLFUNC void imgui_push_font(ImFont* _font) {
	ImGui::PushFont(_font);
}

DLLFUNC void imgui_pop_font() {
	ImGui::PopFont();
}

// ------------------------------------------------------------------------------------------------------------------

DLLFUNC ImFont *imgui_add_ttf_from_file_ranged(char *_chrFont, var _size, var _glyphRangeID) {
	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };

	ImGuiIO& io = ImGui::GetIO();

	ImVector<ImWchar> _ranges;
	ImFontAtlas::GlyphRangesBuilder _rangeBuilder;
//	_rangeBuilder.AddText("Hello world");                        // Add a string (here "Hello world" contains 7 unique characters)

	switch (_INT(_glyphRangeID)) {
		case imgui_fonts::GLYPH_RANGE_ChineseFull:       _rangeBuilder.AddRanges(io.Fonts->GetGlyphRangesChineseFull()); break;
		case imgui_fonts::GLYPH_RANGE_ChineseSimplified: _rangeBuilder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon()); break;
		case imgui_fonts::GLYPH_RANGE_Cyrillic:          _rangeBuilder.AddRanges(io.Fonts->GetGlyphRangesCyrillic()); break;
		case imgui_fonts::GLYPH_RANGE_Japanese:          _rangeBuilder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); break;
		case imgui_fonts::GLYPH_RANGE_Korean:            _rangeBuilder.AddRanges(io.Fonts->GetGlyphRangesKorean()); break;
		case imgui_fonts::GLYPH_RANGE_Thai:              _rangeBuilder.AddRanges(io.Fonts->GetGlyphRangesThai()); break;
		default:                                         _rangeBuilder.AddRanges(io.Fonts->GetGlyphRangesDefault());
	}
	_rangeBuilder.AddChar(0xE801);                               // Add a specific character

	_rangeBuilder.BuildRanges(&_ranges);                         // Build the final result (ordered ranges with all the unique characters submitted)

	ImFontConfig config;
	config.OversampleH = 1;
	config.OversampleV = 1;


	ImFont *_font = io.Fonts->AddFontFromFileTTF(_chrFont, _FLOAT(_size), NULL, _ranges.Data);

	return _font;
}



