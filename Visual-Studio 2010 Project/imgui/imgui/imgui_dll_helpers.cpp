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

	#define UTF8_MAX 520

	typedef struct FolderContent {
		utf8 root[UTF8_MAX] = { NULL };
		utf8 folder[UTF8_MAX] = { NULL };
		utf8 **content = nullptr;
		int count = 0;
		int capacity = 0;
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

DLLFUNC utf8 *imgui_h_UnicodeToUtf8(unicode *_text, int _size) {
	static utf8 _utf8Text[UTF8_MAX];
	WideCharToMultiByte(CP_UTF8, NULL, _text, -1, _utf8Text, UTF8_MAX, NULL, NULL);
	return _utf8Text;
}

DLLFUNC FolderContent * fsSetPath(FolderContent *_fc, ansi *_chrPath) {
	fs::path _path = _chrPath;
	if (fs::is_directory(_path)) {
		if (_fc == NULL)
			_fc = new FolderContent;
		memcpy(_fc->folder, _chrPath, MAX_PATH);
		int _count = std::distance(fs::directory_iterator(_path), {});
		if (_count > _fc->capacity) {
			char **_content = _fc->content;
			_fc->content = (char**)malloc(_count * sizeof(char*));
			memcpy(_fc->content, _content, _fc->capacity * sizeof(char*));
			free(_content);
			char **_chrT = _fc->content + _fc->capacity;
			char **_chrL = _fc->content + _count;
			for (; _chrT < _chrL; _chrT += 1)
				*_chrT = (char*)malloc(MAX_PATH * sizeof(char));
			_fc->capacity = _count;
		}
		_fc->count = 0;
		char **_chrT = _fc->content;
		for (const fs::directory_entry& dirEntry : fs::directory_iterator(_path)) {
			const fs::path& _dirPath = dirEntry.path();
			if (fs::is_directory(_dirPath)) {
				**_chrT = 47;
				memcpy(*_chrT + 1, _dirPath.filename().string().c_str(), (MAX_PATH - 1) * sizeof(char));
				char *_chr0 = *_chrT;
				char **_chrT2 = _chrT;
				for (; _chrT2 > _fc->content; _chrT2 -= 1) {
					if (**(_chrT2 - 1) == 47)
						break;
					*_chrT2 = *(_chrT2 - 1);
				}
				*_chrT2 = _chr0;
			}
			else {
				memcpy(*_chrT, _dirPath.filename().string().c_str(), (MAX_PATH - 1) * sizeof(char));
			}
			_fc->count += 1;
			_chrT += 1;
		}
		char **_chrL = _fc->content + _fc->capacity;
		for (; _chrT < _chrL; _chrT += 1)
			**_chrT = NULL;
		return _fc;
	}
	else {
		return NULL;
	}
}

DLLFUNC FolderContent * fsSetPathW(FolderContent *_fc, unicode *_sRoot, ansi *_chrPath) {
	fs::path _root = _sRoot;
	fs::path _path = _chrPath;
	if (fs::is_directory(_path)) {
		if (_fc == NULL)
			_fc = new FolderContent;
		memcpy(_fc->folder, _chrPath, MAX_PATH);
		int _count = std::distance(fs::directory_iterator(_path), {});
		if (_count > _fc->capacity) {
			char **_content = _fc->content;
			_fc->content = (char**)malloc(_count * sizeof(char*));
			memcpy(_fc->content, _content, _fc->capacity * sizeof(char*));
			free(_content);
			char **_chrT = _fc->content + _fc->capacity;
			char **_chrL = _fc->content + _count;
			for (; _chrT < _chrL; _chrT += 1)
				*_chrT = (char*)malloc(MAX_PATH * sizeof(char));
			_fc->capacity = _count;
		}
		_fc->count = 0;
		char **_chrT = _fc->content;
		for (const fs::directory_entry& dirEntry : fs::directory_iterator(_path)) {
			const fs::path& _dirPath = dirEntry.path();
			if (fs::is_directory(_dirPath)) {
				**_chrT = 47;
				memcpy(*_chrT + 1, _dirPath.filename().string().c_str(), (MAX_PATH - 1) * sizeof(char));
				char *_chr0 = *_chrT;
				char **_chrT2 = _chrT;
				for (; _chrT2 > _fc->content; _chrT2 -= 1) {
					if (**(_chrT2 - 1) == 47)
						break;
					*_chrT2 = *(_chrT2 - 1);
				}
				*_chrT2 = _chr0;
			}
			else {
				memcpy(*_chrT, _dirPath.filename().string().c_str(), (MAX_PATH - 1) * sizeof(char));
			}
			_fc->count += 1;
			_chrT += 1;
		}
		char **_chrL = _fc->content + _fc->capacity;
		for (; _chrT < _chrL; _chrT += 1)
			**_chrT = NULL;
		return _fc;
	}
	else {
		return NULL;
	}
}

DLLFUNC void fsFolderContentRemove(FolderContent *_fc) {
	char **_chrT = _fc->content;
	char **_chrL = _fc->content + _fc->capacity;
	for (; _chrT < _chrL; _chrT += 1)
		free(*_chrT);
	free(_fc->content);
	delete(_fc);
}

DLLFUNC char *fsGetCurrentPath() {
	static char _chrPath[MAX_PATH];
	strcpy_s(_chrPath, fs::current_path().string().c_str());
	return _chrPath;
}

DLLFUNC short *fsGetCurrentPathW() {
	static short _sPath[MAX_PATH];
	memcpy(_sPath, fs::current_path().wstring().c_str(), fs::current_path().wstring().length() * sizeof(short));
	return _sPath;
}

DLLFUNC long fsGetLogicalDrives() {
	return (long)GetLogicalDrives();
}

DLLFUNC var fsPathIsAbsolute(ansi *_chrPath) {
	fs::path _path = _chrPath;
	bool res = _path.is_absolute();
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC char *fsGetKnownFolderPath(var _id) {
	PWSTR _path;
	static char _chrPath[MAX_PATH];
	switch (_INT(_id)) {
	case FID_Documents: SHGetKnownFolderPath(FOLDERID_Documents, NULL, 0, &_path); break;
	case FID_ProgramData: SHGetKnownFolderPath(FOLDERID_ProgramData, NULL, 0, &_path); break;
	case FID_LocalAppData: SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, 0, &_path); break;
	case FID_SavedGames: SHGetKnownFolderPath(FOLDERID_SavedGames, NULL, 0, &_path); break;
	default: return NULL;
	}
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, _path, -1, _chrPath, MAX_PATH, NULL, NULL);
	return _chrPath;
}

