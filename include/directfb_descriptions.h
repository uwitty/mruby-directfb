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

mrb_value mrb_directfb_image_description_new(mrb_state *mrb, const DFBImageDescription* desc);
void mrb_directfb_image_description_get(mrb_state *mrb, mrb_value hash, DFBImageDescription* desc);

mrb_value mrb_directfb_display_layer_description_new(mrb_state *mrb, const DFBDisplayLayerDescription* desc);

mrb_value mrb_directfb_display_layer_source_description_new(mrb_state *mrb, const DFBDisplayLayerSourceDescription* desc);

mrb_value mrb_directfb_display_layer_configuration_new(mrb_state *mrb, const DFBDisplayLayerConfig* conf);
void mrb_directfb_display_layer_configuration_get(mrb_state *mrb, mrb_value value, DFBDisplayLayerConfig* conf);

mrb_value mrb_directfb_graphics_device_description_new(mrb_state *mrb, const DFBGraphicsDeviceDescription* desc);

mrb_value mrb_directfb_screen_description_new(mrb_state *mrb, const DFBScreenDescription* desc);

void mrb_directfb_window_description_get(mrb_state *mrb, mrb_value hash, DFBWindowDescription* desc);

mrb_value mrb_directfb_stream_description_new(mrb_state *mrb, const DFBStreamDescription* desc);

