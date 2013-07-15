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
#include "directfb_descriptions.h"

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
// IDirectFB::Result object - DFBResult

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

static mrb_value mrb_directfb_region_wrap(mrb_state* mrb, struct RClass* c, int x1, int y1, int x2, int y2)
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

static DFBRegion* get_region(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_region_data* data = (struct mrb_directfb_region_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_region_type);
    return (data != NULL)? &data->region : NULL;
}

static mrb_value mrb_directfb_region_new(mrb_state *mrb, mrb_value self)
{
    mrb_int x1, y1, x2, y2;
    mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
    return mrb_directfb_region_wrap(mrb, mrb_class_ptr(self), x1, y1, x2, y2);
}

// ============================================================================
// IDirectFB::DisplayLayer object

struct mrb_directfb_display_layer_data {
    IDirectFBDisplayLayer* layer;
};

static void mrb_directfb_display_layer_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)p;
    if (data != NULL) {
        IDirectFBDisplayLayer* layer = (IDirectFBDisplayLayer*)data->layer;
        if (layer != NULL) {
            layer->Release(layer);
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_display_layer_type = {"DisplayLayer", mrb_directfb_display_layer_free};

static mrb_value mrb_directfb_display_layer_wrap(mrb_state* mrb, struct RClass* c, IDirectFBDisplayLayer* layer)
{
    struct mrb_directfb_display_layer_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_display_layer_data));
    data->layer = layer;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_display_layer_type, data));
}

static IDirectFBDisplayLayer* get_displaylayer(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_display_layer_type);
    if (data != NULL) {
        return data->layer;
    } else {
        return NULL;
    }
}


// ============================================================================
// IDirectFBSurface object

struct mrb_directfb_surface_data {
    IDirectFBSurface* surface;
};

static void mrb_directfb_surface_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_surface_data* data = (struct mrb_directfb_surface_data*)p;
    if (data != NULL) {
        IDirectFBSurface* surface = (IDirectFBSurface*)data->surface;
        if (surface != NULL) {
            surface->Release(surface);
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_surface_type = {"Surface", mrb_directfb_surface_free};

static mrb_value mrb_directfb_surface_wrap(mrb_state* mrb, struct RClass* c, IDirectFBSurface* surface)
{
    struct mrb_directfb_surface_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_surface_data));
    data->surface = surface;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_surface_type, data));
}

static IDirectFBSurface* get_surface(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_surface_data* data = (struct mrb_directfb_surface_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_surface_type);
    if (data != NULL) {
        return data->surface;
    } else {
        return NULL;
    }
}


// ============================================================================
// DirectFB methods

static mrb_value mrb_directfb_init(mrb_state *mrb, mrb_value self)
{
  DFBResult ret = DirectFBInit(0, NULL);
  return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_error(mrb_state *mrb, mrb_value self)
{
  char* m;
  mrb_value v;
  mrb_get_args(mrb, "zi", &m, &v);
  DFBResult ret = DirectFBError(m, mrb_fixnum(v));
  return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_set_option(mrb_state *mrb, mrb_value self)
{
  char* name;
  char* value;
  mrb_get_args(mrb, "zz", &name, &value);
  DFBResult ret = DirectFBSetOption(name, value);
  return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_create(mrb_state *mrb, mrb_value self)
{
  IDirectFB* dfb = NULL;
  DFBResult ret = DirectFBCreate(&dfb);
  if (ret == 0) {
    return mrb_directfb_wrap(mrb, mrb_class_ptr(self), dfb);
  } else {
    return mrb_nil_value();
  }
}

static mrb_value mrb_directfb_usleep(mrb_state *mrb, mrb_value self)
{
  mrb_int usec;
  mrb_get_args(mrb, "i", &usec);
  usleep(usec);
  return mrb_nil_value();
}

static mrb_value mrb_directfb_release(mrb_state *mrb, mrb_value self)
{
  struct mrb_directfb_data* data = (struct mrb_directfb_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_type);
  if ((data != NULL) && (data->dfb != NULL)) {
    data->dfb->Release(data->dfb);
  }
  return mrb_nil_value();
}

static mrb_value mrb_directfb_get_display_layer(mrb_state *mrb, mrb_value self)
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

static mrb_value mrb_directfb_set_cooperative_level(mrb_state *mrb, mrb_value self)
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

static mrb_value mrb_directfb_set_video_mode(mrb_state *mrb, mrb_value self)
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
    struct RClass* class_directfb = mrb_class_get(arg->mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(arg->mrb, mrb_obj_value(class_directfb), mrb_intern(arg->mrb, "InputDeviceDescription")));
    args[1] = mrb_directfb_input_device_description_wrap(arg->mrb, c, &desc);
    mrb_yield_argv(arg->mrb, *(arg->block), 2, args);
    return 0;
}

static mrb_value mrb_directfb_enum_input_devices(mrb_state *mrb, mrb_value self)
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

static mrb_value mrb_directfb_suspend(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->Suspend(dfb);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_resume(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->Resume(dfb);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_wait_idle(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->WaitIdle(dfb);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_wait_for_sync(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFB* dfb = get_directfb(mrb, self);
    if (dfb != NULL) {
        ret = dfb->WaitForSync(dfb);
    }
    return mrb_fixnum_value(ret);
}


// ============================================================================
// DirectFB::DisplayLayer methods

static mrb_value mrb_directfb_display_layer_set_cooperative_level(mrb_state *mrb, mrb_value self)
{
  mrb_int cooperative_level;
  mrb_get_args(mrb, "i", &cooperative_level);

  IDirectFBDisplayLayer* layer = get_displaylayer(mrb, self);
  if (layer != NULL) {
      layer->SetCooperativeLevel(layer, cooperative_level);
  }

  return mrb_nil_value();
}

static mrb_value mrb_directfb_display_layer_release(mrb_state *mrb, mrb_value self)
{
  struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_display_layer_type);
  if ((data != NULL) && (data->layer != NULL)) {
      data->layer->Release(data->layer);
      data->layer = NULL;
  }
  return mrb_nil_value();
}

static mrb_value mrb_directfb_display_layer_get_surface(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = get_displaylayer(mrb, self);
    if (layer != NULL) {
        IDirectFBSurface* surface = NULL;
        DFBResult ret = layer->GetSurface(layer, &surface);
        if (!ret) {
            struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
            struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "Surface")));
            return mrb_directfb_surface_wrap(mrb, c, surface);
        }
    }
    return mrb_nil_value();
}


// ============================================================================
// DirectFB::Surface methods

static mrb_value mrb_directfb_surface_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_surface_data* data = (struct mrb_directfb_surface_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_surface_type);
    if ((data != NULL) && (data->surface != NULL)) {
        data->surface->Release(data->surface);
        data->surface = NULL;
    }
    return mrb_nil_value();
}

