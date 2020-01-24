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

STRING *strT =                          "#1040"; //MAX_PATH * 4

#define FILE_BROWSER_BUFFER_SIZE        4096

IMGUI_H_FILE_BROWSER *fbOpenFile =      NULL;

STRING *strRootUTF8 =                   "";
STRING *strFolderUTF8 =                 "";

STRING *strFolderIcon =                 "/";
STRING *strFileIcon =                   " ";

var hdlFileDialog = 0;

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
	
	hdlFileDialog = imgui_h_get_folder_content_text(NULL, strRootUTF8, strFolderUTF8, _str(""), _str(""), _str(""));
	
//	TEXT *_txtT = (TEXT*)ptr_for_handle(hdlFileDialog);
//	_txtT->flags |= SHOW;
}

// ------------------------------------------------------------------------------------------

void *stMainMenu ();

void *stOpenFile () {
	static BOOL _uiWindow = TRUE;
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-160 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(320, 0, ImGuiCond_Once);
			imgui_begin("Open file", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				var _width = imgui_get_content_region_avail_width();
				imgui_h_file_browser(fbOpenFile);
				if(fbOpenFile->selectedIndex < 0) {
					imgui_h_button_unactive("Select", _width, 0);
				} else {
					if(imgui_h_button("Select", _width, 0)) {
						imgui_h_fb_get_full_path (fbOpenFile, strT);
						printf(strT->chars);
						return stMainMenu;
					}
				}
				if(imgui_is_any_item_hoverd())
					mouse_pointer = 1;
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

void *stOpenFileText () {
	static BOOL _uiWindow = TRUE;
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-160 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(320, 0, ImGuiCond_Once);
			imgui_begin("Open file TEXT", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				var _width = imgui_get_content_region_avail_width();
				imgui_h_text_file_browser(hdlFileDialog);
				if(imgui_h_text_get_selected(hdlFileDialog) < 0) {
					imgui_h_button_unactive("Select", _width, 0);
				} else {
					if(imgui_h_button("Select", _width, 0)) {
						imgui_h_text_get_full_path(hdlFileDialog, strT);
						printf(strT->chars);
						return stMainMenu;
					}
				}
				if(imgui_is_any_item_hoverd())
					mouse_pointer = 1;
			imgui_end();
		imgui_end_imode();
	} else {
		_uiWindow = TRUE;
		return stMainMenu;
	}
	
	if(key_esc)
		return NULL;
	
	return stOpenFileText;
}

void *stSelectFolderText () {
	static BOOL _uiWindow = TRUE;
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-160 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(320, 0, ImGuiCond_Once);
			imgui_begin("Select Folder TEXT", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				var _width = imgui_get_content_region_avail_width();
				imgui_h_text_folder_select(hdlFileDialog);
				if(imgui_h_button("Select", _width, 0)) {
					imgui_h_text_get_full_path(hdlFileDialog, strT);
					printf(strT->chars);
					return stMainMenu;
				}
				if(imgui_is_any_item_hoverd())
					mouse_pointer = 1;
			imgui_end();
		imgui_end_imode();
	} else {
		_uiWindow = TRUE;
		return stMainMenu;
	}
	
	if(key_esc)
		return NULL;
	
	return stSelectFolderText;
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
				// Open File
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
				// Open File TEXT
				if(imgui_h_button("Open file : TEXT* absolute path", _width, 0)) {
					str_cpy(strT, strRootUTF8);
					str_cat(strT, "\\");
					str_cat(strT, strFolderUTF8);
					imgui_h_get_folder_content_text(hdlFileDialog, _str(""), strT, _str(""), strFolderIcon, strFileIcon);
					return stOpenFileText;
				}
				if(imgui_h_button("Open file : TEXT* relative path", _width, 0)) {
					imgui_h_get_folder_content_text(hdlFileDialog, strRootUTF8, strFolderUTF8, _str(""), strFolderIcon, strFileIcon);
					return stOpenFileText;
				}
				// Select folder TEXT
				if(imgui_h_button("Select folder : TEXT* absolute path", _width, 0)) {
					str_cpy(strT, strRootUTF8);
					str_cat(strT, "\\");
					str_cat(strT, strFolderUTF8);
					imgui_h_get_folder_content_text(hdlFileDialog, _str(""), strT, _str("?"), strFolderIcon, strFileIcon);
					return stSelectFolderText;
				}
				if(imgui_h_button("Select folder : TEXT* relative path", _width, 0)) {
					imgui_h_get_folder_content_text(hdlFileDialog, strRootUTF8, strFolderUTF8, _str("?"), strFolderIcon, strFileIcon);
					return stSelectFolderText;
				}
				
				if(imgui_is_any_item_hoverd())
					mouse_pointer = 1;
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
	fncState = stMainMenu;
	
	while(fncState) {
		fncState = fncState();
		wait(1);
	}
	
	imgui_h_file_browser_remove(fbOpenFile);
	imgui_h_folder_content_text_remove(hdlFileDialog);
	
	sys_exit(NULL);
}