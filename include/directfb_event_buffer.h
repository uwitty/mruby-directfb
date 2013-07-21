#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

mrb_value mrb_directfb_event_buffer_wrap(mrb_state* mrb, struct RClass* c, IDirectFBEventBuffer* event_buffer);
IDirectFBSurface* mrb_directfb_get_event_buffer(mrb_state *mrb, mrb_value value);
void mrb_directfb_define_event_buffer(mrb_state* mrb, struct RClass* outer);


