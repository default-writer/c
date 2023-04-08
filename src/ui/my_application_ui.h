#include <array>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <slint.h>

class TodoItem {
    public:
    slint::SharedString title;
    bool checked;
    friend inline auto operator== (const class TodoItem &a, const class TodoItem &b) -> bool = default;
};

class MainWindow;

class Component_image_26 {
    public:
    slint::cbindgen_private::ComponentWeak self_weak;
    const MainWindow * root;
    uintptr_t tree_index_of_first_child;
    uintptr_t tree_index;
    class Button_root_23 const * parent;
    slint::cbindgen_private::ImageItem image_26 = {};
    inline auto init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child,class Button_root_23 const * parent) -> void;
    inline auto user_init () -> void;
    inline auto layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo;
    inline auto accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole;
    inline auto accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString;
    private:
    static inline auto visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t;
    static inline auto get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef;
    static inline auto get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange;
    static inline auto get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void;
    static inline auto get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void;
    static inline auto subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t;
    static inline auto item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto item_array () -> const slint::private_api::ItemArray;
    static inline auto layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo;
    static inline auto accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole;
    static inline auto accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void;
    public:
    static const slint::private_api::ComponentVTable static_vtable;
    static inline auto create (class Button_root_23 const * parent) -> slint::ComponentHandle<Component_image_26>;
    inline ~Component_image_26 ();
    inline auto update_data ([[maybe_unused]] int i, [[maybe_unused]] const int &data) const -> void;
    inline auto init () -> void;
    inline auto box_layout_data (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::BoxLayoutCellData;
    friend class vtable::VRc<slint::private_api::ComponentVTable, Component_image_26>;
};

class Button_root_23 {
    public:
    slint::cbindgen_private::ComponentWeak self_weak;
    const MainWindow * root;
    uintptr_t tree_index_of_first_child;
    uintptr_t tree_index;
    slint::private_api::Property<bool> root_23_checkable;
    slint::private_api::Property<bool> root_23_checked;
    slint::private_api::Callback<void()> root_23_clicked;
    slint::private_api::Property<slint::Image> root_23_icon;
    slint::private_api::Property<slint::SharedVector<float>> root_23_l_25_layout_cache;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_23_l_25_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_23_l_25_layoutinfo_v;
    slint::cbindgen_private::Empty root_23 = {};
    slint::cbindgen_private::BorderRectangle rectangle_24 = {};
    slint::cbindgen_private::Text text_28 = {};
    slint::cbindgen_private::TouchArea touch_29 = {};
    slint::cbindgen_private::FocusScope fs_30 = {};
    slint::cbindgen_private::BorderRectangle rectangle_31 = {};
    slint::private_api::Repeater<class Component_image_26, int> repeater_0;
    inline auto init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void;
    inline auto user_init () -> void;
    inline auto layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo;
    inline auto accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole;
    inline auto accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString;
    inline auto visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t;
    inline auto subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange;
    inline auto subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void;
};

class Component_path_36 {
    public:
    slint::cbindgen_private::ComponentWeak self_weak;
    const MainWindow * root;
    uintptr_t tree_index_of_first_child;
    uintptr_t tree_index;
    class CheckBox_root_32 const * parent;
    slint::cbindgen_private::Path path_36 = {};
    inline auto init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child,class CheckBox_root_32 const * parent) -> void;
    inline auto user_init () -> void;
    inline auto layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo;
    inline auto accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole;
    inline auto accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString;
    private:
    static inline auto visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t;
    static inline auto get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef;
    static inline auto get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange;
    static inline auto get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void;
    static inline auto get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void;
    static inline auto subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t;
    static inline auto item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto item_array () -> const slint::private_api::ItemArray;
    static inline auto layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo;
    static inline auto accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole;
    static inline auto accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void;
    public:
    static const slint::private_api::ComponentVTable static_vtable;
    static inline auto create (class CheckBox_root_32 const * parent) -> slint::ComponentHandle<Component_path_36>;
    inline ~Component_path_36 ();
    inline auto update_data ([[maybe_unused]] int i, [[maybe_unused]] const int &data) const -> void;
    inline auto init () -> void;
    inline auto box_layout_data (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::BoxLayoutCellData;
    friend class vtable::VRc<slint::private_api::ComponentVTable, Component_path_36>;
};

class CheckBox_root_32 {
    public:
    slint::cbindgen_private::ComponentWeak self_weak;
    const MainWindow * root;
    uintptr_t tree_index_of_first_child;
    uintptr_t tree_index;
    slint::private_api::Property<bool> root_32_accessible_checked;
    slint::private_api::Property<float> root_32_empty_33_height;
    slint::private_api::Property<slint::SharedVector<float>> root_32_empty_33_layout_cache;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_32_empty_33_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_32_empty_33_layoutinfo_v;
    slint::private_api::Property<slint::SharedVector<float>> root_32_empty_34_layout_cache;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_32_empty_34_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_32_empty_34_layoutinfo_v;
    slint::private_api::Callback<void()> root_32_toggled;
    slint::cbindgen_private::Empty root_32 = {};
    slint::cbindgen_private::Empty empty_34 = {};
    slint::cbindgen_private::BorderRectangle rectangle_35 = {};
    slint::cbindgen_private::Text text_38 = {};
    slint::cbindgen_private::TouchArea touch_39 = {};
    slint::cbindgen_private::FocusScope fs_40 = {};
    slint::cbindgen_private::BorderRectangle rectangle_41 = {};
    slint::private_api::Repeater<class Component_path_36, int> repeater_0;
    inline auto init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void;
    inline auto user_init () -> void;
    inline auto layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo;
    inline auto accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole;
    inline auto accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString;
    inline auto visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t;
    inline auto subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange;
    inline auto subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void;
};

class ScrollBar_root_42 {
    public:
    slint::cbindgen_private::ComponentWeak self_weak;
    const MainWindow * root;
    uintptr_t tree_index_of_first_child;
    uintptr_t tree_index;
    slint::private_api::Property<bool> root_42_enabled;
    slint::private_api::Property<bool> root_42_horizontal;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_42_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_42_layoutinfo_v;
    slint::private_api::Property<float> root_42_maximum;
    slint::private_api::Property<float> root_42_page_size;
    slint::private_api::Property<float> root_42_touch_area_44_horizontal_stretch;
    slint::private_api::Property<float> root_42_touch_area_44_max_height;
    slint::private_api::Property<float> root_42_touch_area_44_max_width;
    slint::private_api::Property<float> root_42_touch_area_44_min_height;
    slint::private_api::Property<float> root_42_touch_area_44_min_width;
    slint::private_api::Property<float> root_42_touch_area_44_preferred_height;
    slint::private_api::Property<float> root_42_touch_area_44_preferred_width;
    slint::private_api::Property<float> root_42_touch_area_44_pressed_value;
    slint::private_api::Property<float> root_42_touch_area_44_vertical_stretch;
    slint::private_api::Property<float> root_42_value;
    slint::cbindgen_private::BorderRectangle root_42 = {};
    slint::cbindgen_private::BorderRectangle handle_43 = {};
    slint::cbindgen_private::TouchArea touch_area_44 = {};
    inline auto init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void;
    inline auto user_init () -> void;
    inline auto layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo;
    inline auto accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole;
    inline auto accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString;
};

class ColorSchemeSelector_45 {
    public:
    slint::private_api::Property<bool> dark_color_scheme;
    inline ColorSchemeSelector_45 (const MainWindow * root);
    private:
    const MainWindow * root;
};

class Palette_46 {
    public:
    slint::private_api::Property<slint::Color> neutralPrimary;
    slint::private_api::Property<slint::Color> neutralTertiary;
    slint::private_api::Property<slint::Color> white;
    inline Palette_46 (const MainWindow * root);
    private:
    const MainWindow * root;
};

class StyleMetrics_47 {
    public:
    inline StyleMetrics_47 (const MainWindow * root);
    private:
    const MainWindow * root;
};

class Component_rectangle_16 {
    public:
    slint::cbindgen_private::ComponentWeak self_weak;
    const MainWindow * root;
    uintptr_t tree_index_of_first_child;
    uintptr_t tree_index;
    class MainWindow const * parent;
    slint::private_api::Property<TodoItem> model_data;
    slint::private_api::Property<int> model_index;
    slint::private_api::Property<slint::SharedVector<float>> rectangle_16_empty_17_layout_cache_h;
    slint::private_api::Property<slint::SharedVector<float>> rectangle_16_empty_17_layout_cache_v;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> rectangle_16_empty_17_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> rectangle_16_empty_17_layoutinfo_v;
    CheckBox_root_32 checkbox_18;
    slint::cbindgen_private::Empty rectangle_16 = {};
    inline auto init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child,class MainWindow const * parent) -> void;
    inline auto user_init () -> void;
    inline auto layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo;
    inline auto accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole;
    inline auto accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString;
    inline auto visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t;
    inline auto subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange;
    inline auto subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void;
    private:
    static inline auto visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t;
    static inline auto get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef;
    static inline auto get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange;
    static inline auto get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void;
    static inline auto get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void;
    static inline auto subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t;
    static inline auto item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto item_array () -> const slint::private_api::ItemArray;
    static inline auto layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo;
    static inline auto accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole;
    static inline auto accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void;
    public:
    static const slint::private_api::ComponentVTable static_vtable;
    static inline auto create (class MainWindow const * parent) -> slint::ComponentHandle<Component_rectangle_16>;
    inline ~Component_rectangle_16 ();
    inline auto update_data ([[maybe_unused]] int i, [[maybe_unused]] const TodoItem &data) const -> void;
    inline auto init () -> void;
    inline auto listview_layout (float *offset_y, const slint::private_api::Property<float> *viewport_width) const -> void;
    friend class vtable::VRc<slint::private_api::ComponentVTable, Component_rectangle_16>;
};

class MainWindow {
    public:
    slint::Window m_window = slint::Window{slint::private_api::WindowAdapterRc()};
    slint::cbindgen_private::ComponentWeak self_weak;
    private:
    const MainWindow * root;
    uintptr_t tree_index_of_first_child;
    uintptr_t tree_index;
    slint::private_api::Property<slint::SharedVector<float>> root_window_1_empty_3_layout_cache_h;
    slint::private_api::Property<slint::SharedVector<float>> root_window_1_empty_3_layout_cache_v;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_empty_3_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_empty_3_layoutinfo_v;
    slint::private_api::Callback<void(slint::SharedString)> root_window_1_inner_7_accepted;
    slint::private_api::Callback<void(slint::SharedString)> root_window_1_inner_7_edited;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_inner_7_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_inner_7_layoutinfo_v;
    slint::private_api::Property<float> root_window_1_input_10_computed_x;
    slint::private_api::Property<float> root_window_1_input_10_preferred_height;
    slint::private_api::Property<float> root_window_1_input_10_preferred_width;
    slint::private_api::Property<slint::SharedVector<float>> root_window_1_l_6_layout_cache;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_l_6_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_l_6_layoutinfo_v;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_layoutinfo_h;
    slint::private_api::Property<slint::cbindgen_private::LayoutInfo> root_window_1_layoutinfo_v;
    slint::private_api::Property<float> root_window_1_placeholder_9_horizontal_stretch;
    slint::private_api::Property<float> root_window_1_placeholder_9_max_height;
    slint::private_api::Property<float> root_window_1_placeholder_9_max_width;
    slint::private_api::Property<float> root_window_1_placeholder_9_min_height;
    slint::private_api::Property<float> root_window_1_placeholder_9_min_width;
    slint::private_api::Property<float> root_window_1_placeholder_9_preferred_height;
    slint::private_api::Property<float> root_window_1_placeholder_9_preferred_width;
    slint::private_api::Property<float> root_window_1_placeholder_9_vertical_stretch;
    slint::private_api::Callback<void(slint::SharedString)> root_window_1_todo_added;
    Button_root_23 button_11;
    ScrollBar_root_42 vbar_20;
    ScrollBar_root_42 hbar_21;
    slint::cbindgen_private::WindowItem root_window_1 = {};
    slint::cbindgen_private::Empty root_2 = {};
    slint::cbindgen_private::Empty text_edit_4 = {};
    slint::cbindgen_private::BorderRectangle rectangle_5 = {};
    slint::cbindgen_private::Empty inner_7 = {};
    slint::cbindgen_private::Clip inner_clip_8 = {};
    slint::cbindgen_private::Text placeholder_9 = {};
    slint::cbindgen_private::TextInput input_10 = {};
    slint::cbindgen_private::Empty list_view_12 = {};
    slint::cbindgen_private::BorderRectangle rectangle_13 = {};
    slint::cbindgen_private::Flickable fli_14 = {};
    slint::cbindgen_private::Rectangle corner_22 = {};
    slint::private_api::Repeater<class Component_rectangle_16, TodoItem> repeater_0;
    inline auto init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void;
    inline auto user_init () -> void;
    inline auto layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo;
    inline auto accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole;
    inline auto accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString;
    inline auto visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t;
    inline auto subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange;
    inline auto subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void;
    static inline auto visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t;
    static inline auto get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef;
    static inline auto get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange;
    static inline auto get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void;
    static inline auto get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void;
    static inline auto subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t;
    static inline auto item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>;
    static inline auto item_array () -> const slint::private_api::ItemArray;
    static inline auto layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo;
    static inline auto accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole;
    static inline auto accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void;
    public:
    static const slint::private_api::ComponentVTable static_vtable;
    static inline auto create () -> slint::ComponentHandle<MainWindow>;
    inline ~MainWindow ();
    inline auto invoke_todo_added (slint::SharedString arg_0) const -> void;
    template<std::invocable<slint::SharedString> Functor> inline auto on_todo_added (Functor && callback_handler) const;
    private:
    inline auto get_todo_model () const = delete /* the property 'todo-model' is declared as private. Declare it as 'in', 'out', or 'in-out' to make it public */;
    inline auto set_todo_model (const auto &) const = delete /* property 'todo-model' is declared as private. Declare it as 'in' or 'in-out' to make it public */;
    public:
    inline auto show ();
    inline auto hide ();
    inline auto window () const -> slint::Window&;
    inline auto run ();
    private:
    std::shared_ptr<ColorSchemeSelector_45> global_ColorSchemeSelector_45 = std::make_shared<ColorSchemeSelector_45>(this);
    std::shared_ptr<Palette_46> global_Palette_46 = std::make_shared<Palette_46>(this);
    std::shared_ptr<StyleMetrics_47> global_StyleMetrics_47 = std::make_shared<StyleMetrics_47>(this);
    friend class vtable::VRc<slint::private_api::ComponentVTable, MainWindow>;
    friend class Component_rectangle_16;
    friend class slint::private_api::WindowAdapterRc;
    friend class Component_rectangle_16;
    friend class Button_root_23;
    friend class Component_image_26;
    friend class CheckBox_root_32;
    friend class Component_path_36;
    friend class ScrollBar_root_42;
    friend class ColorSchemeSelector_45;
    friend class Palette_46;
    friend class StyleMetrics_47;
};

