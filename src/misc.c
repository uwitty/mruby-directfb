#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_misc.h"

// ============================================================================
// IDirectFB::Region object - DFBRegion

struct mrb_directfb_region_data {
    DFBRegion region;
};

static void mrb_directfb_region_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_region_data* data = (struct mrb_directfb_region_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_region_type = {"Region", mrb_directfb_region_free};

mrb_value mrb_directfb_region_wrap(mrb_state* mrb, struct RClass* c, int x1, int y1, int x2, int y2)
{
    struct mrb_directfb_region_data* data = (struct mrb_directfb_region_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_region_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    data->region.x1 = x1;
    data->region.y1 = y1;
    data->region.x2 = x2;
    data->region.y2 = y2;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_region_type, data));
}

mrb_value mrb_directfb_region_value(mrb_state* mrb, int x1, int y1, int x2, int y2)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "Region")));
    return mrb_directfb_region_wrap(mrb, c, x1, y1, x2, y2);
}

DFBRegion* mrb_directfb_region(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_region_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_region_type, struct mrb_directfb_region_data);
    return (data != NULL)? &data->region : NULL;
}

static mrb_value region_new(mrb_state *mrb, mrb_value self)
{
    mrb_int x1, y1, x2, y2;
    mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
    return mrb_directfb_region_wrap(mrb, mrb_class_ptr(self), x1, y1, x2, y2);
}

static mrb_value region_x1(mrb_state *mrb, mrb_value self)
{
    DFBRegion* region = mrb_directfb_region(mrb, self);
    return (region != NULL)? mrb_fixnum_value(region->x1) : mrb_nil_value();
}

static mrb_value region_y1(mrb_state *mrb, mrb_value self)
{
    DFBRegion* region = mrb_directfb_region(mrb, self);
    return (region != NULL)? mrb_fixnum_value(region->y1) : mrb_nil_value();
}

static mrb_value region_x2(mrb_state *mrb, mrb_value self)
{
    DFBRegion* region = mrb_directfb_region(mrb, self);
    return (region != NULL)? mrb_fixnum_value(region->x2) : mrb_nil_value();
}

static mrb_value region_y2(mrb_state *mrb, mrb_value self)
{
    DFBRegion* region = mrb_directfb_region(mrb, self);
    return (region != NULL)? mrb_fixnum_value(region->y2) : mrb_nil_value();
}

static void define_region(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* region = mrb_define_class_under(mrb, outer, "Region", mrb->object_class);

    mrb_define_class_method(mrb, region, "new", region_new, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, region, "x1", region_x1, MRB_ARGS_NONE());
    mrb_define_method(mrb, region, "y1", region_y1, MRB_ARGS_NONE());
    mrb_define_method(mrb, region, "x2", region_x2, MRB_ARGS_NONE());
    mrb_define_method(mrb, region, "y2", region_y2, MRB_ARGS_NONE());
    //mrb_define_method(mrb, region, "x1=", region_x1_set, MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, region, "y1=", region_y1_set, MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, region, "x2=", region_x2_set, MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, region, "y2=", region_y2_set, MRB_ARGS_REQ(1));
}

// ============================================================================
// IDirectFB::Rectangle object - DFBRectangle

struct mrb_directfb_rectangle_data {
    DFBRectangle rectangle;
};

static void mrb_directfb_rectangle_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_rectangle_data* data = (struct mrb_directfb_rectangle_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_rectangle_type = {"Rectangle", mrb_directfb_rectangle_free};

mrb_value mrb_directfb_rectangle_wrap(mrb_state* mrb, struct RClass* c, int x, int y, int w, int h)
{
    struct mrb_directfb_rectangle_data* data = (struct mrb_directfb_rectangle_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_rectangle_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    data->rectangle.x = x;
    data->rectangle.y = y;
    data->rectangle.w = w;
    data->rectangle.h = h;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_rectangle_type, data));
}

mrb_value mrb_directfb_rectangle_value(mrb_state* mrb, int x, int y, int w, int h)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "Rectangle")));
    return mrb_directfb_rectangle_wrap(mrb, c, x, y, w, h);
}

DFBRectangle* mrb_directfb_rectangle(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_rectangle_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_rectangle_type, struct mrb_directfb_rectangle_data);
    return (data != NULL)? &data->rectangle : NULL;
}

static mrb_value rectangle_new(mrb_state *mrb, mrb_value self)
{
    mrb_int x, y, w, h;
    mrb_get_args(mrb, "iiii", &x, &y, &w, &h);
    return mrb_directfb_rectangle_wrap(mrb, mrb_class_ptr(self), x, y, w, h);
}

static mrb_value rectangle_x(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->x) : mrb_nil_value();
}

static mrb_value rectangle_y(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->y) : mrb_nil_value();
}

static mrb_value rectangle_w(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->w) : mrb_nil_value();
}

