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

void mrb_directfb_define_misc(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* region = region = mrb_define_class_under(mrb, outer, "Region", mrb->object_class);

    mrb_define_class_method(mrb, region, "new", region_new, MRB_ARGS_REQ(4));
}

