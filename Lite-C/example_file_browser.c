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

STRING *strRootUTF8 = NULL;
STRING *strFolderUTF8 = NULL;

STRING *strT = "#520";

#define FILE_BROWSER_BUFFER_SIZE          4096
BYTE fileBrowserBuffer[FILE_BROWSER_BUFFER_SIZE];

FOLDER_CONTENT *fcBrowser =             NULL;

COLOR4* nxs_color_unactive_item =       { red=29;	green=46;   blue=72;	alpha=255; }
COLOR4* nxs_color_unactive_text =       { red=80;	green=80;    blue=100;	alpha=255; }

STRING *strFolderIcon =                 "   ";
STRING *strFileFilter =                 ""; //".c";

void strFolderIcon_startup () {
	char *_chrT = strFolderIcon->chars;
	*_chrT = 0xEE;
	*(_chrT + 1) = 0xA0;
	*(_chrT + 2) = 0x81;
}

// ------------------------------------------------------------------------------------------

char *chrLogicalDrives[32];
var nLogicalDrivesCount = -1;
var nCurrentDrive = -1;

void chrLogicalDrives_startup () {
	long _logicalDrives = imgui_h_get_logical_drives();
	nLogicalDrivesCount = 0;
	long _l = 0;
	for(; _l<32; _l+=1) {
		if(!(_logicalDrives & (1<<_l)))
			continue;
		char *_chrT = chrLogicalDrives[nLogicalDrivesCount] = sys_malloc(sizeof(char) * 3);
		*_chrT = 65 + _l; // drive letter
		*(_chrT + 1) = 58; // doble point
		*(_chrT + 2) = NULL;
		nLogicalDrivesCount += 1;
	}
	_l = nLogicalDrivesCount;
	for(; _l<32; _l+=1)
		chrLogicalDrives[_l] = NULL;
}

void nCurrentDriveSet () {
	int _i = 0;
	for(; _i<nLogicalDrivesCount; _i+=1) {
		if(str_stri(_str(fcBrowser->folder), _str(chrLogicalDrives[_i])) == 1) {
			nCurrentDrive = _i;
			break;
		}
	}
}

void chrLogicalDrivesRemove () {
	int _i = 0;
	for(; _i<nLogicalDrivesCount; _i+=1)
		sys_free(chrLogicalDrives[_i]);
}

// ------------------------------------------------------------------------------------------

TEXT *txtDecomposedPath = {
	strings = 0;
}

void txtBrowserPathDecompose (utf8 *_path) {
	str_cpy(strT, _path);
	int _length = str_len(strT);
	int _count = 0;
	for(; _length > 0; _length = str_len(strT), _count += 1) {
		if(_count > txtDecomposedPath->strings - 1)
			txt_addstring(txtDecomposedPath, str_create(""));
		STRING **_strT = txtDecomposedPath->pstring + (int)txtDecomposedPath->strings - 1;
		STRING **_strL = txtDecomposedPath->pstring;
		STRING *_str0 = *_strT;
		for(; _strT>_strL; _strT-=1)
			*_strT = *(_strT - 1);
		*_strT = _str0;
		str_cpy(_str0, imgui_h_get_path_filename_U(strT->chars));
		_length = minv(str_len(_str0) + 1, _length);
		str_trunc(strT, _length);
	}
	txtDecomposedPath->skill_x = _count;
	for(; _count<txtDecomposedPath->strings; _count+=1)
		str_cpy((txtDecomposedPath->pstring)[_count], "");
}

void txtDecomposedPathRemove () {
	int _i = 0;
	for(; _i<txtDecomposedPath->strings; _i+=1)
		str_remove((txtDecomposedPath->pstring)[_i]);
}

// ------------------------------------------------------------------------------------------

STRING *strBrowserCurrent = "#520";
STRING *strBrowserPath = "#520";
STRING *strBrowserLabel = "#520";