static mrb_value rectangle_h(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->h) : mrb_nil_value();
}

static mrb_value rectangle_x_set(mrb_state *mrb, mrb_value self)
{
    mrb_int x;
    mrb_get_args(mrb, "i", &x);
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    if (rect != NULL) {
        rect->x = x;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_y_set(mrb_state *mrb, mrb_value self)
{
    mrb_int y;
    mrb_get_args(mrb, "i", &y);
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    if (rect != NULL) {
        rect->y = y;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_w_set(mrb_state *mrb, mrb_value self)
{
    mrb_int w;
    mrb_get_args(mrb, "i", &w);
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    if (rect != NULL) {
        rect->w = w;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_h_set(mrb_state *mrb, mrb_value self)
{
    mrb_int h;
    mrb_get_args(mrb, "i", &h);
    DFBRectangle* rect = mrb_directfb_rectangle(mrb, self);
    if (rect != NULL) {
        rect->h = h;
    }
    return mrb_nil_value();
}

static void define_rectangle(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* rectangle = mrb_define_class_under(mrb, outer, "Rectangle", mrb->object_class);
    mrb_define_class_method(mrb, rectangle, "new", rectangle_new, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, rectangle, "x", rectangle_x, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "y", rectangle_y, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "w", rectangle_w, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "h", rectangle_h, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "x=", rectangle_x_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle, "y=", rectangle_y_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle, "w=", rectangle_w_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle, "h=", rectangle_h_set, MRB_ARGS_REQ(1));
}

// ============================================================================
// IDirectFB::InputDeviceKeymapEntry object - DFBInputDeviceKeymapEntry

struct mrb_directfb_input_device_keymap_entry_data {
    DFBInputDeviceKeymapEntry input_device_keymap_entry;
};

static void mrb_directfb_input_device_keymap_entry_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_input_device_keymap_entry_data* data = (struct mrb_directfb_input_device_keymap_entry_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_input_device_keymap_entry_type = {"InputDeviceKeymapEntry", mrb_directfb_input_device_keymap_entry_free};

mrb_value mrb_directfb_input_device_keymap_entry_wrap(mrb_state* mrb, struct RClass* c, DFBInputDeviceKeymapEntry* entry)
{
    struct mrb_directfb_input_device_keymap_entry_data* data = (struct mrb_directfb_input_device_keymap_entry_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_input_device_keymap_entry_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    memcpy(&data->input_device_keymap_entry, entry, sizeof(data->input_device_keymap_entry));
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_input_device_keymap_entry_type, data));
}

mrb_value mrb_directfb_input_device_keymap_entry_value(mrb_state* mrb, DFBInputDeviceKeymapEntry* entry)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "InputDeviceKeymapEntry")));
    return mrb_directfb_input_device_keymap_entry_wrap(mrb, c, entry);
}

DFBInputDeviceKeymapEntry* mrb_directfb_input_device_keymap_entry(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_input_device_keymap_entry_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_input_device_keymap_entry_type, struct mrb_directfb_input_device_keymap_entry_data);
    return (data != NULL)? &data->input_device_keymap_entry : NULL;
}

static mrb_value input_device_keymap_entry_code(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_input_device_keymap_entry(mrb, self);
    return (entry != NULL)? mrb_fixnum_value(entry->code) : mrb_nil_value();
}

static mrb_value input_device_keymap_entry_locks(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_input_device_keymap_entry(mrb, self);
    return (entry != NULL)? mrb_fixnum_value(entry->locks) : mrb_nil_value();
}

static mrb_value input_device_keymap_entry_identifier(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_input_device_keymap_entry(mrb, self);
    return (entry != NULL)? mrb_fixnum_value(entry->identifier) : mrb_nil_value();
}

static mrb_value input_device_keymap_entry_symbols(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_input_device_keymap_entry(mrb, self);
    if (entry != NULL) {
        mrb_value* a = mrb_malloc(mrb, (DIKSI_LAST+1)*sizeof(mrb_value));
        int i;
        for (i = 0; i < DIKSI_LAST+1; i++) {
            a[i] = mrb_fixnum_value(entry->symbols[i]);
        }
        return mrb_ary_new_from_values(mrb, DIKSI_LAST+1, a);
    }
    return mrb_nil_value();
}

static void define_input_device_keymap_entry(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* input_device_keymap_entry = mrb_define_class_under(mrb, outer, "InputDeviceKeymapEntry", mrb->object_class);
    mrb_define_method(mrb, input_device_keymap_entry, "code", input_device_keymap_entry_code, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device_keymap_entry, "locks", input_device_keymap_entry_locks, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device_keymap_entry, "identifier", input_device_keymap_entry_identifier, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device_keymap_entry, "symbols", input_device_keymap_entry_symbols, MRB_ARGS_NONE());
}

// ============================================================================
// IDirectFB::InputEvent object - DFBInputEvent

struct mrb_directfb_input_event_data {
    DFBInputEvent input_event;
};

static void mrb_directfb_input_event_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_input_event_data* data = (struct mrb_directfb_input_event_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_input_event_type = {"InputEvent", mrb_directfb_input_event_free};

mrb_value mrb_directfb_input_event_wrap(mrb_state* mrb, struct RClass* c, DFBInputEvent* input_event)
{
    struct mrb_directfb_input_event_data* data = (struct mrb_directfb_input_event_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_input_event_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    memcpy(&data->input_event, input_event, sizeof(data->input_event));
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_input_event_type, data));
}

mrb_value mrb_directfb_input_event_value(mrb_state* mrb, DFBInputEvent* input_event)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "InputEvent")));
    return mrb_directfb_input_event_wrap(mrb, c, input_event);
}

