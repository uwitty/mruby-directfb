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
    mrb_value caps = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")));
    mrb_value width;
    mrb_value height;
    mrb_value pixelformat;
    mrb_value resource_id;

    memset(desc, 0, sizeof(*desc));
    if (!mrb_nil_p(caps)) {
        desc->flags |= DSDESC_CAPS;
        desc->caps = mrb_fixnum(caps);
    }
    width = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "width")));
    if (!mrb_nil_p(width)) {
        desc->flags |= DSDESC_WIDTH;
        desc->width = mrb_fixnum(width);
    }
    height = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "height")));
    if (!mrb_nil_p(height)) {
        desc->flags |= DSDESC_HEIGHT;
        desc->height = mrb_fixnum(height);
    }
    pixelformat = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "pixelformat")));
    if (!mrb_nil_p(pixelformat)) {
        desc->flags |= DSDESC_PIXELFORMAT;
        desc->pixelformat = mrb_fixnum(pixelformat);
    }
    resource_id = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "resource_id")));
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
    mrb_value caps;
    mrb_value colorkey_r;
    mrb_value colorkey_g;
    mrb_value colorkey_b;

    memset(desc, 0, sizeof(*desc));

    caps       = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")));
    desc->caps = mrb_fixnum(caps);
    colorkey_r = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_r")));
    desc->colorkey_r = mrb_fixnum(colorkey_r);
    colorkey_g = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_g")));
    desc->colorkey_g = mrb_fixnum(colorkey_g);
    colorkey_b = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "colorkey_b")));
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

mrb_value mrb_directfb_graphics_device_description_new(mrb_state *mrb, const DFBGraphicsDeviceDescription* desc)
{
    mrb_value driver = mrb_hash_new(mrb);
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, driver, mrb_symbol_value(mrb_intern_cstr(mrb, "major")), mrb_fixnum_value(desc->driver.major));
    mrb_hash_set(mrb, driver, mrb_symbol_value(mrb_intern_cstr(mrb, "minor")), mrb_fixnum_value(desc->driver.minor));
    mrb_hash_set(mrb, driver, mrb_symbol_value(mrb_intern_cstr(mrb, "name")), mrb_str_new_cstr(mrb, desc->driver.name));
    mrb_hash_set(mrb, driver, mrb_symbol_value(mrb_intern_cstr(mrb, "vendor")), mrb_str_new_cstr(mrb, desc->driver.vendor));

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "acceleration_mask")), mrb_fixnum_value(desc->acceleration_mask));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "blitting_flags")), mrb_fixnum_value(desc->blitting_flags));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "drawing_flags")), mrb_fixnum_value(desc->drawing_flags));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "video_memory")), mrb_fixnum_value(desc->video_memory));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "name")), mrb_str_new_cstr(mrb, desc->name));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "vendor")), mrb_str_new_cstr(mrb, desc->vendor));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "driver")), driver);

    return value;
}

mrb_value mrb_directfb_screen_description_new(mrb_state *mrb, const DFBScreenDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")), mrb_fixnum_value(desc->caps));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "name")), mrb_str_new_cstr(mrb, desc->name));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "mixers")), mrb_fixnum_value(desc->mixers));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "encoders")), mrb_fixnum_value(desc->encoders));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "outputs")), mrb_fixnum_value(desc->outputs));

    return value;
}

void mrb_directfb_window_description_get(mrb_state *mrb, mrb_value hash, DFBWindowDescription* desc)
{
    memset(desc, 0, sizeof(*desc));
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, caps        , DWDESC_CAPS        );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, width       , DWDESC_WIDTH       );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, height      , DWDESC_HEIGHT      );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, pixelformat , DWDESC_PIXELFORMAT );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, posx        , DWDESC_POSX        );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, posy        , DWDESC_POSY        );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, surface_caps, DWDESC_SURFACE_CAPS);
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, parent_id   , DWDESC_PARENT      );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, options     , DWDESC_OPTIONS     );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, stacking    , DWDESC_STACKING    );
    DESC_INT_VALUE_TO_CSTRUCT(mrb, hash, desc, resource_id , DWDESC_RESOURCE_ID );
}

static mrb_value stream_description_video(mrb_state* mrb, const DFBStreamDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "encoding")), mrb_str_new_cstr(mrb, desc->video.encoding));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "framerate")), mrb_float_value(mrb, desc->video.framerate));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "aspect")), mrb_float_value(mrb, desc->video.aspect));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "bitrate")), mrb_fixnum_value(desc->video.bitrate));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "afd")), mrb_fixnum_value(desc->video.afd));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "width")), mrb_fixnum_value(desc->video.width));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "height")), mrb_fixnum_value(desc->video.height));

    return value;
}

static mrb_value stream_description_audio(mrb_state* mrb, const DFBStreamDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "encoding")), mrb_str_new_cstr(mrb, desc->audio.encoding));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "samplerate")), mrb_fixnum_value(desc->audio.samplerate));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "channels")), mrb_fixnum_value(desc->audio.channels));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "bitrate")), mrb_fixnum_value(desc->audio.bitrate));

    return value;
}

mrb_value mrb_directfb_stream_description_new(mrb_state *mrb, const DFBStreamDescription* desc)
{
    mrb_value value = mrb_hash_new(mrb);

    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "caps")), mrb_fixnum_value(desc->caps));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "video")), stream_description_video(mrb, desc));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "audio")), stream_description_audio(mrb, desc));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "title")), mrb_str_new_cstr(mrb, desc->title));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "author")), mrb_str_new_cstr(mrb, desc->author));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "album")), mrb_str_new_cstr(mrb, desc->album));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "year")), mrb_fixnum_value(desc->year));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "genre")), mrb_str_new_cstr(mrb, desc->genre));
    mrb_hash_set(mrb, value, mrb_symbol_value(mrb_intern_cstr(mrb, "comment")), mrb_str_new_cstr(mrb, desc->comment));

    return value;
}