inline const slint::private_api::ComponentVTable Component_image_26::static_vtable = { visit_children, get_item_ref, get_subtree_range, get_subtree_component, get_item_tree, parent_node, subtree_index, layout_info, accessible_role, accessible_string_property, slint::private_api::drop_in_place<Component_image_26>, slint::private_api::dealloc };

inline auto Component_image_26::init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child,class Button_root_23 const * parent) -> void{
    auto self = this;
    self->self_weak = enclosing_component;
    self->root = root;
    this->tree_index_of_first_child = tree_index_of_first_child;
    self->tree_index = tree_index;
    self->parent = parent;
    slint::private_api::Property<slint::Image>::link_two_way(&self->image_26.source, &self->parent->root_23_icon);
    self->image_26.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->parent->root_23.height.get() - 3) - 3);
                        });
    self->image_26.image_fit.set(slint::cbindgen_private::ImageFit::Contain);
    self->image_26.width.set(24);
    self->image_26.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::layout_cache_access(self->parent->root_23_l_25_layout_cache.get(), 0, 0);
                        });
    self->image_26.y.set(3);
}

inline auto Component_image_26::user_init () -> void{
    [[maybe_unused]] auto self = this;
}

inline auto Component_image_26::layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo{
    [[maybe_unused]] auto self = this;
    return o == slint::cbindgen_private::Orientation::Horizontal ? [&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(ImageItemVTable)->layout_info({SLINT_GET_ITEM_VTABLE(ImageItemVTable), const_cast<slint::cbindgen_private::ImageItem*>(&self->image_26)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.preferred = a_0; o.min = a_1; o.stretch = a_2; o.max = a_3; o.min_percent = a_4; o.max_percent = a_5; return o; }(layout_info.preferred, 24, layout_info.stretch, 24, layout_info.min_percent, layout_info.max_percent); }() : [&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(ImageItemVTable)->layout_info({SLINT_GET_ITEM_VTABLE(ImageItemVTable), const_cast<slint::cbindgen_private::ImageItem*>(&self->image_26)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max = a_1; o.min_percent = a_2; o.min = a_3; o.max_percent = a_4; o.preferred = a_5; return o; }(layout_info.stretch, layout_info.max, layout_info.min_percent, layout_info.min, layout_info.max_percent, layout_info.preferred); }();
}

inline auto Component_image_26::accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole{
    [[maybe_unused]] auto self = this;
    switch (index) {
    }
    return {};
}

inline auto Component_image_26::accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString{
    [[maybe_unused]] auto self = this;
    switch ((index << 8) | uintptr_t(what)) {
    }
    return {};
}

inline auto Component_image_26::visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t{
    static const auto dyn_visit = [] (const uint8_t *base,  [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor, [[maybe_unused]] uintptr_t dyn_index) -> uint64_t {
        [[maybe_unused]] auto self = reinterpret_cast<const Component_image_26*>(base);
        std::abort();
    };
    auto self_rc = reinterpret_cast<const Component_image_26*>(component.instance)->self_weak.lock()->into_dyn();
    return slint::cbindgen_private::slint_visit_item_tree(&self_rc, get_item_tree(component) , index, order, visitor, dyn_visit);
}

inline auto Component_image_26::get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef{
    return slint::private_api::get_item_ref(component, get_item_tree(component), item_array(), index);
}

inline auto Component_image_26::get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange{
        std::abort();
}

inline auto Component_image_26::get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void{
        std::abort();
}

inline auto Component_image_26::get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    return item_tree();
}

inline auto Component_image_26::parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void{
    auto self = reinterpret_cast<const Component_image_26*>(component.instance);
    *result = { self->parent->self_weak, self->parent->tree_index_of_first_child + 2 - 1 };
}

inline auto Component_image_26::subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t{
    return std::numeric_limits<uintptr_t>::max();
}

inline auto Component_image_26::item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    static const slint::private_api::ItemTreeNode children[] {
        slint::private_api::make_item_node(0, 1, 0, 0, false) };
    return { const_cast<slint::private_api::ItemTreeNode*>(children), std::size(children) };
}

inline auto Component_image_26::item_array () -> const slint::private_api::ItemArray{
    static const slint::private_api::ItemArrayEntry items[] {
        { SLINT_GET_ITEM_VTABLE(ImageItemVTable),  offsetof(Component_image_26, image_26) } };
    return { const_cast<slint::private_api::ItemArrayEntry*>(items), std::size(items) };
}

inline auto Component_image_26::layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo{
    return reinterpret_cast<const Component_image_26*>(component.instance)->layout_info(o);
}

inline auto Component_image_26::accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole{
    return reinterpret_cast<const Component_image_26*>(component.instance)->accessible_role(index);
}

inline auto Component_image_26::accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void{
    *result = reinterpret_cast<const Component_image_26*>(component.instance)->accessible_string_property(index, what);
}

inline auto Component_image_26::create (class Button_root_23 const * parent) -> slint::ComponentHandle<Component_image_26>{
    auto self_rc = vtable::VRc<slint::private_api::ComponentVTable, Component_image_26>::make();
    auto self = const_cast<Component_image_26 *>(&*self_rc);
    self->self_weak = vtable::VWeak(self_rc).into_dyn();
    parent->root->m_window.window_handle().register_component(self, self->item_array());
    self->init(parent->root, self->self_weak, 0, 1 , parent);
    return slint::ComponentHandle<Component_image_26>{ self_rc };
}

inline Component_image_26::~Component_image_26 (){
    auto self = this;
    parent->root->m_window.window_handle().unregister_component(self, item_array());
}

inline auto Component_image_26::update_data ([[maybe_unused]] int i, [[maybe_unused]] const int &data) const -> void{
    [[maybe_unused]] auto self = this;
}

inline auto Component_image_26::init () -> void{
    user_init();
}

inline auto Component_image_26::box_layout_data (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::BoxLayoutCellData{
    return { layout_info({&static_vtable, const_cast<void *>(static_cast<const void *>(this))}, o) };
}

inline auto Button_root_23::init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void{
    auto self = this;
    self->self_weak = enclosing_component;
    self->root = root;
    this->tree_index_of_first_child = tree_index_of_first_child;
    self->tree_index = tree_index;
    slint::private_api::Property<bool>::link_two_way(&self->touch_29.enabled, &self->fs_30.enabled);
    self->root_23_l_25_layout_cache.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]{ std::array<int, 2> repeated_indices_array; std::vector<slint::cbindgen_private::BoxLayoutCellData> cells_vector;self->repeater_0.ensure_updated(self);repeated_indices_array[0] = cells_vector.size();repeated_indices_array[1] = self->repeater_0.inner ? self->repeater_0.inner->data.size() : 0;if (self->repeater_0.inner) for (auto &&sub_comp : self->repeater_0.inner->data) cells_vector.push_back((*sub_comp.ptr)->box_layout_data(slint::cbindgen_private::Orientation::Horizontal));cells_vector.push_back({ [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }(SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->text_28)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle())) });slint::cbindgen_private::Slice<int> repeated_indices{ repeated_indices_array.data(), repeated_indices_array.size() }; slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>cells{cells_vector.data(), cells_vector.size()}; return slint::private_api::solve_box_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4){ slint::cbindgen_private::BoxLayoutData o{}; o.size = a_0; o.padding = a_1; o.cells = a_2; o.spacing = a_3; o.alignment = a_4; return o; }(self->root_23.width.get(), [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.end = a_0; o.begin = a_1; return o; }(16, 16), cells, 8, slint::cbindgen_private::LayoutAlignment::Stretch),repeated_indices); }();
                        });
    self->root_23_l_25_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]{  std::vector<slint::cbindgen_private::BoxLayoutCellData> cells_vector;self->repeater_0.ensure_updated(self);if (self->repeater_0.inner) for (auto &&sub_comp : self->repeater_0.inner->data) cells_vector.push_back((*sub_comp.ptr)->box_layout_data(slint::cbindgen_private::Orientation::Horizontal));cells_vector.push_back({ [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }(SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->text_28)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle())) }); slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>cells{cells_vector.data(), cells_vector.size()}; return slint::private_api::box_layout_info(cells,8,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(16, 16),slint::cbindgen_private::LayoutAlignment::Stretch); }();
                        });
    self->root_23_l_25_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]{  std::vector<slint::cbindgen_private::BoxLayoutCellData> cells_vector;self->repeater_0.ensure_updated(self);if (self->repeater_0.inner) for (auto &&sub_comp : self->repeater_0.inner->data) cells_vector.push_back((*sub_comp.ptr)->box_layout_data(slint::cbindgen_private::Orientation::Vertical));cells_vector.push_back({ [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }(SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->text_28)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle())) }); slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>cells{cells_vector.data(), cells_vector.size()}; return slint::private_api::box_layout_info_ortho(cells,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(3, 3)); }();
                        });
    self->rectangle_24.background.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->fs_30.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); } else { return [&]() -> slint::Color { if (self->touch_29.pressed.get() || self->root_23_checked.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.293782505e9); } else { return slint::Color::from_argb_encoded(+4.282335039e9); }}(); } else { return [&]() -> slint::Color { if (self->touch_29.has_hover.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294967295e9); } else { return slint::Color::from_argb_encoded(+4.280229663e9); }}(); }}(); }}(); }}());
                        });
    self->rectangle_24.border_color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->fs_30.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); } else { return slint::Color::from_argb_encoded(+4.287268998e9); }}());
                        });
    self->rectangle_24.border_radius.set(2);
    self->rectangle_24.border_width.set(1);
    self->rectangle_24.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_23.height.get();
                        });
    self->rectangle_24.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_23.width.get();
                        });
    self->text_28.color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->fs_30.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.288782237e9); } else { return slint::Color::from_argb_encoded(+4.29134868e9); }}(); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.280295198e9); } else { return slint::Color::from_argb_encoded(+4.294243572e9); }}(); }}());
                        });
    self->text_28.font_weight.set(600);
    self->text_28.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->root_23.height.get() - 3) - 3);
                        });
    self->text_28.horizontal_alignment.set(slint::cbindgen_private::TextHorizontalAlignment::Center);
    self->text_28.vertical_alignment.set(slint::cbindgen_private::TextVerticalAlignment::Center);
    self->text_28.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_23_l_25_layout_cache.get()[3];
                        });
    self->text_28.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_23_l_25_layout_cache.get()[2];
                        });
    self->text_28.y.set(3);
    self->touch_29.clicked.set_handler(
                    [this]() {
                        [[maybe_unused]] auto self = this;
                        return [&]{ [&]() -> void { if (self->root_23_checkable.get()) { [&]{ self->root_23_checked.set((! self->root_23_checked.get())); }(); } else { [&]{  }(); }}();self->root_23_clicked.call(); }();
                    });
    self->touch_29.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_23.height.get();
                        });
    self->touch_29.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_23.width.get();
                        });
    self->fs_30.enabled.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return true;
                        });
    self->fs_30.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_23.height.get();
                        });
    self->fs_30.key_pressed.set_handler(
                    [this]([[maybe_unused]] slint::private_api::KeyEvent arg_0) {
                        [[maybe_unused]] auto self = this;
                        return [&]() -> slint::cbindgen_private::EventResult { try { [&]{ [&]() -> void { if ((arg_0.text == slint::SharedString(u8" ")) || (arg_0.text == slint::SharedString(u8"\n"))) { [&]{ self->touch_29.clicked.call();throw slint::private_api::ReturnWrapper<slint::cbindgen_private::EventResult>(slint::cbindgen_private::EventResult::Accept); }(); } else { [&]{  }(); }}();throw slint::private_api::ReturnWrapper<slint::cbindgen_private::EventResult>(slint::cbindgen_private::EventResult::Reject); }(); return {}; } catch(const slint::private_api::ReturnWrapper<slint::cbindgen_private::EventResult> &w) { return w.value; } }();
                    });
    self->fs_30.width.set(0);
    self->fs_30.x.set(0);
    self->rectangle_31.border_color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.27819008e9); } else { return slint::Color::from_argb_encoded(+4.294506744e9); }}());
                        });
    self->rectangle_31.border_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> float { if (self->fs_30.enabled.get() && self->fs_30.has_focus.get()) { return 1; } else { return 0; }}();
                        });
    self->rectangle_31.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->root_23.height.get() - 6);
                        });
    self->rectangle_31.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->root_23.width.get() - 6);
                        });
    self->rectangle_31.x.set(3);
    self->rectangle_31.y.set(3);
    self->repeater_0.set_model_binding([self] { (void)self; return std::make_shared<slint::private_api::UIntModel>([&]{ auto tmp_root_23_icon = slint::Image();;return ((tmp_root_23_icon.size().width > 0) && (tmp_root_23_icon.size().height > 0)); }()); });
}

inline auto Button_root_23::user_init () -> void{
    [[maybe_unused]] auto self = this;
}

inline auto Button_root_23::layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo{
    [[maybe_unused]] auto self = this;
    return o == slint::cbindgen_private::Orientation::Horizontal ? [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max_percent = a_1; o.max = a_2; o.min_percent = a_3; o.preferred = a_4; o.min = a_5; return o; }(1, 100, +3.4028234663852886e38, 0, 0, 0) + self->root_23_l_25_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.preferred = a_1; o.max_percent = a_2; o.stretch = a_3; o.max = a_4; o.min_percent = a_5; return o; }(layout_info.min, layout_info.preferred, layout_info.max_percent, 0, layout_info.max, layout_info.min_percent); }() : [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.max = a_1; o.preferred = a_2; o.stretch = a_3; o.min = a_4; o.min_percent = a_5; return o; }(100, +3.4028234663852886e38, 0, 1, 0, 0) + self->root_23_l_25_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.stretch = a_1; o.min_percent = a_2; o.max = a_3; o.max_percent = a_4; o.preferred = a_5; return o; }([&]{ auto minmax_lhs1 = 32;;auto minmax_rhs1 = self->root_23_l_25_layoutinfo_v.get().min;;return [&]() -> float { if (minmax_lhs1 > minmax_rhs1) { return minmax_lhs1; } else { return minmax_rhs1; }}(); }(), 0, layout_info.min_percent, layout_info.max, layout_info.max_percent, layout_info.preferred); }();
}

