#include "dllmain.h"

#include <experimental/filesystem>
#include <filesystem>
#include <Shlobj.h>
#include <Knownfolders.h>
#include <comdef.h> 

namespace fs = std::experimental::filesystem;

namespace imgui_helpers {
	typedef wchar_t unicode;
	typedef char utf8;

	#define UTF8_MAX_PATH             1040

	typedef struct FolderContent {
		utf8 *root;
		utf8 *folder;
		int count;
		utf8 *content;
	} FolderContent;

	enum FOLDER_ID {
		FID_Documents,
		FID_ProgramData,
		FID_LocalAppData,
		FID_SavedGames,
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

// ---------------------------------------------------------------------------------------------------------------------------

DLLFUNC imgui_helpers::utf8 *imgui_h_unicode_to_utf8(imgui_helpers::unicode *_text, int _size) {
	static imgui_helpers::utf8 _utf8Text[UTF8_MAX_PATH];
	WideCharToMultiByte(CP_UTF8, NULL, _text, -1, _utf8Text, UTF8_MAX_PATH, NULL, NULL);
	return _utf8Text;
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
	
	const std::string _strInjection = _folderInjection;
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
	int _injSize = _strInjection.size();
	int _filterSize = _strFilter.size();

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
				memcpy(_uText, _strInjection.c_str(), _injSize);
				imgui_helpers::utf8 **_uL2 = _uList;
				for (; _uL2 > &_fc->content; _uL2 -= 1) {
					int _i = 0;
					for (; _i < _injSize; _i += 1)
						if (*(*(_uL2 - 1) + _i) != *(_uText + _i))
							break;
					if (_i == _injSize)
						break;
					*_uL2 = *(_uL2 - 1);
				}
				*_uL2 = _uText;
			} else {
				*_uList = _uText;
			}
		} else {
			*_uList = _uText;
		}
		
		_uList += 1;
		_fc->count += 1;
	}
	return _VAR(1);
}
