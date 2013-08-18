#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>


mrb_value mrb_directfb_display_layer_value(mrb_state* mrb, IDirectFBDisplayLayer* layer);
mrb_value mrb_directfb_display_layer_wrap(mrb_state* mrb, struct RClass* c, IDirectFBDisplayLayer* layer);
IDirectFBDisplayLayer* mrb_directfb_display_layer(mrb_state *mrb, mrb_value value);

void mrb_directfb_define_display_layer(mrb_state* mrb, struct RClass* outer);

