#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

mrb_value mrb_directfb_font_wrap(mrb_state* mrb, struct RClass* c, IDirectFBFont* font);
IDirectFBFont* mrb_directfb_font_get(mrb_state* mrb, mrb_value value);
void mrb_directfb_define_font(mrb_state* mrb, struct RClass* outer);