DFBInputEvent* mrb_directfb_input_event(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_input_event_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_input_event_type, struct mrb_directfb_input_event_data);
    return (data != NULL)? &data->input_event : NULL;
}

static mrb_value input_event_clazz(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->clazz) : mrb_nil_value();
}

static mrb_value input_event_type(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->type) : mrb_nil_value();
}

static mrb_value input_event_device_id(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->device_id) : mrb_nil_value();
}

static mrb_value input_event_flags(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->flags) : mrb_nil_value();
}

static mrb_value input_event_timestamp(mrb_state *mrb, mrb_value self)
{
    mrb_value hash = mrb_hash_new(mrb);
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "tv_sec")), mrb_fixnum_value(input_event->timestamp.tv_sec));
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "tv_usec")), mrb_fixnum_value(input_event->timestamp.tv_usec));
    return hash;
}

static mrb_value input_event_key_code(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->key_code) : mrb_nil_value();
}

static mrb_value input_event_key_id(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->key_id) : mrb_nil_value();
}

static mrb_value input_event_key_symbol(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->key_symbol) : mrb_nil_value();
}

static mrb_value input_event_modifiers(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->modifiers) : mrb_nil_value();
}

static mrb_value input_event_locks(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->locks) : mrb_nil_value();
}

static mrb_value input_event_button(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->button) : mrb_nil_value();
}

static mrb_value input_event_buttons(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->buttons) : mrb_nil_value();
}

static mrb_value input_event_axis(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->axis) : mrb_nil_value();
}

static mrb_value input_event_axisabs(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->axisabs) : mrb_nil_value();
}

static mrb_value input_event_axisrel(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->axisrel) : mrb_nil_value();
}

static mrb_value input_event_min(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->min) : mrb_nil_value();
}

static mrb_value input_event_max(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->max) : mrb_nil_value();
}

static void define_input_event(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* input_event = mrb_define_class_under(mrb, outer, "InputEvent", mrb->object_class);
    mrb_define_method(mrb, input_event, "clazz", input_event_clazz, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "type", input_event_type, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "device_id", input_event_device_id, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "flags", input_event_flags, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "timestamp", input_event_timestamp, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "key_code", input_event_key_code, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "key_id", input_event_key_id, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "key_symbol", input_event_key_symbol, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "modifiers", input_event_modifiers, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "locks", input_event_locks, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "button", input_event_button, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "buttons", input_event_buttons, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "axis", input_event_axis, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "axisabs", input_event_axisabs, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "axisrel", input_event_axisrel, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "min", input_event_min, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_event, "max", input_event_max, MRB_ARGS_NONE());
}

// ============================================================================
// IDirectFB::WindowEvent object - DFBWindowEvent

struct mrb_directfb_window_event_data {
    DFBWindowEvent window_event;
};

static void mrb_directfb_window_event_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_window_event_data* data = (struct mrb_directfb_window_event_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_window_event_type = {"WindowEvent", mrb_directfb_window_event_free};

mrb_value mrb_directfb_window_event_wrap(mrb_state* mrb, struct RClass* c, DFBWindowEvent* window_event)
{
    struct mrb_directfb_window_event_data* data = (struct mrb_directfb_window_event_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_window_event_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    memcpy(&data->window_event, window_event, sizeof(data->window_event));
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_window_event_type, data));
}

mrb_value mrb_directfb_window_event_value(mrb_state* mrb, DFBWindowEvent* window_event)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "WindowEvent")));
    return mrb_directfb_window_event_wrap(mrb, c, window_event);
}

DFBWindowEvent* mrb_directfb_get_window_event(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_window_event_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_window_event_type, struct mrb_directfb_window_event_data);
    return (data != NULL)? &data->window_event : NULL;
}

static mrb_value window_event_clazz(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->clazz) : mrb_nil_value();
}

static mrb_value window_event_type(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->type) : mrb_nil_value();
}