inline auto Button_root_23::accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole{
    [[maybe_unused]] auto self = this;
    switch (index) {
        case 0: return slint::cbindgen_private::AccessibleRole::Button;
        case 3: return slint::cbindgen_private::AccessibleRole::Text;
    }
    return {};
}

inline auto Button_root_23::accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString{
    [[maybe_unused]] auto self = this;
    switch ((index << 8) | uintptr_t(what)) {
        case (0 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Label): return self->text_28.text.get();
        case (3 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Label): return self->text_28.text.get();
    }
    return {};
}

inline auto Button_root_23::visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t{
        auto self = this;
        switch(dyn_index) { 
        case 0: {
                self->repeater_0.ensure_updated(self);
                return self->repeater_0.visit(order, visitor);
            } };
        std::abort();
}

inline auto Button_root_23::subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 0: {
                self->repeater_0.ensure_updated(self);
                return self->repeater_0.index_range();
            } };
        std::abort();
}

inline auto Button_root_23::subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 0: {
                self->repeater_0.ensure_updated(self);
                *result = self->repeater_0.component_at(subtree_index);
                return;
            } };
        std::abort();
}

inline const slint::private_api::ComponentVTable Component_path_36::static_vtable = { visit_children, get_item_ref, get_subtree_range, get_subtree_component, get_item_tree, parent_node, subtree_index, layout_info, accessible_role, accessible_string_property, slint::private_api::drop_in_place<Component_path_36>, slint::private_api::dealloc };

inline auto Component_path_36::init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child,class CheckBox_root_32 const * parent) -> void{
    auto self = this;
    self->self_weak = enclosing_component;
    self->root = root;
    this->tree_index_of_first_child = tree_index_of_first_child;
    self->tree_index = tree_index;
    self->parent = parent;
    self->path_36.elements.set([&](auto events, auto points){
                            return slint::private_api::PathData(events.ptr, events.len, points.ptr, points.len);
                        }(slint::cbindgen_private::Slice<slint::cbindgen_private::PathEvent>{ std::array<slint::cbindgen_private::PathEvent, 7>{ slint::cbindgen_private::PathEvent ( slint::cbindgen_private::PathEvent::Begin ), slint::cbindgen_private::PathEvent ( slint::cbindgen_private::PathEvent::Line ), slint::cbindgen_private::PathEvent ( slint::cbindgen_private::PathEvent::Line ), slint::cbindgen_private::PathEvent ( slint::cbindgen_private::PathEvent::Line ), slint::cbindgen_private::PathEvent ( slint::cbindgen_private::PathEvent::Line ), slint::cbindgen_private::PathEvent ( slint::cbindgen_private::PathEvent::Line ), slint::cbindgen_private::PathEvent ( slint::cbindgen_private::PathEvent::EndClosed ) }.data(), 7 }, slint::cbindgen_private::Slice<slint::cbindgen_private::Point>{ std::array<slint::cbindgen_private::Point, 11>{ slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.x = a_0; o.y = a_1; return o; }(0.2199999988079071, 0.5) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.x = a_0; o.y = a_1; return o; }(0.2199999988079071, 0.5) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.x = a_0; o.y = a_1; return o; }(0.41999998688697815, 0.699999988079071) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.x = a_0; o.y = a_1; return o; }(0.41999998688697815, 0.699999988079071) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.y = a_0; o.x = a_1; return o; }(0.3400000035762787, 0.7799999713897705) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.y = a_0; o.x = a_1; return o; }(0.3400000035762787, 0.7799999713897705) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.y = a_0; o.x = a_1; return o; }(0.30000001192092896, 0.7400000095367432) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.x = a_0; o.y = a_1; return o; }(0.7400000095367432, 0.30000001192092896) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.y = a_0; o.x = a_1; return o; }(0.6200000047683716, 0.41999998688697815) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.x = a_0; o.y = a_1; return o; }(0.41999998688697815, 0.6200000047683716) ), slint::cbindgen_private::Point ( [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Point o{}; o.x = a_0; o.y = a_1; return o; }(0.25999999046325684, 0.5400000214576721) ) }.data(), 11 }));
    self->path_36.fill.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (self->parent->root_32_accessible_checked.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294967295e9); } else { return slint::Color::from_argb_encoded(+4.280229663e9); }}(); } else { return slint::Color::from_argb_encoded(+4.287268998e9); }}());
                        });
    self->path_36.height.set((0.66 * 20));
    self->path_36.width.set((0.66 * 20));
    self->path_36.x.set(((20 - (0.66 * 20)) /(float) 2));
    self->path_36.y.set(((20 - (0.66 * 20)) /(float) 2));
}

inline auto Component_path_36::user_init () -> void{
    [[maybe_unused]] auto self = this;
}

inline auto Component_path_36::layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo{
    [[maybe_unused]] auto self = this;
    return o == slint::cbindgen_private::Orientation::Horizontal ? SLINT_GET_ITEM_VTABLE(PathVTable)->layout_info({SLINT_GET_ITEM_VTABLE(PathVTable), const_cast<slint::cbindgen_private::Path*>(&self->path_36)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()) : SLINT_GET_ITEM_VTABLE(PathVTable)->layout_info({SLINT_GET_ITEM_VTABLE(PathVTable), const_cast<slint::cbindgen_private::Path*>(&self->path_36)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle());
}

inline auto Component_path_36::accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole{
    [[maybe_unused]] auto self = this;
    switch (index) {
    }
    return {};
}

inline auto Component_path_36::accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString{
    [[maybe_unused]] auto self = this;
    switch ((index << 8) | uintptr_t(what)) {
    }
    return {};
}

inline auto Component_path_36::visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t{
    static const auto dyn_visit = [] (const uint8_t *base,  [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor, [[maybe_unused]] uintptr_t dyn_index) -> uint64_t {
        [[maybe_unused]] auto self = reinterpret_cast<const Component_path_36*>(base);
        std::abort();
    };
    auto self_rc = reinterpret_cast<const Component_path_36*>(component.instance)->self_weak.lock()->into_dyn();
    return slint::cbindgen_private::slint_visit_item_tree(&self_rc, get_item_tree(component) , index, order, visitor, dyn_visit);
}

inline auto Component_path_36::get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef{
    return slint::private_api::get_item_ref(component, get_item_tree(component), item_array(), index);
}

inline auto Component_path_36::get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange{
        std::abort();
}

inline auto Component_path_36::get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void{
        std::abort();
}

inline auto Component_path_36::get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    return item_tree();
}

inline auto Component_path_36::parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void{
    auto self = reinterpret_cast<const Component_path_36*>(component.instance);
    *result = { self->parent->self_weak, self->parent->tree_index_of_first_child + 7 - 1 };
}

inline auto Component_path_36::subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t{
    return std::numeric_limits<uintptr_t>::max();
}

inline auto Component_path_36::item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    static const slint::private_api::ItemTreeNode children[] {
        slint::private_api::make_item_node(0, 1, 0, 0, false) };
    return { const_cast<slint::private_api::ItemTreeNode*>(children), std::size(children) };
}

inline auto Component_path_36::item_array () -> const slint::private_api::ItemArray{
    static const slint::private_api::ItemArrayEntry items[] {
        { SLINT_GET_ITEM_VTABLE(PathVTable),  offsetof(Component_path_36, path_36) } };
    return { const_cast<slint::private_api::ItemArrayEntry*>(items), std::size(items) };
}

inline auto Component_path_36::layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo{
    return reinterpret_cast<const Component_path_36*>(component.instance)->layout_info(o);
}

inline auto Component_path_36::accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole{
    return reinterpret_cast<const Component_path_36*>(component.instance)->accessible_role(index);
}

inline auto Component_path_36::accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void{
    *result = reinterpret_cast<const Component_path_36*>(component.instance)->accessible_string_property(index, what);
}

inline auto Component_path_36::create (class CheckBox_root_32 const * parent) -> slint::ComponentHandle<Component_path_36>{
    auto self_rc = vtable::VRc<slint::private_api::ComponentVTable, Component_path_36>::make();
    auto self = const_cast<Component_path_36 *>(&*self_rc);
    self->self_weak = vtable::VWeak(self_rc).into_dyn();
    parent->root->m_window.window_handle().register_component(self, self->item_array());
    self->init(parent->root, self->self_weak, 0, 1 , parent);
    return slint::ComponentHandle<Component_path_36>{ self_rc };
}

inline Component_path_36::~Component_path_36 (){
    auto self = this;
    parent->root->m_window.window_handle().unregister_component(self, item_array());
}

inline auto Component_path_36::update_data ([[maybe_unused]] int i, [[maybe_unused]] const int &data) const -> void{
    [[maybe_unused]] auto self = this;
}

inline auto Component_path_36::init () -> void{
    user_init();
}

inline auto Component_path_36::box_layout_data (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::BoxLayoutCellData{
    return { layout_info({&static_vtable, const_cast<void *>(static_cast<const void *>(this))}, o) };
}

inline auto CheckBox_root_32::init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void{
    auto self = this;
    self->self_weak = enclosing_component;
    self->root = root;
    this->tree_index_of_first_child = tree_index_of_first_child;
    self->tree_index = tree_index;
    slint::private_api::Property<bool>::link_two_way(&self->touch_39.enabled, &self->fs_40.enabled);
    self->root_32_accessible_checked.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return false;
                        });
    self->root_32_empty_33_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32.height.get();
                        });
    self->root_32_empty_33_layout_cache.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::solve_box_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4){ slint::cbindgen_private::BoxLayoutData o{}; o.spacing = a_0; o.cells = a_1; o.size = a_2; o.alignment = a_3; o.padding = a_4; return o; }(8, slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 2>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }(self->root_32_empty_34_layoutinfo_h.get()) ), slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->text_38)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.preferred = a_1; o.max_percent = a_2; o.stretch = a_3; o.min_percent = a_4; o.max = a_5; return o; }(layout_info.min, layout_info.preferred, layout_info.max_percent, layout_info.stretch, layout_info.min_percent, layout_info.max); }()) ) }.data(), 2 }, self->root_32.width.get(), slint::cbindgen_private::LayoutAlignment::Stretch, [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0)),slint::cbindgen_private::Slice<int>{ std::array<int, 0>{  }.data(), 0 });
                        });
    self->root_32_empty_33_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::box_layout_info(slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 2>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }(self->root_32_empty_34_layoutinfo_h.get()) ), slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->text_38)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.preferred = a_0; o.max = a_1; o.min_percent = a_2; o.min = a_3; o.max_percent = a_4; o.stretch = a_5; return o; }(layout_info.preferred, layout_info.max, layout_info.min_percent, layout_info.min, layout_info.max_percent, layout_info.stretch); }()) ) }.data(), 2 },8,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0),slint::cbindgen_private::LayoutAlignment::Stretch);
                        });
    self->root_32_empty_33_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::box_layout_info_ortho(slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 2>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }(self->root_32_empty_34_layoutinfo_v.get()) ), slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->text_38)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.min = a_1; o.max_percent = a_2; o.stretch = a_3; o.preferred = a_4; o.max = a_5; return o; }(layout_info.min_percent, layout_info.min, layout_info.max_percent, 1, layout_info.preferred, layout_info.max); }()) ) }.data(), 2 },[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0));
                        });
    self->root_32_empty_34_layout_cache.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::solve_box_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4){ slint::cbindgen_private::BoxLayoutData o{}; o.cells = a_0; o.size = a_1; o.spacing = a_2; o.padding = a_3; o.alignment = a_4; return o; }(slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 1>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(BorderRectangleVTable)->layout_info({SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), const_cast<slint::cbindgen_private::BorderRectangle*>(&self->rectangle_35)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.stretch = a_1; o.min = a_2; o.preferred = a_3; o.max = a_4; o.max_percent = a_5; return o; }(layout_info.min_percent, 0, 20, layout_info.preferred, 20, layout_info.max_percent); }()) ) }.data(), 1 }, self->root_32_empty_33_height.get(), 0, [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0), slint::cbindgen_private::LayoutAlignment::Center),slint::cbindgen_private::Slice<int>{ std::array<int, 0>{  }.data(), 0 });
                        });
    self->root_32_empty_34_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::box_layout_info_ortho(slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 1>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(BorderRectangleVTable)->layout_info({SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), const_cast<slint::cbindgen_private::BorderRectangle*>(&self->rectangle_35)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.max = a_1; o.max_percent = a_2; o.preferred = a_3; o.min = a_4; o.stretch = a_5; return o; }(layout_info.min_percent, 20, layout_info.max_percent, layout_info.preferred, 20, layout_info.stretch); }()) ) }.data(), 1 },[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.end = a_0; o.begin = a_1; return o; }(0, 0));
                        });
    self->root_32_empty_34_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::box_layout_info(slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 1>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(BorderRectangleVTable)->layout_info({SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), const_cast<slint::cbindgen_private::BorderRectangle*>(&self->rectangle_35)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.preferred = a_0; o.max = a_1; o.min_percent = a_2; o.min = a_3; o.max_percent = a_4; o.stretch = a_5; return o; }(layout_info.preferred, 20, layout_info.min_percent, 20, layout_info.max_percent, 0); }()) ) }.data(), 1 },0,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.end = a_0; o.begin = a_1; return o; }(0, 0),slint::cbindgen_private::LayoutAlignment::Center);
                        });
    self->empty_34.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32.height.get();
                        });
    self->empty_34.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32_empty_33_layout_cache.get()[1];
                        });
    self->empty_34.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32_empty_33_layout_cache.get()[0];
                        });
    self->rectangle_35.background.set_animated_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->root_32_accessible_checked.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294967295e9); } else { return slint::Color::from_argb_encoded(+4.280229663e9); }}(); } else { return [&]() -> slint::Color { if (! self->fs_40.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.291348164e9); } else { return slint::Color::from_argb_encoded(+4.285361517e9); }}(); } else { return [&]() -> slint::Color { if (self->touch_39.has_hover.get() || self->touch_39.pressed.get()) { return slint::Color::from_argb_encoded(+4.278213278e9); } else { return slint::Color::from_argb_encoded(+4.278221012e9); }}(); }}(); }}());
                        }, [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3){ slint::cbindgen_private::PropertyAnimation o{}; o.iteration_count = a_0; o.delay = a_1; o.easing = a_2; o.duration = a_3; return o; }(1, 0, slint::cbindgen_private::EasingCurve(slint::cbindgen_private::EasingCurve::Tag::CubicBezier, 0.25, 0.1, 0.25, 1), 250));
    self->rectangle_35.border_color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> slint::Brush { if (self->root_32_accessible_checked.get()) { return self->rectangle_35.background.get(); } else { return slint::Brush([&]() -> slint::Color { if (! self->fs_40.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.291348164e9); } else { return slint::Color::from_argb_encoded(+4.285361517e9); }}(); } else { return slint::Color::from_argb_encoded(+4.287268998e9); }}()); }}();
                        });
    self->rectangle_35.border_radius.set(2);
    self->rectangle_35.border_width.set(1);
    self->rectangle_35.height.set(20);
    self->rectangle_35.width.set(20);
    self->rectangle_35.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32_empty_34_layout_cache.get()[0];
                        });
    self->text_38.color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->fs_40.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.288782237e9); } else { return slint::Color::from_argb_encoded(+4.29134868e9); }}(); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.280295198e9); } else { return slint::Color::from_argb_encoded(+4.294243572e9); }}(); }}());
                        });
    self->text_38.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32.height.get();
                        });
    self->text_38.horizontal_alignment.set(slint::cbindgen_private::TextHorizontalAlignment::Left);
    self->text_38.vertical_alignment.set(slint::cbindgen_private::TextVerticalAlignment::Center);
    self->text_38.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32_empty_33_layout_cache.get()[3];
                        });
    self->text_38.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32_empty_33_layout_cache.get()[2];
                        });
    self->touch_39.clicked.set_handler(
                    [this]() {
                        [[maybe_unused]] auto self = this;
                        return [&]{ [&]() -> void { if (self->fs_40.enabled.get()) { [&]{ self->root_32_accessible_checked.set((! self->root_32_accessible_checked.get()));self->root_32_toggled.call(); }(); } else { [&]{  }(); }}(); }();
                    });
    self->touch_39.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32.height.get();
                        });
    self->touch_39.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32.width.get();
                        });
    self->fs_40.enabled.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return true;
                        });
    self->fs_40.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_32.height.get();
                        });
    self->fs_40.key_pressed.set_handler(
                    [this]([[maybe_unused]] slint::private_api::KeyEvent arg_0) {
                        [[maybe_unused]] auto self = this;
                        return [&]() -> slint::cbindgen_private::EventResult { try { [&]{ [&]() -> void { if ((arg_0.text == slint::SharedString(u8" ")) || (arg_0.text == slint::SharedString(u8"\n"))) { [&]{ self->touch_39.clicked.call();throw slint::private_api::ReturnWrapper<slint::cbindgen_private::EventResult>(slint::cbindgen_private::EventResult::Accept); }(); } else { [&]{  }(); }}();throw slint::private_api::ReturnWrapper<slint::cbindgen_private::EventResult>(slint::cbindgen_private::EventResult::Reject); }(); return {}; } catch(const slint::private_api::ReturnWrapper<slint::cbindgen_private::EventResult> &w) { return w.value; } }();
                    });
    self->fs_40.width.set(0);
    self->fs_40.x.set(0);
    self->rectangle_41.border_color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.27819008e9); } else { return slint::Color::from_argb_encoded(+4.294506744e9); }}());
                        });
    self->rectangle_41.border_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> float { if (self->fs_40.enabled.get() && self->fs_40.has_focus.get()) { return 1; } else { return 0; }}();
                        });
    self->rectangle_41.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->root_32.height.get() - (2 * (- 3)));
                        });
    self->rectangle_41.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->root_32.width.get() - (2 * (- 3)));
                        });
    self->rectangle_41.x.set((- 3));
    self->rectangle_41.y.set((- 3));
    self->repeater_0.set_model_binding([self] { (void)self; return std::make_shared<slint::private_api::UIntModel>(((self->root_32_accessible_checked.get() || self->touch_39.has_hover.get()) || self->touch_39.pressed.get())); });
}

