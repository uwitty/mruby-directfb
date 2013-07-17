#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/hash.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_descriptions.h"

void mrb_directfb_input_device_description_get(mrb_state *mrb, mrb_value value, DFBInputDeviceDescription* desc)
{
    memset(desc, 0, sizeof(*desc));
}

mrb_value mrb_directfb_input_device_description_new(mrb_state *mrb, const DFBInputDeviceDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "type")), mrb_fixnum_value(desc->type));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")), mrb_fixnum_value(desc->caps));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "min_keycode")), mrb_fixnum_value(desc->min_keycode));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "max_keycode")), mrb_fixnum_value(desc->max_keycode));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "max_axis")), mrb_fixnum_value(desc->max_axis));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "max_button")), mrb_fixnum_value(desc->max_button));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "name")), mrb_str_new_cstr(mrb, desc->name));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "vendor")), mrb_str_new_cstr(mrb, desc->vendor));

    return value;
}

void mrb_directfb_surface_description_get(mrb_state *mrb, mrb_value hash, DFBSurfaceDescription* desc)
{
    memset(desc, 0, sizeof(*desc));
    mrb_value caps = mrb_iv_get(mrb, hash, mrb_intern_cstr(mrb, "caps"));
    if (!mrb_nil_p(caps)) {
        desc->flags |= DSDESC_CAPS;
        desc->caps = mrb_fixnum(caps);
    }
    mrb_value width = mrb_iv_get(mrb, hash, mrb_intern_cstr(mrb, "width"));
    if (!mrb_nil_p(width)) {
        desc->flags |= DSDESC_WIDTH;
        desc->width = mrb_fixnum(width);
    }
    mrb_value height = mrb_iv_get(mrb, hash, mrb_intern_cstr(mrb, "height"));
    if (!mrb_nil_p(height)) {
        desc->flags |= DSDESC_HEIGHT;
        desc->height = mrb_fixnum(height);
    }
    mrb_value pixelformat = mrb_iv_get(mrb, hash, mrb_intern_cstr(mrb, "pixelformat"));
    if (!mrb_nil_p(pixelformat)) {
        desc->flags |= DSDESC_PIXELFORMAT;
        desc->pixelformat = mrb_fixnum(pixelformat);
    }
    mrb_value resource_id = mrb_iv_get(mrb, hash, mrb_intern_cstr(mrb, "resource_id"));
    if (!mrb_nil_p(resource_id)) {
        desc->flags |= DSDESC_RESOURCE_ID;
        desc->resource_id = mrb_fixnum(resource_id);
    }
}

