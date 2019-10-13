#include <acknex.h>
#include <windows.h>
#include <d3d9.h>
#include <default.c>

#define PRAGMA_BIND "imgui.dll";

#define PRAGMA_PATH "imgui_header"

#include "imgui.h"
#include "imgui_helpers.h"
#include "imgui_h_file_dialogs.h"

#define PRAGMA_POINTER
//#define _DEBUG_FILEBROWSER_DATA

STRING *strT =                          "#1040"; //MAX_PATH * 4

#define FILE_BROWSER_BUFFER_SIZE        4096

IMGUI_H_FILE_BROWSER *fbOpenFile =      NULL;

STRING *strRootUTF8 =                   "";
STRING *strFolderUTF8 =                 "";

STRING *strFolderIcon =                 "    ";

void strFolderIcon_startup () {
	char *_chrT = strFolderIcon->chars;
	*(_chrT + 0) = 0xEE; // private U-E801
	*(_chrT + 1) = 0xA0;
	*(_chrT + 2) = 0x81;
//	*(_chrT + 0) = 0xF0; // folder icon
//	*(_chrT + 1) = 0x9F;
//	*(_chrT + 2) = 0x93;
//	*(_chrT + 3) = 0x81;
}

TEXT *txtList = {
	string = (
		"eoeoeo 1"
		"eoeoeo 2"
		"eoeoeo 3"
		"eoeoeo 4"
		"eoeoeo 5"
		"eoeoeo 6"
	);
}


// ------------------------------------------------------------------------------------------

BOOL folderTreeBuild (STRING *_strRoot, STRING *_strFolder) {
	str_cpy(strT, _strRoot);
	str_cat(strT, "\\");
	str_cat(strT, _strFolder);
	if(!imgui_h_path_exists_U(strT->chars)) {
		if(!imgui_h_create_folder_U(strT->chars)) {
			printf("Unable to create saving folder");
			sys_exit(NULL);
			return;
		}
	}
}

void fileBrowserInit () {
	utf8 *_pathUTF8 = imgui_h_get_current_path_U();
//	utf8 *_pathUTF8 = imgui_h_get_known_folder_path_U(FOLDERID_Documents);
	if(_pathUTF8 == NULL) {
		printf("unable to retrieve path");
		sys_exit(NULL);
		return;
	}
	
	str_cpy(strRootUTF8, _pathUTF8);
	str_cpy(strFolderUTF8, "My game");
	folderTreeBuild (strRootUTF8, strFolderUTF8);
	
	fbOpenFile = imgui_h_file_browser_create(FILE_BROWSER_BUFFER_SIZE);
	
	char *_chrFont1 = "..\\Visual-Studio 2010 Project\\imgui\\imgui\\misc\\fonts\\03_FreeSans.ttf";
	imgui_add_ttf_from_file_ranged(_chrFont1, 24, GLYPH_RANGE_Cyrillic);
//	imgui_add_ttf_from_file(_chrFont1, 24, GLYPH_RANGE_Cyrillic);
	
	imgui_h_text_add_string(txtList);
}

// ------------------------------------------------------------------------------------------

void *stMainMenu ();

BOOL _label_for_text(TEXT *_txt, int _index, char **_label) {
	*_label = (_txt->pstring)[_index]->chars;
	return TRUE;
}

void *stTest () {
	static int _currentItem = -1;
	static BOOL _uiWindow = TRUE;
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-160 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(320, 0, ImGuiCond_Once);
			imgui_begin("Open file", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				
				imgui_h_TEXT_list_box ("##TEXT list box", &_currentItem, _label_for_text, txtList, (int)txtList->strings, 10);
				
			imgui_end();
		imgui_end_imode();
	} else {
		_currentItem = -1;
		_uiWindow = TRUE;
		return stMainMenu;
	}
	
	if(key_esc)
		return NULL;
	
	return stTest;
}

void *stOpenFile () {
	static BOOL _uiWindow = TRUE;
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-160 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(320, 0, ImGuiCond_Once);
			imgui_begin("Open file", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				if(imgui_h_file_browser(fbOpenFile)) {
					imgui_h_fb_get_full_path (fbOpenFile, strT);
					printf(strT->chars);
					return stMainMenu;
				}
			imgui_end();
		imgui_end_imode();
	} else {
		_uiWindow = TRUE;
		return stMainMenu;
	}
	
	if(key_esc)
		return NULL;
	
	return stOpenFile;
}

void *stMainMenu () {
	static BOOL _uiWindow = TRUE;
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-160 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(320, 0, ImGuiCond_Once);
			imgui_begin("Main menu", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				var _width = imgui_get_content_region_avail_width();
				if(imgui_h_button("Open file : absolute path", _width, 0)) {
					str_cpy(strT, strRootUTF8);
					str_cat(strT, "\\");
					str_cat(strT, strFolderUTF8);
					imgui_h_file_browser_set_path(fbOpenFile, "", strT->chars, "", strFolderIcon->chars);
					return stOpenFile;
				}
				if(imgui_h_button("Open file : relative path", _width, 0)) {
					imgui_h_file_browser_set_path(fbOpenFile, strRootUTF8->chars, strFolderUTF8->chars, "", strFolderIcon->chars);
					return stOpenFile;
				}
			imgui_end();
		imgui_end_imode();
	} else {
		_uiWindow = TRUE;
		return NULL;
	}
	
	if(key_esc)
		return NULL;
	
	return stMainMenu;
}

void main () {
	video_mode = 10;
	warn_level = 6;
	mouse_mode = 4;
	mouse_pointer = 2;
	random_seed(0);
	level_load("");
	camera->flags &= ~SHOW;
	wait(3);
	on_esc = NULL;
	imgui_init(0);
	on_d3d_lost = imgui_reset;
	on_scanmessage = custom_scan_message;
	
	fileBrowserInit ();
	void *fncState ();
	fncState = stMainMenu; //stTest; //
	while(fncState) {
		fncState = fncState();
		wait(1);
	}
	
	int _i = 0;
	for(; _i<txtList->strings; _i+=1)
		str_remove(*(txtList->pstring + _i));
	imgui_h_file_browser_remove(fbOpenFile);
	
	sys_exit(NULL);
}