inline auto CheckBox_root_32::user_init () -> void{
    [[maybe_unused]] auto self = this;
}

inline auto CheckBox_root_32::layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo{
    [[maybe_unused]] auto self = this;
    return o == slint::cbindgen_private::Orientation::Horizontal ? [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max = a_1; o.max_percent = a_2; o.min = a_3; o.preferred = a_4; o.min_percent = a_5; return o; }(1, +3.4028234663852886e38, 100, 0, 0, 0) + self->root_32_empty_33_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.stretch = a_1; o.preferred = a_2; o.min_percent = a_3; o.max_percent = a_4; o.max = a_5; return o; }(layout_info.min, 0, layout_info.preferred, layout_info.min_percent, layout_info.max_percent, layout_info.max); }() : [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.stretch = a_1; o.max_percent = a_2; o.min = a_3; o.max = a_4; o.preferred = a_5; return o; }(0, 1, 100, 0, +3.4028234663852886e38, 0) + self->root_32_empty_33_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.preferred = a_1; o.min_percent = a_2; o.stretch = a_3; o.min = a_4; o.max = a_5; return o; }(layout_info.max_percent, layout_info.preferred, layout_info.min_percent, 0, 20, layout_info.max); }();
}

inline auto CheckBox_root_32::accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole{
    [[maybe_unused]] auto self = this;
    switch (index) {
        case 0: return slint::cbindgen_private::AccessibleRole::Checkbox;
        case 2: return slint::cbindgen_private::AccessibleRole::Text;
    }
    return {};
}

inline auto CheckBox_root_32::accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString{
    [[maybe_unused]] auto self = this;
    switch ((index << 8) | uintptr_t(what)) {
        case (0 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Checkable): return [&]() -> slint::SharedString { if (true) { return slint::SharedString(u8"true"); } else { return slint::SharedString(u8"false"); }}();
        case (0 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Checked): return [&]() -> slint::SharedString { if (self->root_32_accessible_checked.get()) { return slint::SharedString(u8"true"); } else { return slint::SharedString(u8"false"); }}();
        case (0 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Label): return self->text_38.text.get();
        case (2 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Label): return self->text_38.text.get();
    }
    return {};
}

inline auto CheckBox_root_32::visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t{
        auto self = this;
        switch(dyn_index) { 
        case 0: {
                self->repeater_0.ensure_updated(self);
                return self->repeater_0.visit(order, visitor);
            } };
        std::abort();
}

inline auto CheckBox_root_32::subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 0: {
                self->repeater_0.ensure_updated(self);
                return self->repeater_0.index_range();
            } };
        std::abort();
}

inline auto CheckBox_root_32::subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 0: {
                self->repeater_0.ensure_updated(self);
                *result = self->repeater_0.component_at(subtree_index);
                return;
            } };
        std::abort();
}

inline auto ScrollBar_root_42::init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void{
    auto self = this;
    self->self_weak = enclosing_component;
    self->root = root;
    this->tree_index_of_first_child = tree_index_of_first_child;
    self->tree_index = tree_index;
    self->root_42.background.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (self->root_42_enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294967295e9); } else { return slint::Color::from_argb_encoded(+4.280229663e9); }}(); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); }}());
                        });
    self->root_42.border_width.set(1);
    self->root_42_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.min = a_1; o.max_percent = a_2; o.min_percent = a_3; o.preferred = a_4; o.stretch = a_5; return o; }(+3.4028234663852886e38, 0, 100, 0, 0, 1) + [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.min_percent = a_1; o.min = a_2; o.max = a_3; o.preferred = a_4; o.stretch = a_5; return o; }(100, 0, self->root_42_touch_area_44_min_width.get(), self->root_42_touch_area_44_max_width.get(), self->root_42_touch_area_44_preferred_width.get(), self->root_42_touch_area_44_horizontal_stretch.get()));
                        });
    self->root_42_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.stretch = a_1; o.max = a_2; o.min_percent = a_3; o.max_percent = a_4; o.preferred = a_5; return o; }(0, 1, +3.4028234663852886e38, 0, 100, 0) + [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.preferred = a_1; o.max_percent = a_2; o.max = a_3; o.min = a_4; o.min_percent = a_5; return o; }(self->root_42_touch_area_44_vertical_stretch.get(), self->root_42_touch_area_44_preferred_height.get(), 100, self->root_42_touch_area_44_max_height.get(), self->root_42_touch_area_44_min_height.get(), 0));
                        });
    self->root_42_touch_area_44_horizontal_stretch.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).stretch;
                        });
    self->root_42_touch_area_44_max_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).max;
                        });
    self->root_42_touch_area_44_max_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).max;
                        });
    self->root_42_touch_area_44_min_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).min;
                        });
    self->root_42_touch_area_44_min_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).min;
                        });
    self->root_42_touch_area_44_preferred_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).preferred;
                        });
    self->root_42_touch_area_44_preferred_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).preferred;
                        });
    self->root_42_touch_area_44_vertical_stretch.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TouchAreaVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TouchAreaVTable), const_cast<slint::cbindgen_private::TouchArea*>(&self->touch_area_44)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).stretch;
                        });
    self->handle_43.background.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (self->touch_area_44.pressed.get()) { return slint::Color::from_argb_encoded(+4.278221012e9); } else { return [&]() -> slint::Color { if (self->touch_area_44.has_hover.get()) { return slint::Color::from_argb_encoded(+4.28104316e9); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.288782237e9); } else { return slint::Color::from_argb_encoded(+4.29134868e9); }}(); }}(); }}());
                        });
    self->handle_43.border_radius.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ([&]() -> float { if (self->root_42_horizontal.get()) { return self->handle_43.height.get(); } else { return self->handle_43.width.get(); }}() /(float) 2);
                        });
    self->handle_43.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> float { if (self->root_42_horizontal.get()) { return self->root_42.height.get(); } else { return [&]{ auto tmp_root_42_maximum = self->root_42_maximum.get();;return ([&]() -> float { if (tmp_root_42_maximum <= (0 /(float) self->root->window().window_handle().scale_factor())) { return 0; } else { return [&]{ auto tmp_root_42_page_size = self->root_42_page_size.get();;return ([&]{ auto minmax_lhs2 = 32;;auto minmax_rhs2 = (self->root_42.height.get() * (tmp_root_42_page_size /(float) (tmp_root_42_maximum + tmp_root_42_page_size)));;return [&]() -> float { if (minmax_lhs2 > minmax_rhs2) { return minmax_lhs2; } else { return minmax_rhs2; }}(); }() * self->root->window().window_handle().scale_factor()); }(); }}() /(float) self->root->window().window_handle().scale_factor()); }(); }}();
                        });
    self->handle_43.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> float { if (! self->root_42_horizontal.get()) { return self->root_42.width.get(); } else { return [&]{ auto tmp_root_42_maximum = self->root_42_maximum.get();;return ([&]() -> float { if (tmp_root_42_maximum <= (0 /(float) self->root->window().window_handle().scale_factor())) { return 0; } else { return [&]{ auto tmp_root_42_page_size = self->root_42_page_size.get();;return ([&]{ auto minmax_lhs3 = 32;;auto minmax_rhs3 = ((self->root_42.width.get() * tmp_root_42_page_size) /(float) (tmp_root_42_maximum + tmp_root_42_page_size));;return [&]() -> float { if (minmax_lhs3 > minmax_rhs3) { return minmax_lhs3; } else { return minmax_rhs3; }}(); }() * self->root->window().window_handle().scale_factor()); }(); }}() /(float) self->root->window().window_handle().scale_factor()); }(); }}();
                        });
    self->handle_43.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ([&]() -> float { if (! self->root_42_horizontal.get()) { return 0; } else { return (((self->root_42.width.get() - self->handle_43.width.get()) * ((- self->root_42_value.get()) /(float) self->root_42_maximum.get())) * self->root->window().window_handle().scale_factor()); }}() /(float) self->root->window().window_handle().scale_factor());
                        });
    self->handle_43.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ([&]() -> float { if (self->root_42_horizontal.get()) { return 0; } else { return (((self->root_42.height.get() - self->handle_43.height.get()) * ((- self->root_42_value.get()) /(float) self->root_42_maximum.get())) * self->root->window().window_handle().scale_factor()); }}() /(float) self->root->window().window_handle().scale_factor());
                        });
    self->touch_area_44.enabled.set(true);
    self->touch_area_44.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_42.height.get();
                        });
    self->touch_area_44.moved.set_handler(
                    [this]() {
                        [[maybe_unused]] auto self = this;
                        return [&]{ [&]() -> void { if (true && self->touch_area_44.pressed.get()) { [&]{ self->root_42_value.set((- [&]{ auto minmax_lhs5 = 0;;auto minmax_rhs5 = [&]{ auto minmax_lhs4 = self->root_42_maximum.get();;auto minmax_rhs4 = (self->root_42_touch_area_44_pressed_value.get() + [&]() -> float { if (self->root_42_horizontal.get()) { return ((self->touch_area_44.mouse_x.get() - self->touch_area_44.pressed_x.get()) * (self->root_42_maximum.get() /(float) (self->root_42.width.get() - self->handle_43.width.get()))); } else { return ((self->touch_area_44.mouse_y.get() - self->touch_area_44.pressed_y.get()) * (self->root_42_maximum.get() /(float) (self->root_42.height.get() - self->handle_43.height.get()))); }}());;return [&]() -> float { if (minmax_lhs4 < minmax_rhs4) { return minmax_lhs4; } else { return minmax_rhs4; }}(); }();;return [&]() -> float { if (minmax_lhs5 > minmax_rhs5) { return minmax_lhs5; } else { return minmax_rhs5; }}(); }())); }(); } else { [&]{  }(); }}(); }();
                    });
    self->touch_area_44.pointer_event.set_handler(
                    [this]([[maybe_unused]] slint::private_api::PointerEvent arg_0) {
                        [[maybe_unused]] auto self = this;
                        return [&]{ [&]() -> void { if ((arg_0.button == slint::cbindgen_private::PointerEventButton::Left) && (arg_0.kind == slint::cbindgen_private::PointerEventKind::Down)) { [&]{ self->root_42_touch_area_44_pressed_value.set((- self->root_42_value.get())); }(); } else { [&]{  }(); }}(); }();
                    });
    self->touch_area_44.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_42.width.get();
                        });
    self->touch_area_44.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->root_42.width.get() - self->root_42.width.get()) /(float) 2);
                        });
    self->touch_area_44.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->root_42.height.get() - self->root_42.height.get()) /(float) 2);
                        });
}

