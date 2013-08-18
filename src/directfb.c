#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_constants.h"
#include "directfb_misc.h"
#include "directfb_descriptions.h"
#include "directfb_display_layer.h"
#include "directfb_surface.h"
#include "directfb_window.h"
#include "directfb_font.h"
#include "directfb_image_provider.h"
#include "directfb_input_device.h"
#include "directfb_event_buffer.h"

// ============================================================================
// IDirectFB object

struct mrb_directfb_data {
    IDirectFB* dfb;
};

static void mrb_directfb_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_data* data = (struct mrb_directfb_data*)p;
    if (data != NULL) {
        if (data->dfb != NULL) {
            data->dfb->Release(data->dfb);
            data->dfb = NULL;
        }
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_type = { "DirectFB", mrb_directfb_free };

static mrb_value mrb_directfb_wrap(mrb_state* mrb, struct RClass* c, IDirectFB* dfb)
{
    struct mrb_directfb_data* data = (struct mrb_directfb_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    data->dfb = dfb;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_type, data));
}

IDirectFB* get_directfb(mrb_state* mrb, mrb_value value)
{
    struct mrb_directfb_data* data = (struct mrb_directfb_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_type);
    ;
    return (data != NULL)? data->dfb : NULL;
}

// ============================================================================
// DirectFB methods