static mrb_value window_event_flags(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->flags) : mrb_nil_value();
}

static mrb_value window_event_window_id(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->window_id) : mrb_nil_value();
}

static mrb_value window_event_x(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->x) : mrb_nil_value();
}

static mrb_value window_event_y(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->y) : mrb_nil_value();
}

static mrb_value window_event_cx(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->cx) : mrb_nil_value();
}

static mrb_value window_event_cy(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->cy) : mrb_nil_value();
}

static mrb_value window_event_step(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->step) : mrb_nil_value();
}

static mrb_value window_event_w(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->w) : mrb_nil_value();
}

static mrb_value window_event_h(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->h) : mrb_nil_value();
}

static mrb_value window_event_key_code(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->key_code) : mrb_nil_value();
}

static mrb_value window_event_key_id(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->key_id) : mrb_nil_value();
}

static mrb_value window_event_key_symbol(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->key_symbol) : mrb_nil_value();
}

static mrb_value window_event_modifiers(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->modifiers) : mrb_nil_value();
}

static mrb_value window_event_locks(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->locks) : mrb_nil_value();
}

static mrb_value window_event_button(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->button) : mrb_nil_value();
}

static mrb_value window_event_buttons(mrb_state *mrb, mrb_value self)
{
    DFBWindowEvent* window_event = mrb_directfb_get_window_event(mrb, self);
    return (window_event != NULL)? mrb_fixnum_value(window_event->buttons) : mrb_nil_value();
}

static mrb_value window_event_timestamp(mrb_state *mrb, mrb_value self)
{
    mrb_value hash = mrb_hash_new(mrb);
    DFBInputEvent* input_event = mrb_directfb_input_event(mrb, self);
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "tv_sec")), mrb_fixnum_value(input_event->timestamp.tv_sec));
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "tv_usec")), mrb_fixnum_value(input_event->timestamp.tv_usec));
    return hash;
}

static void define_window_event(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* window_event = mrb_define_class_under(mrb, outer, "WindowEvent", mrb->object_class);
    mrb_define_method(mrb, window_event, "clazz", window_event_clazz, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "type", window_event_type, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "flags", window_event_flags, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "window_id", window_event_window_id, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "x", window_event_x, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "y", window_event_y, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "cx", window_event_cx, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "cy", window_event_cy, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "step", window_event_step, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "w", window_event_w, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "h", window_event_h, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "key_code", window_event_key_code, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "key_id", window_event_key_id, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "key_symbol", window_event_key_symbol, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "modifiers", window_event_modifiers, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "locks", window_event_locks, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "button", window_event_button, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "buttons", window_event_buttons, MRB_ARGS_NONE());
    mrb_define_method(mrb, window_event, "timestamp", window_event_timestamp, MRB_ARGS_NONE());
}

// ============================================================================
// IDirectFB::VideoProviderEvent object - DFBVideoProviderEvent

struct mrb_directfb_video_provider_event_data {
    DFBVideoProviderEvent video_provider_event;
};

static void mrb_directfb_video_provider_event_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_video_provider_event_data* data = (struct mrb_directfb_video_provider_event_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_video_provider_event_type = {"VideoProviderEvent", mrb_directfb_video_provider_event_free};

mrb_value mrb_directfb_video_provider_event_wrap(mrb_state* mrb, struct RClass* c, DFBVideoProviderEvent* video_provider_event)
{
    struct mrb_directfb_video_provider_event_data* data = (struct mrb_directfb_video_provider_event_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_video_provider_event_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    memcpy(&data->video_provider_event, video_provider_event, sizeof(data->video_provider_event));
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_video_provider_event_type, data));
}

mrb_value mrb_directfb_video_provider_event_value(mrb_state* mrb, DFBVideoProviderEvent* video_provider_event)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "VideoProviderEvent")));
    return mrb_directfb_video_provider_event_wrap(mrb, c, video_provider_event);
}

DFBVideoProviderEvent* mrb_directfb_get_video_provider_event(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_video_provider_event_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_video_provider_event_type, struct mrb_directfb_video_provider_event_data);
    return (data != NULL)? &data->video_provider_event : NULL;
}

static mrb_value video_provider_event_clazz(mrb_state *mrb, mrb_value self)
{
    DFBVideoProviderEvent* video_provider_event = mrb_directfb_get_video_provider_event(mrb, self);
    return (video_provider_event != NULL)? mrb_fixnum_value(video_provider_event->clazz) : mrb_nil_value();
}

static mrb_value video_provider_event_type(mrb_state *mrb, mrb_value self)
{
    DFBVideoProviderEvent* video_provider_event = mrb_directfb_get_video_provider_event(mrb, self);
    return (video_provider_event != NULL)? mrb_fixnum_value(video_provider_event->type) : mrb_nil_value();
}

