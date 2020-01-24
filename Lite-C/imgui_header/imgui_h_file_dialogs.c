
STRING *strT = "";

// LOGICAL DRIVES
// ------------------------------------------------------------------------------------------

char *_chrLogicalDrives[33];
char _chrLogicalDrivesCharacters[99];
var _nLogicalDrivesCount = -1;

void _chrLogicalDrives_startup () {
	memset(_chrLogicalDrives, 0, sizeof(char*) * 33);
	long _logicalDrives = imgui_h_get_logical_drives();
	char *_chrT = _chrLogicalDrivesCharacters;
	_nLogicalDrivesCount = 0;
	long _l = 0;
	for(; _l<32; _l+=1) {
		if(!(_logicalDrives & (1<<_l)))
			continue;
		_chrLogicalDrives[_nLogicalDrivesCount] = _chrT;
		*_chrT = 65 + _l; // drive letter
		*(_chrT + 1) = 58; // doble point
		*(_chrT + 2) = NULL;
		_chrT += 3;
		_nLogicalDrivesCount += 1;
	}
	_l = _nLogicalDrivesCount;
}

BOOL _findLogicalDrive (IMGUI_H_FILE_BROWSER *_fb) {
	int _i = 0;
	for(; _i<_nLogicalDrivesCount; _i+=1) {
		if(str_stri(_str(_fb->buffer->folder), _str(_chrLogicalDrives[_i])) != 1) 
			continue;
		_fb->logicalDriveIndex = _i;
		return TRUE;
	}
	return FALSE;
}

// ------------------------------------------------------------------------------------------

void _buildDecomposedPath (IMGUI_H_FILE_BROWSER *_fb, utf8 *_path) {
	TEXT *_txtT = _fb->decomposedPath;
	str_cpy(strT, _path);
	int _length = str_len(strT);
	int _count = 0;
	for(; _length > 0; _length = str_len(strT), _count += 1) {
		if(_count > _txtT->strings - 1)
			txt_addstring(_txtT, str_create(""));
		STRING **_strT = _txtT->pstring + (int)_txtT->strings - 1;
		STRING **_strL = _txtT->pstring;
		STRING *_str0 = *_strT;
		for(; _strT>_strL; _strT-=1)
			*_strT = *(_strT - 1);
		*_strT = _str0;
		str_cpy(_str0, imgui_h_get_path_filename_U(strT->chars));
		_length = minv(str_len(_str0) + 1, _length);
		str_trunc(strT, _length);
	}
	_txtT->skill_x = _count;
	for(; _count<_txtT->strings; _count+=1)
		str_cpy((_txtT->pstring)[_count], "");
}

// ------------------------------------------------------------------------------------------

IMGUI_H_FILE_BROWSER *imgui_h_file_browser_create (int _bufferSize) {
	IMGUI_H_FILE_BROWSER *_fb = sys_malloc(sizeof(IMGUI_H_FILE_BROWSER));
	_fb->buffer = sys_malloc(_bufferSize);
	_fb->bufferSize = _bufferSize;
	_fb->logicalDriveIndex = -1;
	_fb->selectedIndex = -1;
	_fb->decomposedPath = txt_create(0, 1);
	_fb->extensionFilter = str_create("");
	_fb->folderPrefix = str_create("");
	return _fb;
}

void imgui_h_file_browser_remove (IMGUI_H_FILE_BROWSER *_fb) {
	sys_free(_fb->buffer);
	STRING **_strT = _fb->decomposedPath->pstring;
	STRING **_strL = _strT + (int)_fb->decomposedPath->strings;
	for(; _strT<_strL; _strT+=1)
		str_remove(*_strT);
	txt_remove(_fb->decomposedPath);
	str_remove(_fb->extensionFilter);
	str_remove(_fb->folderPrefix);
	sys_free(_fb);
}

void imgui_h_file_browser_set_path (IMGUI_H_FILE_BROWSER *_fb, utf8 *_uRoot, utf8 *_uFolder, utf8 *_uFilter, utf8 *_uPrefix) {
	str_cpy(_fb->extensionFilter, _uFilter);
	str_cpy(_fb->folderPrefix, _uPrefix);
	imgui_h_get_folder_content_U(_uRoot, _uFolder, _fb->buffer, _fb->bufferSize, _uFilter, _uPrefix);
	_buildDecomposedPath(_fb, _uFolder);
	_findLogicalDrive(_fb);
	_fb->selectedIndex = -1;
}

