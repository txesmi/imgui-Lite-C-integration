#include "dllmain.h"

#include <malloc.h>
#include <string>
#include <experimental/filesystem>
#include <filesystem>

namespace fs = std::experimental::filesystem;

namespace imgui_helpers {
	typedef struct FolderContent {
		char folder[MAX_PATH] = "";
		char **content = nullptr;
		int count = 0;
		int capacity = 0;
	} FolderContent;
};

using namespace imgui_helpers;

DLLFUNC var imgui_button_(char *label, var width, var height)
{
	bool res = ImGui::Button(label, ImVec2(_FLOAT(width), _FLOAT(height)));
	return res ? _VAR(1) : _VAR(0);
}

DLLFUNC var imgui_calc_item_width() {
	float _f = ImGui::CalcItemWidth();
	return _VAR(_f);
}

DLLFUNC FolderContent * fsSetPath(FolderContent *_fc, char *_chrPath) {
	fs::path _path = _chrPath;
	if (fs::is_directory(_path)) {
		if (_fc == NULL) {
			_fc = new FolderContent;
			memset(_fc, 0, sizeof(FolderContent));
		}
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
	static char chrPath[MAX_PATH];
	strcpy_s(chrPath, fs::current_path().string().c_str());
	return chrPath;
}

DLLFUNC long fsGetLogicalDrives() {
	return (long)GetLogicalDrives();
}

DLLFUNC bool fsPathIsAbsolute(char *_chrPath) {
	fs::path _path = _chrPath;
	if (_path.is_absolute())
		return true;
	else
		return false;
}