static mrb_value video_provider_event_data_type(mrb_state *mrb, mrb_value self)
{
    DFBVideoProviderEvent* video_provider_event = mrb_directfb_get_video_provider_event(mrb, self);
    return (video_provider_event != NULL)? mrb_fixnum_value(video_provider_event->data_type) : mrb_nil_value();
}

static mrb_value video_provider_event_data(mrb_state *mrb, mrb_value self)
{
    DFBVideoProviderEvent* event = mrb_directfb_get_video_provider_event(mrb, self);
    if (event != NULL) {
        mrb_value a[4];
        a[0] = mrb_fixnum_value(event->data[0]);
        a[1] = mrb_fixnum_value(event->data[1]);
        a[2] = mrb_fixnum_value(event->data[2]);
        a[3] = mrb_fixnum_value(event->data[3]);
        return mrb_ary_new_from_values(mrb, 4, a);
    }
    return mrb_nil_value();
}

static void define_video_provider_event(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* video_provider_event = mrb_define_class_under(mrb, outer, "VideoProviderEvent", mrb->object_class);
    mrb_define_method(mrb, video_provider_event, "clazz", video_provider_event_clazz, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider_event, "type", video_provider_event_type, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider_event, "data_type", video_provider_event_data_type, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider_event, "data", video_provider_event_data, MRB_ARGS_NONE());
}

// ============================================================================

mrb_value mrb_directfb_event_value(mrb_state* mrb, DFBEvent* event)
{
    if (event == NULL) {
        return mrb_nil_value();
    }

    switch(event->clazz) {
        case DFEC_INPUT:
            return mrb_directfb_input_event_value(mrb, &event->input);

        case DFEC_WINDOW:
            return mrb_directfb_window_event_value(mrb, &event->window);

        case DFEC_VIDEOPROVIDER:
            return mrb_directfb_video_provider_event_value(mrb, &event->videoprovider);

        case DFEC_USER:
        case DFEC_UNIVERSAL:
        case DFEC_NONE:
        default:
            return mrb_nil_value();
    };
}

void mrb_directfb_event(mrb_state* mrb, mrb_value event_object, DFBEvent* event)
{
    memset(event, 0, sizeof(*event));

    DFBEvent* e = NULL;

    if (   ((e = (DFBEvent*)mrb_directfb_input_event(mrb, event_object)) != NULL)
        || ((e = (DFBEvent*)mrb_directfb_get_window_event(mrb, event_object)) != NULL)
        || ((e = (DFBEvent*)mrb_directfb_get_video_provider_event(mrb, event_object)) != NULL)
        ) {
        memcpy(event, e, sizeof(*event));
    }
}

// ============================================================================
// IDirectFB::EventBufferStats object - DFBEventBufferStats

struct mrb_directfb_event_buffer_stats_data {
    DFBEventBufferStats stats;
};

static void mrb_directfb_event_buffer_stats_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_event_buffer_stats_data* data = (struct mrb_directfb_event_buffer_stats_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_event_buffer_stats_type = {"EventBufferStats", mrb_directfb_event_buffer_stats_free};

mrb_value mrb_directfb_event_buffer_stats_wrap(mrb_state* mrb, struct RClass* c, DFBEventBufferStats* stats)
{
    struct mrb_directfb_event_buffer_stats_data* data = (struct mrb_directfb_event_buffer_stats_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_event_buffer_stats_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    memcpy(&data->stats, stats, sizeof(data->stats));
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_event_buffer_stats_type, data));
}

mrb_value mrb_directfb_event_buffer_stats_value(mrb_state* mrb, DFBEventBufferStats* stats)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "EventBufferStats")));
    return mrb_directfb_event_buffer_stats_wrap(mrb, c, stats);
}

DFBEventBufferStats* mrb_directfb_event_buffer_stats(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_event_buffer_stats_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_event_buffer_stats_type, struct mrb_directfb_event_buffer_stats_data);
    return (data != NULL)? &data->stats : NULL;
}

static mrb_value event_buffer_stats_num_events(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->num_events) : mrb_nil_value();
}

static mrb_value event_buffer_stats_input(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DFEC_INPUT) : mrb_nil_value();
}

static mrb_value event_buffer_stats_window(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DFEC_WINDOW) : mrb_nil_value();
}

static mrb_value event_buffer_stats_user(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DFEC_USER) : mrb_nil_value();
}

static mrb_value event_buffer_stats_universal(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DFEC_UNIVERSAL) : mrb_nil_value();
}

static mrb_value event_buffer_stats_videoprovider(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DFEC_VIDEOPROVIDER) : mrb_nil_value();
}

static mrb_value event_buffer_stats_keypress(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DIET_KEYPRESS) : mrb_nil_value();
}

static mrb_value event_buffer_stats_keyrelease(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DIET_KEYRELEASE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_buttonpress(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DIET_BUTTONPRESS) : mrb_nil_value();
}

