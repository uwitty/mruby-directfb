
#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

mrb_value mrb_directfb_surface_wrap(mrb_state* mrb, struct RClass* c, IDirectFBSurface* surface);
IDirectFBSurface* mrb_directfb_get_surface(mrb_state *mrb, mrb_value value);
void mrb_directfb_define_surface(mrb_state* mrb, struct RClass* outer);
