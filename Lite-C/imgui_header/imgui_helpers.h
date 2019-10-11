
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

ImFont *         imgui_add_ttf_from_file(char *_chrFont, var _size, var _glyphRange);
ImFont *         imgui_add_ttf_from_file_ranged(char *_chrFont, var _size, var _glyphRangeID);

void             imgui_push_font(ImFont *_font);

void             imgui_pop_font();


// imgui_helpers.ccp

var              imgui_h_button(char *label, var sizeX, var sizeY);

var              imgui_h_calc_item_width();

// HELPERS
// ------------------------------------------------------------------------------------------------------

#define UNICODE_MAX                       520
#define FOLDER_ICON                       0xEEA081
#define FOLDER_ICON_SIZE                  3


utf8 *           imgui_h_unicode_to_utf8 (unicode *_uText, int _size);

long             imgui_h_get_logical_drives ();

utf8 *           imgui_h_get_current_path_U ();

void             imgui_h_set_current_path_U (utf8 *_uPath);

utf8 *           imgui_h_get_path_filename_U (utf8 *_uPath);

#define FOLDERID_Documents                0
#define FOLDERID_ProgramData              1
#define FOLDERID_LocalAppData             2
#define FOLDERID_SavedGames               3
#define FOLDERID_COUNT                    4

utf8 *           imgui_h_get_known_folder_path_U (var _id);

var              imgui_h_path_is_absolute_U (utf8 *_uPath);

var              imgui_h_path_is_root_U (utf8 *_uPath);

var              imgui_h_path_exists_U (utf8 *_uPath);

var              imgui_h_path_is_folder_U (utf8 *_uPath);

var              imgui_h_create_folder_U (utf8 *_uPath);

typedef struct FOLDER_CONTENT2 {
	utf8 root[UNICODE_MAX];
	utf8 folder[UNICODE_MAX];
	utf8 **content;
	int count;
	int capacity;
} FOLDER_CONTENT2;

typedef struct FOLDER_CONTENT {
	utf8 *root;
	utf8 *folder;
	int count;
	utf8 **content;
} FOLDER_CONTENT;

FOLDER_CONTENT * imgui_h_get_folder_content_U2 (FOLDER_CONTENT *_fc, utf8 *_root, utf8 *_folder);

void             imgui_h_remove_folder_content (FOLDER_CONTENT *_fc);





var              imgui_h_get_folder_content_U (utf8 *_root, utf8 *_folder, BYTE *_buffer, int _bufferSize, utf8 *_filter, utf8 *_folderInjection);