static mrb_value directfb_init(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = DirectFBInit(0, NULL);
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_error(mrb_state *mrb, mrb_value self)
{
    char* m;
    mrb_value v;
    mrb_get_args(mrb, "zi", &m, &v);
    DFBResult ret = DirectFBError(m, mrb_fixnum(v));
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_set_option(mrb_state *mrb, mrb_value self)
{
    char* name;
    char* value;
    mrb_get_args(mrb, "zz", &name, &value);
    DFBResult ret = DirectFBSetOption(name, value);
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_create(mrb_state *mrb, mrb_value self)
{
    IDirectFB* dfb = NULL;
    DFBResult ret = DirectFBCreate(&dfb);
    if (ret == 0) {
        return mrb_directfb_wrap(mrb, mrb_class_ptr(self), dfb);
    } else {
        return mrb_nil_value();
    }
}

static mrb_value directfb_usleep(mrb_state *mrb, mrb_value self)
{
    mrb_int usec;
    mrb_get_args(mrb, "i", &usec);
    usleep(usec);
    return mrb_nil_value();
}

static mrb_value directfb_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_data* data = (struct mrb_directfb_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_type);
    if ((data != NULL) && (data->dfb != NULL)) {
        data->dfb->Release(data->dfb);
        data->dfb = NULL;
    }
    return mrb_nil_value();
}

static mrb_value directfb_set_cooperative_level(mrb_state *mrb, mrb_value self)
{
    mrb_int level;
    mrb_get_args(mrb, "i", &level);

    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->SetCooperativeLevel(dfb, level);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_set_video_mode(mrb_state *mrb, mrb_value self)
{
    mrb_int width;
    mrb_int height;
    mrb_int bpp;
    mrb_get_args(mrb, "iii", &width, &height, &bpp);

    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->SetVideoMode(dfb, width, height, bpp);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_get_device_description(mrb_state *mrb, mrb_value self)
{
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        DFBGraphicsDeviceDescription desc;
        DFBResult ret = dfb->GetDeviceDescription(dfb, &desc);
        if (!ret) {
            return mrb_directfb_graphics_device_description_new(mrb, &desc);
        }
    }
    return mrb_nil_value();
}

struct enum_video_modes_callback_arg {
    mrb_state* mrb;
    mrb_value* block;
};

static DFBEnumerationResult enum_video_modes_callback(int width, int height, int bpp, void* callbackdata)
{
    struct enum_video_modes_callback_arg* arg = (struct enum_video_modes_callback_arg*)callbackdata;
    int ai = mrb_gc_arena_save(arg->mrb);
    mrb_value args[3];
    args[0] = mrb_fixnum_value(width);
    args[1] = mrb_fixnum_value(height);
    args[2] = mrb_fixnum_value(bpp);
    mrb_yield_argv(arg->mrb, *(arg->block), 3, args);
    mrb_gc_arena_restore(arg->mrb, ai);
    return 0;
}

static mrb_value directfb_enum_video_modes(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    mrb_value block;
    mrb_get_args(mrb, "&", &block);

    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        struct enum_video_modes_callback_arg arg = {mrb, &block};
        ret = dfb->EnumVideoModes(dfb, enum_video_modes_callback, (void*)&arg);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_create_surface(mrb_state *mrb, mrb_value self)
{
    mrb_value desc_object;
    mrb_get_args(mrb, "o", &desc_object);

    DFBSurfaceDescription desc;
    mrb_directfb_surface_description_get(mrb, desc_object, &desc);

    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb == NULL) {
        return mrb_nil_value();
    }

    IDirectFBSurface* surface = NULL;
    DFBResult ret = dfb->CreateSurface(dfb, &desc, &surface);
    if (ret) {
        //DirectFBError("CreateSurface", ret);
        return mrb_nil_value();
    }

    return mrb_directfb_surface_value(mrb, surface);
}

struct enum_screens_callback_arg {
    mrb_state* mrb;
    mrb_value* block;
};

static DFBEnumerationResult enum_screens_callback(DFBScreenID id, DFBScreenDescription desc, void* callbackdata)
{
    struct enum_screens_callback_arg* arg = (struct enum_screens_callback_arg*)callbackdata;
    int ai = mrb_gc_arena_save(arg->mrb);
    mrb_value args[2];
    args[0] = mrb_fixnum_value(id);
    args[1] = mrb_directfb_screen_description_new(arg->mrb, &desc);
    mrb_yield_argv(arg->mrb, *(arg->block), 2, args);
    mrb_gc_arena_restore(arg->mrb, ai);
    return 0;
}

static mrb_value directfb_enum_screens(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    mrb_value block;
    mrb_get_args(mrb, "&", &block);

    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        struct enum_screens_callback_arg arg = {mrb, &block};
        ret = dfb->EnumScreens(dfb, enum_screens_callback, (void*)&arg);
    }
    return mrb_fixnum_value(ret);
}

struct enum_display_layers_callback_arg {
    mrb_state* mrb;
    mrb_value* block;
};

static DFBEnumerationResult enum_display_layers_callback(DFBDisplayLayerID id, DFBDisplayLayerDescription desc, void* callbackdata)
{
    struct enum_display_layers_callback_arg* arg = (struct enum_display_layers_callback_arg*)callbackdata;
    int ai = mrb_gc_arena_save(arg->mrb);
    mrb_value args[2];
    args[0] = mrb_fixnum_value(id);
    args[1] = mrb_directfb_display_layer_description_new(arg->mrb, &desc);
    mrb_yield_argv(arg->mrb, *(arg->block), 2, args);
    mrb_gc_arena_restore(arg->mrb, ai);
    return 0;
}

static mrb_value directfb_enum_display_layers(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    mrb_value block;
    mrb_get_args(mrb, "&", &block);

    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        struct enum_display_layers_callback_arg arg = {mrb, &block};
        ret = dfb->EnumDisplayLayers(dfb, enum_display_layers_callback, (void*)&arg);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_get_display_layer(mrb_state *mrb, mrb_value self)
{
    mrb_int layer_id;
    mrb_get_args(mrb, "i", &layer_id);

    struct mrb_directfb_data* data = (struct mrb_directfb_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_type);
    if (data->dfb != NULL) {
        IDirectFBDisplayLayer* layer;
        DFBResult ret = data->dfb->GetDisplayLayer(data->dfb, layer_id, &layer);
        if (!ret) {
            return mrb_directfb_display_layer_value(mrb, layer);
        }
    }
    return mrb_nil_value();
}

struct enum_input_devices_callback_arg {
    mrb_state* mrb;
    mrb_value* block;
};

static DFBEnumerationResult enum_input_devices_callback(DFBInputDeviceID device_id, DFBInputDeviceDescription desc, void* callbackdata)
{
    struct enum_input_devices_callback_arg* arg = (struct enum_input_devices_callback_arg*)callbackdata;
    mrb_value args[2];
    args[0] = mrb_fixnum_value(device_id);
    args[1] = mrb_directfb_input_device_description_new(arg->mrb, &desc);
    mrb_yield_argv(arg->mrb, *(arg->block), 2, args);
    return 0;
}

static mrb_value directfb_enum_input_devices(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    mrb_value block;
    mrb_get_args(mrb, "&", &block);

    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        struct enum_input_devices_callback_arg arg = {mrb, &block};
        ret = dfb->EnumInputDevices(dfb, enum_input_devices_callback, (void*)&arg);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_get_input_device(mrb_state *mrb, mrb_value self)
{
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        mrb_int device_id;
        mrb_get_args(mrb, "i", &device_id);
        IDirectFBInputDevice* device;
        DFBResult ret = dfb->GetInputDevice(dfb, device_id, &device);
        if (!ret) {
            return mrb_directfb_input_device_value(mrb, device);
        }
    }
    return mrb_nil_value();
}

static mrb_value directfb_create_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        IDirectFBEventBuffer* buffer;
        DFBResult ret = dfb->CreateEventBuffer(dfb, &buffer);
        if (!ret) {
            return mrb_directfb_event_buffer_value(mrb, buffer);
        }
    }
    return mrb_nil_value();
}

static mrb_value directfb_create_input_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        mrb_int caps;
        mrb_bool b;
        mrb_get_args(mrb, "ib", &caps, &b);
        DFBBoolean global = (b != FALSE)? DFB_TRUE : DFB_FALSE;
        IDirectFBEventBuffer* buffer;
        DFBResult ret = dfb->CreateInputEventBuffer(dfb, caps, global, &buffer);
        if (!ret) {
            return mrb_directfb_event_buffer_value(mrb, buffer);
        }
    }
    return mrb_nil_value();
}