static mrb_value event_buffer_stats_buttonrelease(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DIET_BUTTONRELEASE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_axismotion(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DIET_AXISMOTION) : mrb_nil_value();
}

static mrb_value event_buffer_stats_position(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_MOTION) : mrb_nil_value();
}

static mrb_value event_buffer_stats_size(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_SIZE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_close(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_CLOSE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_destroyed(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_DESTROYED) : mrb_nil_value();
}

static mrb_value event_buffer_stats_gotfocus(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_GOTFOCUS) : mrb_nil_value();
}

static mrb_value event_buffer_stats_lostfocus(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_LOSTFOCUS) : mrb_nil_value();
}

static mrb_value event_buffer_stats_keydown(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_KEYDOWN) : mrb_nil_value();
}

static mrb_value event_buffer_stats_keyup(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_KEYUP) : mrb_nil_value();
}

static mrb_value event_buffer_stats_buttondown(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_BUTTONDOWN) : mrb_nil_value();
}

static mrb_value event_buffer_stats_buttonup(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_BUTTONUP) : mrb_nil_value();
}

static mrb_value event_buffer_stats_motion(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_MOTION) : mrb_nil_value();
}

static mrb_value event_buffer_stats_enter(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_ENTER) : mrb_nil_value();
}

static mrb_value event_buffer_stats_leave(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_LEAVE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_wheel(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_WHEEL) : mrb_nil_value();
}

static mrb_value event_buffer_stats_position_size(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DWET_POSITION_SIZE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_started(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_STARTED) : mrb_nil_value();
}

static mrb_value event_buffer_stats_stopped(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_STOPPED) : mrb_nil_value();
}

static mrb_value event_buffer_stats_speedchange(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_SPEEDCHANGE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_streamchange(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_STREAMCHANGE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_fatalerror(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_FATALERROR) : mrb_nil_value();
}

static mrb_value event_buffer_stats_finished(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_FINISHED) : mrb_nil_value();
}

static mrb_value event_buffer_stats_surfacechange(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_SURFACECHANGE) : mrb_nil_value();
}

static mrb_value event_buffer_stats_framedecoded(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_FRAMEDECODED) : mrb_nil_value();
}

static mrb_value event_buffer_stats_framedisplayed(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_FRAMEDISPLAYED) : mrb_nil_value();
}

static mrb_value event_buffer_stats_dataexhausted(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_DATAEXHAUSTED) : mrb_nil_value();
}

static mrb_value event_buffer_stats_datalow(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_DATALOW) : mrb_nil_value();
}

static mrb_value event_buffer_stats_videoaction(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_VIDEOACTION) : mrb_nil_value();
}

static mrb_value event_buffer_stats_datahigh(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_DATAHIGH) : mrb_nil_value();
}

static mrb_value event_buffer_stats_buffertimelow(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_BUFFERTIMELOW) : mrb_nil_value();
}

static mrb_value event_buffer_stats_buffertimehigh(mrb_state *mrb, mrb_value self)
{
    DFBEventBufferStats* stats = mrb_directfb_event_buffer_stats(mrb, self);
    return (stats != NULL)? mrb_fixnum_value(stats->DVPET_BUFFERTIMEHIGH) : mrb_nil_value();
}

static void define_event_buffer_stats(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* stats = mrb_define_class_under(mrb, outer, "EventBufferStats", mrb->object_class);
    mrb_define_method(mrb, stats, "num_events", event_buffer_stats_num_events, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "input", event_buffer_stats_input, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "window", event_buffer_stats_window, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "user", event_buffer_stats_user, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "universal", event_buffer_stats_universal, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "videoprovider", event_buffer_stats_videoprovider, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "keypress", event_buffer_stats_keypress, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "keyrelease", event_buffer_stats_keyrelease, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "buttonpress", event_buffer_stats_buttonpress, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "buttonrelease", event_buffer_stats_buttonrelease, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "axismotion", event_buffer_stats_axismotion, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "position", event_buffer_stats_position, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "size", event_buffer_stats_size, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "close", event_buffer_stats_close, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "destroyed", event_buffer_stats_destroyed, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "gotfocus", event_buffer_stats_gotfocus, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "lostfocus", event_buffer_stats_lostfocus, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "keydown", event_buffer_stats_keydown, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "keyup", event_buffer_stats_keyup, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "buttondown", event_buffer_stats_buttondown, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "buttonup", event_buffer_stats_buttonup, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "motion", event_buffer_stats_motion, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "enter", event_buffer_stats_enter, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "leave", event_buffer_stats_leave, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "wheel", event_buffer_stats_wheel, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "position_size", event_buffer_stats_position_size, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "started", event_buffer_stats_started, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "stopped", event_buffer_stats_stopped, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "speedchange", event_buffer_stats_speedchange, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "streamchange", event_buffer_stats_streamchange, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "fatalerror", event_buffer_stats_fatalerror, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "finished", event_buffer_stats_finished, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "surfacechange", event_buffer_stats_surfacechange, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "framedecoded", event_buffer_stats_framedecoded, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "framedisplayed", event_buffer_stats_framedisplayed, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "dataexhausted", event_buffer_stats_dataexhausted, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "datalow", event_buffer_stats_datalow, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "videoaction", event_buffer_stats_videoaction, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "datahigh", event_buffer_stats_datahigh, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "buffertimelow", event_buffer_stats_buffertimelow, MRB_ARGS_NONE());
    mrb_define_method(mrb, stats, "buffertimehigh", event_buffer_stats_buffertimehigh, MRB_ARGS_NONE());
}

