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

#include "directfb_surface.h"
#include "directfb_constants.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"
#include "directfb_font.h"

struct mrb_directfb_surface_data {
    IDirectFBSurface* surface;
    int width;
    int height;
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

mrb_value mrb_directfb_surface_value(mrb_state* mrb, IDirectFBSurface* surface)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "Surface")));
    return mrb_directfb_surface_wrap(mrb, c, surface);
}

mrb_value mrb_directfb_surface_wrap(mrb_state* mrb, struct RClass* c, IDirectFBSurface* surface)
{
    struct mrb_directfb_surface_data* data = NULL;

    int width = 0;
    int height = 0;
    surface->GetSize(surface, &width, &height);

    data = mrb_malloc(mrb, sizeof(struct mrb_directfb_surface_data));
    data->surface = surface;
    data->width  = width;
    data->height = height;

    {
        mrb_value obj = mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_surface_type, data));
        mrb_iv_set(mrb, obj, mrb_intern_lit(mrb, "font"), mrb_nil_value());
        return obj;
    }
}

IDirectFBSurface* mrb_directfb_surface(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_surface_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_surface_type, struct mrb_directfb_surface_data);
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
    struct mrb_directfb_surface_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_surface_type, struct mrb_directfb_surface_data);
    if ((data != NULL) && (data->surface != NULL)) {
        data->surface->Release(data->surface);
        data->surface = NULL;
    }
    return mrb_nil_value();
}

