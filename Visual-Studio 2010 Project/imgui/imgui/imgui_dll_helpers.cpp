#include "dllmain.h"

#include <experimental/filesystem>
#include <filesystem>
#include <Shlobj.h>
#include <Knownfolders.h>
#include <comdef.h> 
#include <string>

namespace fs = std::experimental::filesystem;

namespace imgui_helpers {
	typedef wchar_t unicode;
	typedef char utf8;

	#define UTF8_MAX_PATH             1040

	typedef struct FolderContent {
		utf8 *root;
		utf8 *folder;
		int count;
		int folderCount;
		utf8 *content;
	} FolderContent;

	enum FOLDER_ID {
		FID_Documents,
		FID_ProgramData,
		FID_LocalAppData,
		FID_SavedGames,
		FID_Games,
		FID_COUNT
	};
};

DLLFUNC var imgui_h_button(char *label, var width, var height) {
	bool res = ImGui::Button(label, ImVec2(_FLOAT(width), _FLOAT(height)));
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_calc_item_width() {
	float _f = ImGui::CalcItemWidth();
	return _VAR(_f);
}

DLLFUNC var imgui_h_button_unactive(char *label, var width, var height) {
	ImGuiStyle& style = ImGui::GetStyle(); 
	ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_Border]);
	ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_WindowBg]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_WindowBg]);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_WindowBg]);
	bool ret = ImGui::Button(label, ImVec2(_FLOAT(width), _FLOAT(height)));
	ImGui::PopStyleColor(4);
	return ret;
}

//DLLFUNC var imgui_h_getter_list_box (const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items) {
//	bool res = ImGui::ListBox(label, current_item, items_getter, data, items_count, height_in_items);
//	return res ? _VAR(1) : _VAR(0);
//}

DLLFUNC var imgui_h_getter_list_box(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items, int scroll_to_item) {
	if (!ImGui::ListBoxHeader(label, items_count, height_in_items))
		return false;
	if (scroll_to_item >= 0)
		ImGui::SetScrollY(ImGui::GetTextLineHeightWithSpacing() * scroll_to_item);

	// Assume all items have even height (= 1 line of text). If you need items of different or variable sizes you can create a custom version of ListBox() in your code without using the clipper.
	ImGuiContext& g = *ImGui::GetCurrentContext();
	bool value_changed = false;
	ImGuiListClipper clipper(items_count, ImGui::GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
	while (clipper.Step())
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			const bool item_selected = (i == *current_item);
			const char* item_text;
			if (!items_getter(data, i, &item_text))
				item_text = "*Unknown item*";

			ImGui::PushID(i);
			if (ImGui::Selectable(item_text, item_selected))
			{
				*current_item = i;
				value_changed = true;
			}
			if (item_selected)
				ImGui::SetItemDefaultFocus();
			ImGui::PopID();
		}
	ImGui::ListBoxFooter();
	//if (value_changed)
	//	ImGui::MarkItemEdited(g.CurrentWindow->DC.LastItemId);

	return value_changed;
}

//IMGUI_API float         GetScrollX();                                                   // get scrolling amount [0..GetScrollMaxX()]
//IMGUI_API float         GetScrollY();                                                   // get scrolling amount [0..GetScrollMaxY()]
//IMGUI_API float         GetScrollMaxX();                                                // get maximum scrolling amount ~~ ContentSize.X - WindowSize.X
//IMGUI_API float         GetScrollMaxY();                                                // get maximum scrolling amount ~~ ContentSize.Y - WindowSize.Y
//IMGUI_API void          SetScrollX(float scroll_x);                                     // set scrolling amount [0..GetScrollMaxX()]
//IMGUI_API void          SetScrollY(float scroll_y);                                     // set scrolling amount [0..GetScrollMaxY()]
//IMGUI_API void          SetScrollHereY(float center_y_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
//IMGUI_API void          SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.

DLLFUNC var imgui_h_get_scroll_y() {
	return _VAR(ImGui::GetScrollY());
}

DLLFUNC void imgui_h_set_scroll_y(var _pos) {
//	ImGui::SetScrollY(_FLOAT(_pos));
	ImGui::SetScrollY(0);
}

//DLLFUNC void imgui_h_VSpacing (var _size) {
//	ImGui::ImGuiWindow* window = ImGui::GetCurrentWindow();
//	if (window->SkipItems)
//		return;
//	ImGui::ItemSize(ImVec2(0, _FLOAT(_size));
//}