inline auto ScrollBar_root_42::user_init () -> void{
    [[maybe_unused]] auto self = this;
}

inline auto ScrollBar_root_42::layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo{
    [[maybe_unused]] auto self = this;
    return o == slint::cbindgen_private::Orientation::Horizontal ? self->root_42_layoutinfo_h.get() : self->root_42_layoutinfo_v.get();
}

inline auto ScrollBar_root_42::accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole{
    [[maybe_unused]] auto self = this;
    switch (index) {
    }
    return {};
}

inline auto ScrollBar_root_42::accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString{
    [[maybe_unused]] auto self = this;
    switch ((index << 8) | uintptr_t(what)) {
    }
    return {};
}

inline ColorSchemeSelector_45::ColorSchemeSelector_45 (const MainWindow * root)
 : root(root)
{
    (void)this->root;
    this->dark_color_scheme.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return this->root->window().window_handle().dark_color_scheme();
                        });
}

inline Palette_46::Palette_46 (const MainWindow * root)
 : root(root)
{
    (void)this->root;
    this->neutralPrimary.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> slint::Color { if (! this->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.281479472e9); } else { return slint::Color::from_argb_encoded(+4.294967295e9); }}();
                        });
    this->neutralTertiary.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> slint::Color { if (! this->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.288782237e9); } else { return slint::Color::from_argb_encoded(+4.29134868e9); }}();
                        });
    this->white.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> slint::Color { if (! this->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294967295e9); } else { return slint::Color::from_argb_encoded(+4.280229663e9); }}();
                        });
}

inline StyleMetrics_47::StyleMetrics_47 (const MainWindow * root)
 : root(root)
{
    (void)this->root;
}

inline const slint::private_api::ComponentVTable Component_rectangle_16::static_vtable = { visit_children, get_item_ref, get_subtree_range, get_subtree_component, get_item_tree, parent_node, subtree_index, layout_info, accessible_role, accessible_string_property, slint::private_api::drop_in_place<Component_rectangle_16>, slint::private_api::dealloc };

inline auto Component_rectangle_16::init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child,class MainWindow const * parent) -> void{
    auto self = this;
    self->self_weak = enclosing_component;
    self->root = root;
    this->tree_index_of_first_child = tree_index_of_first_child;
    self->tree_index = tree_index;
    self->parent = parent;
    this->checkbox_18.init(root, self_weak.into_dyn(), tree_index_of_first_child + 1 - 1, tree_index_of_first_child + 2 - 1);
    self->rectangle_16_empty_17_layout_cache_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::solve_grid_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3){ slint::cbindgen_private::GridLayoutData o{}; o.size = a_0; o.spacing = a_1; o.padding = a_2; o.cells = a_3; return o; }(self->rectangle_16.width.get(), 0, [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0), slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 1>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.col_or_row = a_0; o.span = a_1; o.constraint = a_2; return o; }(0, 1, [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max = a_1; o.max_percent = a_2; o.min = a_3; o.preferred = a_4; o.min_percent = a_5; return o; }(1, +3.4028234663852886e38, 100, 0, 0, 0) + self->checkbox_18.root_32_empty_33_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.preferred = a_1; o.max_percent = a_2; o.min = a_3; o.stretch = a_4; o.min_percent = a_5; return o; }(layout_info.max, layout_info.preferred, layout_info.max_percent, layout_info.min, 0, layout_info.min_percent); }()) ) }.data(), 1 }));
                        });
    self->rectangle_16_empty_17_layout_cache_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::solve_grid_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3){ slint::cbindgen_private::GridLayoutData o{}; o.cells = a_0; o.padding = a_1; o.spacing = a_2; o.size = a_3; return o; }(slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 1>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.constraint = a_0; o.col_or_row = a_1; o.span = a_2; return o; }([&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.stretch = a_1; o.max_percent = a_2; o.min = a_3; o.max = a_4; o.preferred = a_5; return o; }(0, 1, 100, 0, +3.4028234663852886e38, 0) + self->checkbox_18.root_32_empty_33_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.max_percent = a_1; o.min = a_2; o.min_percent = a_3; o.preferred = a_4; o.stretch = a_5; return o; }(layout_info.max, layout_info.max_percent, 20, layout_info.min_percent, layout_info.preferred, 0); }(), 0, 1) ) }.data(), 1 }, [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.end = a_0; o.begin = a_1; return o; }(0, 0), 0, 20));
                        });
    self->rectangle_16_empty_17_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::grid_layout_info(slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 1>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.constraint = a_0; o.col_or_row = a_1; o.span = a_2; return o; }([&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max = a_1; o.max_percent = a_2; o.min = a_3; o.preferred = a_4; o.min_percent = a_5; return o; }(1, +3.4028234663852886e38, 100, 0, 0, 0) + self->checkbox_18.root_32_empty_33_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.preferred = a_1; o.max = a_2; o.max_percent = a_3; o.min_percent = a_4; o.stretch = a_5; return o; }(layout_info.min, layout_info.preferred, layout_info.max, layout_info.max_percent, layout_info.min_percent, 0); }(), 0, 1) ) }.data(), 1 },0,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0));
                        });
    self->rectangle_16_empty_17_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::grid_layout_info(slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 1>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.constraint = a_0; o.span = a_1; o.col_or_row = a_2; return o; }([&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.stretch = a_1; o.max_percent = a_2; o.min = a_3; o.max = a_4; o.preferred = a_5; return o; }(0, 1, 100, 0, +3.4028234663852886e38, 0) + self->checkbox_18.root_32_empty_33_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max_percent = a_1; o.min = a_2; o.min_percent = a_3; o.max = a_4; o.preferred = a_5; return o; }(0, layout_info.max_percent, 20, layout_info.min_percent, layout_info.max, layout_info.preferred); }(), 1, 0) ) }.data(), 1 },0,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0));
                        });
    self->rectangle_16.height.set(20);
    self->rectangle_16.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->parent->fli_14.width.get();
                        });
    self->checkbox_18.root_32_accessible_checked.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->model_data.get().checked;
                        });
    self->checkbox_18.root_32.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->rectangle_16_empty_17_layout_cache_v.get()[1];
                        });
    self->checkbox_18.text_38.text.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->model_data.get().title;
                        });
    self->checkbox_18.root_32_toggled.set_handler(
                    [this]() {
                        [[maybe_unused]] auto self = this;
                        return [&]{ self->parent->repeater_0.model_set_row_data(self->model_index.get(), [&]{ auto struct_assignment0 = self->model_data.get();;return [&](const auto &a_0, const auto &a_1){ TodoItem o{}; o.title = a_0; o.checked = a_1; return o; }(struct_assignment0.title, self->checkbox_18.root_32_accessible_checked.get()); }()); }();
                    });
    self->checkbox_18.root_32.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->rectangle_16_empty_17_layout_cache_h.get()[1];
                        });
    self->checkbox_18.root_32.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->rectangle_16_empty_17_layout_cache_h.get()[0];
                        });
    self->checkbox_18.root_32.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->rectangle_16_empty_17_layout_cache_v.get()[0];
                        });
}

inline auto Component_rectangle_16::user_init () -> void{
    [[maybe_unused]] auto self = this;
    this->checkbox_18.user_init();
}

inline auto Component_rectangle_16::layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo{
    [[maybe_unused]] auto self = this;
    return o == slint::cbindgen_private::Orientation::Horizontal ? ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.preferred = a_1; o.min_percent = a_2; o.stretch = a_3; o.max_percent = a_4; o.min = a_5; return o; }(+3.4028234663852886e38, 0, 0, 1, 100, 0) + self->rectangle_16_empty_17_layoutinfo_h.get()) : ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.stretch = a_1; o.max_percent = a_2; o.min_percent = a_3; o.preferred = a_4; o.min = a_5; return o; }(+3.4028234663852886e38, 1, 100, 0, 0, 0) + self->rectangle_16_empty_17_layoutinfo_v.get());
}

inline auto Component_rectangle_16::accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole{
    [[maybe_unused]] auto self = this;
    switch (index) {
        case 1: return slint::cbindgen_private::AccessibleRole::Checkbox;
    }
    if (index == 1) {
        return self->checkbox_18.accessible_role(0);
    } else if (index >= 2 && index < 8) {
        return self->checkbox_18.accessible_role(index - 1);
    } else return {};
}

inline auto Component_rectangle_16::accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString{
    [[maybe_unused]] auto self = this;
    switch ((index << 8) | uintptr_t(what)) {
        case (1 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Checkable): return [&]() -> slint::SharedString { if (true) { return slint::SharedString(u8"true"); } else { return slint::SharedString(u8"false"); }}();
        case (1 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Checked): return [&]() -> slint::SharedString { if (self->checkbox_18.root_32_accessible_checked.get()) { return slint::SharedString(u8"true"); } else { return slint::SharedString(u8"false"); }}();
        case (1 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Label): return self->model_data.get().title;
    }
    if (index == 1) {
        return self->checkbox_18.accessible_string_property(0, what);
    } else if (index >= 2 && index < 8) {
        return self->checkbox_18.accessible_string_property(index - 1, what);
    } else return {};
}

inline auto Component_rectangle_16::visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t{
        auto self = this;
        switch(dyn_index) { 
        case 0:  {
                        return self->checkbox_18.visit_dynamic_children(dyn_index - 0, order, visitor);
                    } };
        std::abort();
}

inline auto Component_rectangle_16::subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 0:  {
                        return self->checkbox_18.subtree_range(dyn_index - 0);
                    } };
        std::abort();
}

inline auto Component_rectangle_16::subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 0:  {
                        self->checkbox_18.subtree_component(dyn_index - 0, subtree_index, result);
                        return;
                    } };
        std::abort();
}

inline auto Component_rectangle_16::visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t{
    static const auto dyn_visit = [] (const uint8_t *base,  [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor, [[maybe_unused]] uintptr_t dyn_index) -> uint64_t {
        [[maybe_unused]] auto self = reinterpret_cast<const Component_rectangle_16*>(base);
        return self->visit_dynamic_children(dyn_index, order, visitor);
    };
    auto self_rc = reinterpret_cast<const Component_rectangle_16*>(component.instance)->self_weak.lock()->into_dyn();
    return slint::cbindgen_private::slint_visit_item_tree(&self_rc, get_item_tree(component) , index, order, visitor, dyn_visit);
}

inline auto Component_rectangle_16::get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef{
    return slint::private_api::get_item_ref(component, get_item_tree(component), item_array(), index);
}

inline auto Component_rectangle_16::get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange{
    auto self = reinterpret_cast<const Component_rectangle_16*>(component.instance);
    return self->subtree_range(dyn_index);
}

inline auto Component_rectangle_16::get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void{
    auto self = reinterpret_cast<const Component_rectangle_16*>(component.instance);
    self->subtree_component(dyn_index, subtree_index, result);
}

inline auto Component_rectangle_16::get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    return item_tree();
}

inline auto Component_rectangle_16::parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void{
    auto self = reinterpret_cast<const Component_rectangle_16*>(component.instance);
    *result = { self->parent->self_weak, self->parent->tree_index_of_first_child + 22 - 1 };
}

inline auto Component_rectangle_16::subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t{
    auto self = reinterpret_cast<const Component_rectangle_16*>(component.instance);
    return self->model_index.get();
}

inline auto Component_rectangle_16::item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    static const slint::private_api::ItemTreeNode children[] {
        slint::private_api::make_item_node(1, 1, 0, 0, false), 
slint::private_api::make_item_node(5, 2, 0, 1, true), 
slint::private_api::make_item_node(1, 7, 1, 2, false), 
slint::private_api::make_item_node(0, 9, 1, 3, true), 
slint::private_api::make_item_node(0, 9, 1, 4, false), 
slint::private_api::make_item_node(0, 9, 1, 5, false), 
slint::private_api::make_item_node(0, 9, 1, 6, false), 
slint::private_api::make_item_node(1, 8, 2, 7, false), 
slint::private_api::make_dyn_node(0, 7) };
    return { const_cast<slint::private_api::ItemTreeNode*>(children), std::size(children) };
}

inline auto Component_rectangle_16::item_array () -> const slint::private_api::ItemArray{
    static const slint::private_api::ItemArrayEntry items[] {
        { SLINT_GET_ITEM_VTABLE(EmptyVTable),  offsetof(Component_rectangle_16, rectangle_16) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable), offsetof(Component_rectangle_16, checkbox_18) +  offsetof(CheckBox_root_32, root_32) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable), offsetof(Component_rectangle_16, checkbox_18) +  offsetof(CheckBox_root_32, empty_34) }, 
{ SLINT_GET_ITEM_VTABLE(TextVTable), offsetof(Component_rectangle_16, checkbox_18) +  offsetof(CheckBox_root_32, text_38) }, 
{ SLINT_GET_ITEM_VTABLE(TouchAreaVTable), offsetof(Component_rectangle_16, checkbox_18) +  offsetof(CheckBox_root_32, touch_39) }, 
{ SLINT_GET_ITEM_VTABLE(FocusScopeVTable), offsetof(Component_rectangle_16, checkbox_18) +  offsetof(CheckBox_root_32, fs_40) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(Component_rectangle_16, checkbox_18) +  offsetof(CheckBox_root_32, rectangle_41) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(Component_rectangle_16, checkbox_18) +  offsetof(CheckBox_root_32, rectangle_35) } };
    return { const_cast<slint::private_api::ItemArrayEntry*>(items), std::size(items) };
}

inline auto Component_rectangle_16::layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo{
    return reinterpret_cast<const Component_rectangle_16*>(component.instance)->layout_info(o);
}

inline auto Component_rectangle_16::accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole{
    return reinterpret_cast<const Component_rectangle_16*>(component.instance)->accessible_role(index);
}

inline auto Component_rectangle_16::accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void{
    *result = reinterpret_cast<const Component_rectangle_16*>(component.instance)->accessible_string_property(index, what);
}

