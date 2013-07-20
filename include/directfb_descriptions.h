#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

void mrb_directfb_input_device_description_get(mrb_state *mrb, mrb_value value, DFBInputDeviceDescription* desc);
mrb_value mrb_directfb_input_device_description_new(mrb_state *mrb, const DFBInputDeviceDescription* desc);

mrb_value mrb_directfb_surface_description_new(mrb_state *mrb, const DFBSurfaceDescription* desc);
void mrb_directfb_surface_description_get(mrb_state *mrb, mrb_value value, DFBSurfaceDescription* desc);

void mrb_directfb_font_description_get(mrb_state *mrb, mrb_value hash, DFBFontDescription* desc);