// ---------------------------------------------------------------------------------------------------------------------------

DLLFUNC imgui_helpers::utf8 *imgui_h_unicode_to_utf8(imgui_helpers::unicode *_wText) {
	static imgui_helpers::utf8 _uText[4096];
	WideCharToMultiByte(CP_UTF8, NULL, _wText, -1, _uText, 4096, NULL, NULL);
	return _uText;
}

DLLFUNC imgui_helpers::unicode *imgui_h_unicode_for_utf8(imgui_helpers::utf8 *_uText) {
	static imgui_helpers::unicode _wText[UTF8_MAX_PATH];
	MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _uText, -1, _wText, UTF8_MAX_PATH);
	return _wText;
}

DLLFUNC long imgui_h_get_logical_drives() {
	return (long)GetLogicalDrives();
}

DLLFUNC imgui_helpers::utf8 *imgui_h_get_current_path_U() {
	static imgui_helpers::utf8 _uPath[UTF8_MAX_PATH];
	strcpy_s(_uPath, fs::current_path().u8string().c_str());
	*(_uPath + UTF8_MAX_PATH - 1) = NULL;
	return _uPath;
}

DLLFUNC void imgui_h_set_current_path_U(imgui_helpers::utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	fs::current_path(_path);
}

DLLFUNC imgui_helpers::utf8 *imgui_h_get_path_filename_U(imgui_helpers::utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	static imgui_helpers::utf8 _strPath[UTF8_MAX_PATH];
	const std::string _filename = _path.filename().u8string();
	int _length = min(_filename.size() + 1, UTF8_MAX_PATH - 2);
	memcpy(_strPath, _filename.c_str(), _length);
	*(_strPath + UTF8_MAX_PATH - 1) = NULL;
	return _strPath;
}

DLLFUNC imgui_helpers::utf8 *imgui_h_get_known_folder_path_U(var _id) {
	imgui_helpers::unicode *_pathW;
	switch (_INT(_id)) {
		case imgui_helpers::FID_Documents: SHGetKnownFolderPath(FOLDERID_Documents, NULL, 0, &_pathW); break;
		case imgui_helpers::FID_ProgramData: SHGetKnownFolderPath(FOLDERID_ProgramData, NULL, 0, &_pathW); break;
		case imgui_helpers::FID_LocalAppData: SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, 0, &_pathW); break;
		case imgui_helpers::FID_SavedGames: SHGetKnownFolderPath(FOLDERID_SavedGames, NULL, 0, &_pathW); break;
		case imgui_helpers::FID_Games: SHGetKnownFolderPath(FOLDERID_Games, NULL, 0, &_pathW); break;
		default: return NULL;
	}
	fs::path _path = _pathW;
	static imgui_helpers::utf8 _pathUTF8[UTF8_MAX_PATH];
	memcpy(_pathUTF8, _path.u8string().c_str(), UTF8_MAX_PATH);
	*(_pathUTF8 + UTF8_MAX_PATH - 1) = NULL;
	return _pathUTF8;
}

