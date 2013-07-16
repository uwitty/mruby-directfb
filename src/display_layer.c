#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_surface.h"
#include "directfb_constants.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"

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

mrb_value mrb_directfb_display_layer_wrap(mrb_state* mrb, struct RClass* c, IDirectFBDisplayLayer* layer)
{
    struct mrb_directfb_display_layer_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_display_layer_data));
    data->layer = layer;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_display_layer_type, data));
}

IDirectFBDisplayLayer* mrb_directfb_get_display_layer(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_display_layer_type);
    if (data != NULL) {
        return data->layer;
    } else {
        return NULL;
    }
}

// ============================================================================
// DirectFB::DisplayLayer methods

static mrb_value display_layer_set_cooperative_level(mrb_state *mrb, mrb_value self)
{
  mrb_int cooperative_level;
  mrb_get_args(mrb, "i", &cooperative_level);

  IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
  if (layer != NULL) {
      layer->SetCooperativeLevel(layer, cooperative_level);
  }

  return mrb_nil_value();
}

static mrb_value display_layer_release(mrb_state *mrb, mrb_value self)
{
  struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_display_layer_type);
  if ((data != NULL) && (data->layer != NULL)) {
      data->layer->Release(data->layer);
      data->layer = NULL;
  }
  return mrb_nil_value();
}

static mrb_value display_layer_get_surface(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
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



void mrb_directfb_define_display_layer(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* display_layer = mrb_define_class_under(mrb, outer, "DisplayLayer", mrb->object_class);

    mrb_define_method(mrb, display_layer, "set_cooperative_level", display_layer_set_cooperative_level, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, display_layer, "get_surface", display_layer_get_surface, MRB_ARGS_NONE());
    mrb_define_method(mrb, display_layer, "release", display_layer_release, MRB_ARGS_NONE());
}

