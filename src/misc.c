#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_misc.h"

// ============================================================================
// IDirectFB::Region object - DFBRegion

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

mrb_value mrb_directfb_region_wrap(mrb_state* mrb, struct RClass* c, int x1, int y1, int x2, int y2)
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

DFBRegion* mrb_directfb_get_region(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_region_data* data = (struct mrb_directfb_region_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_region_type);
    return (data != NULL)? &data->region : NULL;
}

static mrb_value region_new(mrb_state *mrb, mrb_value self)
{
    mrb_int x1, y1, x2, y2;
    mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
    return mrb_directfb_region_wrap(mrb, mrb_class_ptr(self), x1, y1, x2, y2);
}

// ============================================================================
// IDirectFB::Rectangle object - DFBRectangle

struct mrb_directfb_rectangle_data {
    DFBRectangle rectangle;
};

static void mrb_directfb_rectangle_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_rectangle_data* data = (struct mrb_directfb_rectangle_data*)p;
    if (data != NULL) {
        mrb_free(mrb, p);
    }
}

static struct mrb_data_type mrb_directfb_rectangle_type = {"Rectangle", mrb_directfb_rectangle_free};

mrb_value mrb_directfb_rectangle_wrap(mrb_state* mrb, struct RClass* c, int x, int y, int w, int h)
{
    struct mrb_directfb_rectangle_data* data = (struct mrb_directfb_rectangle_data*)mrb_malloc(mrb, sizeof(struct mrb_directfb_rectangle_data));
    if (data == NULL) {
        return mrb_nil_value();
    }
    data->rectangle.x = x;
    data->rectangle.y = y;
    data->rectangle.w = w;
    data->rectangle.h = h;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_rectangle_type, data));
}

DFBRectangle* mrb_directfb_get_rectangle(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_rectangle_data* data = (struct mrb_directfb_rectangle_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_rectangle_type);
    return (data != NULL)? &data->rectangle : NULL;
}

static mrb_value rectangle_new(mrb_state *mrb, mrb_value self)
{
    mrb_int x, y, w, h;
    mrb_get_args(mrb, "iiii", &x, &y, &w, &h);
    return mrb_directfb_rectangle_wrap(mrb, mrb_class_ptr(self), x, y, w, h);
}

static mrb_value rectangle_x(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->x) : mrb_nil_value();
}

static mrb_value rectangle_y(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->y) : mrb_nil_value();
}

static mrb_value rectangle_w(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->w) : mrb_nil_value();
}

static mrb_value rectangle_h(mrb_state *mrb, mrb_value self)
{
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    return (rect != NULL)? mrb_fixnum_value(rect->h) : mrb_nil_value();
}

static mrb_value rectangle_x_set(mrb_state *mrb, mrb_value self)
{
    mrb_int x;
    mrb_get_args(mrb, "i", &x);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    if (rect != NULL) {
        rect->x = x;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_y_set(mrb_state *mrb, mrb_value self)
{
    mrb_int y;
    mrb_get_args(mrb, "i", &y);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    if (rect != NULL) {
        rect->y = y;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_w_set(mrb_state *mrb, mrb_value self)
{
    mrb_int w;
    mrb_get_args(mrb, "i", &w);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    if (rect != NULL) {
        rect->w = w;
    }
    return mrb_nil_value();
}

static mrb_value rectangle_h_set(mrb_state *mrb, mrb_value self)
{
    mrb_int h;
    mrb_get_args(mrb, "i", &h);
    DFBRectangle* rect = mrb_directfb_get_rectangle(mrb, self);
    if (rect != NULL) {
        rect->h = h;
    }
    return mrb_nil_value();
}

static void define_rectangle(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* rectangle = mrb_define_class_under(mrb, outer, "Rectangle", mrb->object_class);
    mrb_define_class_method(mrb, rectangle, "new", rectangle_new, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, rectangle, "x", rectangle_x, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "y", rectangle_y, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "w", rectangle_w, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "h", rectangle_h, MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle, "x=", rectangle_x_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle, "y=", rectangle_y_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle, "w=", rectangle_w_set, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle, "h=", rectangle_h_set, MRB_ARGS_REQ(1));
}

// ============================================================================

void mrb_directfb_define_misc(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* region = mrb_define_class_under(mrb, outer, "Region", mrb->object_class);
    mrb_define_class_method(mrb, region, "new", region_new, MRB_ARGS_REQ(4));

    define_rectangle(mrb, outer);
}