inline auto Component_rectangle_16::create (class MainWindow const * parent) -> slint::ComponentHandle<Component_rectangle_16>{
    auto self_rc = vtable::VRc<slint::private_api::ComponentVTable, Component_rectangle_16>::make();
    auto self = const_cast<Component_rectangle_16 *>(&*self_rc);
    self->self_weak = vtable::VWeak(self_rc).into_dyn();
    parent->root->m_window.window_handle().register_component(self, self->item_array());
    self->init(parent->root, self->self_weak, 0, 1 , parent);
    return slint::ComponentHandle<Component_rectangle_16>{ self_rc };
}

inline Component_rectangle_16::~Component_rectangle_16 (){
    auto self = this;
    parent->root->m_window.window_handle().unregister_component(self, item_array());
}

inline auto Component_rectangle_16::update_data ([[maybe_unused]] int i, [[maybe_unused]] const TodoItem &data) const -> void{
    [[maybe_unused]] auto self = this;
    self->model_index.set(i);
    self->model_data.set(data);
}

inline auto Component_rectangle_16::init () -> void{
    user_init();
}

inline auto Component_rectangle_16::listview_layout (float *offset_y, const slint::private_api::Property<float> *viewport_width) const -> void{
    [[maybe_unused]] auto self = this;
    float vp_w = viewport_width->get();
    self->rectangle_16.y.set(*offset_y);
    *offset_y += self->rectangle_16.height.get();
    float w = self->rectangle_16.width.get();
    if (vp_w < w)
        viewport_width->set(w);
}

inline const slint::private_api::ComponentVTable MainWindow::static_vtable = { visit_children, get_item_ref, get_subtree_range, get_subtree_component, get_item_tree, parent_node, subtree_index, layout_info, accessible_role, accessible_string_property, slint::private_api::drop_in_place<MainWindow>, slint::private_api::dealloc };

