#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_font.h"

// ============================================================================
// IDirectFB object

struct mrb_directfb_font_data {
    IDirectFBFont* font;
};

static void mrb_directfb_font_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_font_data* data = (struct mrb_directfb_font_data*)p;
    if (data != NULL) {
        if (data->font != NULL) {
            data->font->Release(data->font);
            data->font = NULL;
        }
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_font_type = {"Font", mrb_directfb_font_free };

static mrb_value mrb_directfb_wrap(mrb_state* mrb, struct RClass* c, IDirectFBFont* font)
{
    struct mrb_directfb_font_data* data = (struct mrb_directfb_font_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_font_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    data->font = font;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_font_type, data));
}

IDirectFBFont* get_directfb_font(mrb_state* mrb, mrb_value value)
{
    struct mrb_directfb_font_data* data = (struct mrb_directfb_font_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_font_type);
    return (data != NULL)? data->font : NULL;
}

// ============================================================================
// DirectFB methods

static mrb_value font_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_font_data* data = (struct mrb_directfb_font_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_font_type);
    if ((data != NULL) && (data->font != NULL)) {
        data->font->Release(data->font);
        data->font = NULL;
    }
    return mrb_nil_value();
}

static mrb_value font_get_ascender(mrb_state *mrb, mrb_value self)
{
    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        int ascender = 0;
        if (!font->GetAscender(font, &ascender)) {
            return mrb_fixnum_value(ascender);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_descender(mrb_state *mrb, mrb_value self)
{
    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        int descender = 0;
        if (!font->GetDescender(font, &descender)) {
            return mrb_fixnum_value(descender);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_height(mrb_state *mrb, mrb_value self)
{
    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        int height = 0;
        if (!font->GetHeight(font, &height)) {
            return mrb_fixnum_value(height);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_max_advance(mrb_state *mrb, mrb_value self)
{
    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        int max_advance = 0;
        if (!font->GetMaxAdvance(font, &max_advance)) {
            return mrb_fixnum_value(max_advance);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_kerning_x(mrb_state *mrb, mrb_value self)
{
    mrb_int prev, current;
    mrb_get_args(mrb, "ii", &prev, &current);

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        int kern_x, kern_y;
        if (!font->GetKerning(font, prev, current, &kern_x, &kern_y)) {
            return mrb_fixnum_value(kern_x);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_kerning_y(mrb_state *mrb, mrb_value self)
{
    mrb_int prev, current;
    mrb_get_args(mrb, "ii", &prev, &current);

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        int kern_x, kern_y;
        if (!font->GetKerning(font, prev, current, &kern_x, &kern_y)) {
            return mrb_fixnum_value(kern_y);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_string_width(mrb_state *mrb, mrb_value self)
{
    char* s;
    mrb_get_args(mrb, "z", &s);
    int bytes = strlen(s);
    int width;

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        if (!font->GetStringWidth(font, s, bytes, &width)) {
            return mrb_fixnum_value(width);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_string_extents(mrb_state *mrb, mrb_value self)
{
    char* s;
    mrb_get_args(mrb, "z", &s);
    int bytes = strlen(s);
    int width;

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        DFBRectangle logical_rect, ink_rect;
        if (!font->GetStringExtents(font, s, bytes, &logical_rect, &ink_rect)) {
            // TODO:
            return mrb_fixnum_value(width);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_glyph_extents(mrb_state *mrb, mrb_value self)
{
    mrb_int c;
    mrb_get_args(mrb, "i", &c);

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        DFBRectangle rect;
        int advance;
        if (!font->GetGlyphExtents(font, c, &rect, &advance)) {
            // TODO:
            return mrb_fixnum_value(advance);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_get_string_break(mrb_state *mrb, mrb_value self)
{
    char* s;
    mrb_int max_width;
    mrb_get_args(mrb, "zi", &s, &max_width);

    int bytes = strlen(s);

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        int ret_width;
        int ret_string_length;
        const char* next_line;
        if (!font->GetStringBreak(font, s, bytes, max_width, &ret_width, &ret_string_length, &next_line)) {
            // TODO:
            return mrb_str_new_cstr(mrb, next_line);
        }
    }
    return mrb_nil_value();
}

static mrb_value font_set_encoding(mrb_state *mrb, mrb_value self)
{
    mrb_int encoding;
    mrb_get_args(mrb, "i", &encoding);

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        font->SetEncoding(font, encoding);
    }
    return mrb_nil_value();
}

struct enum_encoding_callback_arg {
    mrb_state* mrb;
    mrb_value* block;
};

static DFBEnumerationResult enum_encoding_callback(DFBTextEncodingID device_id, const char* name, void* callbackdata)
{
    struct enum_encoding_callback_arg* arg = (struct enum_encoding_callback_arg*)callbackdata;
    printf("%s(device_id:%d)\n", __func__, device_id);
    mrb_value args[2];
    args[0] = mrb_fixnum_value(device_id);
    args[1] = mrb_str_new_cstr(arg->mrb, name);
    mrb_yield_argv(arg->mrb, *(arg->block), 2, args);
    return 0;
}

static mrb_value font_enum_encodings(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    mrb_value block;
    mrb_get_args(mrb, "&", &block);

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        struct enum_encoding_callback_arg arg = {mrb, &block};
        ret = font->EnumEncodings(font, enum_encoding_callback, (void*)&arg);
    }
    printf("end:%s()\n", __func__);
    return mrb_fixnum_value(ret);
}

static mrb_value font_find_encoding(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    char* name;
    mrb_get_args(mrb, "z", &name);

    IDirectFBFont* font = get_directfb_font(mrb, self);
    if (font != NULL) {
        DFBTextEncodingID encoding_id;
        ret = font->FindEncoding(font, name, &encoding_id);
        return mrb_fixnum_value(encoding_id);
    }
    printf("end:%s()\n", __func__);
    return mrb_nil_value();
}

void mrb_directfb_define_font(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::Font
    struct RClass* font = mrb_define_class_under(mrb, outer, "Font", mrb->object_class);

    mrb_define_method(mrb, font, "release", font_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, font, "get_ascender", font_get_ascender, MRB_ARGS_NONE());
    mrb_define_method(mrb, font, "get_descender", font_get_descender, MRB_ARGS_NONE());
    mrb_define_method(mrb, font, "get_height", font_get_height, MRB_ARGS_NONE());
    mrb_define_method(mrb, font, "get_max_advance", font_get_max_advance, MRB_ARGS_NONE());
    mrb_define_method(mrb, font, "get_kerning_x", font_get_kerning_x, MRB_ARGS_NONE());
    mrb_define_method(mrb, font, "get_kerning_y", font_get_kerning_y, MRB_ARGS_NONE());
    mrb_define_method(mrb, font, "get_string_width", font_get_string_width, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, font, "get_string_extents", font_get_string_extents, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, font, "get_glyph_extents", font_get_glyph_extents, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, font, "get_string_break", font_get_string_break, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, font, "set_encoding", font_set_encoding, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, font, "enum_encodings", font_enum_encodings, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, font, "find_encoding", font_find_encoding, MRB_ARGS_REQ(1));
}


