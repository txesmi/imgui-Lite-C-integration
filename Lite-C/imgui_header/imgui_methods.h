var imgui_begin_popup_modals(char *name);
var imgui_begin_popup_modals_params(char *name, void *p_open, long flags);
void imgui_close_current_popup();
var imgui_begin_popup(char *label, long flags);
void imgui_end_popup();
var imgui_begin_popup_context_item(char *label);
void imgui_open_popup(char *str_id);
VECTOR* imgui_get_mouse_pos_on_opening_current_popup();

//imgui_dll_button.cpp
var imgui_button(char *label);
var imgui_button_color(char *label, VECTOR *normal, VECTOR *hover, VECTOR *pushed);
var imgui_button_img(char *label, BMAP *input, var width, var height);
var imgui_button_img_params(char *label, BMAP *input, var width, var height, var u1, var v1, var u2, var v2, var padding, COLOR4 *background, COLOR4 *tint);
var imgui_checkbox(char *label, void *state);
var imgui_checkbox_flag(char *label, long *flags, long flag);
var imgui_radiobutton(char *label, int *result_pointer, int value);
void imgui_push_button_repeat(int value);
void imgui_pop_button_repeat();
var imgui_arrow_button(char *str_id, long dir);
var imgui_invisible_button(char *str_id, VECTOR *size);
var imgui_radio_button(char *label, int* v, int v_button);
var imgui_small_button(char *label);
var imgui_image_button(BMAP *input, VECTOR *size, VECTOR *uv0, VECTOR *uv1, int frame_padding, VECTOR* bg_col, var bg_alpha, VECTOR *tint_color, var tint_alpha );
var imgui_image_button_tex_id(void *my_tex_id, VECTOR *size, VECTOR *uv0, VECTOR *uv1, int frame_padding, VECTOR* bg_col, var bg_alpha, VECTOR *tint_color, var tint_alpha );

//imgui_dll_draw.cpp
void* imgui_get_window_draw_list();
void imgui_drawlist_add_line(void *draw_list, var xa, var ya, var xb, var yb, COLOR4 *color, var thickness);
void imgui_drawlist_channel_split(void *draw_list, int channels_count);
void imgui_drawlist_channel_set_current(void *draw_list, int channel_index);
void imgui_drawlist_add_bezier_curve(void *draw_list, VECTOR *pos0, VECTOR *cp0, VECTOR *cp1, VECTOR *pos1, VECTOR *color, var thickness, int num_segments);
void imgui_drawlist_channel_merge(void *draw_list);
void imgui_drawlist_add_rect_filled(void *draw_list, VECTOR *a, VECTOR *b, VECTOR *color, var rounding);
void imgui_drawlist_add_rect(void *draw_list, VECTOR *a, VECTOR *b, VECTOR *color, var rounding);
void imgui_drawlist_add_circle_filled(void *draw_list, VECTOR* center, var radius, VECTOR *color);
void imgui_drawlist_add_triangle_filled(void *draw_list, VECTOR *a, VECTOR *b, VECTOR *c, COLOR4 *color);
void imgui_drawlist_add_triangle(void *draw_list, VECTOR *a, VECTOR *b, VECTOR *c, COLOR4 *color);
void imgui_drawlist_add_text(void *draw_list, VECTOR *pos, COLOR4 *color, char *text_begin);
void imgui_drawlist_add_polygon(void *draw_list, POINT **points, int num_points, COLOR4 *col, int closed, var thickness);
void imgui_drawlist_add_image(void *draw_list, BMAP *input, VECTOR *a, VECTOR *b, VECTOR *uv_a, VECTOR *uv_b, VECTOR *tint_color, var tint_alpha);

//imgui_dll_layout.cpp
var imgui_font_allow_user_scaling();
void imgui_indent(var indent_w);
void imgui_unindent(var indent_w);
var imgui_get_font_size();
void imgui_push_item_width(var item_width);
void imgui_same_line();
void imgui_same_line_params(var local_pos_x, var spacing_w);
void imgui_push_style_color(long type, COLOR4 *color);
void imgui_push_style_color_hsv(int type, float h, float s, float v, float a);
void imgui_pop_style_color(int count);
void imgui_align_text_to_frame_padding();
void imgui_separator();
void imgui_push_style_var(int idx, var val);
void imgui_push_style_vec(int idx, VECTOR *val);
void imgui_pop_style_var(int count);
void imgui_columns(int count, char *id, int border);
void imgui_next_column();
void imgui_pop_item_width();
void imgui_set_column_offset(int column_index, var offset_x);
void imgui_set_column_width(int column_index, var width);
void imgui_spacing();
VECTOR* imgui_get_style_item_inner_spacing();
VECTOR* imgui_get_style_item_spacing();
VECTOR* imgui_get_item_rect_min();
VECTOR* imgui_get_item_rect_max();
void imgui_new_line();

