#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

mrb_value mrb_directfb_window_value(mrb_state* mrb, IDirectFBWindow* window);
IDirectFBWindow* mrb_directfb_window(mrb_state *mrb, mrb_value value);

mrb_value mrb_directfb_window_wrap(mrb_state* mrb, struct RClass* c, IDirectFBWindow* window);
void mrb_directfb_define_window(mrb_state* mrb, struct RClass* outer);