void imgui_h_fb_get_full_path (IMGUI_H_FILE_BROWSER *_fb, STRING *_strT) {
	if(*_fb->buffer->root != NULL) {
		str_cpy(_strT, _fb->buffer->root);
		str_cat(_strT, "\\");
		str_cat(_strT, _fb->buffer->folder);
	} else {
		str_cpy(_strT, _fb->buffer->folder);
	}
	if(_fb->selectedIndex >= 0) {
		str_cat(_strT, "\\");
		str_cat(_strT, *(&_fb->buffer->content + _fb->selectedIndex));
	}
}

// ------------------------------------------------------------------------------------------

STRING *strBrowserCurrent = "#520";
STRING *strBrowserPath = "#520";
//STRING *strBrowserLabel = "#520";


BOOL imgui_h_file_browser (IMGUI_H_FILE_BROWSER *_fb) {
	TEXT *_txtDecomposed = _fb->decomposedPath;
	FOLDER_CONTENT *_fcBrower = _fb->buffer;
sys_marker("100");	
	// Retrieve the available width
	var _width = imgui_get_content_region_avail_width();
	
	// Init temporal variables
	str_cpy(strBrowserPath, ""); // will contain the full path of the path buttons
	var _column = 0; // current width of the shown path buttons
	
sys_marker("200");	
	// ----- CURRENT PATH BUTTONS -----
	// Loop through the decomposed current path
	int _i = 0;
	for(; _i<_txtDecomposed->skill_x; _i+=1) { // _txtDecomposed->skill_x contains the current count of members
		STRING *_strT = *(_txtDecomposed->pstring + _i);
		// If the root of the current path is a logical drive
		if(imgui_h_path_is_root_U(_strT->chars)) {
			imgui_push_item_width(24);
			// Set a combo with all the logical drives
			if (imgui_begin_combo("##DrivesCombo", _chrLogicalDrives[_fb->logicalDriveIndex], ImGuiComboFlags_NoArrowButton)) {
				int _ii=0;
				// Loop trough the _chrLogicalDrives array
				for(; _ii<_nLogicalDrivesCount; _ii+=1) {
					BOOL _isSelected = (_fb->logicalDriveIndex == _ii);
					if (imgui_selectable(_chrLogicalDrives[_ii], &_isSelected, NULL)) {
						str_cpy(strBrowserPath, _chrLogicalDrives[_ii]);
						str_cat(strBrowserPath, "\\"); // add a counter bar to the logical drive, otherway it will point to the drive mounting pòint
						if(imgui_h_get_folder_content_U("", strBrowserPath->chars, _fb->buffer, _fb->bufferSize, _fb->extensionFilter->chars, _fb->folderPrefix->chars)) {
							_fb->logicalDriveIndex = _ii; // set as current drive
							_buildDecomposedPath(_fb, _chrLogicalDrives[_ii]); // build the decomposed current path
						}
						_fb->selectedIndex = -1; // clear any selection in the files list
					}     
				}
				imgui_end_combo();
			} else {
				str_cpy(strBrowserPath, _chrLogicalDrives[_fb->logicalDriveIndex]); // copy the drive to the full path
			}
		} else { // if it is not a logical drive
sys_marker("300");	
			if(_i != 0) { // if it is not the first member of the decomposed path
				str_cat(strBrowserPath, "\\"); // add a counter bar to the full path
				if(_column < _width * 0.7) // if the new button fits in the same line
					imgui_same_line();
				else
					_column = 0;
			}
			str_cat(strBrowserPath, _strT); // copy the current decomposed member to the full path
			if(imgui_h_button(_strT->chars, 0, 0)) {
				// Set the new current path
				imgui_h_get_folder_content_U(_fcBrower->root, strBrowserPath->chars, _fb->buffer, _fb->bufferSize, _fb->extensionFilter->chars, _fb->folderPrefix->chars);
				_buildDecomposedPath(_fb, strBrowserPath->chars); // build the decomposed current path
				_fb->selectedIndex = -1; // clear any selection in the file list
			}
		}
		VECTOR _vPos;
		vec_set(&_vPos, imgui_get_item_rect_size());
		_column += _vPos.x; // add the last item width to the measuring variable
	}
	
sys_marker("400");	
	// ----- CURRENT PATH CONTENT LIST BOX -----
	// Set a list box with the result of 'imgui_h_get_folder_content_U'
	imgui_push_item_width(_width);
	if(imgui_list_box("##PathContentList", &_fb->selectedIndex, &_fcBrower->content, _fcBrower->count, 6)) {
		// Build the visible part of the new path
		str_cpy(strBrowserCurrent, _fcBrower->folder); // copy the current path to a temporary string
		if(*(strBrowserCurrent->chars + (int)str_len(strBrowserCurrent) - 1) != 92) // 92 -> counter slash bar
			str_cat(strBrowserCurrent, "\\"); // add an ending counter bar to the new path when it ends with another character 
		utf8 *_label = *(&_fcBrower->content + _fb->selectedIndex); // get a pointer to the name of the clicked member
		if(str_stri(_str(_label), _fb->folderPrefix) == 1) // if it starts with the injected folder icon 
			_label += (int)str_len(_fb->folderPrefix); // move the pointer forward
		str_cat(strBrowserCurrent, _label);// copy the label to the temporary string
		// Build the hidden part of the new path
		if(*_fcBrower->root == NULL) // if the previous path has no root path
			str_cpy(strBrowserPath, "");
		else {
			str_cpy(strBrowserPath, _fcBrower->root); // copy the current path to a temporary string
			if(*(strBrowserPath->chars + (int)str_len(strBrowserPath) - 1) != 92)
				str_cat(strBrowserPath, "\\"); // add an ending counter bar to the new path when it ends with another character 
		}
		// Build the full path
		str_cat(strBrowserPath, strBrowserCurrent); 
		
sys_marker("500");	
		if(imgui_h_path_is_folder_U(strBrowserPath->chars)) { // if the full path is a folder
			// Set the new current path
			imgui_h_get_folder_content_U(_fcBrower->root, strBrowserCurrent->chars, _fb->buffer, _fb->bufferSize, _fb->extensionFilter->chars, _fb->folderPrefix->chars);
			_buildDecomposedPath(_fb, strBrowserCurrent->chars); // build the decomposed current path
			_fb->selectedIndex = -1; // clear any selection in the files list
			
		} else {
//						printf("%s\nis not a folder", strBrowserLabel->chars);
		}
	}
	imgui_pop_item_width();
	
#ifdef _DEBUG_FILEBROWSER_DATA		
	imgui_text(_fcBrower->root);
	imgui_text(_fcBrower->folder);
	
	utf8 **_uT = &_fcBrower->content;
	utf8 **_uL = _uT + _fcBrower->count;
	for(; _uT<_uL; _uT+=1)
		imgui_text(*_uT);
#endif				
sys_marker("700");	
	
	return FALSE;
}

