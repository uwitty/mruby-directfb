#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

mrb_value mrb_directfb_video_provider_value(mrb_state* mrb, IDirectFBVideoProvider* video_provider);
mrb_value mrb_directfb_video_provider_wrap(mrb_state* mrb, struct RClass* c, IDirectFBVideoProvider* video_provider);
IDirectFBVideoProvider* mrb_directfb_video_provider(mrb_state *mrb, mrb_value value);
void mrb_directfb_define_video_provider(mrb_state* mrb, struct RClass* outer);