DLLFUNC var imgui_h_path_is_absolute_U(imgui_helpers::utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = _path.is_absolute();
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_path_is_root_U(imgui_helpers::utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = _path.has_root_name();
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_path_exists_U(imgui_helpers::utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = false;
	if (fs::exists(_path))
		res = true;
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_path_is_folder_U(imgui_helpers::utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = false;
	if (fs::is_directory(_path))
		res = true;
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_create_folder_U(imgui_helpers::utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = false;
	if (CreateDirectoryW(_path.wstring().c_str(), NULL))
		res = true;
	return res ? _VAR(1) : _VAR(0);
}

// ---------------------------------------------------------------------------------------------------------------------------

DLLFUNC var imgui_h_get_folder_content_U(imgui_helpers::utf8 *_uRoot, imgui_helpers::utf8 *_uFolder, imgui_helpers::utf8 *_buffer, int bufferSize, imgui_helpers::utf8 *_filter, imgui_helpers::utf8 *_folderInjection) {

	const fs::path _rootPath = fs::u8path(_uRoot);
	const fs::path _folderPath = fs::u8path(_uFolder);
	fs::path _fullPath;

	if (*_uRoot == NULL) {
		_fullPath = _folderPath;
	} else {
		_fullPath = _rootPath;
		_fullPath += "\\";
		_fullPath += _folderPath;
	}

	if (!fs::exists(_fullPath))
		return NULL;

	if (!fs::is_directory(_fullPath))
		return NULL;
	
	const std::string _strFolderInj = _folderInjection;
	const std::string _strFilter = _filter;
	imgui_helpers::FolderContent *_fc = (imgui_helpers::FolderContent*)_buffer;
	std::string _strPath = "";
	int _pathSize = 0;
	imgui_helpers::utf8 *_uText = _buffer + bufferSize - 1;

	_strPath = _rootPath.u8string();
	_pathSize = _strPath.size() + 1;
	_uText -= _pathSize;
	memcpy(_uText, _strPath.c_str(), _pathSize);
	_fc->root = _uText;

	_strPath = _folderPath.u8string();
	_pathSize = _strPath.size() + 1;
	_uText -= _pathSize;
	memcpy(_uText, _strPath.c_str(), _pathSize);
	_fc->folder = _uText;

//	int _count = std::distance(fs::directory_iterator(_fullPath), {});
	_fc->count = 0;
	imgui_helpers::utf8 **_uList = &_fc->content;
	int _injSize = _strFolderInj.size();
	int _filterSize = _strFilter.size();
	_fc->folderCount = 0;
	for (const fs::directory_entry& _p : fs::directory_iterator(_fullPath)) {
		const fs::path& _path = _p.path();

		DWORD _attb = GetFileAttributesW(_path.wstring().c_str());
		if (_attb & FILE_ATTRIBUTE_HIDDEN)
			continue;
		if (_attb & FILE_ATTRIBUTE_SYSTEM)
			continue;

		if (_filterSize > 0)
			if (fs::is_regular_file(_path))
				if (_path.extension().u8string() != _strFilter)
					continue;

		_strPath = _path.filename().u8string();
		_pathSize = _strPath.size() + 1;
		_uText -= _pathSize;
		if ((unsigned long)_uText <= (unsigned long)_uList)
			return NULL;
		memcpy(_uText, _strPath.c_str(), _pathSize);

		if (fs::is_directory(_path)) {
			if (_injSize > 0) {
				_uText -= _injSize;
				if ((unsigned long)_uText <= (unsigned long)_uList)
					return NULL;
				memcpy(_uText, _strFolderInj.c_str(), _injSize);
			}
			imgui_helpers::utf8 **_uL = _uList;
			imgui_helpers::utf8 **_uLLast = &_fc->content + _fc->folderCount;
			for (; _uL > _uLLast; _uL -= 1)
				*_uL = *(_uL - 1);
			*_uL = _uText;
			_fc->folderCount += 1;
		} else {
			*_uList = _uText;
		}
		
		_uList += 1;
		_fc->count += 1;
	}
	return _VAR(1);
}

// ---------------------------------------------------------------------------------------------------------------------------

DLLFUNC var imgui_h_get_folder_content_text(var _hdlFileDialog, STRING *_uRoot, STRING *_uFolder, STRING *_filter, STRING *_folderInjection, STRING *_fileInjection) {
	const fs::path _rootPath = fs::u8path(_uRoot->chars);
	fs::path _folderPath = fs::u8path(_uFolder->chars);
	fs::path _fullPath;

	// build the full path
	if (*(_uRoot->chars) == NULL) {
		_fullPath = _folderPath;
	} else {
		_fullPath = _rootPath;
		_fullPath += "\\";
		_fullPath += _folderPath;
	}
	if (!fs::exists(_fullPath))
		return NULL;
	if (!fs::is_directory(_fullPath))
		return NULL;

	// get the pointer to the TEXT struct
	TEXT *_txtT = NULL;
	if (_hdlFileDialog == NULL)
		_txtT = txt_create(_VAR(0), _VAR(1));
	else
		_txtT = (TEXT*)ptr_for_handle(_hdlFileDialog);

	// clear selection
	*((int*)(&_txtT->skill_x)) = -1;

	// check header strings existence
	int _count = 6;
	int _i = _INT(_txtT->strings);
	for (; _i < _count; _i += 1)
		txt_addstring(_txtT, "....----...."); // almost 3x int

	// cast the first string into the three needed integers
	int *_countPtr = (int*)(*_txtT->pstring)->chars;
	int *_folderCountPtr = _countPtr + 1;
	int *_pathDecomposedPtr = _countPtr + 2;

	// add enough strings to hold all the folder content
	int _cT = std::distance(fs::directory_iterator(_fullPath), {});
	int _iL = _count + _cT;
	for (; _i < _iL; _i += 1)
		txt_addstring(_txtT, "");

	// fill header strings
	str_cpy(*(_txtT->pstring + 1), (char*)_rootPath.u8string().c_str());
	str_cpy(*(_txtT->pstring + 2), (char*)_folderPath.u8string().c_str());
	if(*(_txtT->pstring + 3) != _filter)
		str_cpy(*(_txtT->pstring + 3), _filter->chars);
	if (*(_txtT->pstring + 4) != _folderInjection)
		str_cpy(*(_txtT->pstring + 4), _folderInjection->chars);
	if (*(_txtT->pstring + 5) != _fileInjection)
		str_cpy(*(_txtT->pstring + 5), _fileInjection->chars);

	// get patterns sizes
	int _filterSize = _INT(str_len(_filter->chars));
	int _folderInjSize = _INT(str_len(_folderInjection->chars));
	int _fileInjSize = _INT(str_len(_fileInjection->chars));
	
	// loop throgh he whole folder content
	int _countOld = _count;
	int _folderCount = _count;
	for (const fs::directory_entry& _p : fs::directory_iterator(_fullPath)) {
		const fs::path& _path = _p.path();

		// havoid system and hidden files
		DWORD _attb = GetFileAttributesW(_path.wstring().c_str());
		if (_attb & FILE_ATTRIBUTE_HIDDEN)
			continue;
		if (_attb & FILE_ATTRIBUTE_SYSTEM)
			continue;

		// sort directories and add the injection strings
		STRING *_strT = *(_txtT->pstring + _count);
		if (fs::is_directory(_path)) { 
			// copy directory injection string
			str_cpy(_strT, _folderInjection->chars);
			// sort to last directory
			int _ii = _count;
			for (; _ii > _folderCount; _ii -= 1)
				(_txtT->pstring)[_ii] = (_txtT->pstring)[_ii - 1];
			(_txtT->pstring)[_ii] = _strT;
			// encrease folder count
			_folderCount += 1;
		} else {
			// check file filter
			if (_filterSize > 0)
				if (strcmp(_path.extension().u8string().c_str(), _filter->chars) != 0)
					continue;
			str_cpy(_strT, _fileInjection->chars);
		}

		// save the filename of the current paths
		str_cat(_strT, (char*)_path.filename().u8string().c_str());

		// encrease content count
		_count += 1;
	}
	*_countPtr = _count - _countOld;
	*_folderCountPtr = _folderCount - _countOld;

	// decompose the path to its members
	_countOld = _count;
	do {
		// check if the TEXT struct has strings enough
		if (_count == _INT(_txtT->strings))
			txt_addstring(_txtT, (char*)_folderPath.filename().u8string().c_str());
		else
			str_cpy(*(_txtT->pstring + _count), (char*)_folderPath.filename().u8string().c_str());

		// 
		STRING *_strT = *(_txtT->pstring + _count);
		if (_INT(str_stri(_strT, "\\")) == 1) {
			_folderPath = _folderPath.parent_path();
			str_cpy(_strT, (char*)_folderPath.filename().u8string().c_str());
			if (_INT(str_len(_strT->chars)) == 0) {
				str_cpy(_strT, "ERROR");
			}
			_count += 1;
			break;
		}

		_count += 1;
		_folderPath = _folderPath.parent_path();
	} while (_folderPath.has_filename());
	*_pathDecomposedPtr = _count - _countOld;

	//for (; _count < _INT(_txtT->strings); _count += 1)
	//	str_cpy(*(_txtT->pstring + _count), "");

	return handle(_txtT);
}

DLLFUNC void imgui_h_folder_content_text_remove(var _hdl) {
	TEXT *_txt = (TEXT*)ptr_for_handle(_hdl);
	if (_txt == NULL)
		return;
	int _i = 0;
	for (; _i < _INT(_txt->strings); _i += 1)
		str_remove(*(_txt->pstring + _i));
	txt_remove(_txt);
}