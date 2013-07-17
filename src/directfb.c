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
    }
    return mrb_nil_value();
}

static mrb_value directfb_create_surface(mrb_state *mrb, mrb_value self)
{
#if 1
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

    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "Surface")));
    return mrb_directfb_surface_wrap(mrb, c, surface);
#else
    mrb_value desc_object;
    mrb_get_args(mrb, "o", &desc_object);

    IDirectFB* dfb = get_directfb(mrb, self);
    DFBSurfaceDescription* desc = mrb_directfb_surface_description_get(mrb, desc_object);
    if ((dfb == NULL) || (desc == NULL)) {
        return mrb_nil_value();
    }

    IDirectFBSurface* surface = NULL;
    DFBResult ret = dfb->CreateSurface(dfb, desc, &surface);
    if (ret) {
        //DirectFBError("CreateSurface", ret);
        return mrb_nil_value();
    }

    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "Surface")));
    return mrb_directfb_surface_wrap(mrb, c, surface);
#endif
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
            struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
            struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "DisplayLayer")));
            return mrb_directfb_display_layer_wrap(mrb, c, layer);
        }
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

struct enum_input_devices_callback_arg {
    mrb_state* mrb;
    mrb_value* block;
};

static DFBEnumerationResult enum_input_devices_callback(DFBInputDeviceID device_id, DFBInputDeviceDescription desc, void* callbackdata)
{
    struct enum_input_devices_callback_arg* arg = (struct enum_input_devices_callback_arg*)callbackdata;
    printf("%s(device_id:%d)\n", __func__, device_id);
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
    printf("end:%s()\n", __func__);
    return mrb_fixnum_value(ret);
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



    void
mrb_mruby_directfb_gem_init(mrb_state* mrb)
{
    int ai = mrb_gc_arena_save(mrb);

    // def DirectFB
    struct RClass* dfb = mrb_define_class(mrb, "DirectFB", mrb->object_class);

    mrb_directfb_define_constants(mrb, dfb);
    mrb_directfb_define_misc(mrb, dfb);
    mrb_directfb_define_surface(mrb, dfb);
    mrb_directfb_define_display_layer(mrb, dfb);

    mrb_define_class_method(mrb, dfb, "init", directfb_init, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, dfb, "error", directfb_error, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, dfb, "set_option", directfb_set_option, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, dfb, "create", directfb_create, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, dfb, "usleep", directfb_usleep, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, dfb, "release", directfb_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, dfb, "set_cooperative_level", directfb_set_cooperative_level, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "set_video_mode", directfb_set_video_mode, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, dfb, "create_surface", directfb_create_surface, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "get_display_layer", directfb_get_display_layer, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "enum_input_devices", directfb_enum_input_devices, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "suspend", directfb_suspend, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "resume", directfb_resume, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "wait_idle", directfb_wait_idle, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "wait_for_sync", directfb_wait_for_sync, MRB_ARGS_REQ(1));

    mrb_gc_arena_restore(mrb, ai);
}

void
mrb_mruby_directfb_gem_final(mrb_state* mrb) {
    // finalizer
}