// ============================================================================
// IDirectFB::ColorAdjustment object - DFBColorAdjustment

struct mrb_directfb_color_adjustment_data {
    DFBColorAdjustment adjustment;
};

static void mrb_directfb_color_adjustment_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_color_adjustment_data* data = (struct mrb_directfb_color_adjustment_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_color_adjustment_type = {"ColorAdjustment", mrb_directfb_color_adjustment_free};

mrb_value mrb_directfb_color_adjustment_wrap(mrb_state* mrb, struct RClass* c, DFBColorAdjustment* adjustment)
{
    struct mrb_directfb_color_adjustment_data* data = (struct mrb_directfb_color_adjustment_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_color_adjustment_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    memcpy(&data->adjustment, adjustment, sizeof(data->adjustment));
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_color_adjustment_type, data));
}

mrb_value mrb_directfb_color_adjustment_value(mrb_state* mrb, DFBColorAdjustment* adjustment)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "ColorAdjustment")));
    return mrb_directfb_color_adjustment_wrap(mrb, c, adjustment);
}

DFBColorAdjustment* mrb_directfb_color_adjustment(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_color_adjustment_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_color_adjustment_type, struct mrb_directfb_color_adjustment_data);
    return (data != NULL)? &data->adjustment : NULL;
}

static mrb_value adjustment_flags(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        return mrb_fixnum_value(adjustment->flags);
    }
    return mrb_nil_value();
}

static mrb_value adjustment_brightness(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        return mrb_fixnum_value(adjustment->brightness);
    }
    return mrb_nil_value();
}

static mrb_value adjustment_contrast(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        return mrb_fixnum_value(adjustment->contrast);
    }
    return mrb_nil_value();
}

static mrb_value adjustment_hue(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        return mrb_fixnum_value(adjustment->hue);
    }
    return mrb_nil_value();
}

static mrb_value adjustment_saturation(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        return mrb_fixnum_value(adjustment->saturation);
    }
    return mrb_nil_value();
}

static mrb_value adjustment_flags_set(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        mrb_int flags;
        mrb_get_args(mrb, "i", &flags);
        adjustment->flags = flags;
    }
    return mrb_nil_value();
}

static mrb_value adjustment_brightness_set(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        mrb_int brightness;
        mrb_get_args(mrb, "i", &brightness);
        adjustment->brightness = brightness;
    }
    return mrb_nil_value();
}

static mrb_value adjustment_contrast_set(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        mrb_int contrast;
        mrb_get_args(mrb, "i", &contrast);
        adjustment->contrast = contrast;
    }
    return mrb_nil_value();
}

static mrb_value adjustment_hue_set(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        mrb_int hue;
        mrb_get_args(mrb, "i", &hue);
        adjustment->hue = hue;
    }
    return mrb_nil_value();
}

static mrb_value adjustment_saturation_set(mrb_state *mrb, mrb_value self)
{
    DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, self);
    if (adjustment != NULL) {
        mrb_int saturation;
        mrb_get_args(mrb, "i", &saturation);
        adjustment->saturation = saturation;
    }
    return mrb_nil_value();
}

static void define_color_adjustment(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* adjustment = mrb_define_class_under(mrb, outer, "ColorAdjustment", mrb->object_class);
    mrb_define_method(mrb, adjustment, "flags", adjustment_flags, MRB_ARGS_NONE());
    mrb_define_method(mrb, adjustment, "brightness", adjustment_brightness, MRB_ARGS_NONE());
    mrb_define_method(mrb, adjustment, "contrast", adjustment_contrast, MRB_ARGS_NONE());
    mrb_define_method(mrb, adjustment, "hue", adjustment_hue, MRB_ARGS_NONE());
    mrb_define_method(mrb, adjustment, "saturation", adjustment_saturation, MRB_ARGS_NONE());
    mrb_define_method(mrb, adjustment, "flags=", adjustment_flags_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, adjustment, "brightness=", adjustment_brightness_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, adjustment, "contrast=", adjustment_contrast_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, adjustment, "hue=", adjustment_hue_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, adjustment, "saturation=", adjustment_saturation_set, MRB_ARGS_REQ(1));
}

