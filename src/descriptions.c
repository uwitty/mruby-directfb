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
    mrb_value caps = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")));
    if (!mrb_nil_p(caps)) {
        desc->flags |= DSDESC_CAPS;
        desc->caps = mrb_fixnum(caps);
    }
    mrb_value width = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "width")));
    if (!mrb_nil_p(width)) {
        desc->flags |= DSDESC_WIDTH;
        desc->width = mrb_fixnum(width);
    }
    mrb_value height = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "height")));
    if (!mrb_nil_p(height)) {
        desc->flags |= DSDESC_HEIGHT;
        desc->height = mrb_fixnum(height);
    }
    mrb_value pixelformat = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "pixelformat")));
    if (!mrb_nil_p(pixelformat)) {
        desc->flags |= DSDESC_PIXELFORMAT;
        desc->pixelformat = mrb_fixnum(pixelformat);
    }
    mrb_value resource_id = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "resource_id")));
    if (!mrb_nil_p(resource_id)) {
        desc->flags |= DSDESC_RESOURCE_ID;
        desc->resource_id = mrb_fixnum(resource_id);
    }
}

#define DESC_INT_VALUE_TO_CSTRUCT(__mrb__, __hash__, __desc__, __name__, __flag__) \
    do { \
        mrb_value __name__ = mrb_hash_get((__mrb__), (__hash__), mrb_symbol_value(mrb_intern_cstr((__mrb__), #__name__))); \
        if (!mrb_nil_p(__name__)) { \
            (__desc__)->flags |= (__flag__); \
            (__desc__)->__name__ = mrb_fixnum(__name__); \
        } \
    } while(0);

#define DESC_INT_VALUE_TO_MRBHASH(__mrb__, __hash__, __desc__, __name__, __flag__) \
    do { \
        if (((__desc__)->flags & (__flag__)) != 0) { \
            mrb_hash_set((__mrb__), (__hash__), mrb_symbol_value(mrb_intern_cstr((__mrb__), #__name__)), mrb_fixnum_value((__desc__)->__name__)); \
        } \
    } while(0);

mrb_value mrb_directfb_surface_description_new(mrb_state *mrb, const DFBSurfaceDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    DESC_INT_VALUE_TO_MRBHASH(mrb, value, desc, caps       , DSDESC_CAPS);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, desc, width      , DSDESC_WIDTH);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, desc, height     , DSDESC_HEIGHT);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, desc, pixelformat, DSDESC_PIXELFORMAT);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, desc, resource_id, DSDESC_RESOURCE_ID);

    return value;
}

void mrb_directfb_font_description_get(mrb_state *mrb, mrb_value hash, DFBFontDescription* desc)
{
    memset(desc, 0, sizeof(*desc));
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, attributes   , DFDESC_ATTRIBUTES);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, height       , DFDESC_HEIGHT);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, width        , DFDESC_WIDTH);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, index        , DFDESC_INDEX);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, fixed_advance, DFDESC_FIXEDADVANCE);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, fract_height , DFDESC_FRACT_HEIGHT);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, fract_width  , DFDESC_FRACT_WIDTH);

#if 0
    printf("DFBFontDescription:%p\n", desc);
    printf("  flags:0x%08x\n", desc->flags);
    printf("  attributes:0x%08x\n", desc->attributes);
    printf("  height:%d\n", desc->height);
    printf("  width:%d\n", desc->width);
    printf("  index:%d\n", desc->index);
    printf("  fixed_advance:%d\n", desc->fixed_advance);
    printf("  fract_height:%d\n", desc->fract_height);
    printf("  fract_width:%d\n", desc->fract_width);
#endif
}

mrb_value mrb_directfb_image_description_new(mrb_state *mrb, const DFBImageDescription* desc)
{
    mrb_value hash = mrb_hash_new(mrb);

    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")), mrb_fixnum_value(desc->caps));
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_r")), mrb_fixnum_value(desc->colorkey_r));
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_g")), mrb_fixnum_value(desc->colorkey_g));
    mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_b")), mrb_fixnum_value(desc->colorkey_b));

    return hash;
}

void mrb_directfb_image_description_get(mrb_state *mrb, mrb_value hash, DFBImageDescription* desc)
{
    memset(desc, 0, sizeof(*desc));

    mrb_value caps       = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")));
    desc->caps = mrb_fixnum(caps);
    mrb_value colorkey_r = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_r")));
    desc->colorkey_r = mrb_fixnum(colorkey_r);
    mrb_value colorkey_g = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_g")));
    desc->colorkey_g = mrb_fixnum(colorkey_g);
    mrb_value colorkey_b = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_b")));
    desc->colorkey_b = mrb_fixnum(colorkey_b);

#if 1
    printf("DFBImageDescription:%p\n", desc);
    printf("  caps:0x%08x\n", desc->caps);
    printf("  colorkey_r:0x%02x\n", desc->colorkey_r);
    printf("  colorkey_g:0x%02x\n", desc->colorkey_g);
    printf("  colorkey_b:0x%02x\n", desc->colorkey_b);
#endif
}

mrb_value mrb_directfb_display_layer_description_new(mrb_state *mrb, const DFBDisplayLayerDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "type")), mrb_fixnum_value(desc->type));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")), mrb_fixnum_value(desc->caps));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "name")), mrb_str_new_cstr(mrb, desc->name));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "level")), mrb_fixnum_value(desc->level));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "regions")), mrb_fixnum_value(desc->regions));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "sources")), mrb_fixnum_value(desc->sources));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "clip_regions")), mrb_fixnum_value(desc->clip_regions));

    return value;
}

mrb_value mrb_directfb_display_layer_source_description_new(mrb_state *mrb, const DFBDisplayLayerSourceDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "source_id")), mrb_fixnum_value(desc->source_id));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "name")), mrb_str_new_cstr(mrb, desc->name));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")), mrb_fixnum_value(desc->caps));

    return value;
}

mrb_value mrb_directfb_display_layer_configuration_new(mrb_state *mrb, const DFBDisplayLayerConfig* conf)
{
    mrb_value value = mrb_hash_new(mrb);

    DESC_INT_VALUE_TO_MRBHASH(mrb, value, conf, width       , DLCONF_WIDTH);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, conf, height      , DLCONF_HEIGHT);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, conf, pixelformat , DLCONF_PIXELFORMAT);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, conf, buffermode  , DLCONF_BUFFERMODE);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, conf, options     , DLCONF_OPTIONS);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, conf, source      , DLCONF_SOURCE);
    DESC_INT_VALUE_TO_MRBHASH(mrb, value, conf, surface_caps, DLCONF_SURFACE_CAPS);

    return value;
}

void mrb_directfb_display_layer_configuration_get(mrb_state *mrb, mrb_value value, DFBDisplayLayerConfig* conf)
{
    memset(conf, 0, sizeof(*conf));
    DESC_INT_VALUE_TO_CSTRUCT(mrb, value, conf, width       , DLCONF_WIDTH);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, value, conf, height      , DLCONF_HEIGHT);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, value, conf, pixelformat , DLCONF_PIXELFORMAT);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, value, conf, buffermode  , DLCONF_BUFFERMODE);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, value, conf, options     , DLCONF_OPTIONS);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, value, conf, source      , DLCONF_SOURCE);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, value, conf, surface_caps, DLCONF_SURFACE_CAPS);
}

