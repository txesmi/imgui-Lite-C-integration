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

	#define UTF8_MAX 1024

	typedef struct FolderContent {
		char folder[MAX_PATH] = "";
		char **content = nullptr;
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

DLLFUNC short *fsGetKnownFolderPathW(var _id) {
	PWSTR _path;
	switch (_INT(_id)) {
	case FID_Documents: SHGetKnownFolderPath(FOLDERID_Documents, NULL, 0, &_path); break;
	case FID_ProgramData: SHGetKnownFolderPath(FOLDERID_ProgramData, NULL, 0, &_path); break;
	case FID_LocalAppData: SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, 0, &_path); break;
	case FID_SavedGames: SHGetKnownFolderPath(FOLDERID_SavedGames, NULL, 0, &_path); break;
	default: return NULL;
	}
	return (short*)_path;
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