void *stFileBrowser () {
	static BOOL _uiWindow = TRUE;
	static int _listIndex = -1;
	
	mouse_pointer = 2;
	if(_uiWindow) {
		imgui_start_imode();
			imgui_set_next_window_pos(-160 + screen_size.x / 2, -180 + screen_size.y / 2, ImGuiCond_Once);
			imgui_set_next_window_size(320, 0, ImGuiCond_Once);
			imgui_begin("Open file", &_uiWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				// Retrieve the available width
				var _width = imgui_get_content_region_avail_width();
				
				// Init temporal variables
				str_cpy(strBrowserPath, ""); // will contain the full path of the path buttons
				var _column = 0; // current width of the shown path buttons
				
				// ----- CURRENT PATH BUTTONS -----
				// Loop through the decomposed current path
				int _i = 0;
				for(; _i<txtDecomposedPath->skill_x; _i+=1) { // txtDecomposedPath->skill_x contains the current count of members
					STRING *_strT = *(txtDecomposedPath->pstring + _i);
					// If the root of the current path is a logical drive
					if(imgui_h_path_is_root_U(_strT->chars)) {
						imgui_push_item_width(24);
						// Set a combo with all the logical drives
						if (imgui_begin_combo("##DrivesCombo", chrLogicalDrives[nCurrentDrive], ImGuiComboFlags_NoArrowButton)) {
							int _ii=0;
							// Loop trough the chrLogicalDrives array
							for(; _ii<nLogicalDrivesCount; _ii+=1) {
								BOOL _isSelected = (nCurrentDrive == _ii);
								if (imgui_selectable(chrLogicalDrives[_ii], &_isSelected, NULL)) {
									str_cpy(strBrowserPath, chrLogicalDrives[_ii]);
									str_cat(strBrowserPath, "\\"); // add a counter bar to the logical drive, otherway it will point to the drive mounting pòint
									if(imgui_h_get_folder_content_U("", strBrowserPath->chars, fileBrowserBuffer, FILE_BROWSER_BUFFER_SIZE, strFileFilter->chars, strFolderIcon->chars)) {
										nCurrentDrive = _ii; // set as actual drive
										txtBrowserPathDecompose (chrLogicalDrives[_ii]); // build the decomposed current path
									}
									_listIndex = -1; // clear any selection in the files list
								}     
							}
							imgui_end_combo();
						} else {
							str_cpy(strBrowserPath, chrLogicalDrives[nCurrentDrive]); // copy the drive to the full path
						}
					} else { // if it is not a logical drive
						if(_i != 0) // if it is not the first member of the decomposed path
							str_cat(strBrowserPath, "\\"); // add a counter bar to the full path
						if((_column != 0) && (_column < _width * 0.7)) // if the new button fits in the same line
							imgui_same_line();
						else
							_column = 0;
						str_cat(strBrowserPath, _strT); // copy the current decomposited member to the full path
						if(imgui_h_button(_strT->chars, 0, 0)) {
							// Set the new current path
							imgui_h_get_folder_content_U(fcBrowser->root, strBrowserPath->chars, fileBrowserBuffer, FILE_BROWSER_BUFFER_SIZE, strFileFilter->chars, strFolderIcon->chars);
							txtBrowserPathDecompose (strBrowserPath->chars); // build the decomposed current path
							_listIndex = -1; // clear any selection in the file list
						}
					}
					VECTOR _vPos;
					vec_set(&_vPos, imgui_get_item_rect_size());
					_column += _vPos.x; // add the last item width to the measuring variable
				}
				
				// ----- CURRENT PATH CONTENT LIST BOX -----
				// Set a list box with the result of 'imgui_h_get_folder_content_U'
				imgui_push_item_width(_width);
				if(imgui_list_box("##PathContentList", &_listIndex, &fcBrowser->content, fcBrowser->count, 6)) {
					// Build the visible part of the new path
					str_cpy(strBrowserCurrent, fcBrowser->folder); // copy the current path to a temporary string
					if(*(strBrowserCurrent->chars + (int)str_len(strBrowserCurrent) - 1) != 92) // 92 -> counter slash bar
						str_cat(strBrowserCurrent, "\\"); // add an ending counter bar to the new path when it ends with another character 
					utf8 *_label = *(&fcBrowser->content + _listIndex); // get a pointer to the name of the clicked member
					if(str_stri(_str(_label), strFolderIcon) == 1) // if it starts with the injected folder icon 
						_label += (int)str_len(strFolderIcon); // move the pointer forward
					str_cat(strBrowserCurrent, _label);// copy the label to the temporary string
					// Build the hidden part of the new path
					if(*fcBrowser->root == NULL) // if the previous path has no root path
						str_cpy(strBrowserPath, "");
					else {
						str_cpy(strBrowserPath, fcBrowser->root); // copy the current path to a temporary string
						if(*(strBrowserPath->chars + (int)str_len(strBrowserPath) - 1) != 92)
							str_cat(strBrowserPath, "\\"); // add an ending counter bar to the new path when it ends with another character 
					}
					// Build the full path
					str_cat(strBrowserPath, strBrowserCurrent); 
					
					if(imgui_h_path_is_folder_U(strBrowserPath->chars)) { // if the full path is a folder
						// Set the new current path
						imgui_h_get_folder_content_U(fcBrowser->root, strBrowserCurrent->chars, fileBrowserBuffer, FILE_BROWSER_BUFFER_SIZE, strFileFilter->chars, strFolderIcon->chars);
						txtBrowserPathDecompose (strBrowserCurrent->chars); // build the decomposed current path
						_listIndex = -1; // clear any selection in the files list
					} else {
//						printf("%s\nis not a folder", strBrowserLabel->chars);
					}
				}
				imgui_pop_item_width();
				
				// ----- SELECT BUTTON -----
				// Build the selection button, only activated when the selected item is a file
				if(_listIndex >= 0) {
					if(imgui_h_button("Select", _width, 0)) {
						/* -------- DO STUFF -------- */
						_listIndex = -1; // clear any selection in the files list
					}
				} else {
					imgui_push_style_color(ImGuiCol_Text, nxs_color_unactive_text);
					imgui_push_style_color(ImGuiCol_Button, nxs_color_unactive_item);
					imgui_push_style_color(ImGuiCol_ButtonHovered, nxs_color_unactive_item);
					imgui_push_style_color(ImGuiCol_ButtonActive, nxs_color_unactive_item);
					imgui_h_button("Select", _width, 0);
					imgui_pop_style_color(4);
				}
				
				if(imgui_is_any_item_hoverd())
					mouse_pointer = 1;
				
#ifdef _DEBUG_FILEBROWSER_DATA		
				imgui_text(fcBrowser->root);
				imgui_text(fcBrowser->folder);
				
				utf8 **_uT = &fcBrowser->content;
				utf8 **_uL = _uT + fcBrowser->count;
				for(; _uT<_uL; _uT+=1)
					imgui_text(*_uT);
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

BOOL folderTreeBuild (STRING *_strRoot, STRING *_strFolder) {
	str_cpy(strT, strRootUTF8);
	str_cat(strT, "\\");
	str_cat(strT, strFolderUTF8);
	if(!imgui_h_path_exists_U(strT->chars)) {
		if(!imgui_h_create_folder_U(strT->chars)) {
			printf("Unable to create saving folder");
			sys_exit(NULL);
			return;
		}
	}
}

void fcBrowserInit () {
	imgui_init(0);
	on_d3d_lost = imgui_reset;
	on_scanmessage = custom_scan_message;
	
	
	utf8 *_pathUTF8 = imgui_h_get_current_path_U();
//	utf8 *_pathUTF8 = imgui_h_get_known_folder_path_U(FOLDERID_Documents);
	if(_pathUTF8 == NULL) {
		printf("unable to retrieve path");
		sys_exit(NULL);
		return;
	} else {
		strRootUTF8 = str_create(_pathUTF8);
	}

	strFolderUTF8 = str_create("My game");
	
	folderTreeBuild (strRootUTF8, strFolderUTF8);
	
	
	fcBrowser = (FOLDER_CONTENT*)fileBrowserBuffer; // A tricky cast that allows a straight access to the addresses of the buffer
	
	imgui_h_get_folder_content_U("", strT->chars, fileBrowserBuffer, FILE_BROWSER_BUFFER_SIZE, strFileFilter->chars, strFolderIcon->chars);
//	imgui_h_get_folder_content_U(strRootUTF8->chars, strFolderUTF8->chars, fileBrowserBuffer, FILE_BROWSER_BUFFER_SIZE, strFileFilter->chars, strFolderIcon->chars);
	
	txtBrowserPathDecompose (fcBrowser->folder);
	
	
	char *_chrFont1 = "..\\Visual-Studio 2010 Project\\imgui\\imgui\\misc\\fonts\\03_FreeSans.ttf";
	
	imgui_add_ttf_from_file_ranged(_chrFont1, 24, GLYPH_RANGE_Cyrillic);
//	imgui_add_ttf_from_file(_chrFont1, 24, GLYPH_RANGE_Cyrillic);
	
	
	nCurrentDriveSet ();
	
}

void main () {
	video_mode = 10;
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
	
	txtDecomposedPathRemove();
	chrLogicalDrivesRemove();
	
	sys_exit(NULL);
}