inline auto MainWindow::init (const MainWindow * root,slint::cbindgen_private::ComponentWeak enclosing_component,uintptr_t tree_index,uintptr_t tree_index_of_first_child) -> void{
    auto self = this;
    self->self_weak = enclosing_component;
    self->root = root;
    this->tree_index_of_first_child = tree_index_of_first_child;
    self->tree_index = tree_index;
    this->button_11.init(root, self_weak.into_dyn(), tree_index_of_first_child + 3 - 1, tree_index_of_first_child + 10 - 1);
    this->vbar_20.init(root, self_weak.into_dyn(), tree_index_of_first_child + 18 - 1, tree_index_of_first_child + 23 - 1);
    this->hbar_21.init(root, self_weak.into_dyn(), tree_index_of_first_child + 19 - 1, tree_index_of_first_child + 25 - 1);
    slint::private_api::Property<float>::link_two_way(&self->root_window_1.height, &self->root_2.height);
    slint::private_api::Property<float>::link_two_way(&self->root_window_1.width, &self->root_2.width);
    slint::private_api::Property<float>::link_two_way(&self->vbar_20.root_42_value, &self->fli_14.viewport.y);
    slint::private_api::Property<float>::link_two_way(&self->hbar_21.root_42_value, &self->fli_14.viewport.x);
    self->root_window_1.background.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush(self->root->global_Palette_46->white.get());
                        });
    self->root_window_1_empty_3_layout_cache_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::solve_grid_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3){ slint::cbindgen_private::GridLayoutData o{}; o.spacing = a_0; o.padding = a_1; o.size = a_2; o.cells = a_3; return o; }(0, [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.end = a_0; o.begin = a_1; return o; }(0, 0), self->root_2.width.get(), slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 3>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.constraint = a_0; o.col_or_row = a_1; o.span = a_2; return o; }([&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.min = a_1; o.preferred = a_2; o.max = a_3; o.stretch = a_4; o.min_percent = a_5; return o; }(100, 0, 0, +3.4028234663852886e38, 1, 0) + self->root_window_1_l_6_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.stretch = a_1; o.min_percent = a_2; o.preferred = a_3; o.max = a_4; o.max_percent = a_5; return o; }(layout_info.min, 1, layout_info.min_percent, layout_info.preferred, layout_info.max, layout_info.max_percent); }(), 0, 1) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.col_or_row = a_0; o.span = a_1; o.constraint = a_2; return o; }(1, 1, [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max_percent = a_1; o.max = a_2; o.min_percent = a_3; o.preferred = a_4; o.min = a_5; return o; }(1, 100, +3.4028234663852886e38, 0, 0, 0) + self->button_11.root_23_l_25_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.preferred = a_1; o.stretch = a_2; o.max_percent = a_3; o.max = a_4; o.min_percent = a_5; return o; }(layout_info.min, layout_info.preferred, 0, layout_info.max_percent, layout_info.max, layout_info.min_percent); }()) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.col_or_row = a_0; o.constraint = a_1; o.span = a_2; return o; }(0, [&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(EmptyVTable)->layout_info({SLINT_GET_ITEM_VTABLE(EmptyVTable), const_cast<slint::cbindgen_private::Empty*>(&self->list_view_12)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.stretch = a_1; o.preferred = a_2; o.max_percent = a_3; o.min = a_4; o.min_percent = a_5; return o; }(layout_info.max, 1, layout_info.preferred, layout_info.max_percent, 50, layout_info.min_percent); }(), 1) ) }.data(), 3 }));
                        });
    self->root_window_1_empty_3_layout_cache_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::solve_grid_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3){ slint::cbindgen_private::GridLayoutData o{}; o.size = a_0; o.cells = a_1; o.padding = a_2; o.spacing = a_3; return o; }(0, slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 3>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.span = a_0; o.constraint = a_1; o.col_or_row = a_2; return o; }(1, [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.stretch = a_1; o.max = a_2; o.max_percent = a_3; o.min_percent = a_4; o.preferred = a_5; return o; }(0, 1, +3.4028234663852886e38, 100, 0, 0) + self->root_window_1_l_6_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.min_percent = a_1; o.preferred = a_2; o.stretch = a_3; o.min = a_4; o.max = a_5; return o; }(layout_info.max_percent, layout_info.min_percent, layout_info.preferred, 0, [&]{ auto minmax_lhs34 = 32;;auto minmax_rhs34 = self->root_window_1_l_6_layoutinfo_v.get().min;;return [&]() -> float { if (minmax_lhs34 > minmax_rhs34) { return minmax_lhs34; } else { return minmax_rhs34; }}(); }(), layout_info.max); }(), 0) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.span = a_0; o.constraint = a_1; o.col_or_row = a_2; return o; }(1, [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.max = a_1; o.preferred = a_2; o.stretch = a_3; o.min = a_4; o.min_percent = a_5; return o; }(100, +3.4028234663852886e38, 0, 1, 0, 0) + self->button_11.root_23_l_25_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.preferred = a_0; o.stretch = a_1; o.min_percent = a_2; o.max_percent = a_3; o.max = a_4; o.min = a_5; return o; }(layout_info.preferred, 0, layout_info.min_percent, layout_info.max_percent, layout_info.max, [&]{ auto minmax_lhs1 = 32;;auto minmax_rhs1 = self->button_11.root_23_l_25_layoutinfo_v.get().min;;return [&]() -> float { if (minmax_lhs1 > minmax_rhs1) { return minmax_lhs1; } else { return minmax_rhs1; }}(); }()); }(), 0) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.span = a_0; o.constraint = a_1; o.col_or_row = a_2; return o; }(2, [&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(EmptyVTable)->layout_info({SLINT_GET_ITEM_VTABLE(EmptyVTable), const_cast<slint::cbindgen_private::Empty*>(&self->list_view_12)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.preferred = a_0; o.max_percent = a_1; o.stretch = a_2; o.max = a_3; o.min = a_4; o.min_percent = a_5; return o; }(layout_info.preferred, layout_info.max_percent, 1, layout_info.max, 50, layout_info.min_percent); }(), 2) ) }.data(), 3 }, [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0), 0));
                        });
    self->root_window_1_empty_3_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::grid_layout_info(slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 3>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.constraint = a_0; o.col_or_row = a_1; o.span = a_2; return o; }([&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.min = a_1; o.preferred = a_2; o.max = a_3; o.stretch = a_4; o.min_percent = a_5; return o; }(100, 0, 0, +3.4028234663852886e38, 1, 0) + self->root_window_1_l_6_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.preferred = a_1; o.stretch = a_2; o.min = a_3; o.min_percent = a_4; o.max = a_5; return o; }(layout_info.max_percent, layout_info.preferred, 1, layout_info.min, layout_info.min_percent, layout_info.max); }(), 0, 1) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.col_or_row = a_0; o.span = a_1; o.constraint = a_2; return o; }(1, 1, [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.max_percent = a_1; o.max = a_2; o.min_percent = a_3; o.preferred = a_4; o.min = a_5; return o; }(1, 100, +3.4028234663852886e38, 0, 0, 0) + self->button_11.root_23_l_25_layoutinfo_h.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.min_percent = a_1; o.min = a_2; o.preferred = a_3; o.stretch = a_4; o.max_percent = a_5; return o; }(layout_info.max, layout_info.min_percent, layout_info.min, layout_info.preferred, 0, layout_info.max_percent); }()) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.constraint = a_0; o.col_or_row = a_1; o.span = a_2; return o; }([&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(EmptyVTable)->layout_info({SLINT_GET_ITEM_VTABLE(EmptyVTable), const_cast<slint::cbindgen_private::Empty*>(&self->list_view_12)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.min_percent = a_1; o.stretch = a_2; o.preferred = a_3; o.max_percent = a_4; o.max = a_5; return o; }(50, layout_info.min_percent, 1, layout_info.preferred, layout_info.max_percent, layout_info.max); }(), 0, 1) ) }.data(), 3 },0,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0));
                        });
    self->root_window_1_empty_3_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::grid_layout_info(slint::cbindgen_private::Slice<slint::cbindgen_private::GridLayoutCellData>{ std::array<slint::cbindgen_private::GridLayoutCellData, 3>{ slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.col_or_row = a_0; o.constraint = a_1; o.span = a_2; return o; }(0, [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.stretch = a_1; o.max = a_2; o.max_percent = a_3; o.min_percent = a_4; o.preferred = a_5; return o; }(0, 1, +3.4028234663852886e38, 100, 0, 0) + self->root_window_1_l_6_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.preferred = a_0; o.max = a_1; o.max_percent = a_2; o.stretch = a_3; o.min = a_4; o.min_percent = a_5; return o; }(layout_info.preferred, layout_info.max, layout_info.max_percent, 0, [&]{ auto minmax_lhs34 = 32;;auto minmax_rhs34 = self->root_window_1_l_6_layoutinfo_v.get().min;;return [&]() -> float { if (minmax_lhs34 > minmax_rhs34) { return minmax_lhs34; } else { return minmax_rhs34; }}(); }(), layout_info.min_percent); }(), 1) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.span = a_0; o.col_or_row = a_1; o.constraint = a_2; return o; }(1, 0, [&]{ auto layout_info = ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.max = a_1; o.preferred = a_2; o.stretch = a_3; o.min = a_4; o.min_percent = a_5; return o; }(100, +3.4028234663852886e38, 0, 1, 0, 0) + self->button_11.root_23_l_25_layoutinfo_v.get());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.min_percent = a_1; o.preferred = a_2; o.max = a_3; o.min = a_4; o.stretch = a_5; return o; }(layout_info.max_percent, layout_info.min_percent, layout_info.preferred, layout_info.max, [&]{ auto minmax_lhs1 = 32;;auto minmax_rhs1 = self->button_11.root_23_l_25_layoutinfo_v.get().min;;return [&]() -> float { if (minmax_lhs1 > minmax_rhs1) { return minmax_lhs1; } else { return minmax_rhs1; }}(); }(), 0); }()) ), slint::cbindgen_private::GridLayoutCellData ( [&](const auto &a_0, const auto &a_1, const auto &a_2){ slint::cbindgen_private::GridLayoutCellData o{}; o.span = a_0; o.constraint = a_1; o.col_or_row = a_2; return o; }(2, [&]{ auto layout_info = SLINT_GET_ITEM_VTABLE(EmptyVTable)->layout_info({SLINT_GET_ITEM_VTABLE(EmptyVTable), const_cast<slint::cbindgen_private::Empty*>(&self->list_view_12)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle());;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.min_percent = a_1; o.preferred = a_2; o.max = a_3; o.stretch = a_4; o.min = a_5; return o; }(layout_info.max_percent, layout_info.min_percent, layout_info.preferred, layout_info.max, 1, 50); }(), 2) ) }.data(), 3 },0,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(0, 0));
                        });
    self->root_window_1_inner_7_accepted.set_handler(
                    [this]([[maybe_unused]] slint::SharedString arg_0) {
                        [[maybe_unused]] auto self = this;
                        return [&]{ self->root_window_1_todo_added.call(arg_0); }();
                    });
    self->root_window_1_inner_7_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.preferred = a_1; o.stretch = a_2; o.max_percent = a_3; o.min = a_4; o.max = a_5; return o; }(0, 0, 1, 100, 0, +3.4028234663852886e38) + [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min = a_0; o.stretch = a_1; o.min_percent = a_2; o.max_percent = a_3; o.max = a_4; o.preferred = a_5; return o; }(self->root_window_1_placeholder_9_min_width.get(), self->root_window_1_placeholder_9_horizontal_stretch.get(), 0, 100, self->root_window_1_placeholder_9_max_width.get(), self->root_window_1_placeholder_9_preferred_width.get()));
                        });
    self->root_window_1_inner_7_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.min = a_1; o.min_percent = a_2; o.max = a_3; o.preferred = a_4; o.max_percent = a_5; return o; }(1, 0, 0, +3.4028234663852886e38, 0, 100) + [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.stretch = a_0; o.preferred = a_1; o.min = a_2; o.max_percent = a_3; o.min_percent = a_4; o.max = a_5; return o; }(self->root_window_1_placeholder_9_vertical_stretch.get(), self->root_window_1_placeholder_9_preferred_height.get(), self->root_window_1_placeholder_9_min_height.get(), 100, 0, self->root_window_1_placeholder_9_max_height.get()));
                        });
    self->root_window_1_input_10_preferred_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextInputVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextInputVTable), const_cast<slint::cbindgen_private::TextInput*>(&self->input_10)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).preferred;
                        });
    self->root_window_1_input_10_preferred_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextInputVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextInputVTable), const_cast<slint::cbindgen_private::TextInput*>(&self->input_10)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).preferred;
                        });
    self->root_window_1_l_6_layout_cache.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::solve_box_layout([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4){ slint::cbindgen_private::BoxLayoutData o{}; o.alignment = a_0; o.cells = a_1; o.size = a_2; o.padding = a_3; o.spacing = a_4; return o; }(slint::cbindgen_private::LayoutAlignment::Stretch, slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 1>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = self->root_window_1_inner_7_layoutinfo_h.get();;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.stretch = a_1; o.min = a_2; o.min_percent = a_3; o.preferred = a_4; o.max_percent = a_5; return o; }(layout_info.max, layout_info.stretch, [&]{ auto minmax_lhs6 = 50;;auto minmax_rhs6 = self->root_window_1_placeholder_9_min_width.get();;return [&]() -> float { if (minmax_lhs6 > minmax_rhs6) { return minmax_lhs6; } else { return minmax_rhs6; }}(); }(), layout_info.min_percent, layout_info.preferred, layout_info.max_percent); }()) ) }.data(), 1 }, self->text_edit_4.width.get(), [&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.end = a_0; o.begin = a_1; return o; }(8, 8), 0),slint::cbindgen_private::Slice<int>{ std::array<int, 0>{  }.data(), 0 });
                        });
    self->root_window_1_l_6_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::box_layout_info(slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 1>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = self->root_window_1_inner_7_layoutinfo_h.get();;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.min = a_1; o.preferred = a_2; o.max_percent = a_3; o.stretch = a_4; o.min_percent = a_5; return o; }(layout_info.max, [&]{ auto minmax_lhs6 = 50;;auto minmax_rhs6 = self->root_window_1_placeholder_9_min_width.get();;return [&]() -> float { if (minmax_lhs6 > minmax_rhs6) { return minmax_lhs6; } else { return minmax_rhs6; }}(); }(), layout_info.preferred, layout_info.max_percent, layout_info.stretch, layout_info.min_percent); }()) ) }.data(), 1 },0,[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(8, 8),slint::cbindgen_private::LayoutAlignment::Stretch);
                        });
    self->root_window_1_l_6_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::private_api::box_layout_info_ortho(slint::cbindgen_private::Slice<slint::cbindgen_private::BoxLayoutCellData>{ std::array<slint::cbindgen_private::BoxLayoutCellData, 1>{ slint::cbindgen_private::BoxLayoutCellData ( [&](const auto &a_0){ slint::cbindgen_private::BoxLayoutCellData o{}; o.constraint = a_0; return o; }([&]{ auto layout_info = self->root_window_1_inner_7_layoutinfo_v.get();;return [&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.min_percent = a_0; o.preferred = a_1; o.stretch = a_2; o.max_percent = a_3; o.min = a_4; o.max = a_5; return o; }(layout_info.min_percent, layout_info.preferred, layout_info.stretch, layout_info.max_percent, self->root_window_1_input_10_preferred_height.get(), layout_info.max); }()) ) }.data(), 1 },[&](const auto &a_0, const auto &a_1){ slint::cbindgen_private::Padding o{}; o.begin = a_0; o.end = a_1; return o; }(3, 3));
                        });
    self->root_window_1_layoutinfo_h.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (SLINT_GET_ITEM_VTABLE(WindowItemVTable)->layout_info({SLINT_GET_ITEM_VTABLE(WindowItemVTable), const_cast<slint::cbindgen_private::WindowItem*>(&self->root_window_1)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()) + ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max = a_0; o.preferred = a_1; o.stretch = a_2; o.min = a_3; o.min_percent = a_4; o.max_percent = a_5; return o; }(+3.4028234663852886e38, 0, 1, 0, 0, 100) + self->root_window_1_empty_3_layoutinfo_h.get()));
                        });
    self->root_window_1_layoutinfo_v.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (SLINT_GET_ITEM_VTABLE(WindowItemVTable)->layout_info({SLINT_GET_ITEM_VTABLE(WindowItemVTable), const_cast<slint::cbindgen_private::WindowItem*>(&self->root_window_1)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()) + ([&](const auto &a_0, const auto &a_1, const auto &a_2, const auto &a_3, const auto &a_4, const auto &a_5){ slint::cbindgen_private::LayoutInfo o{}; o.max_percent = a_0; o.min = a_1; o.min_percent = a_2; o.stretch = a_3; o.preferred = a_4; o.max = a_5; return o; }(100, 0, 0, 1, 0, +3.4028234663852886e38) + self->root_window_1_empty_3_layoutinfo_v.get()));
                        });
    self->root_window_1_placeholder_9_horizontal_stretch.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).stretch;
                        });
    self->root_window_1_placeholder_9_max_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).max;
                        });
    self->root_window_1_placeholder_9_max_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).max;
                        });
    self->root_window_1_placeholder_9_min_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).min;
                        });
    self->root_window_1_placeholder_9_min_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).min;
                        });
    self->root_window_1_placeholder_9_preferred_height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).preferred;
                        });
    self->root_window_1_placeholder_9_preferred_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Horizontal, &self->root->window().window_handle()).preferred;
                        });
    self->root_window_1_placeholder_9_vertical_stretch.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return SLINT_GET_ITEM_VTABLE(TextVTable)->layout_info({SLINT_GET_ITEM_VTABLE(TextVTable), const_cast<slint::cbindgen_private::Text*>(&self->placeholder_9)}, slint::cbindgen_private::Orientation::Vertical, &self->root->window().window_handle()).stretch;
                        });
    self->root_2.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return 0;
                        });
    self->root_2.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return 0;
                        });
    self->root_2.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((0 - 0) /(float) 2);
                        });
    self->root_2.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((0 - 0) /(float) 2);
                        });
    self->text_edit_4.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[1];
                        });
    self->text_edit_4.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[1];
                        });
    self->text_edit_4.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[0];
                        });
    self->text_edit_4.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[0];
                        });
    self->rectangle_5.background.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->input_10.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294967295e9); } else { return slint::Color::from_argb_encoded(+4.280229663e9); }}(); }}());
                        });
    self->rectangle_5.border_color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->input_10.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); } else { return [&]() -> slint::Color { if (self->input_10.has_focus.get()) { return slint::Color::from_argb_encoded(+4.28104316e9); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.281479472e9); } else { return slint::Color::from_argb_encoded(+4.294967295e9); }}(); }}(); }}());
                        });
    self->rectangle_5.border_radius.set(2);
    self->rectangle_5.border_width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> float { if (! self->input_10.enabled.get()) { return 0; } else { return [&]() -> float { if (self->input_10.has_focus.get()) { return 2; } else { return 1; }}(); }}();
                        });
    self->rectangle_5.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[1];
                        });
    self->rectangle_5.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[1];
                        });
    self->inner_7.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->root_window_1_empty_3_layout_cache_v.get()[1] - 3) - 3);
                        });
    self->inner_7.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_l_6_layout_cache.get()[1];
                        });
    self->inner_7.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_l_6_layout_cache.get()[0];
                        });
    self->inner_7.y.set(3);
    self->inner_clip_8.clip.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return true;
                        });
    self->inner_clip_8.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->root_window_1_empty_3_layout_cache_v.get()[1] - 3) - 3);
                        });
    self->inner_clip_8.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_l_6_layout_cache.get()[1];
                        });
    self->placeholder_9.color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! self->input_10.enabled.get()) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.288782237e9); } else { return slint::Color::from_argb_encoded(+4.29134868e9); }}(); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.284505692e9); } else { return slint::Color::from_argb_encoded(+4.291875024e9); }}(); }}());
                        });
    self->placeholder_9.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (1 * ((self->root_window_1_empty_3_layout_cache_v.get()[1] - 3) - 3));
                        });
    self->placeholder_9.text.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]() -> slint::SharedString { if ((self->input_10.text.get() == slint::SharedString(u8"")) && (self->input_10.preedit_text.get() == slint::SharedString(u8""))) { return slint::SharedString(u8""); } else { return slint::SharedString(u8""); }}();
                        });
    self->placeholder_9.vertical_alignment.set(slint::cbindgen_private::TextVerticalAlignment::Center);
    self->placeholder_9.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (1 * self->root_window_1_l_6_layout_cache.get()[1]);
                        });
    self->input_10.accepted.set_handler(
                    [this]() {
                        [[maybe_unused]] auto self = this;
                        return [&]{ self->root_window_1_inner_7_accepted.call(self->input_10.text.get()); }();
                    });
    self->input_10.color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (self->input_10.enabled.get()) { return self->root->global_Palette_46->neutralPrimary.get(); } else { return self->root->global_Palette_46->neutralTertiary.get(); }}());
                        });
    self->input_10.cursor_position_changed.set_handler(
                    [this]([[maybe_unused]] slint::private_api::Point arg_0) {
                        [[maybe_unused]] auto self = this;
                        return [&]{ [&]() -> void { if ((arg_0.x + self->root_window_1_input_10_computed_x.get()) < 8) { [&]{ self->root_window_1_input_10_computed_x.set(((- arg_0.x) + 8)); }(); } else { [&]() -> void { if ((arg_0.x + self->root_window_1_input_10_computed_x.get()) > (self->root_window_1_l_6_layout_cache.get()[1] - 8)) { [&]{ self->root_window_1_input_10_computed_x.set(((self->root_window_1_l_6_layout_cache.get()[1] - arg_0.x) - 8)); }(); } else { [&]{  }(); }}(); }}(); }();
                    });
    self->input_10.edited.set_handler(
                    [this]() {
                        [[maybe_unused]] auto self = this;
                        return [&]{ self->root_window_1_inner_7_edited.call(self->input_10.text.get()); }();
                    });
    self->input_10.enabled.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return true;
                        });
    self->input_10.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (1 * ((self->root_window_1_empty_3_layout_cache_v.get()[1] - 3) - 3));
                        });
    self->input_10.read_only.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return false;
                        });
    self->input_10.selection_background_color.set(slint::Color::from_argb_encoded(+4.286611584e9));
    self->input_10.selection_foreground_color.set(slint::Color::from_argb_encoded(+4.27819008e9));
    self->input_10.single_line.set(true);
    self->input_10.text_cursor_width.set(2);
    self->input_10.vertical_alignment.set(slint::cbindgen_private::TextVerticalAlignment::Center);
    self->input_10.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]{ auto minmax_lhs7 = self->root_window_1_l_6_layout_cache.get()[1];;auto minmax_rhs7 = self->root_window_1_input_10_preferred_width.get();;return [&]() -> float { if (minmax_lhs7 > minmax_rhs7) { return minmax_lhs7; } else { return minmax_rhs7; }}(); }();
                        });
    self->input_10.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return [&]{ auto minmax_lhs9 = 0;;auto minmax_rhs9 = [&]{ auto minmax_lhs8 = (self->root_window_1_l_6_layout_cache.get()[1] - self->input_10.width.get());;auto minmax_rhs8 = self->root_window_1_input_10_computed_x.get();;return [&]() -> float { if (minmax_lhs8 > minmax_rhs8) { return minmax_lhs8; } else { return minmax_rhs8; }}(); }();;return [&]() -> float { if (minmax_lhs9 < minmax_rhs9) { return minmax_lhs9; } else { return minmax_rhs9; }}(); }();
                        });
    self->button_11.root_23_clicked.set_handler(
                    [this]() {
                        [[maybe_unused]] auto self = this;
                        return [&]{ self->root_window_1_todo_added.call(self->input_10.text.get()); }();
                    });
    self->button_11.root_23.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[3];
                        });
    self->button_11.text_28.text.set(slint::SharedString(u8"Add Todo"));
    self->button_11.root_23.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[3];
                        });
    self->button_11.root_23.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[2];
                        });
    self->button_11.root_23.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[2];
                        });
    self->list_view_12.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[5];
                        });
    self->list_view_12.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[5];
                        });
    self->list_view_12.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[4];
                        });
    self->list_view_12.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[4];
                        });
    self->rectangle_13.border_color.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (! true) { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); } else { return [&]() -> slint::Color { if (false) { return slint::Color::from_argb_encoded(+4.28104316e9); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.281479472e9); } else { return slint::Color::from_argb_encoded(+4.294967295e9); }}(); }}(); }}());
                        });
    self->rectangle_13.border_radius.set(2);
    self->rectangle_13.border_width.set([&]() -> float { if (! true) { return 0; } else { return [&]() -> float { if (false) { return 2; } else { return 1; }}(); }}());
    self->rectangle_13.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_v.get()[5];
                        });
    self->rectangle_13.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->root_window_1_empty_3_layout_cache_h.get()[5];
                        });
    self->fli_14.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->root_window_1_empty_3_layout_cache_v.get()[5] - 16) - 4);
                        });
    self->fli_14.interactive.set(false);
    self->fli_14.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return ((self->root_window_1_empty_3_layout_cache_h.get()[5] - 16) - 4);
                        });
    self->fli_14.x.set(2);
    self->fli_14.y.set(2);
    self->fli_14.viewport.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->fli_14.height.get();
                        });
    self->fli_14.viewport.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->fli_14.width.get();
                        });
    self->fli_14.viewport.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return 0;
                        });
    self->fli_14.viewport.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return 0;
                        });
    self->vbar_20.root_42_enabled.set(true);
    self->vbar_20.root_42.height.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->fli_14.height.get();
                        });
    self->vbar_20.root_42_horizontal.set(false);
    self->vbar_20.root_42_maximum.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->fli_14.viewport.height.get() - self->fli_14.height.get());
                        });
    self->vbar_20.root_42_page_size.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->fli_14.height.get();
                        });
    self->vbar_20.root_42.width.set(16);
    self->vbar_20.root_42.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->fli_14.width.get() + 2);
                        });
    self->vbar_20.root_42.y.set(2);
    self->hbar_21.root_42_enabled.set(true);
    self->hbar_21.root_42.height.set(16);
    self->hbar_21.root_42_horizontal.set(true);
    self->hbar_21.root_42_maximum.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->fli_14.viewport.width.get() - self->fli_14.width.get());
                        });
    self->hbar_21.root_42_page_size.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->fli_14.width.get();
                        });
    self->hbar_21.root_42.width.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return self->fli_14.width.get();
                        });
    self->hbar_21.root_42.x.set(2);
    self->hbar_21.root_42.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->fli_14.height.get() + 2);
                        });
    self->corner_22.background.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return slint::Brush([&]() -> slint::Color { if (true) { return slint::Color::from_argb_encoded(0); } else { return [&]() -> slint::Color { if (! self->root->global_ColorSchemeSelector_45->dark_color_scheme.get()) { return slint::Color::from_argb_encoded(+4.294177521e9); } else { return slint::Color::from_argb_encoded(+4.281413937e9); }}(); }}());
                        });
    self->corner_22.height.set(16);
    self->corner_22.width.set(16);
    self->corner_22.x.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->fli_14.width.get() + 2);
                        });
    self->corner_22.y.set_binding([this]() {
                            [[maybe_unused]] auto self = this;
                            return (self->fli_14.height.get() + 2);
                        });
    self->repeater_0.set_model_binding([self] { (void)self; return std::make_shared<slint::private_api::ArrayModel<0,TodoItem>>(); });
}

