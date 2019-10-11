#include "dllmain.h"

#include <experimental/filesystem>
#include <filesystem>
#include <Shlobj.h>
#include <Knownfolders.h>
#include <comdef.h> 

namespace fs = std::experimental::filesystem;

namespace imgui_helpers {
	typedef char ansi;
	typedef wchar_t unicode;
	typedef char utf8;

	#define UTF8_MAX             520

	#define FOLDER_ICON          0xE801 //47 //128194 //0xf09f9382 //4036989826 //
	#define FOLDER_ICON_SIZE     3 //1  //4

	#define PATH_SIZE_MAX        (UTF8_MAX - FOLDER_ICON_SIZE + 2)

	typedef struct FolderContent2 {
		utf8 root[UTF8_MAX] = { NULL };
		utf8 folder[UTF8_MAX] = { NULL };
		utf8 **content = nullptr;
		int count = 0;
		int capacity = 0;
	} FolderContent2;

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

using namespace imgui_helpers;

DLLFUNC var imgui_h_button(char *label, var width, var height) {
	bool res = ImGui::Button(label, ImVec2(_FLOAT(width), _FLOAT(height)));
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_calc_item_width() {
	float _f = ImGui::CalcItemWidth();
	return _VAR(_f);
}

// ---------------------------------------------------------------------------------------------------------------------------

DLLFUNC unicode *imgui_h_get_known_folder_path_W(var _id) {
	unicode *_pathW;
	switch (_INT(_id)) {
	case FID_Documents: SHGetKnownFolderPath(FOLDERID_Documents, NULL, 0, &_pathW); break;
	case FID_ProgramData: SHGetKnownFolderPath(FOLDERID_ProgramData, NULL, 0, &_pathW); break;
	case FID_LocalAppData: SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, 0, &_pathW); break;
	case FID_SavedGames: SHGetKnownFolderPath(FOLDERID_SavedGames, NULL, 0, &_pathW); break;
	default: return NULL;
	}
	return _pathW;
}

DLLFUNC var imgui_h_path_is_absolute_W(unicode *_wPath) {
	fs::path _path = _wPath;
	bool res = _path.is_absolute();
	return res ? _VAR(1) : _VAR(0);
}

// ---------------------------------------------------------------------------------------------------------------------------

DLLFUNC utf8 *imgui_h_unicode_to_utf8(unicode *_text, int _size) {
	static utf8 _utf8Text[UTF8_MAX];
	WideCharToMultiByte(CP_UTF8, NULL, _text, -1, _utf8Text, UTF8_MAX, NULL, NULL);
	return _utf8Text;
}

DLLFUNC long imgui_h_get_logical_drives() {
	return (long)GetLogicalDrives();
}

DLLFUNC utf8 *imgui_h_get_current_path_U() {
	static utf8 _uPath[UTF8_MAX];
	strcpy_s(_uPath, fs::current_path().u8string().c_str());
	*(_uPath + UTF8_MAX - 1) = NULL;
	return _uPath;
}

DLLFUNC utf8 *imgui_h_get_path_root_U(utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	static utf8 _strPath[UTF8_MAX];
	const std::string _filename = _path.filename().u8string();
	int _length = min(_filename.size() + 1, UTF8_MAX - 2);
	memcpy(_strPath, _filename.c_str(), _length);
	*(_strPath + UTF8_MAX - 1) = NULL;
	return _strPath;
}

DLLFUNC void imgui_h_set_current_path_U(utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	fs::current_path(_path);
}

DLLFUNC utf8 *imgui_h_get_known_folder_path_U(var _id) {
	unicode *_pathW;
	switch (_INT(_id)) {
		case FID_Documents: SHGetKnownFolderPath(FOLDERID_Documents, NULL, 0, &_pathW); break;
		case FID_ProgramData: SHGetKnownFolderPath(FOLDERID_ProgramData, NULL, 0, &_pathW); break;
		case FID_LocalAppData: SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, 0, &_pathW); break;
		case FID_SavedGames: SHGetKnownFolderPath(FOLDERID_SavedGames, NULL, 0, &_pathW); break;
		default: return NULL;
	}
	fs::path _path = _pathW;
	static utf8 _pathUTF8[UTF8_MAX];
	memcpy(_pathUTF8, _path.u8string().c_str(), UTF8_MAX);
	*(_pathUTF8 + UTF8_MAX - 1) = NULL;
	return _pathUTF8;
}

