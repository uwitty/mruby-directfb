#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_input_device.h"
#include "directfb_surface.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"

struct mrb_directfb_input_device_data {
    IDirectFBInputDevice* input_device;
};

static void mrb_directfb_input_device_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)p;
    if (data != NULL) {
        IDirectFBInputDevice* input_device = (IDirectFBInputDevice*)data->input_device;
        if (input_device != NULL) {
            input_device->Release(input_device);
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_input_device_type = {"InputDevice", mrb_directfb_input_device_free};

mrb_value mrb_directfb_input_device_wrap(mrb_state* mrb, struct RClass* c, IDirectFBInputDevice* input_device)
{
    struct mrb_directfb_input_device_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_input_device_data));
    data->input_device = input_device;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_input_device_type, data));
}

IDirectFBInputDevice* mrb_directfb_input_device_get(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_input_device_type);
    if (data != NULL) {
        return data->input_device;
    } else {
        return NULL;
    }
}

// ============================================================================
// DirectFB::InputDevice methods

static mrb_value input_device_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        data->input_device->Release(data->input_device);
        data->input_device = NULL;
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_id(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        DFBInputDeviceID id;
        DFBResult ret = device->GetID(device, &id);
        if (!ret) {
            return mrb_fixnum_value(id);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_description(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        DFBInputDeviceDescription desc;
        DFBResult ret = device->GetDescription(device, &desc);
        if (!ret) {
            return mrb_directfb_input_device_description_new(mrb, &desc);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_keymap_entry(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;

        mrb_int keycode;
        mrb_get_args(mrb, "i", &keycode);

        DFBInputDeviceKeymapEntry entry;

        DFBResult ret = device->GetKeymapEntry(device, keycode, &entry);
        if (!ret) {
            struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
            struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "InputDeviceKeymapEntry")));
            return mrb_directfb_input_device_keymap_entry_wrap(mrb, c, &entry);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_create_event_buffer(mrb_state *mrb, mrb_value self)
{
    return mrb_nil_value();
}

static mrb_value input_device_attach_event_buffer(mrb_state *mrb, mrb_value self)
{
    return mrb_nil_value();
}

static mrb_value input_device_detach_event_buffer(mrb_state *mrb, mrb_value self)
{
    return mrb_nil_value();
}

static mrb_value input_device_get_key_state(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        mrb_int key_id;
        mrb_get_args(mrb, "i", &key_id);
        DFBInputDeviceKeyState state;
        DFBResult ret = device->GetKeyState(device, key_id, &state);
        if (!ret) {
            return mrb_fixnum_value(state);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_modifiers(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        DFBInputDeviceModifierMask mask;
        DFBResult ret = device->GetModifiers(device, &mask);
        if (!ret) {
            return mrb_fixnum_value(mask);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_lock_state(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        DFBInputDeviceLockState state;
        DFBResult ret = device->GetLockState(device, &state);
        if (!ret) {
            return mrb_fixnum_value(state);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_buttons(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        DFBInputDeviceButtonMask mask;
        DFBResult ret = device->GetButtons(device, &mask);
        if (!ret) {
            return mrb_fixnum_value(mask);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_button_state(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        mrb_int button;
        mrb_get_args(mrb, "i", &button);
        DFBInputDeviceButtonState state;
        DFBResult ret = device->GetButtonState(device, button, &state);
        if (!ret) {
            return mrb_fixnum_value(state);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_axis(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        mrb_int axis;
        mrb_get_args(mrb, "i", &axis);
        int pos;
        DFBResult ret = device->GetAxis(device, axis, &pos);
        if (!ret) {
            return mrb_fixnum_value(pos);
        }
    }
    return mrb_nil_value();
}

static mrb_value input_device_get_xy(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_input_device_data* data = (struct mrb_directfb_input_device_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_input_device_type);
    if ((data != NULL) && (data->input_device != NULL)) {
        IDirectFBInputDevice* device = data->input_device;
        int x, y;
        DFBResult ret = device->GetXY(device, &x, &y);
        if (!ret) {
            mrb_value a[2];
            a[0] = mrb_fixnum_value(x);
            a[1] = mrb_fixnum_value(y);
            return mrb_ary_new_from_values(mrb, 2, a);
        }
    }
    return mrb_nil_value();
}


void mrb_directfb_define_input_device(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::InputDevice
    struct RClass* input_device = mrb_define_class_under(mrb, outer, "InputDevice", mrb->object_class);

    mrb_define_method(mrb, input_device, "release", input_device_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "get_id", input_device_get_id, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "get_description", input_device_get_description, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "get_keymap_entry", input_device_get_keymap_entry, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, input_device, "create_event_buffer", input_device_create_event_buffer, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "attach_event_buffer", input_device_attach_event_buffer, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, input_device, "detach_event_buffer", input_device_detach_event_buffer, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, input_device, "get_key_state", input_device_get_key_state, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, input_device, "get_modifiers", input_device_get_modifiers, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "get_lock_state", input_device_get_lock_state, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "get_buttons", input_device_get_buttons, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "get_button_state", input_device_get_button_state, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, input_device, "get_axis", input_device_get_axis, MRB_ARGS_NONE());
    mrb_define_method(mrb, input_device, "get_xy", input_device_get_xy, MRB_ARGS_NONE());
}



