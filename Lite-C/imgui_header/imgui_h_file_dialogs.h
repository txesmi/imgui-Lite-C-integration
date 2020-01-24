#ifndef _IMGUI_H_FILE_DIALOGS_H_
#define _IMGUI_H_FILE_DIALOGS_H_

#include "imgui.h"
#include "imgui_helpers.h"

// ------------------------------------------------------------------------------------------

typedef struct IMGUI_H_FILE_BROWSER {
	FOLDER_CONTENT *buffer;
	int bufferSize;
	int selectedIndex;
	int logicalDriveIndex;
	TEXT *decomposedPath;
	STRING *extensionFilter;
	STRING *folderPrefix;
} IMGUI_H_FILE_BROWSER;


IMGUI_H_FILE_BROWSER *imgui_h_file_browser_create (int _bufferSize);

void imgui_h_file_browser_remove (IMGUI_H_FILE_BROWSER *_fb);

void imgui_h_file_browser_set_path (IMGUI_H_FILE_BROWSER *_fb, utf8 *_uRoot, utf8 *_uFolder, utf8 *_uFilter, utf8 *_uPrefix);

BOOL imgui_h_file_browser (IMGUI_H_FILE_BROWSER *_fb);

void imgui_h_get_full_path (IMGUI_H_FILE_BROWSER *_fb, STRING *_strT);

// ------------------------------------------------------------------------------------------

BOOL imgui_h_text_file_browser (var _hdlText);

void imgui_h_text_get_full_path (var _hdlText, STRING *_strT);

void imgui_h_text_get_selected (var _hdlText);

// ------------------------------------------------------------------------------------------

#include "imgui_h_file_dialogs.c"
#endif