#ifndef _IMGUI_HELPERS_H_
#define _IMGUI_HELPERS_H_

typedef void ImFont;
typedef char utf8;
typedef short unicode;

// IMGUI
// ------------------------------------------------------------------------------------------------------

// imgui_fonts.ccp

#define GLYPH_RANGE_Default               0
#define GLYPH_RANGE_ChineseFull           1
#define GLYPH_RANGE_ChineseSimplified     2
#define GLYPH_RANGE_Cyrillic              3
#define GLYPH_RANGE_Japanese              4
#define GLYPH_RANGE_Korean                5
#define GLYPH_RANGE_Thai                  6
#define GLYPH_RANGE_COUNT                 7

// Add a font with an ImGui standard glyph range
ImFont *         imgui_add_ttf_from_file(char *_chrFont, var _size, var _glyphRange);

void             imgui_push_font(ImFont *_font);

void             imgui_pop_font();

// choppy custom glyph range implemented for test purpouses
//ImFont *         imgui_add_ttf_from_file_ranged(char *_chrFont, var _size, var _glyphRangeID, var _offX, var _offY);
ImFont *         imgui_add_ttf_from_file_ranged(char *_chrFont, var _size, short *_ranges);


// imgui_helpers.ccp

var              imgui_h_button(char *label, var sizeX, var sizeY);

var              imgui_h_button_unactive(char *label, var sizeX, var sizeY);

var              imgui_h_calc_item_width();


var              imgui_h_getter_list_box (char* label, int* current_item, void *items_getter, void* data, int items_count, int height_in_items, int scrool_to_item);


var              imgui_h_get_scroll_y();

var              imgui_h_set_scroll_y(var _pos);


// HELPERS
// ------------------------------------------------------------------------------------------------------

#define UNICODE_MAX                       520 // (MAX_PATH * 2)

// Convert an unicode character set to an UTF8 encoded one
utf8 *           imgui_h_unicode_to_utf8 (unicode *_wText);

// Convert an UTF8 character set to an unicode encoded one
unicode *        imgui_h_unicode_for_utf8 (utf8 *_uText);

// An straight implementation of WINAPI GetLogicalDrives() which crashes on engines implementation XP
long             imgui_h_get_logical_drives ();

// Get the program execution path in UTF8
utf8 *           imgui_h_get_current_path_U ();

// Set the current execution path
void             imgui_h_set_current_path_U (utf8 *_uPath);

// Get the last member of a path
utf8 *           imgui_h_get_path_filename_U (utf8 *_uPath);

#define FOLDERID_Documents                0
#define FOLDERID_ProgramData              1
#define FOLDERID_LocalAppData             2
#define FOLDERID_SavedGames               3
#define FOLDERID_Games                    4
#define FOLDERID_COUNT                    5

// An indexed implementation of WINAPI SHGetKnownFolderPath
utf8 *           imgui_h_get_known_folder_path_U (var _id);

// Check if a path is absolute
var              imgui_h_path_is_absolute_U (utf8 *_uPath);

// Check if a path is a drive root
var              imgui_h_path_is_root_U (utf8 *_uPath);

// Check if a path exists
var              imgui_h_path_exists_U (utf8 *_uPath);

// Check if a path is a folder
var              imgui_h_path_is_folder_U (utf8 *_uPath);

// Create a folder
var              imgui_h_create_folder_U (utf8 *_uPath);

// ------------------------------------------------------------------------------------------------------

typedef struct FOLDER_CONTENT {
	utf8 *root;
	utf8 *folder;
	int count;
	int folderCount;
	utf8 *content;
} FOLDER_CONTENT;

// Get the content of a folder, formatted to be used in a imgui list box
var              imgui_h_get_folder_content_U (utf8 *_root, utf8 *_folder, BYTE *_buffer, int _bufferSize, utf8 *_filter, utf8 *_folderInjection);




// ------------------------------------------------------------------------------------------------------

var              imgui_h_get_folder_content_text (var _hdlText, STRING *_uRoot, STRING *_uFolder, STRING *_filter, STRING *_folderInjection, STRING *_fileInjection);

void             imgui_h_folder_content_text_remove (var _hdlText);


#endif