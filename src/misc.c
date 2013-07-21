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

DFBRegion* mrb_directfb_get_region(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_region_data* data = (struct mrb_directfb_region_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_region_type);
    return (data != NULL)? &data->region : NULL;
}

static mrb_value region_new(mrb_state *mrb, mrb_value self)
{
    mrb_int x1, y1, x2, y2;
    mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
    return mrb_directfb_region_wrap(mrb, mrb_class_ptr(self), x1, y1, x2, y2);
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

DFBRectangle* mrb_directfb_get_rectangle(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_rectangle_data* data = (struct mrb_directfb_rectangle_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_rectangle_type);
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
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->x) : mrb_nil_value();
}

static mrb_value rectangle_y(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->y) : mrb_nil_value();
}

static mrb_value rectangle_w(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->w) : mrb_nil_value();
}

static mrb_value rectangle_h(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->h) : mrb_nil_value();
}

static mrb_value rectangle_x_set(mrb_state *mrb, mrb_value self)
{
    mrb_int x;
    mrb_get_args(mrb, "i", &x);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    if (rect != NULL) {
        rect->x = x;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_y_set(mrb_state *mrb, mrb_value self)
{
    mrb_int y;
    mrb_get_args(mrb, "i", &y);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    if (rect != NULL) {
        rect->y = y;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_w_set(mrb_state *mrb, mrb_value self)
{
    mrb_int w;
    mrb_get_args(mrb, "i", &w);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    if (rect != NULL) {
        rect->w = w;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_h_set(mrb_state *mrb, mrb_value self)
{
    mrb_int h;
    mrb_get_args(mrb, "i", &h);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
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

DFBInputDeviceKeymapEntry* mrb_directfb_get_input_device_keymap_entry(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_input_device_keymap_entry_data* data = (struct mrb_directfb_input_device_keymap_entry_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_input_device_keymap_entry_type);
    return (data != NULL)? &data->input_device_keymap_entry : NULL;
}

static mrb_value input_device_keymap_entry_code(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_get_input_device_keymap_entry(mrb, self);
    return (entry != NULL)? mrb_fixnum_value(entry->code) : mrb_nil_value();
}

static mrb_value input_device_keymap_entry_locks(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_get_input_device_keymap_entry(mrb, self);
    return (entry != NULL)? mrb_fixnum_value(entry->locks) : mrb_nil_value();
}

static mrb_value input_device_keymap_entry_identifier(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_get_input_device_keymap_entry(mrb, self);
    return (entry != NULL)? mrb_fixnum_value(entry->identifier) : mrb_nil_value();
}

static mrb_value input_device_keymap_entry_symbols(mrb_state *mrb, mrb_value self)
{
    DFBInputDeviceKeymapEntry* entry = mrb_directfb_get_input_device_keymap_entry(mrb, self);
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

DFBInputEvent* mrb_directfb_get_input_event(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_input_event_data* data = (struct mrb_directfb_input_event_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_input_event_type);
    return (data != NULL)? &data->input_event : NULL;
}

static mrb_value input_event_clazz(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->clazz) : mrb_nil_value();
}

static mrb_value input_event_type(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->type) : mrb_nil_value();
}

static mrb_value input_event_device_id(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->device_id) : mrb_nil_value();
}

static mrb_value input_event_flags(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->flags) : mrb_nil_value();
}

static mrb_value input_event_timestamp(mrb_state *mrb, mrb_value self)
{
    mrb_value hash = mrb_hash_new(mrb);
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "tv_sec")), mrb_fixnum_value(input_event->timestamp.tv_sec));
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "tv_usec")), mrb_fixnum_value(input_event->timestamp.tv_usec));
    return hash;
}

static mrb_value input_event_key_code(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->key_code) : mrb_nil_value();
}

static mrb_value input_event_key_id(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->key_id) : mrb_nil_value();
}

static mrb_value input_event_key_symbol(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->key_symbol) : mrb_nil_value();
}

static mrb_value input_event_modifiers(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->modifiers) : mrb_nil_value();
}

static mrb_value input_event_locks(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->locks) : mrb_nil_value();
}

static mrb_value input_event_button(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->button) : mrb_nil_value();
}

static mrb_value input_event_buttons(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->buttons) : mrb_nil_value();
}

static mrb_value input_event_axis(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->axis) : mrb_nil_value();
}

static mrb_value input_event_axisabs(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->axisabs) : mrb_nil_value();
}

static mrb_value input_event_axisrel(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->axisrel) : mrb_nil_value();
}

static mrb_value input_event_min(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    return (input_event != NULL)? mrb_fixnum_value(input_event->min) : mrb_nil_value();
}

static mrb_value input_event_max(mrb_state *mrb, mrb_value self)
{
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
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

DFBWindowEvent* mrb_directfb_get_window_event(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_window_event_data* data = (struct mrb_directfb_window_event_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_window_event_type);
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
    DFBInputEvent* input_event = mrb_directfb_get_input_event(mrb, self);
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "tv_sec")), mrb_fixnum_value(input_event->timestamp.tv_sec));
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "tv_usec")), mrb_fixnum_value(input_event->timestamp.tv_usec));
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

DFBVideoProviderEvent* mrb_directfb_get_video_provider_event(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_video_provider_event_data* data = (struct mrb_directfb_video_provider_event_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_video_provider_event_type);
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

mrb_value mrb_directfb_event_new(mrb_state* mrb, DFBEvent* event)
{
    if (event == NULL) {
        return mrb_nil_value();
    }

    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* class = NULL;
    switch(event->clazz) {
        case DFEC_INPUT:
            class = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "InputEvent")));
            return mrb_directfb_input_event_wrap(mrb, class, &event->input);

        case DFEC_WINDOW:
            class = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "WindowEvent")));
            return mrb_directfb_window_event_wrap(mrb, class, &event->window);

        case DFEC_VIDEOPROVIDER:
            class = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "WindowEvent")));
            return mrb_directfb_video_provider_event_wrap(mrb, class, &event->videoprovider);

        case DFEC_USER:
        case DFEC_UNIVERSAL:
        case DFEC_NONE:
        default:
            return mrb_nil_value();
    };
}

// ============================================================================

void mrb_directfb_define_misc(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* region = mrb_define_class_under(mrb, outer, "Region", mrb->object_class);
    mrb_define_class_method(mrb, region, "new", region_new, MRB_ARGS_REQ(4));

    define_rectangle(mrb, outer);
    define_input_device_keymap_entry(mrb, outer);
    define_input_event(mrb, outer);
    define_window_event(mrb, outer);
    define_video_provider_event(mrb, outer);
}

