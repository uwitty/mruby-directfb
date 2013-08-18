#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

mrb_value mrb_directfb_input_device_value(mrb_state* mrb, IDirectFBInputDevice* input_device);
mrb_value mrb_directfb_input_device_wrap(mrb_state* mrb, struct RClass* c, IDirectFBInputDevice* input_device);
IDirectFBInputDevice* mrb_directfb_input_device(mrb_state *mrb, mrb_value value);
void mrb_directfb_define_input_device(mrb_state* mrb, struct RClass* outer);