static mrb_value mrb_directfb_surface_set_color(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = get_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int r, g, b, a;
        mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
        ret = surface->SetColor(surface, r, g, b, a);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_surface_fill_rectangle(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = get_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int x, y, width, height;
        mrb_get_args(mrb, "iiii", &x, &y, &width, &height);
        ret = surface->FillRectangle(surface, x, y, width, height);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value mrb_directfb_surface_flip(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = get_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_value o;
        mrb_int flags = 0;
        mrb_get_args(mrb, "|oi", &o, &flags);
        ret = surface->Flip(surface, get_region(mrb, o), flags);
    }
    return mrb_fixnum_value(ret);
}

void
mrb_mruby_directfb_gem_init(mrb_state* mrb)
{
    int ai = mrb_gc_arena_save(mrb);

    // def DirectFB
    struct RClass* dfb = NULL;
    dfb = mrb_define_class(mrb, "DirectFB", mrb->object_class);

    mrb_directfb_define_constants(mrb, dfb);
    mrb_directfb_define_descriptions(mrb, dfb);
    mrb_define_class_method(mrb, dfb, "init", mrb_directfb_init, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, dfb, "error", mrb_directfb_error, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, dfb, "set_option", mrb_directfb_set_option, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, dfb, "create", mrb_directfb_create, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, dfb, "usleep", mrb_directfb_usleep, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "release", mrb_directfb_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, dfb, "set_cooperative_level", mrb_directfb_set_cooperative_level, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "set_video_mode", mrb_directfb_set_video_mode, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, dfb, "get_display_layer", mrb_directfb_get_display_layer, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "enum_input_devices", mrb_directfb_enum_input_devices, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "suspend", mrb_directfb_suspend, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "resume", mrb_directfb_resume, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "wait_idle", mrb_directfb_wait_idle, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, dfb, "wait_for_sync", mrb_directfb_wait_for_sync, MRB_ARGS_REQ(1));


    // def DirectFB::Region
    struct RClass* region = NULL;
    region = mrb_define_class_under(mrb, dfb, "Region", mrb->object_class);

    mrb_define_class_method(mrb, region, "new", mrb_directfb_region_new, MRB_ARGS_REQ(4));

    // def DirectFB::DisplayLayer
    struct RClass* display_layer = NULL;
    display_layer = mrb_define_class_under(mrb, dfb, "DisplayLayer", mrb->object_class);

    mrb_define_method(mrb, display_layer, "set_cooperative_level", mrb_directfb_display_layer_set_cooperative_level, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, display_layer, "get_surface", mrb_directfb_display_layer_get_surface, MRB_ARGS_NONE());
    mrb_define_method(mrb, display_layer, "release", mrb_directfb_display_layer_release, MRB_ARGS_NONE());

    // def DirectFB::Surface
    struct RClass* surface = NULL;
    surface = mrb_define_class_under(mrb, dfb, "Surface", mrb->object_class);

    mrb_define_method(mrb, surface, "release", mrb_directfb_surface_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "set_color", mrb_directfb_surface_set_color, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "fill_rectangle", mrb_directfb_surface_fill_rectangle, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "flip", mrb_directfb_surface_flip, MRB_ARGS_OPT(2));

    mrb_gc_arena_restore(mrb, ai);
}

void
mrb_mruby_directfb_gem_final(mrb_state* mrb) {
  // finalizer
}