DLLFUNC void fsSetCurrentDirectory(ansi *_chrPath) {
	fs::path _path = _chrPath;
	fs::current_path(_path);
}

DLLFUNC bool fsAddFolderW(unicode *_sRoot, unicode *_sFolder) {
	fs::path _path = _sRoot;
	if (fs::is_directory(_path)) {
		_path += _sFolder;
		CreateDirectoryW(_path.wstring().c_str(), NULL);
		return true;
	} else {
		return false;
	}
}









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

// ---------------------------------------------------------------------------------------------------------------------------

DLLFUNC long imgui_h_get_logical_drives() {
	return (long)GetLogicalDrives();
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

DLLFUNC FolderContent * imgui_h_get_folder_content_U(FolderContent *_fc, utf8 *_uRoot, utf8 *_uFolder) {
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
			_fc = new FolderContent;
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
				**_uT = 47;
				memcpy(*_uT + 1, _dirPath.filename().u8string().c_str(), (UTF8_MAX - 2) * sizeof(utf8));
				*(*_uT + UTF8_MAX - 1) = NULL;
				utf8 *_u0 = *_uT;
				utf8 **_uT2 = _uT;
				for (; _uT2 > _fc->content; _uT2 -= 1) {
					if (**(_uT2 - 1) == 47)
						break;
					*_uT2 = *(_uT2 - 1);
				}
				*_uT2 = _u0;
			}
			else {
				memcpy(*_uT, _dirPath.filename().u8string().c_str(), (UTF8_MAX - 2) * sizeof(utf8));
				*(*_uT + UTF8_MAX - 1) = NULL;
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

DLLFUNC void imgui_h_remove_folder_content(FolderContent *_fc) {
	char **_chrT = _fc->content;
	char **_chrL = _fc->content + _fc->capacity;
	for (; _chrT < _chrL; _chrT += 1)
		free(*_chrT);
	free(_fc->content);
	delete(_fc);
}