// ------------------------------------------------------------------------------------------

#define TEXT_BROWSER_COUNT        0
#define TEXT_BROWSER_ROOT         1
#define TEXT_BROWSER_FOLDER       2
#define TEXT_BROWSER_FILTER       3
#define TEXT_BROWSER_FOLDER_ICON  4
#define TEXT_BROWSER_FILE_ICON    5
#define TEXT_BROWSER_HEADER_SIZE  6

int imgui_h_text_get_selected (var _hdlText) {
//	TEXT *_txtT = ptr_for_handle(_hdlText);
//	int *_current = &_txtT->skill_x;
//	return *_current;
	return *((int*)(&((TEXT*)ptr_for_handle(_hdlText))->skill_x));
}

void imgui_h_text_get_full_path (var _hdlText, STRING *_strT) {
	TEXT *_txtT = ptr_for_handle(_hdlText);
	
	if(str_len(*(_txtT->pstring + TEXT_BROWSER_ROOT)) > 0) {
		str_cpy(_strT, *(_txtT->pstring + TEXT_BROWSER_ROOT));
		str_cat(_strT, "\\");
		str_cat(_strT, *(_txtT->pstring + TEXT_BROWSER_FOLDER));
	} else {
		str_cpy(_strT, *(_txtT->pstring + TEXT_BROWSER_FOLDER));
	}
	int *_current = &_txtT->skill_x;
	if(*_current >= 0) {
		str_cat(_strT, "\\");
		utf8 *_label = (*(_txtT->pstring + TEXT_BROWSER_HEADER_SIZE + *_current))->chars;
		if(str_stri(_str(_label), *(_txtT->pstring + TEXT_BROWSER_FILE_ICON)) == 1) // if the selected file starts with the injected file icon 
			_label += (int)str_len(*(_txtT->pstring + TEXT_BROWSER_FILE_ICON)); // move the pointer forward
		str_cat(_strT, _label);
	}
}

BOOL _label_for_text(TEXT *_txt, int _index, char **_label) {
	*_label = (_txt->pstring)[_index + TEXT_BROWSER_HEADER_SIZE]->chars;
	return TRUE;
}

