#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

void mrb_directfb_define_descriptions(mrb_state* mrb, struct RClass* outer);

mrb_value mrb_directfb_input_device_description_wrap(mrb_state* mrb, struct RClass* c, DFBInputDeviceDescription* desc);
DFBInputDeviceDescription* mrb_directfb_input_device_description_get(mrb_state *mrb, mrb_value value);

mrb_value mrb_directfb_surface_description_wrap(mrb_state* mrb, struct RClass* c, DFBSurfaceDescription* desc);
DFBSurfaceDescription* mrb_directfb_surface_description_get(mrb_state *mrb, mrb_value value);
