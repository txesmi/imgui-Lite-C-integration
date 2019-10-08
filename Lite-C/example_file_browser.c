#include <acknex.h>
#include <windows.h>
#include <d3d9.h>
#include <default.c>

#define PRAGMA_BIND "imgui.dll";

#define PRAGMA_PATH "imgui_header"

#include "imgui.h"
#include "imgui_helpers.h"

#define PRAGMA_POINTER
//#define _DEBUG_FILEBROWSER_DATA

STRING *strDocumentsUTF8 = NULL;
STRING *strDocsSaveFolderUTF8 = NULL;

FOLDER_CONTENT *fcBrowser = NULL;
STRING *strT = "#520";
STRING *strT2 = "#520";
STRING *strT3 = "#520";

// ------------------------------------------------------------------------------------------

char *chrLogicalDrives[32];
var nLogicalDrivesCount = -1;
var nCurrentDrive = -1;

void chrLogicalDrivesInit () {
	while(!fcBrowser)
		wait(1);
	long _logicalDrives = imgui_h_get_logical_drives();
	nLogicalDrivesCount = 0;
	long _l = 0;
	for(; _l<32; _l+=1) {
		if(!(_logicalDrives & (1<<_l)))
			continue;
		char *_chrT = chrLogicalDrives[nLogicalDrivesCount] = sys_malloc(sizeof(char) * 3);
		*_chrT = 65 + _l;
		*(_chrT + 1) = 58;
		*(_chrT + 2) = NULL;
		if(str_stri(_str(fcBrowser->folder), _str(_chrT)))
			nCurrentDrive = nLogicalDrivesCount;
		nLogicalDrivesCount += 1;
	}
}

// ------------------------------------------------------------------------------------------

void *stFileBrowser () {
	static BOOL _uiWindow = TRUE;
	static int _listIndex = -1;
	
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-256 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(512, 0, ImGuiCond_Once);
			imgui_begin("Open file", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				var _width = imgui_get_content_region_avail_width();
				str_cpy(strT, fcBrowser->folder);
				str_cpy(strT3, "");
				var _column = 0;
				
				while(str_len(strT) > 0) {
					int _pos = str_stri(strT, "\\");
					if (_pos == 0)
						_pos = str_len(strT) + 1;
					str_cpy(strT2, strT);
					str_clip(strT, minv(_pos, str_len(strT)));
					str_trunc(strT2, str_len(strT2) - _pos + 1);
					if(_column == 0) {
						if (fcBrowser->root[0] == NULL) {
							str_cat(strT3, chrLogicalDrives[nCurrentDrive]);
							imgui_push_item_width(28);
							if (imgui_begin_combo("##DrivesCombo", chrLogicalDrives[nCurrentDrive], ImGuiComboFlags_NoArrowButton)) {
								int _i=0;
								for(; _i<nLogicalDrivesCount; _i+=1) {
									BOOL _isSelected = (nCurrentDrive == _i);
									if (imgui_selectable(chrLogicalDrives[_i], &_isSelected, NULL)) {
										str_cpy(strT3, chrLogicalDrives[_i]);
										str_cat(strT3, "\\");
										if(imgui_h_get_folder_content_U(fcBrowser, "", strT3->chars))
											nCurrentDrive = _i;
										_listIndex = -1;
									}     
								}
								imgui_end_combo();
							} else {
								str_cpy(strT3, chrLogicalDrives[nCurrentDrive]);
							}
						} else {
							str_cpy(strT3, strT2);
							if(imgui_h_button(strT2->chars, 0, 0)) {
								imgui_h_get_folder_content_U(fcBrowser, fcBrowser->root, strT2->chars);
								_listIndex = -1;
							}
						}
					} else {
						if(_column < _width * 0.7)
							imgui_same_line();
						else
							_column = 0.01;
						str_cat(strT3, "\\");
						str_cat(strT3, strT2);
						if(imgui_h_button(strT2->chars, 0, 0)) {
							imgui_h_get_folder_content_U(fcBrowser, fcBrowser->root, strT3->chars);
							_listIndex = -1;
						}
					}
					
					VECTOR _vPos;
					vec_set(&_vPos, imgui_get_item_rect_size());
					_column += _vPos.x;
				}
				
				imgui_push_item_width(_width);
				if(imgui_list_box("##PathContentList", &_listIndex, fcBrowser->content, fcBrowser->count, 10)) {
					str_cpy(strT, fcBrowser->folder);
					if(*(strT->chars + (int)str_len(strT) - 1) != 92) // 92 -> counter slash bar
						str_cat(strT, "\\");
					str_cat(strT, *(fcBrowser->content + _listIndex) + 1);
					
					if(fcBrowser->root[0] == NULL)
						str_cpy(strT2, "");
					else {
						str_cpy(strT2, fcBrowser->root);
						if(*(strT2->chars + (int)str_len(strT2) - 1) != 92)
							str_cat(strT2, "\\");
					}
					str_cat(strT2, strT);
					
					if(imgui_h_path_is_folder_U(strT2->chars)) {
						if(imgui_h_get_folder_content_U(fcBrowser, fcBrowser->root, strT->chars))
							_listIndex = -1;
					} else {
						printf("%s\nis not a folder", strT2->chars);
					}
				}
				
				if(_listIndex >= 0) {
					if(imgui_h_button("Select", _width, 0)) {
						
					}
				} else {
//					imgui_push_style_color(ImGuiCol_Text, nxs_color_grey);
//					imgui_push_style_color(ImGuiCol_Border, nxs_color_grey);
//					imgui_push_style_color(ImGuiCol_Button, nxs_color_lightgrey);
//					imgui_push_style_color(ImGuiCol_ButtonHovered, nxs_color_lightgrey);
//					imgui_push_style_color(ImGuiCol_ButtonActive, nxs_color_lightgrey);
//					imgui_h_button("Select", _width, 0);
//					imgui_pop_style_color(5);
				}
				
				if(imgui_is_any_item_hoverd())
					mouse_pointer = 1;
			
#ifdef _DEBUG_FILEBROWSER_DATA		
				imgui_text(fcBrowser->root);
				imgui_text(fcBrowser->folder);
				
				utf8 **_uT = fcBrowser->content;
				utf8 **_uL = _uT + fcBrowser->capacity;
				for(; _uT<_uL; _uT+=1) {
					imgui_text(*_uT);
				}
#endif				
			imgui_end();
		imgui_end_imode();
	} else {
		_uiWindow = TRUE;
		return NULL;
	}
	
	if(key_esc)
		return NULL;
	
	return stFileBrowser;
}