DLLFUNC var imgui_h_path_is_absolute_U(utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = _path.is_absolute();
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_path_is_root_U(utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = _path.has_root_name();
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_path_exists_U(utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = false;
	if (fs::exists(_path))
		res = true;
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_path_is_folder_U(utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = false;
	if (fs::is_directory(_path))
		res = true;
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_h_create_folder_U(utf8 *_uPath) {
	fs::path _path = fs::u8path(_uPath);
	bool res = false;
	if (CreateDirectoryW(_path.wstring().c_str(), NULL))
		res = true;
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC FolderContent2 * imgui_h_get_folder_content_U2(FolderContent2 *_fc, utf8 *_uRoot, utf8 *_uFolder) {
	fs::path _fullPath;

	if (_uFolder == NULL)
		return NULL;

	if (*_uRoot == NULL)
		_fullPath = fs::u8path(_uFolder);
	else {
		_fullPath = fs::u8path(_uRoot);
		_fullPath += "\\";
		_fullPath += fs::u8path(_uFolder);
	}

	if (!fs::exists(_fullPath))
		return NULL;

	if (fs::is_directory(_fullPath)) {
		if (_fc == NULL)
			_fc = new FolderContent2;
		memcpy(_fc->root, _uRoot, UTF8_MAX);
		memcpy(_fc->folder, _uFolder, UTF8_MAX);
		int _count = std::distance(fs::directory_iterator(_fullPath), {});
		if (_count > _fc->capacity) {
			utf8 **_content = _fc->content;
			_fc->content = (utf8**)malloc(_count * sizeof(utf8*));
			if (_content != NULL) {
				memcpy(_fc->content, _content, _fc->capacity * sizeof(utf8*));
				free(_content);
			}
			utf8 **_uT = _fc->content + _fc->capacity;
			utf8 **_uL = _fc->content + _count;
			for (; _uT < _uL; _uT += 1)
				*_uT = (utf8*)malloc(UTF8_MAX * sizeof(utf8));
			_fc->capacity = _count;
		}
			
		_fc->count = 0;
		utf8 **_uT = _fc->content;
		for (const fs::directory_entry& dirEntry : fs::directory_iterator(_fullPath)) {
			const fs::path& _dirPath = dirEntry.path();
			if (fs::is_directory(_dirPath)) {
				// 0xEEA081  -> U+E801
				*(*_uT + 0) = 0xEE;
				*(*_uT + 1) = 0xA0;
				*(*_uT + 2) = 0x81;
				std::string _pathName = _dirPath.filename().u8string();
				size_t _length = min(_pathName.size(), PATH_SIZE_MAX) + 1;
				memcpy(*_uT + FOLDER_ICON_SIZE, _pathName.c_str(), _length * sizeof(utf8));
				utf8 *_u0 = *_uT;
				utf8 **_uT2 = _uT;
				for (; _uT2 > _fc->content; _uT2 -= 1) {
					if (**(_uT2 - 1) == 47)
						break;
					*_uT2 = *(_uT2 - 1);
				}
				*_uT2 = _u0;
			} else {
				*(*_uT + 0) = 32;
				*(*_uT + 1) = 32;
				*(*_uT + 2) = 32;
				std::string _pathName = _dirPath.filename().u8string();
				size_t _length = min(_pathName.size(), PATH_SIZE_MAX) + 1;
				memcpy(*_uT + FOLDER_ICON_SIZE, _pathName.c_str(), _length * sizeof(utf8));
			}
			_fc->count += 1;
			_uT += 1;
		}
		utf8 **_uL = _fc->content + _fc->capacity;
		for (; _uT < _uL; _uT += 1)
			**_uT = NULL;
		
		return _fc;
	} else {
		return NULL;
	}
}

DLLFUNC void imgui_h_remove_folder_content(FolderContent2 *_fc) {
	char **_chrT = _fc->content;
	char **_chrL = _fc->content + _fc->capacity;
	for (; _chrT < _chrL; _chrT += 1)
		free(*_chrT);
	free(_fc->content);
	delete(_fc);
}

DLLFUNC var imgui_h_get_folder_content_U(utf8 *_uRoot, utf8 *_uFolder, utf8 *_buffer, int bufferSize, utf8 *_filter, utf8 *_folderInjection) {

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
	FolderContent *_fc = (FolderContent*)_buffer;
	std::string _strPath = "";
	int _pathSize = 0;
	utf8 *_uText = _buffer + bufferSize - 1;

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
	utf8 **_uList = &_fc->content;
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
				utf8 **_uL2 = _uList;
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