int _findLogicalDriveText (STRING *_strT) {
	int _i = 0;
	for(; _i<_nLogicalDrivesCount; _i+=1) {
		if(str_stri(_strT, _str(_chrLogicalDrives[_i])) != 1) 
			continue;
		return _i;
	}
	return -1;
}

BOOL imgui_h_text_file_browser (var _hdlText) {
	
	TEXT *_txtT = ptr_for_handle(_hdlText);
	int *_current = (int*)(&_txtT->skill_x);
	
	// Retrieve the available width
	var _width = imgui_get_content_region_avail_width();
	
	int *_count = (_txtT->pstring)[TEXT_BROWSER_COUNT]->chars;
	int *_folderCount = _count + 1;
	int *_decomposedCount = _count + 2;
	
	// Init temporal variables
	str_cpy(strBrowserPath, ""); // will contain the full path of the path buttons
	var _column = 0; // current width of the shown path buttons
	
sys_marker("200");	
	// ----- CURRENT PATH BUTTONS -----
	// Loop through the decomposed current path
	int _i0 = TEXT_BROWSER_HEADER_SIZE + *_count + *_decomposedCount - 1;
	int _i = _i0;
	int _iL = _i0 - *_decomposedCount;
	for(; _i>_iL; _i-=1) { // _txtDecomposed->skill_x contains the current count of members
		STRING *_strT = *(_txtT->pstring + _i);
		// If the root of the current path is a logical drive
		if(imgui_h_path_is_root_U(_strT->chars)) {
			int _logicalDrive = _findLogicalDriveText(_strT);
			if(_logicalDrive >= 0) {
				imgui_push_item_width(imgui_get_font_size() * 2);
				// Set a combo with all the logical drives
				if (imgui_begin_combo("##DrivesCombo", _chrLogicalDrives[_logicalDrive], ImGuiComboFlags_NoArrowButton)) {
					int _ii=0;
					// Loop trough the _chrLogicalDrives array
					for(; _ii<_nLogicalDrivesCount; _ii+=1) {
						BOOL _isSelected = (_logicalDrive == _ii);
						if (imgui_selectable(_chrLogicalDrives[_ii], &_isSelected, NULL)) {
							// Build the current path string 
							str_cpy(strBrowserPath, _chrLogicalDrives[_ii]);
							str_cat(strBrowserPath, "\\");
							// Set the new current path
							imgui_h_get_folder_content_text(_hdlText, 
								*(_txtT->pstring + TEXT_BROWSER_ROOT), 
								strBrowserPath, 
								*(_txtT->pstring + TEXT_BROWSER_FILTER), 
								*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON), 
								*(_txtT->pstring + TEXT_BROWSER_FILE_ICON));
//							imgui_h_set_scroll_y(0);
							*_current = -1; // clear any selection in the files list
						}     
					}
					imgui_end_combo();
				} else {
					str_cpy(strBrowserPath, _strT); // copy the drive to the full path
				}
			}
		} else { // if it is not a logical drive
sys_marker("300");	
			if(_i != _i0) { // if it is not the first member of the decomposed path
				str_cat(strBrowserPath, "\\"); // add a counter bar to the full path
				if(_column < _width * 0.7) // if the new button fits in the same line
					imgui_same_line();
				else
					_column = 0;
			}
			str_cat(strBrowserPath, _strT); // copy the current decomposed member to the full path
			if(imgui_h_button(_strT->chars, 0, 0)) {
				// Set the new current path
				imgui_h_get_folder_content_text(_hdlText, 
					*(_txtT->pstring + TEXT_BROWSER_ROOT), 
					strBrowserPath, 
					*(_txtT->pstring + TEXT_BROWSER_FILTER), 
					*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON), 
					*(_txtT->pstring + TEXT_BROWSER_FILE_ICON));
//				imgui_h_set_scroll_y(0);
				*_current = -1; // clear any selection in the list box
			}
		}
		VECTOR _vPos;
		vec_set(&_vPos, imgui_get_item_rect_size());
		_column += _vPos.x; // add the last item width to the measuring variable
	}
	
	// ----- CURRENT PATH CONTENT LIST BOX -----
	// Set a list box with the result of 'imgui_h_get_folder_content_U_text'
	imgui_push_item_width(_width);
	
	if(imgui_h_getter_list_box ("##PathContentList", _current, _label_for_text, _txtT, *_count, 6, -1)) {
		
		// Build the visible part of the new path
		str_cpy(strBrowserCurrent, *(_txtT->pstring + 2)); // copy the current path to a temporary string
		if(*(strBrowserCurrent->chars + (int)str_len(strBrowserCurrent) - 1) != 92) // 92 -> counter slash bar
			str_cat(strBrowserCurrent, "\\"); // add an ending counter bar to the new path when it ends with another character 
//printf("%d", *_current)	
		utf8 *_label = (_txtT->pstring)[TEXT_BROWSER_HEADER_SIZE + *_current]->chars; // get a pointer to the name of the clicked member
		if(str_stri(_str(_label), *(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON)) == 1) // if it starts with the injected folder icon 
			_label += (int)str_len(*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON)); // move the pointer forward
		if(str_stri(_str(_label), *(_txtT->pstring + TEXT_BROWSER_FILE_ICON)) == 1) // if it starts with the injected file icon 
			_label += (int)str_len(*(_txtT->pstring + TEXT_BROWSER_FILE_ICON)); // move the pointer forward
		str_cat(strBrowserCurrent, _label);// copy the label to the temporary string
		
		
		// Build the hidden part of the new path
		if(str_len(*(_txtT->pstring + 1)) == 0) // if the previous path has no root path
			str_cpy(strBrowserPath, "");
		else {
			str_cpy(strBrowserPath, *(_txtT->pstring + 1)); // copy the current path to a temporary string
			if(*(strBrowserPath->chars + (int)str_len(strBrowserPath) - 1) != 92)
				str_cat(strBrowserPath, "\\"); // add an ending counter bar to the new path when it ends with another character 
		}
		
		// Build the full path
		str_cat(strBrowserPath, strBrowserCurrent); 
		
		if(imgui_h_path_is_folder_U(strBrowserPath->chars)) { // if the full path is a folder
			// Set the new current path
			imgui_h_get_folder_content_text(_hdlText, 
				*(_txtT->pstring + TEXT_BROWSER_ROOT), 
				strBrowserCurrent, 
				*(_txtT->pstring + TEXT_BROWSER_FILTER), 
				*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON), 
				*(_txtT->pstring + TEXT_BROWSER_FILE_ICON));