//imgui_dll_window.cpp
void imgui_set_next_window_pos(var x, var y, long condition);
void imgui_set_next_window_size(var width, var height, long condition);
var imgui_get_window_width();
var imgui_get_window_height();
var imgui_get_window_x();
var imgui_get_window_y();
var imgui_begin(char *name, void *p_close, long flags);
void imgui_end();
var imgui_get_frame_height_with_spacing();
void imgui_begin_child(char *id, VECTOR *size, int border, long flags);
void imgui_end_child();
var imgui_get_content_region_avail_width();
VECTOR* imgui_get_window_size();

//imgui_dll_lists.cpp
var imgui_listbox_header(char *label, int items_count, int height_in_items);
void imgui_listbox_footer();
int imgui_selectable(char *label, int *p_selected, int flags);
var imgui_selectable_direct(char *label, int p_selected);
var imgui_selectable_simple(char *label);
int imgui_selectable_size(char *label, int *p_selected, int flags, VECTOR *size);
var imgui_combo(char *label, int* current_item, char *items_separated_by_zeros, int popup_max_height_in_items);
var imgui_begin_combo(char *label, char *preview_value, long flags);
void imgui_end_combo();
var imgui_list_box(char *label, int *current_item, char **items, int items_count, int height_in_items);

//imgui_dll_sliders.cpp
var imgui_slider_int(char *label, int *v, int v_min, int v_max);
var imgui_slider_float(char *label, float *v, float v_min, float v_max, char *format, float power);
var imgui_slider_angle(char *label, float *v_rad, float v_degrees_min, float v_degrees_max, char *format);
var imgui_slider_var(char *label, var *v, var v_min, var v_max);
var imgui_slider_angle_var(char *label, var *v_rad, var v_degrees_min, var v_degrees_max);
var imgui_slider_int2(char *label, int *v, int v_min, int v_max);
var imgui_slider_int3(char *label, int *v, int v_min, int v_max);
var imgui_slider_int4(char *label, int *v, int v_min, int v_max);
var imgui_slider_float2(char *label, float *v, float v_min, float v_max, char *format, float power);
var imgui_slider_float3(char *label, float *v, float v_min, float v_max, char *format, float power);
var imgui_slider_float4(char *label, float *v, float v_min, float v_max, char *format, float power);
var imgui_slider_var2(char *label, var *v, var v_min, var v_max);
var imgui_slider_var3(char *label, var *v, var v_min, var v_max);
var imgui_slider_var4(char *label, var *v, var v_min, var v_max);
var imgui_slider_vector(char *label, VECTOR *v, var v_min, var v_max);

//imgui_dll_image.cpp
void imgui_image(BMAP *input);
void imgui_image_scale(BMAP *input, VECTOR *size, VECTOR *uv_start, VECTOR *uv_end);
void imgui_image_scale_color(BMAP *input, VECTOR *size, VECTOR *uv_start, VECTOR *uv_end, VECTOR* tint_color, var tint_alpha, VECTOR *border_col, var border_alpha);
void imgui_image_scale_color_tex_id(void *my_tex_id, VECTOR *size, VECTOR *uv_start, VECTOR *uv_end, VECTOR* tint_color, var tint_alpha, VECTOR *border_col, var border_alpha);

//imgui_dll_plots.cpp
void imgui_plot_lines(char *label, float *values, int values_count, float values_offset, char *overlay_text, float scale_min, float scale_max, VECTOR *graph_size);
void imgui_plotlines(char *label, float *values, int values_count);
void imgui_plot_histogram(char *label, float *values, int values_count, int values_offset, char *overlay_text, float scale_min, float scale_max, VECTOR *graph_size, int stride);
void imgui_progress_bar(var fraction, VECTOR *size_arg, char *overlay);

//imgui_dll_text.cpp
void imgui_text(char *fmt);
void imgui_text_disabled(char *fmt);
void imgui_push_text_wrap_pos(var wrap_local_pos_x);
void imgui_pop_text_wrap_pos();
void imgui_text_unformatted(char *text);
void imgui_bullet_text(char *fmt);
void imgui_text_wrapped(char *fmt);
void imgui_label_text(char *label, char *fmt);
void imgui_bullet();
void imgui_text_colored(var r, var g, var b, var a, char* fmt);

//imgui_dll_grouping.cpp
var imgui_collapsing_header(char *label, void *p_open, long flags);
void imgui_tree_pop();
void imgui_begin_group();
void imgui_end_group();
var imgui_tree_node(char *label);
var imgui_treenode(void *ptr_id, char *fmt);
var imgui_get_tree_node_to_label_spacing();
int imgui_tree_node_ex(void *ptr_id, int flags, char *fmt);
int imgui_begin_drag_drop_target();
void imgui_end_drag_drop_target();
void* imgui_accept_drag_drop_payload(char *type, int flags);
void* imgui_drag_drop_payload_data(void *pl);
int imgui_begin_tabbar(char* str_id, long flags);
void imgui_end_tabbar();
int imgui_begin_tabitem(char* label,void* state,long flags);
void imgui_end_tabitem();

//imgui_dll_main.cpp
void imgui_ext_strcpy(char *dest, char* source);
void imgui_start_imode();
void imgui_end_imode();