static mrb_value directfb_create_image_provider(mrb_state *mrb, mrb_value self)
{
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        char* filename;
        mrb_get_args(mrb, "z", &filename);

        IDirectFBImageProvider* provider;
        DFBResult ret = dfb->CreateImageProvider(dfb, filename, &provider);
        if (!ret) {
            return mrb_directfb_image_provider_value(mrb, provider);
        }
    }
    return mrb_nil_value();
}

static mrb_value directfb_create_font(mrb_state *mrb, mrb_value self)
{
    char* fontname;
    mrb_value desc_object;

    mrb_get_args(mrb, "zo", &fontname, &desc_object);

    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        IDirectFBFont* font;
        DFBFontDescription desc;
        mrb_directfb_font_description_get(mrb, desc_object, &desc);
        if (!dfb->CreateFont(dfb, fontname, &desc, &font)) {
            return mrb_directfb_font_value(mrb, font);
        }
    }
    return mrb_nil_value();
}

static mrb_value directfb_suspend(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->Suspend(dfb);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_resume(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->Resume(dfb);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_wait_idle(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->WaitIdle(dfb);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value directfb_wait_for_sync(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->WaitForSync(dfb);
    }
    return mrb_fixnum_value(ret);
}



void mrb_mruby_directfb_gem_init(mrb_state* mrb)
{
    int ai = mrb_gc_arena_save(mrb);

    // def DirectFB
    struct RClass* dfb = mrb_define_class(mrb, "DirectFB", mrb->object_class);

    mrb_directfb_define_constants(mrb, dfb);
    mrb_directfb_define_misc(mrb, dfb);
    mrb_directfb_define_surface(mrb, dfb);
    mrb_directfb_define_display_layer(mrb, dfb);
    mrb_directfb_define_window(mrb, dfb);
    mrb_directfb_define_font(mrb, dfb);
    mrb_directfb_define_image_provider(mrb, dfb);
    mrb_directfb_define_input_device(mrb, dfb);
    mrb_directfb_define_event_buffer(mrb, dfb);

    mrb_define_class_method(mrb, dfb, "init", directfb_init, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, dfb, "error", directfb_error, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, dfb, "set_option", directfb_set_option, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, dfb, "create", directfb_create, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, dfb, "usleep", directfb_usleep, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, dfb, "release", directfb_release, MRB_ARGS_NONE());

    // cooperative level, video mode
    mrb_define_method(mrb, dfb, "set_cooperative_level", directfb_set_cooperative_level, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "set_video_mode", directfb_set_video_mode, MRB_ARGS_REQ(3));

    // hardware capabilities
    mrb_define_method(mrb, dfb, "get_device_description", directfb_get_device_description, MRB_ARGS_NONE());
    mrb_define_method(mrb, dfb, "enum_video_modes", directfb_enum_video_modes, MRB_ARGS_REQ(1));

    // surfaces & palettes
    mrb_define_method(mrb, dfb, "create_surface", directfb_create_surface, MRB_ARGS_REQ(1));

    // screens
    mrb_define_method(mrb, dfb, "enum_screens", directfb_enum_screens, MRB_ARGS_REQ(1));

    // display layers
    mrb_define_method(mrb, dfb, "enum_display_layers", directfb_enum_display_layers, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "get_display_layer", directfb_get_display_layer, MRB_ARGS_REQ(1));

    // input devices
    mrb_define_method(mrb, dfb, "enum_input_devices", directfb_enum_input_devices, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "get_input_device", directfb_get_input_device, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "create_event_buffer", directfb_create_event_buffer, MRB_ARGS_NONE());
    mrb_define_method(mrb, dfb, "create_input_event_buffer", directfb_create_input_event_buffer, MRB_ARGS_REQ(2));

    // media
    mrb_define_method(mrb, dfb, "create_image_provider_impl", directfb_create_image_provider, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "create_font", directfb_create_font, MRB_ARGS_REQ(2));

    // misc
    mrb_define_method(mrb, dfb, "suspend", directfb_suspend, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "resume", directfb_resume, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "wait_idle", directfb_wait_idle, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "wait_for_sync", directfb_wait_for_sync, MRB_ARGS_REQ(1));

    mrb_gc_arena_restore(mrb, ai);
}

void mrb_mruby_directfb_gem_final(mrb_state* mrb)
{
    // finalizer
}

