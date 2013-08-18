#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>


mrb_value mrb_directfb_region_value(mrb_state* mrb, int x1, int y1, int x2, int y2);
DFBRegion* mrb_directfb_region(mrb_state *mrb, mrb_value value);

mrb_value mrb_directfb_rectangle_value(mrb_state* mrb, int x, int y, int w, int h);
DFBRectangle* mrb_directfb_rectangle(mrb_state *mrb, mrb_value value);

mrb_value mrb_directfb_input_device_keymap_entry_value(mrb_state* mrb, DFBInputDeviceKeymapEntry* entry);
DFBInputDeviceKeymapEntry* mrb_directfb_input_device_keymap_entry(mrb_state *mrb, mrb_value value);

mrb_value mrb_directfb_input_event_value(mrb_state* mrb, DFBInputEvent* event);
DFBInputEvent* mrb_directfb_input_event(mrb_state *mrb, mrb_value value);

mrb_value mrb_directfb_event_value(mrb_state* mrb, DFBEvent* event);
void mrb_directfb_event(mrb_state* mrb, mrb_value event_object, DFBEvent* event);

mrb_value mrb_directfb_event_buffer_stats_value(mrb_state* mrb, DFBEventBufferStats* stats);
DFBEventBufferStats* mrb_directfb_event_buffer_stats(mrb_state *mrb, mrb_value value);

void mrb_directfb_define_misc(mrb_state* mrb, struct RClass* outer);