// ============================================================================
// IDirectFB::StreamAttributes object - DFBStreamAttributes

struct mrb_directfb_stream_attributes_data {
    DFBStreamAttributes attributes;
};

static void mrb_directfb_stream_attributes_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_stream_attributes_data* data = (struct mrb_directfb_stream_attributes_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_stream_attributes_type = {"StreamAttributes", mrb_directfb_stream_attributes_free};

mrb_value mrb_directfb_stream_attributes_wrap(mrb_state* mrb, struct RClass* c, DFBStreamAttributes* attributes)
{
    struct mrb_directfb_stream_attributes_data* data = (struct mrb_directfb_stream_attributes_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_stream_attributes_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    memcpy(&data->attributes, attributes, sizeof(data->attributes));
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_stream_attributes_type, data));
}

mrb_value mrb_directfb_stream_attributes_value(mrb_state* mrb, DFBStreamAttributes* attributes)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "StreamAttributes")));
    return mrb_directfb_stream_attributes_wrap(mrb, c, attributes);
}

DFBStreamAttributes* mrb_directfb_stream_attributes(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_stream_attributes_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_stream_attributes_type, struct mrb_directfb_stream_attributes_data);
    return (data != NULL)? &data->attributes : NULL;
}

static mrb_value stream_attributes_video_encoding(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        return mrb_str_new_cstr(mrb, attributes->video.encoding);
    }
    return mrb_nil_value();
}

static mrb_value stream_attributes_video_format(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        return mrb_fixnum_value(attributes->video.format);
    }
    return mrb_nil_value();
}

static mrb_value stream_attributes_audio_encoding(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        return mrb_str_new_cstr(mrb, attributes->audio.encoding);
    }
    return mrb_nil_value();
}

static mrb_value stream_attributes_audio_format(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        return mrb_fixnum_value(attributes->audio.format);
    }
    return mrb_nil_value();
}

static mrb_value stream_attributes_video_encoding_set(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        char* encoding = NULL;
        mrb_get_args(mrb, "z", encoding);
        strncpy(attributes->video.encoding, encoding, DFB_STREAM_DESC_ENCODING_LENGTH);
        attributes->video.encoding[DFB_STREAM_DESC_ENCODING_LENGTH - 1] = '\0';
    }
    return mrb_nil_value();
}

static mrb_value stream_attributes_video_format_set(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        mrb_int format = 0;
        mrb_get_args(mrb, "i", &format);
        attributes->video.format = format;
    }
    return mrb_nil_value();
}

static mrb_value stream_attributes_audio_encoding_set(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        char* encoding = NULL;
        mrb_get_args(mrb, "z", encoding);
        strncpy(attributes->audio.encoding, encoding, DFB_STREAM_DESC_ENCODING_LENGTH);
        attributes->audio.encoding[DFB_STREAM_DESC_ENCODING_LENGTH - 1] = '\0';
    }
    return mrb_nil_value();
}

static mrb_value stream_attributes_audio_format_set(mrb_state *mrb, mrb_value self)
{
    DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, self);
    if (attributes != NULL) {
        mrb_int format = 0;
        mrb_get_args(mrb, "i", &format);
        attributes->audio.format = format;
    }
    return mrb_nil_value();
}

static void define_stream_attributes(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* attributes = mrb_define_class_under(mrb, outer, "StreamAttributes", mrb->object_class);
    mrb_define_method(mrb, attributes, "video_encoding", stream_attributes_video_encoding, MRB_ARGS_NONE());
    mrb_define_method(mrb, attributes, "video_format", stream_attributes_video_format, MRB_ARGS_NONE());
    mrb_define_method(mrb, attributes, "audio_encoding", stream_attributes_audio_encoding, MRB_ARGS_NONE());
    mrb_define_method(mrb, attributes, "audio_format", stream_attributes_audio_format, MRB_ARGS_NONE());
    mrb_define_method(mrb, attributes, "video_encoding=", stream_attributes_video_encoding_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, attributes, "video_format=", stream_attributes_video_format_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, attributes, "audio_encoding=", stream_attributes_audio_encoding_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, attributes, "audio_format=", stream_attributes_audio_format_set, MRB_ARGS_REQ(1));
}

// ============================================================================

void mrb_directfb_define_misc(mrb_state* mrb, struct RClass* outer)
{
    define_region(mrb, outer);
    define_rectangle(mrb, outer);
    define_input_device_keymap_entry(mrb, outer);
    define_input_event(mrb, outer);
    define_window_event(mrb, outer);
    define_video_provider_event(mrb, outer);
    define_event_buffer_stats(mrb, outer);
    define_color_adjustment(mrb, outer);
    define_stream_attributes(mrb, outer);
}