inline auto MainWindow::user_init () -> void{
    [[maybe_unused]] auto self = this;
    this->button_11.user_init();
    this->vbar_20.user_init();
    this->hbar_21.user_init();
}

inline auto MainWindow::layout_info (slint::cbindgen_private::Orientation o) const -> slint::cbindgen_private::LayoutInfo{
    [[maybe_unused]] auto self = this;
    return o == slint::cbindgen_private::Orientation::Horizontal ? self->root_window_1_layoutinfo_h.get() : self->root_window_1_layoutinfo_v.get();
}

inline auto MainWindow::accessible_role (uintptr_t index) const -> slint::cbindgen_private::AccessibleRole{
    [[maybe_unused]] auto self = this;
    switch (index) {
        case 3: return slint::cbindgen_private::AccessibleRole::Button;
        case 8: return slint::cbindgen_private::AccessibleRole::Text;
    }
    if (index == 3) {
        return self->button_11.accessible_role(0);
    } else if (index >= 10 && index < 15) {
        return self->button_11.accessible_role(index - 9);
    } else if (index == 18) {
        return self->vbar_20.accessible_role(0);
    } else if (index >= 23 && index < 25) {
        return self->vbar_20.accessible_role(index - 22);
    } else if (index == 19) {
        return self->hbar_21.accessible_role(0);
    } else if (index >= 25 && index < 27) {
        return self->hbar_21.accessible_role(index - 24);
    } else return {};
}

inline auto MainWindow::accessible_string_property (uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what) const -> slint::SharedString{
    [[maybe_unused]] auto self = this;
    switch ((index << 8) | uintptr_t(what)) {
        case (3 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Label): return slint::SharedString(u8"Add Todo");
        case (8 << 8) | uintptr_t(slint::cbindgen_private::AccessibleStringProperty::Label): return self->placeholder_9.text.get();
    }
    if (index == 3) {
        return self->button_11.accessible_string_property(0, what);
    } else if (index >= 10 && index < 15) {
        return self->button_11.accessible_string_property(index - 9, what);
    } else if (index == 18) {
        return self->vbar_20.accessible_string_property(0, what);
    } else if (index >= 23 && index < 25) {
        return self->vbar_20.accessible_string_property(index - 22, what);
    } else if (index == 19) {
        return self->hbar_21.accessible_string_property(0, what);
    } else if (index >= 25 && index < 27) {
        return self->hbar_21.accessible_string_property(index - 24, what);
    } else return {};
}

inline auto MainWindow::visit_dynamic_children (intptr_t dyn_index, [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor) const -> uint64_t{
        auto self = this;
        switch(dyn_index) { 
        case 1:  {
                        return self->button_11.visit_dynamic_children(dyn_index - 1, order, visitor);
                    }
        case 0: {
                self->repeater_0.ensure_updated_listview(self, &self->fli_14.viewport.width, &self->fli_14.viewport.height, &self->fli_14.viewport.y, self->fli_14.width.get(), self->fli_14.height.get());
                return self->repeater_0.visit(order, visitor);
            } };
        std::abort();
}

inline auto MainWindow::subtree_range (uintptr_t dyn_index) const -> slint::private_api::IndexRange{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 1:  {
                        return self->button_11.subtree_range(dyn_index - 1);
                    }
        case 0: {
                self->repeater_0.ensure_updated_listview(self, &self->fli_14.viewport.width, &self->fli_14.viewport.height, &self->fli_14.viewport.y, self->fli_14.width.get(), self->fli_14.height.get());
                return self->repeater_0.index_range();
            } };
        std::abort();
}

inline auto MainWindow::subtree_component (uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) const -> void{
    [[maybe_unused]] auto self = this;
        switch(dyn_index) { 
        case 1:  {
                        self->button_11.subtree_component(dyn_index - 1, subtree_index, result);
                        return;
                    }
        case 0: {
                self->repeater_0.ensure_updated_listview(self, &self->fli_14.viewport.width, &self->fli_14.viewport.height, &self->fli_14.viewport.y, self->fli_14.width.get(), self->fli_14.height.get());
                *result = self->repeater_0.component_at(subtree_index);
                return;
            } };
        std::abort();
}

inline auto MainWindow::visit_children (slint::private_api::ComponentRef component, intptr_t index, slint::private_api::TraversalOrder order, slint::private_api::ItemVisitorRefMut visitor) -> uint64_t{
    static const auto dyn_visit = [] (const uint8_t *base,  [[maybe_unused]] slint::private_api::TraversalOrder order, [[maybe_unused]] slint::private_api::ItemVisitorRefMut visitor, [[maybe_unused]] uintptr_t dyn_index) -> uint64_t {
        [[maybe_unused]] auto self = reinterpret_cast<const MainWindow*>(base);
        return self->visit_dynamic_children(dyn_index, order, visitor);
    };
    auto self_rc = reinterpret_cast<const MainWindow*>(component.instance)->self_weak.lock()->into_dyn();
    return slint::cbindgen_private::slint_visit_item_tree(&self_rc, get_item_tree(component) , index, order, visitor, dyn_visit);
}

inline auto MainWindow::get_item_ref (slint::private_api::ComponentRef component, uintptr_t index) -> slint::private_api::ItemRef{
    return slint::private_api::get_item_ref(component, get_item_tree(component), item_array(), index);
}

inline auto MainWindow::get_subtree_range ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index) -> slint::private_api::IndexRange{
    auto self = reinterpret_cast<const MainWindow*>(component.instance);
    return self->subtree_range(dyn_index);
}

inline auto MainWindow::get_subtree_component ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] uintptr_t dyn_index, [[maybe_unused]] uintptr_t subtree_index, [[maybe_unused]] slint::private_api::ComponentWeak *result) -> void{
    auto self = reinterpret_cast<const MainWindow*>(component.instance);
    self->subtree_component(dyn_index, subtree_index, result);
}

inline auto MainWindow::get_item_tree (slint::private_api::ComponentRef) -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    return item_tree();
}

inline auto MainWindow::parent_node ([[maybe_unused]] slint::private_api::ComponentRef component, [[maybe_unused]] slint::private_api::ItemWeak *result) -> void{
}

inline auto MainWindow::subtree_index ([[maybe_unused]] slint::private_api::ComponentRef component) -> uintptr_t{
    return std::numeric_limits<uintptr_t>::max();
}

inline auto MainWindow::item_tree () -> slint::cbindgen_private::Slice<slint::private_api::ItemTreeNode>{
    static const slint::private_api::ItemTreeNode children[] {
        slint::private_api::make_item_node(1, 1, 0, 0, false), 
slint::private_api::make_item_node(3, 2, 0, 1, false), 
slint::private_api::make_item_node(2, 5, 1, 2, false), 
slint::private_api::make_item_node(6, 10, 1, 3, true), 
slint::private_api::make_item_node(5, 16, 1, 4, false), 
slint::private_api::make_item_node(0, 7, 2, 5, false), 
slint::private_api::make_item_node(1, 7, 2, 6, false), 
slint::private_api::make_item_node(2, 8, 6, 7, false), 
slint::private_api::make_item_node(0, 10, 7, 8, true), 
slint::private_api::make_item_node(0, 10, 7, 9, false), 
slint::private_api::make_item_node(0, 16, 3, 10, false), 
slint::private_api::make_dyn_node(1, 3), 
slint::private_api::make_item_node(0, 16, 3, 11, true), 
slint::private_api::make_item_node(0, 16, 3, 12, false), 
slint::private_api::make_item_node(0, 16, 3, 13, false), 
slint::private_api::make_item_node(0, 16, 3, 14, false), 
slint::private_api::make_item_node(0, 21, 4, 15, false), 
slint::private_api::make_item_node(1, 21, 4, 16, false), 
slint::private_api::make_item_node(2, 23, 4, 17, false), 
slint::private_api::make_item_node(2, 25, 4, 18, false), 
slint::private_api::make_item_node(0, 27, 4, 19, false), 
slint::private_api::make_item_node(1, 22, 17, 20, false), 
slint::private_api::make_dyn_node(0, 21), 
slint::private_api::make_item_node(0, 25, 18, 21, false), 
slint::private_api::make_item_node(0, 25, 18, 22, false), 
slint::private_api::make_item_node(0, 27, 19, 23, false), 
slint::private_api::make_item_node(0, 27, 19, 24, false) };
    return { const_cast<slint::private_api::ItemTreeNode*>(children), std::size(children) };
}

inline auto MainWindow::item_array () -> const slint::private_api::ItemArray{
    static const slint::private_api::ItemArrayEntry items[] {
        { SLINT_GET_ITEM_VTABLE(WindowItemVTable),  offsetof(MainWindow, root_window_1) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable),  offsetof(MainWindow, root_2) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable),  offsetof(MainWindow, text_edit_4) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable), offsetof(MainWindow, button_11) +  offsetof(Button_root_23, root_23) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable),  offsetof(MainWindow, list_view_12) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable),  offsetof(MainWindow, rectangle_5) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable),  offsetof(MainWindow, inner_7) }, 
{ SLINT_GET_ITEM_VTABLE(ClipVTable),  offsetof(MainWindow, inner_clip_8) }, 
{ SLINT_GET_ITEM_VTABLE(TextVTable),  offsetof(MainWindow, placeholder_9) }, 
{ SLINT_GET_ITEM_VTABLE(TextInputVTable),  offsetof(MainWindow, input_10) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(MainWindow, button_11) +  offsetof(Button_root_23, rectangle_24) }, 
{ SLINT_GET_ITEM_VTABLE(TextVTable), offsetof(MainWindow, button_11) +  offsetof(Button_root_23, text_28) }, 
{ SLINT_GET_ITEM_VTABLE(TouchAreaVTable), offsetof(MainWindow, button_11) +  offsetof(Button_root_23, touch_29) }, 
{ SLINT_GET_ITEM_VTABLE(FocusScopeVTable), offsetof(MainWindow, button_11) +  offsetof(Button_root_23, fs_30) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(MainWindow, button_11) +  offsetof(Button_root_23, rectangle_31) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable),  offsetof(MainWindow, rectangle_13) }, 
{ SLINT_GET_ITEM_VTABLE(FlickableVTable),  offsetof(MainWindow, fli_14) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(MainWindow, vbar_20) +  offsetof(ScrollBar_root_42, root_42) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(MainWindow, hbar_21) +  offsetof(ScrollBar_root_42, root_42) }, 
{ SLINT_GET_ITEM_VTABLE(RectangleVTable),  offsetof(MainWindow, corner_22) }, 
{ SLINT_GET_ITEM_VTABLE(EmptyVTable), offsetof(slint::cbindgen_private::Flickable, viewport) +  offsetof(MainWindow, fli_14) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(MainWindow, vbar_20) +  offsetof(ScrollBar_root_42, handle_43) }, 
{ SLINT_GET_ITEM_VTABLE(TouchAreaVTable), offsetof(MainWindow, vbar_20) +  offsetof(ScrollBar_root_42, touch_area_44) }, 
{ SLINT_GET_ITEM_VTABLE(BorderRectangleVTable), offsetof(MainWindow, hbar_21) +  offsetof(ScrollBar_root_42, handle_43) }, 
{ SLINT_GET_ITEM_VTABLE(TouchAreaVTable), offsetof(MainWindow, hbar_21) +  offsetof(ScrollBar_root_42, touch_area_44) } };
    return { const_cast<slint::private_api::ItemArrayEntry*>(items), std::size(items) };
}

inline auto MainWindow::layout_info ([[maybe_unused]] slint::private_api::ComponentRef component, slint::cbindgen_private::Orientation o) -> slint::cbindgen_private::LayoutInfo{
    return reinterpret_cast<const MainWindow*>(component.instance)->layout_info(o);
}

inline auto MainWindow::accessible_role ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index) -> slint::cbindgen_private::AccessibleRole{
    return reinterpret_cast<const MainWindow*>(component.instance)->accessible_role(index);
}

inline auto MainWindow::accessible_string_property ([[maybe_unused]] slint::private_api::ComponentRef component, uintptr_t index, slint::cbindgen_private::AccessibleStringProperty what, slint::SharedString *result) -> void{
    *result = reinterpret_cast<const MainWindow*>(component.instance)->accessible_string_property(index, what);
}

inline auto MainWindow::create () -> slint::ComponentHandle<MainWindow>{
    auto self_rc = vtable::VRc<slint::private_api::ComponentVTable, MainWindow>::make();
    auto self = const_cast<MainWindow *>(&*self_rc);
    self->self_weak = vtable::VWeak(self_rc).into_dyn();
    self->m_window.window_handle().set_component(*self_rc);
    self->m_window.window_handle().register_component(self, self->item_array());
    self->init(self, self->self_weak, 0, 1 );
    self->user_init();
    return slint::ComponentHandle<MainWindow>{ self_rc };
}

inline MainWindow::~MainWindow (){
    auto self = this;
    self->m_window.window_handle().unregister_component(self, item_array());
}

inline auto MainWindow::invoke_todo_added (slint::SharedString arg_0) const -> void{
    [[maybe_unused]] auto self = this;
    return self->root_window_1_todo_added.call(arg_0);
}

template<std::invocable<slint::SharedString> Functor> inline auto MainWindow::on_todo_added (Functor && callback_handler) const{
    [[maybe_unused]] auto self = this;
    self->root_window_1_todo_added.set_handler(std::forward<Functor>(callback_handler));
}

inline auto MainWindow::show (){
    m_window.show();
}

inline auto MainWindow::hide (){
    m_window.hide();
}

inline auto MainWindow::window () const -> slint::Window&{
    return const_cast<MainWindow *>(this)->m_window;
}

inline auto MainWindow::run (){
    show();
    slint::run_event_loop();
    hide();
}

[[maybe_unused]] constexpr slint::private_api::VersionCheckHelper<1, 0, 0> THE_SAME_VERSION_MUST_BE_USED_FOR_THE_COMPILER_AND_THE_RUNTIME = slint::private_api::VersionCheckHelper<SLINT_VERSION_MAJOR, SLINT_VERSION_MINOR, SLINT_VERSION_PATCH>();