//imgui_dll_inputs.cpp
var imgui_input_text(char *label, char *buffer, int buffer_size, long flags);
var imgui_input_text_with_hint(char *label, char *hint, char *buf, int buf_size, long flags);
var imgui_input_text_multiline(char *label, char *buf, int buf_size, int width, int height, long flags);
var imgui_input_double(char *label, double *v, double step, double step_fast, char *format, long flags);
var imgui_input_int(char *label, int *v, int step, int step_fast, long flags);
var imgui_input_float(char *label, float *v, float step, float step_fast, int decimal_precision, long flags);
var imgui_input_float_format(char *label, float *v, float step, float step_fast, char *format, long flags);
var imgui_input_var(char *label, var *v, var step, var step_fast, long flags);
var imgui_input_int2(char *label, int *v, long flags);
var imgui_input_int3(char *label, int *v, long flags);
var imgui_input_int4(char *label, int *v, long flags);
var imgui_input_float2(char *label, float *v, int decimal_precision, long flags);
var imgui_input_float3(char *label, float *v, int decimal_precision, long flags);
var imgui_input_float4(char *label, float *v, int decimal_precision, long flags);
var imgui_input_var2(char *label, var *v, int decimal_precision, long flags);
var imgui_input_var3(char *label, var *v, int decimal_precision, long flags);
var imgui_input_var4(char *label, var *v, int decimal_precision, long flags);
var imgui_input_vector(char *label, VECTOR *v, int decimal_precision, long flags);
var imgui_drag_int(char *label, int *v, float v_speed, int v_min, int v_max, char *format);
var imgui_drag_float(char *label, float *v, float v_speed, float v_min, float v_max, char *format, float power);
var imgui_drag_float4(char *label, float *v, float v_speed, float v_min, float v_max, char *format, float power);
int imgui_drag_float_range2(char *label, float *v_current_min, float *v_current_max, var v_speed, var v_min, var v_max, char* format, char *format_max, var power);
int imgui_drag_int_range2(char *label, int *v_current_min, int *v_current_max, var v_speed, var v_min, var v_max, char* format, char *format_max);

//imgui_dll_system.cpp
var imgui_is_item_active();
var imgui_is_anytime_active();
VECTOR* imgui_get_item_rect_size();
int imgui_is_item_hovered();
void imgui_push_id(var id);
void imgui_pop_id();
var imgui_is_mouse_clicked(var btn);
VECTOR* imgui_get_cursor_screen_pos();
void imgui_set_cursor_screen_pos(VECTOR *pos);
var imgui_is_any_item_active();
VECTOR* imgui_get_io_mouse_delta();
var imgui_is_mouse_dragging(int btn, var lock_threshold);
var imgui_is_window_hovered();
var imgui_is_any_item_hoverd();
var imgui_is_mouse_hovering_window();
int* imgui_get_io_config_flags();
var imgui_get_time();
var imgui_is_key_pressed(int user_key_index, int repeat);
int imgui_get_key_index(int imgui_key);
int* imgui_get_io_config_input_text_cursor_blink();
int* imgui_get_io_config_windows_resize_from_edges();
int* imgui_get_io_config_windows_move_from_title_bar_only();
int* imgui_get_io_mouse_draw_cursor();
int* imgui_get_io_backend_flags();
void imgui_log_buttons();
void imgui_log_to_clipboard();
void imgui_log_text(char *fmt);
void imgui_log_finish();
var imgui_is_item_clicked(int mouse_button);
int imgui_get_io_key_ctrl();
var imgui_get_text_line_height();
VECTOR* imgui_get_cursor_pos();
void* imgui_get_io_fonts_tex_id();
int imgui_get_io_fonts_tex_width();
int imgui_get_io_fonts_tex_height();
VECTOR* imgui_get_io_mouse_pos();
void imgui_set_item_default_focus();
int imgui_is_mouse_double_clicked(int button);
var imgui_get_io_delta_time();
void imgui_color_convert_HSVto_RGB(var h, var s, var v, var *out_r, var *out_g, var *out_b);

//imgui_dll_menu.cpp
var imgui_begin_menu_bar();
void imgui_end_menu_bar();
var imgui_begin_menu(char *label, int enabled);
void imgui_end_menu();
var imgui_menu_item(char *label, char *shortcut, int selected, int enabled);

//imgui_dll_combi.cpp
var imgui_color_edit3(char *label, float *col, long flags);
var imgui_color_edit4(char *label, float *col, long flags);
void imgui_show_metrics_window(int *p_open);
void imgui_show_style_editor();
void imgui_show_user_guide();
void imgui_show_about_window(int *p_open);
int imgui_color_button(char *desc_id, float *col, int flags, VECTOR *size);
int imgui_color_picker_4(char *label, float *col, int flags, float *ref_col);
void imgui_set_color_edit_options(int flags);

//imgui_dll_tooltip.cpp
void imgui_set_tooltip(char *tooltip);
void imgui_begin_tooltip();
void imgui_end_tooltip();


void imgui_init(long config_flags);
void imgui_reset();
long custom_scan_message(UINT message, WPARAM wParam, LPARAM lParam);
