#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>


mrb_value mrb_directfb_region_wrap(mrb_state* mrb, struct RClass* c, int x1, int y1, int x2, int y2);
DFBRegion* mrb_directfb_get_region(mrb_state *mrb, mrb_value value);

mrb_value mrb_directfb_rectangle_wrap(mrb_state* mrb, struct RClass* c, int x, int y, int w, int h);
DFBRectangle* mrb_directfb_get_rectangle(mrb_state *mrb, mrb_value value);

void mrb_directfb_define_misc(mrb_state* mrb, struct RClass* outer);