void fcBrowserInit () {
	imgui_init(0);
	on_d3d_lost = imgui_reset;
	on_scanmessage = custom_scan_message;
	
	utf8 *_pathUTF8 = imgui_h_get_known_folder_path_U(FOLDERID_Documents);
	if(_pathUTF8 == NULL) {
		printf("unable to retrieve 'My Documents' folder path");
		sys_exit(NULL);
		return;
	} else {
		strDocumentsUTF8 = str_create(_pathUTF8);
	}
	
	strDocsSaveFolderUTF8 = str_create("My game");
	
	str_cpy(strT, strDocumentsUTF8);
	str_cat(strT, "\\");
	str_cat(strT, strDocsSaveFolderUTF8);
	if(!imgui_h_path_exists_U(strT->chars)) {
		if(!imgui_h_create_folder_U(strT->chars)) {
			printf("Unable to create saving folder");
			sys_exit(NULL);
			return;
		}
	}
	
	fcBrowser = imgui_h_get_folder_content_U(NULL, strDocumentsUTF8->chars, strDocsSaveFolderUTF8->chars);
	if(!fcBrowser) {
		printf("utf8 folder content error");
		sys_exit(NULL);
		return;
	}	

	char *_chrFont1 = "..\\Visual-Studio 2010 Project\\imgui\\imgui\\misc\\fonts\\arial-unicode-ms.ttf";
	imgui_add_ttf_from_file(_chrFont1, 24, GLYPH_RANGE_Cyrillic);
	
	chrLogicalDrivesInit ();
}

void main () {
	warn_level = 6;
	mouse_mode = 4;
	mouse_pointer = 2;
	random_seed(0);
	wait(3);
	on_esc = NULL;
	fcBrowserInit ();
	
	void *fncState ();
	fncState = stFileBrowser;
	while(fncState) {
		fncState = fncState();
		wait(1);
	}
	
	imgui_h_remove_folder_content(fcBrowser);
	
	sys_exit(NULL);
}