//			imgui_h_set_scroll_y(0);
			*_current = -1; // clear any selection in the files list
		} else {
//			printf("%s\nis not a folder", strBrowserLabel->chars);
		}
	}
	
	return FALSE;
}

BOOL imgui_h_text_folder_select (var _hdlText) {
	
	TEXT *_txtT = ptr_for_handle(_hdlText);
	int *_current = (int*)(&_txtT->skill_x);
	
	// Retrieve the available width
	var _width = imgui_get_content_region_avail_width();
	
	int *_count = (_txtT->pstring)[TEXT_BROWSER_COUNT]->chars;
	int *_folderCount = _count + 1;
	int *_decomposedCount = _count + 2;
	
	// Init temporal variables
	str_cpy(strBrowserPath, ""); // will contain the full path of the path buttons
	var _column = 0; // current width of the shown path buttons
	
	// ----- CURRENT PATH BUTTONS -----
	// Loop through the decomposed current path
	int _i0 = TEXT_BROWSER_HEADER_SIZE + *_count + *_decomposedCount - 1;
	int _i = _i0;
	int _iL = _i0 - *_decomposedCount;
	for(; _i>_iL; _i-=1) { // _txtDecomposed->skill_x contains the current count of members
		STRING *_strT = *(_txtT->pstring + _i);
		// If the root of the current path is a logical drive
		if(imgui_h_path_is_root_U(_strT->chars)) {
			int _logicalDrive = _findLogicalDriveText(_strT);
			if(_logicalDrive >= 0) {
				imgui_push_item_width(imgui_get_font_size() * 2);
				// Set a combo with all the logical drives
				if (imgui_begin_combo("##DrivesCombo", _chrLogicalDrives[_logicalDrive], ImGuiComboFlags_NoArrowButton)) {
					int _ii=0;
					// Loop trough the _chrLogicalDrives array
					for(; _ii<_nLogicalDrivesCount; _ii+=1) {
						BOOL _isSelected = (_logicalDrive == _ii);
						if (imgui_selectable(_chrLogicalDrives[_ii], &_isSelected, NULL)) {
							// Build the current path string 
							str_cpy(strBrowserPath, _chrLogicalDrives[_ii]);
							str_cat(strBrowserPath, "\\");
							// Set the new current path
							imgui_h_get_folder_content_text(_hdlText, 
								*(_txtT->pstring + TEXT_BROWSER_ROOT), 
								strBrowserPath, 
								*(_txtT->pstring + TEXT_BROWSER_FILTER), 
								*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON), 
								*(_txtT->pstring + TEXT_BROWSER_FILE_ICON));
//							imgui_h_set_scroll_y(0);
							*_current = -1; // clear any selection in the files list
						}     
					}
					imgui_end_combo();
				} else {
					str_cpy(strBrowserPath, _strT); // copy the drive to the full path
				}
			}
		} else { // if it is not a logical drive
			if(_i != _i0) { // if it is not the first member of the decomposed path
				str_cat(strBrowserPath, "\\"); // add a counter bar to the full path
				if(_column < _width * 0.7) // if the new button fits in the same line
					imgui_same_line();
				else
					_column = 0;
			}
			str_cat(strBrowserPath, _strT); // copy the current decomposed member to the full path
			if(imgui_h_button(_strT->chars, 0, 0)) {
				// Set the new current path
				imgui_h_get_folder_content_text(_hdlText, 
					*(_txtT->pstring + TEXT_BROWSER_ROOT), 
					strBrowserPath, 
					*(_txtT->pstring + TEXT_BROWSER_FILTER), 
					*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON), 
					*(_txtT->pstring + TEXT_BROWSER_FILE_ICON));
//				imgui_h_set_scroll_y(0);
				*_current = -1; // clear any selection in the list box
			}
		}
		VECTOR _vPos;
		vec_set(&_vPos, imgui_get_item_rect_size());
		_column += _vPos.x; // add the last item width to the measuring variable
	}
	
	// ----- CURRENT PATH CONTENT LIST BOX -----
	// Set a list box with the result of 'imgui_h_get_folder_content_U_text'
	imgui_push_item_width(_width);
	
	if(imgui_h_getter_list_box ("##PathContentList", _current, _label_for_text, _txtT, *_count, 6, -1)) {
		
		// Build the visible part of the new path
		str_cpy(strBrowserCurrent, *(_txtT->pstring + 2)); // copy the current path to a temporary string
		if(*(strBrowserCurrent->chars + (int)str_len(strBrowserCurrent) - 1) != 92) // 92 -> counter slash bar
			str_cat(strBrowserCurrent, "\\"); // add an ending counter bar to the new path when it ends with another character 
//printf("%d", *_current)	
		utf8 *_label = (_txtT->pstring)[TEXT_BROWSER_HEADER_SIZE + *_current]->chars; // get a pointer to the name of the clicked member
		if(str_stri(_str(_label), *(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON)) == 1) // if it starts with the injected folder icon 
			_label += (int)str_len(*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON)); // move the pointer forward
		if(str_stri(_str(_label), *(_txtT->pstring + TEXT_BROWSER_FILE_ICON)) == 1) // if it starts with the injected file icon 
			_label += (int)str_len(*(_txtT->pstring + TEXT_BROWSER_FILE_ICON)); // move the pointer forward
		str_cat(strBrowserCurrent, _label);// copy the label to the temporary string
		
		// Build the hidden part of the new path
		if(str_len(*(_txtT->pstring + 1)) == 0) // if the previous path has no root path
			str_cpy(strBrowserPath, "");
		else {
			str_cpy(strBrowserPath, *(_txtT->pstring + 1)); // copy the current path to a temporary string
			if(*(strBrowserPath->chars + (int)str_len(strBrowserPath) - 1) != 92)
				str_cat(strBrowserPath, "\\"); // add an ending counter bar to the new path when it ends with another character 
		}
		
		// Build the full path
		str_cat(strBrowserPath, strBrowserCurrent); 
		
		if(imgui_h_path_is_folder_U(strBrowserPath->chars)) { // if the full path is a folder
			// Set the new current path
			imgui_h_get_folder_content_text(_hdlText, 
				*(_txtT->pstring + TEXT_BROWSER_ROOT), 
				strBrowserCurrent, 
				*(_txtT->pstring + TEXT_BROWSER_FILTER), 
				*(_txtT->pstring + TEXT_BROWSER_FOLDER_ICON), 
				*(_txtT->pstring + TEXT_BROWSER_FILE_ICON));
//			imgui_h_set_scroll_y(0);
			*_current = -1; // clear any selection in the files list
		} else {
//			printf("%s\nis not a folder", strBrowserLabel->chars);
		}
			
	}
	
	
	return FALSE;
}



