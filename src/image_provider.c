#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_image_provider.h"
#include "directfb_surface.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"

struct mrb_directfb_image_provider_data {
    IDirectFBImageProvider* image_provider;
    struct context {
        mrb_state* mrb;
        mrb_value self;
    } context;
};

static void mrb_directfb_image_provider_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_image_provider_data* data = (struct mrb_directfb_image_provider_data*)p;
    if (data != NULL) {
        IDirectFBImageProvider* image_provider = (IDirectFBImageProvider*)data->image_provider;
        if (image_provider != NULL) {
            image_provider->Release(image_provider);
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_image_provider_type = {"ImageProvider", mrb_directfb_image_provider_free};

mrb_value mrb_directfb_image_provider_value(mrb_state* mrb, IDirectFBImageProvider* image_provider)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "ImageProvider")));
    return mrb_directfb_image_provider_wrap(mrb, c, image_provider);
}

mrb_value mrb_directfb_image_provider_wrap(mrb_state* mrb, struct RClass* c, IDirectFBImageProvider* image_provider)
{
    struct mrb_directfb_image_provider_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_image_provider_data));
    data->image_provider = image_provider;

    mrb_value obj = mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_image_provider_type, data));
    return obj;
}

IDirectFBImageProvider* mrb_directfb_image_provider(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_image_provider_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_image_provider_type, struct mrb_directfb_image_provider_data);
    if (data != NULL) {
        return data->image_provider;
    } else {
        return NULL;
    }
}

static struct mrb_directfb_image_provider_data* setup_render_callback(mrb_state *mrb, mrb_value self, mrb_value block)
{
    struct mrb_directfb_image_provider_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_image_provider_data));
    if (data != NULL) {
        data->context.mrb = mrb;
        data->context.self = self;
        mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "internal_context_callback"), block);
    }
    return data;
}

// ============================================================================
// DirectFB::ImageProvider methods

static mrb_value image_provider_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_image_provider_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_image_provider_type, struct mrb_directfb_image_provider_data);
    if ((data != NULL) && (data->image_provider != NULL)) {
        data->image_provider->Release(data->image_provider);
        data->image_provider = NULL;
    }
    return mrb_nil_value();
}

static mrb_value image_provider_get_surface_description(mrb_state *mrb, mrb_value self)
{
    IDirectFBImageProvider* provider = mrb_directfb_image_provider(mrb, self);
    if (provider != NULL) {
        DFBSurfaceDescription desc;
        memset(&desc, 0, sizeof(desc));
        DFBResult ret = provider->GetSurfaceDescription(provider, &desc);
        if (!ret) {
            return mrb_directfb_surface_description_new(mrb, &desc);
        }
    }
    return mrb_nil_value();
}

static mrb_value image_provider_get_image_description(mrb_state *mrb, mrb_value self)
{
    IDirectFBImageProvider* provider = mrb_directfb_image_provider(mrb, self);
    if (provider != NULL) {
        DFBImageDescription desc;
        memset(&desc, 0, sizeof(desc));
        DFBResult ret = provider->GetImageDescription(provider, &desc);
        if (!ret) {
            return mrb_directfb_image_description_new(mrb, &desc);
        }
    }
    return mrb_nil_value();
}

static mrb_value image_provider_render_to(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBImageProvider* provider = mrb_directfb_image_provider(mrb, self);
    if (provider != NULL) {
        mrb_value surface_object;
        mrb_value rect_object;
        int num = mrb_get_args(mrb, "o|o", &surface_object, &rect_object);

        IDirectFBSurface* surface = mrb_directfb_surface(mrb, surface_object);
        DFBRectangle* rect = NULL;
        if ((num >= 2) && (!mrb_nil_p(rect_object))) {
            rect = mrb_directfb_rectangle(mrb, rect_object);
        }
        ret = provider->RenderTo(provider, surface, rect);
    }
    return mrb_fixnum_value(ret);
}

static DIRenderCallbackResult render_callback(DFBRectangle* rect, void* callbackdata)
{
    //printf("c %s()\n", __func__);
    struct mrb_directfb_image_provider_data* data = (struct mrb_directfb_image_provider_data*)callbackdata;
    if (data != NULL) {
        mrb_state* mrb = data->context.mrb;
        mrb_value block = mrb_iv_get(mrb, data->context.self, mrb_intern_cstr(mrb, "internal_context_callback"));
        if (!mrb_nil_p(block)) {
            int ai = mrb_gc_arena_save(mrb);
            mrb_value rect_object = mrb_directfb_rectangle_value(mrb, rect->x, rect->y, rect->w, rect->h);
            mrb_yield(mrb, block, rect_object);
            mrb_gc_arena_restore(mrb, ai);
        }
    }
    return 0;
}

static mrb_value image_provider_set_render_callback(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBImageProvider* provider = mrb_directfb_image_provider(mrb, self);
    if (provider != NULL) {
        mrb_value block;
        mrb_get_args(mrb, "&", &block);

        struct mrb_directfb_image_provider_data* data = setup_render_callback(mrb, self, block);
        ret = provider->SetRenderCallback(provider, render_callback, data);
        printf("end:%s()\n", __func__);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value image_provider_write_back(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBImageProvider* provider = mrb_directfb_image_provider(mrb, self);
    if (provider != NULL) {
        mrb_value surface_object;
        mrb_value rect_object;
        char* filename;
        mrb_get_args(mrb, "ooz", &surface_object, &rect_object, &filename);

        IDirectFBSurface* surface = mrb_directfb_surface(mrb, surface_object);
        DFBRectangle* rect = mrb_directfb_rectangle(mrb, rect_object);
        ret = provider->WriteBack(provider, surface, rect, filename);
    }
    return mrb_fixnum_value(ret);
}

void mrb_directfb_define_image_provider(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::ImageProvider
    struct RClass* image_provider = mrb_define_class_under(mrb, outer, "ImageProvider", mrb->object_class);

    mrb_define_method(mrb, image_provider, "release", image_provider_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, image_provider, "get_surface_description", image_provider_get_surface_description, MRB_ARGS_NONE());
    mrb_define_method(mrb, image_provider, "get_image_description", image_provider_get_image_description, MRB_ARGS_NONE());
    mrb_define_method(mrb, image_provider, "render_to", image_provider_render_to, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
    mrb_define_method(mrb, image_provider, "set_render_callback", image_provider_set_render_callback, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, image_provider, "write_back", image_provider_write_back, MRB_ARGS_REQ(3));
}