static mrb_value surface_get_capabilities(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        DFBSurfaceCapabilities caps;
        DFBResult ret = surface->GetCapabilities(surface, &caps);
        if (!ret) {
            return mrb_fixnum_value(caps);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_get_position(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        int x, y;
        DFBResult ret = surface->GetPosition(surface, &x, &y);
        if (!ret) {
            mrb_value pos[2];
            pos[0] = mrb_fixnum_value(x);
            pos[1] = mrb_fixnum_value(y);
            return mrb_ary_new_from_values(mrb, 2, pos);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_x(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        int x, dummy;
        DFBResult ret = surface->GetPosition(surface, &x, &dummy);
        if (!ret) {
            return mrb_fixnum_value(x);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_y(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        int y, dummy;
        DFBResult ret = surface->GetPosition(surface, &dummy, &y);
        if (!ret) {
            return mrb_fixnum_value(y);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_get_size(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_surface_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_surface_type, struct mrb_directfb_surface_data);
    if ((data != NULL) && (data->surface != NULL)) {
        mrb_value size[2];
        size[0] = mrb_fixnum_value(data->width);
        size[1] = mrb_fixnum_value(data->height);
        return mrb_ary_new_from_values(mrb, 2, size);
    }
    return mrb_nil_value();
}

static mrb_value surface_width(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_surface_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_surface_type, struct mrb_directfb_surface_data);
    if ((data != NULL) && (data->surface != NULL)) {
        return mrb_fixnum_value(data->width);
    }
    return mrb_nil_value();
}

static mrb_value surface_height(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_surface_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_surface_type, struct mrb_directfb_surface_data);
    if ((data != NULL) && (data->surface != NULL)) {
        return mrb_fixnum_value(data->height);
    }
    return mrb_nil_value();
}

static mrb_value surface_get_pixel_format(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        DFBSurfacePixelFormat format;
        DFBResult ret = surface->GetPixelFormat(surface, &format);
        if (!ret) {
            return mrb_fixnum_value(format);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_get_acceleration_mask(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        DFBResult ret;
        DFBAccelerationMask mask;
        IDirectFBSurface* source = NULL;
        mrb_value source_object;
        if (mrb_get_args(mrb, "|o", &source_object) > 0) {
            source = mrb_directfb_surface(mrb, source_object);
        }

        ret = surface->GetAccelerationMask(surface, source, &mask);
        if (!ret) {
            return mrb_fixnum_value(mask);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_lock(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        void* p = NULL;
        int pitch;
        DFBResult ret;
        mrb_int flags = 0;
        mrb_get_args(mrb, "i", &flags);
        pitch = 0;
        ret = surface->Lock(surface, flags, &p, &pitch);
        if (!ret) {
            mrb_value a[2];
            a[0] = mrb_cptr_value(mrb, p);
            a[1] = mrb_fixnum_value(pitch);
            return mrb_ary_new_from_values(mrb, 2, a);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_get_framebuffer_offset(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    if (surface != NULL) {
        int offset = -1;
        DFBResult ret = surface->GetFramebufferOffset(surface, &offset);
        if (!ret) {
            return mrb_fixnum_value(offset);
        }
    }
    return mrb_nil_value();
}

static mrb_value surface_unlock(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        ret = surface->Unlock(surface);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_flip(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_value o = mrb_nil_value();
        mrb_int flags = 0;
        mrb_get_args(mrb, "|oi", &o, &flags);
        ret = surface->Flip(surface, mrb_directfb_region(mrb, o), flags);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_clear(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int r, g, b, a;
        mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
        ret = surface->Clear(surface, r, g, b, a);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_set_clip(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        const DFBRegion* clip;
        mrb_value clip_object;
        mrb_get_args(mrb, "o", &clip_object);
        clip = mrb_directfb_region(mrb, clip_object);
        ret = surface->SetClip(surface, clip);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_get_clip(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        DFBRegion clip;
        ret = surface->GetClip(surface, &clip);
        if (!ret) {
            return mrb_directfb_region_value(mrb, clip.x1, clip.y1, clip.x2, clip.y2);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_set_color(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int r, g, b, a;
        mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
        ret = surface->SetColor(surface, r, g, b, a);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_set_porter_duff(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int rule;
        mrb_get_args(mrb, "i", &rule);
        ret = surface->SetPorterDuff(surface, rule);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_set_blitting_flags(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int flags;
        mrb_get_args(mrb, "i", &flags);
        ret = surface->SetBlittingFlags(surface, flags);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_blit(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_value source_object;
        mrb_value rect_object;
        IDirectFBSurface* source;
        DFBRectangle* rect;
        mrb_int x;
        mrb_int y;
        mrb_get_args(mrb, "ooii", &source_object, &rect_object, &x, &y);
        source = mrb_directfb_surface(mrb, source_object);
        rect = mrb_directfb_rectangle(mrb, rect_object);
        ret = surface->Blit(surface, source, rect, x, y);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_tile_blit(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_value source_object;
        mrb_value rect_object;
        IDirectFBSurface* source;
        DFBRectangle* rect;
        mrb_int x;
        mrb_int y;
        mrb_get_args(mrb, "ooii", &source_object, &rect_object, &x, &y);
        source = mrb_directfb_surface(mrb, source_object);
        rect = mrb_directfb_rectangle(mrb, rect_object);
        ret = surface->TileBlit(surface, source, rect, x, y);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_stretch_blit(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_value src_object;
        mrb_value src_rect_object;
        mrb_value dst_rect_object;
        mrb_get_args(mrb, "ooo", &src_object, &src_rect_object, &dst_rect_object);
        {
            IDirectFBSurface* src = mrb_directfb_surface(mrb, src_object);
            DFBRectangle* src_rect = mrb_directfb_rectangle(mrb, src_rect_object);
            DFBRectangle* dst_rect = mrb_directfb_rectangle(mrb, dst_rect_object);
            ret = surface->StretchBlit(surface, src, src_rect, dst_rect);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_set_drawing_flags(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int flags;
        mrb_get_args(mrb, "i", &flags);
        ret = surface->SetDrawingFlags(surface, flags);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_fill_rectangle(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int x, y, width, height;
        mrb_get_args(mrb, "iiii", &x, &y, &width, &height);
        ret = surface->FillRectangle(surface, x, y, width, height);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_draw_line(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int x1, y1, x2, y2;
        mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
        ret = surface->DrawLine(surface, x1, y1, x2, y2);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_fill_triangle(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int x1, y1, x2, y2, x3, y3;
        mrb_get_args(mrb, "iiiiii", &x1, &y1, &x2, &y2, &x3, &y3);
        ret = surface->FillTriangle(surface, x1, y1, x2, y2, x3, y3);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_set_font(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_value font_object;
        IDirectFBFont* font = NULL;
        mrb_get_args(mrb, "o", &font_object);
        font = mrb_directfb_font(mrb, font_object);
        ret = surface->SetFont(surface, font);
        if (!ret) {
            mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "font"), font_object);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_get_font(mrb_state *mrb, mrb_value self)
{
    return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "font"));
}

static mrb_value surface_draw_string(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        int bytes;
        char* s;
        mrb_int x;
        mrb_int y;
        mrb_int flags;
        mrb_get_args(mrb, "ziii", &s, &x, &y, &flags);
        bytes = strlen(s);
        ret = surface->DrawString(surface, s, bytes, x, y, flags);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_draw_glyph(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int glyph;
        mrb_int x;
        mrb_int y;
        mrb_int flags;
        mrb_get_args(mrb, "iiii", &glyph, &x, &y, &flags);
        ret = surface->DrawGlyph(surface, glyph, x, y, flags);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value surface_set_encoding(mrb_state *mrb, mrb_value self)
{
    IDirectFBSurface* surface = mrb_directfb_surface(mrb, self);
    DFBResult ret = -1;
    if (surface != NULL) {
        mrb_int encoding;
        mrb_get_args(mrb, "i", &encoding);
        ret = surface->SetEncoding(surface, encoding);
    }
    return mrb_fixnum_value(ret);
}

void mrb_directfb_define_surface(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::Surface
    struct RClass* surface = NULL;
    surface = mrb_define_class_under(mrb, outer, "Surface", mrb->object_class);
    MRB_SET_INSTANCE_TT(surface, MRB_TT_DATA);

    mrb_define_method(mrb, surface, "release", surface_release, MRB_ARGS_NONE());

    // retrieving information
    mrb_define_method(mrb, surface, "get_capabilities", surface_get_capabilities, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "get_position", surface_get_position, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "x", surface_x, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "y", surface_y, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "get_size", surface_get_size, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "width", surface_width, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "height", surface_height, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "get_pixel_format", surface_get_pixel_format, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "get_acceleration_mask", surface_get_acceleration_mask, MRB_ARGS_OPT(1));

    // buffer operations
    mrb_define_method(mrb, surface, "lock_impl", surface_lock, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, surface, "get_framebuffer_offset", surface_get_framebuffer_offset, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "unlock", surface_unlock, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "flip", surface_flip, MRB_ARGS_OPT(2));
    mrb_define_method(mrb, surface, "clear", surface_clear, MRB_ARGS_REQ(4));

    // drawing/blitting control
    mrb_define_method(mrb, surface, "set_clip", surface_set_clip, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, surface, "get_clip", surface_get_clip, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "set_color", surface_set_color, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "set_porter_duff", surface_set_porter_duff, MRB_ARGS_REQ(1));

    // blitting functions
    mrb_define_method(mrb, surface, "set_blitting_flags", surface_set_blitting_flags, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, surface, "blit", surface_blit, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "tile_blit", surface_tile_blit, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "stretch_blit", surface_stretch_blit, MRB_ARGS_REQ(3));

    // drawing functions
    mrb_define_method(mrb, surface, "set_drawing_flags", surface_set_drawing_flags, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, surface, "fill_rectangle", surface_fill_rectangle, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "draw_line", surface_draw_line, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, surface, "fill_triangle", surface_fill_triangle, MRB_ARGS_REQ(6));

    // text functions
    mrb_define_method(mrb, surface, "set_font", surface_set_font, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, surface, "get_font", surface_get_font, MRB_ARGS_NONE());
    mrb_define_method(mrb, surface, "draw_string", surface_draw_string, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, surface, "draw_glyph", surface_draw_glyph, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, surface, "set_encoding", surface_set_encoding, MRB_ARGS_REQ(1));
}

