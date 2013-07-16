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

mrb_value mrb_directfb_surface_wrap(mrb_state* mrb, struct RClass* c, IDirectFBSurface* surface)
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
// DirectFB::Surface methods

static mrb_value surface_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_surface_data* data = (struct mrb_directfb_surface_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_surface_type);
    if ((data != NULL) && (data->surface != NULL)) {
        data->surface->Release(data->surface);
        data->surface = NULL;
    }
    return mrb_nil_value();
}

static mrb_value surface_set_color(mrb_state *mrb, mrb_value self)
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

static mrb_value surface_fill_rectangle(mrb_state *mrb, mrb_value self)
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

static mrb_value surface_flip(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = get_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_value o;
        mrb_int flags = 0;
        mrb_get_args(mrb, "|oi", &o, &flags);
        ret = surface->Flip(surface, mrb_directfb_get_region(mrb, o), flags);
    }
    return mrb_fixnum_value(ret);
}

void mrb_directfb_define_surface(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::Surface
    struct RClass* surface = NULL;
    surface = mrb_define_class_under(mrb, outer, "Surface", mrb->object_class);

    mrb_define_method(mrb, surface, "release", surface_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "set_color", surface_set_color, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "fill_rectangle", surface_fill_rectangle, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "flip", surface_flip, MRB_ARGS_OPT(2));
}

