#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_video_provider.h"
#include "directfb_surface.h"
#include "directfb_event_buffer.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"

// ============================================================================
// DirectFB::VideoProvider DATA

struct mrb_directfb_video_provider_play_to_callback_arg {
	IDirectFBSurface* surface;
	DFBRegion region;
};

struct mrb_directfb_video_provider_data {
    IDirectFBVideoProvider* video_provider;
    struct mrb_directfb_video_provider_play_to_callback_arg* play_to_callback_arg;
};

static void mrb_directfb_video_provider_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_video_provider_data* data = (struct mrb_directfb_video_provider_data*)p;
    if (data != NULL) {
        IDirectFBVideoProvider* video_provider = (IDirectFBVideoProvider*)data->video_provider;
        if (video_provider != NULL) {
            video_provider->Release(video_provider);
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_video_provider_type = {"VideoProvider", mrb_directfb_video_provider_free};

mrb_value mrb_directfb_video_provider_value(mrb_state* mrb, IDirectFBVideoProvider* video_provider)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "VideoProvider")));
    return mrb_directfb_video_provider_wrap(mrb, c, video_provider);
}

mrb_value mrb_directfb_video_provider_wrap(mrb_state* mrb, struct RClass* c, IDirectFBVideoProvider* video_provider)
{
    struct mrb_directfb_video_provider_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_video_provider_data));
    data->video_provider = video_provider;
    data->play_to_callback_arg = NULL;

    mrb_value obj = mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_video_provider_type, data));
    return obj;
}

IDirectFBVideoProvider* mrb_directfb_video_provider(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_video_provider_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_video_provider_type, struct mrb_directfb_video_provider_data);
    if (data != NULL) {
        return data->video_provider;
    } else {
        return NULL;
    }
}

void play_to_callback(void *ctx)
{
    struct mrb_directfb_video_provider_play_to_callback_arg* arg = (struct mrb_directfb_video_provider_play_to_callback_arg*)ctx;
    if (arg == NULL) {
        return ;
    }

    IDirectFBSurface* surface = arg->surface;
    surface->Flip(surface, &arg->region, 0);
}

static void free_play_to_callback(mrb_state* mrb, struct mrb_directfb_video_provider_data* data)
{
    if (data->play_to_callback_arg != NULL) {
        mrb_free(mrb, data->play_to_callback_arg);
    }
}

static struct mrb_directfb_video_provider_play_to_callback_arg* alloc_play_to_callback_arg(mrb_state* mrb, IDirectFBSurface* surface, const DFBRectangle* rect)
{
    struct mrb_directfb_video_provider_play_to_callback_arg* arg = mrb_malloc(mrb, sizeof(struct mrb_directfb_video_provider_play_to_callback_arg));
    if (arg != NULL) {
        arg->surface = surface;
        arg->region.x1 = rect->x;
        arg->region.y1 = rect->y;
        arg->region.x2 = rect->x + rect->w;
        arg->region.y2 = rect->y + rect->h;

        return arg;
    } else {
        return NULL;
    }
}

// ============================================================================
// DirectFB::VideoProvider methods

static mrb_value video_provider_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_video_provider_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_video_provider_type, struct mrb_directfb_video_provider_data);
    if ((data != NULL) && (data->video_provider != NULL)) {
        data->video_provider->Release(data->video_provider);
        data->video_provider = NULL;
    }
    return mrb_nil_value();
}

static mrb_value video_provider_get_capabilities(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        DFBVideoProviderCapabilities caps;
        DFBResult ret = provider->GetCapabilities(provider, &caps);
        if (!ret) {
            return mrb_fixnum_value(caps);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_get_surface_description(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        DFBSurfaceDescription desc;
        DFBResult ret = provider->GetSurfaceDescription(provider, &desc);
        if (!ret) {
            return mrb_directfb_surface_description_new(mrb, &desc);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_get_stream_description(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        DFBStreamDescription desc;
        DFBResult ret = provider->GetStreamDescription(provider, &desc);
        if (!ret) {
            return mrb_directfb_stream_description_new(mrb, &desc);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_play_to(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_video_provider_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_video_provider_type, struct mrb_directfb_video_provider_data);
    IDirectFBVideoProvider* provider = (data != NULL)? data->video_provider : NULL;

    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_value surface_object = mrb_nil_value();
        mrb_value rect_object= mrb_nil_value();
        mrb_value block = mrb_nil_value();
        mrb_get_args(mrb, "oo|&", &surface_object, &rect_object, &block);

        IDirectFBSurface* surface = mrb_directfb_surface(mrb, surface_object);
        DFBRectangle* rect = mrb_directfb_rectangle(mrb, rect_object);

        DVFrameCallback callback = play_to_callback;
        void* callback_arg = (void*)alloc_play_to_callback_arg(mrb, surface, rect);
        if (callback_arg == NULL) {
            return mrb_fixnum_value(ret);
        }

        ret = provider->PlayTo(provider, surface, rect, callback, callback_arg);
        if (!ret) {
            free_play_to_callback(mrb, data);
            data->play_to_callback_arg = callback_arg;
        } else {
            free_play_to_callback(mrb, data);
            mrb_free(mrb, callback_arg);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_stop(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_video_provider_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_video_provider_type, struct mrb_directfb_video_provider_data);
    IDirectFBVideoProvider* provider = (data != NULL)? data->video_provider : NULL;
    DFBResult ret = -1;

    if (provider != NULL) {
        DFBResult ret = provider->Stop(provider);
        if (!ret) {
            mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "play_to_callback"), mrb_nil_value());
            free_play_to_callback(mrb, data);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_get_status(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        DFBVideoProviderStatus status;
        DFBResult ret = provider->GetStatus(provider, &status);
        if (!ret) {
            return mrb_fixnum_value(status);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_seek_to(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_float seconds;
        mrb_get_args(mrb, "f", &seconds);
        ret = provider->SeekTo(provider, (double)seconds);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_get_pos(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        double seconds;
        DFBResult ret = provider->GetPos(provider, &seconds);
        if (!ret) {
            return mrb_float_value(mrb, ret);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_get_length(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        double seconds;
        DFBResult ret = provider->GetLength(provider, &seconds);
        if (!ret) {
            return mrb_float_value(mrb, ret);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_get_color_adjustment(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        DFBColorAdjustment adjustment;
        DFBResult ret = provider->GetColorAdjustment(provider, &adjustment);
        if (!ret) {
            return mrb_directfb_color_adjustment_value(mrb, &adjustment);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_set_color_adjustment(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_value adjustment_object = mrb_nil_value();
        mrb_get_args(mrb, "o", &adjustment_object);
        DFBColorAdjustment* adjustment = mrb_directfb_color_adjustment(mrb, adjustment_object);
        ret = provider->SetColorAdjustment(provider, adjustment);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_send_event(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_value event_object = mrb_nil_value();
        mrb_get_args(mrb, "o", &event_object);
        DFBEvent event;
        mrb_directfb_event(mrb, event_object, &event);
        ret = provider->SendEvent(provider, &event);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_set_playback_flags(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_int flags;
        mrb_get_args(mrb, "i", &flags);
        ret = provider->SetPlaybackFlags(provider, flags);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_set_speed(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_float multiplier;
        mrb_get_args(mrb, "f", &multiplier);
        ret = provider->SetSpeed(provider, (double)multiplier);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_get_speed(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        double multiplier;
        DFBResult ret = provider->GetSpeed(provider, &multiplier);
        if (!ret) {
            return mrb_float_value(mrb, multiplier);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_set_volume(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_float volume;
        mrb_get_args(mrb, "f", &volume);
        ret = provider->SetVolume(provider, (float)volume);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_get_volume(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        float volume;
        DFBResult ret = provider->GetVolume(provider, &volume);
        if (!ret) {
            return mrb_float_value(mrb, volume);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_set_stream_attributes(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_value attributes_object;
        mrb_get_args(mrb, "o", &attributes_object);
        DFBStreamAttributes* attributes = mrb_directfb_stream_attributes(mrb, attributes_object);
        ret = provider->SetStreamAttributes(provider, *attributes);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_set_audio_outputs(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_int outputs;
        mrb_get_args(mrb, "i", &outputs);
        DFBVideoProviderAudioUnits units = outputs;
        ret = provider->SetAudioOutputs(provider, &units);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_get_audio_outputs(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    if (provider != NULL) {
        DFBVideoProviderAudioUnits units;
        DFBResult ret = provider->GetAudioOutputs(provider, &units);
        if (!ret) {
            return mrb_fixnum_value(units);
        }
    }
    return mrb_nil_value();
}

static mrb_value video_provider_set_audio_delay(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_int delay;
        mrb_get_args(mrb, "i", &delay);
        ret = provider->SetAudioDelay(provider, delay);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_create_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        IDirectFBEventBuffer* buffer;
        ret = provider->CreateEventBuffer(provider, &buffer);
        if (!ret) {
            return mrb_directfb_event_buffer_value(mrb, buffer);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_enable_events(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_int mask;
        mrb_get_args(mrb, "i", &mask);
        ret = provider->EnableEvents(provider, (DFBVideoProviderEventType)mask);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_disable_events(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (provider != NULL) {
        mrb_int mask;
        mrb_get_args(mrb, "i", &mask);
        ret = provider->DisableEvents(provider, (DFBVideoProviderEventType)mask);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_attach_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* video_provider = mrb_directfb_video_provider(mrb, self);
    DFBResult ret = -1;
    if (video_provider != NULL) {
        mrb_value buffer_object;
        mrb_get_args(mrb, "o", &buffer_object);
        IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, buffer_object);
        ret = video_provider->AttachEventBuffer(video_provider, buffer);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value video_provider_detach_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFBVideoProvider* video_provider = mrb_directfb_video_provider(mrb, self);

    DFBResult ret = -1;
    if (video_provider != NULL) {
        mrb_value buffer_object;
        mrb_get_args(mrb, "o", &buffer_object);
        IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, buffer_object);
        ret = video_provider->DetachEventBuffer(video_provider, buffer);
    }
    return mrb_fixnum_value(ret);
}

void mrb_directfb_define_video_provider(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::VideoProvider
    struct RClass* video_provider = mrb_define_class_under(mrb, outer, "VideoProvider", mrb->object_class);
    MRB_SET_INSTANCE_TT(video_provider, MRB_TT_DATA);

    mrb_define_method(mrb, video_provider, "release", video_provider_release, MRB_ARGS_NONE());

    // retrieving information
    mrb_define_method(mrb, video_provider, "get_capabilities", video_provider_get_capabilities, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "get_surface_description", video_provider_get_surface_description, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "get_stream_description", video_provider_get_stream_description, MRB_ARGS_NONE());

    // playback
    mrb_define_method(mrb, video_provider, "play_to", video_provider_play_to, MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
    mrb_define_method(mrb, video_provider, "stop", video_provider_stop, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "get_status", video_provider_get_status, MRB_ARGS_NONE());

    // media controll
    mrb_define_method(mrb, video_provider, "seek_to", video_provider_seek_to, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "get_pos", video_provider_get_pos, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "get_length", video_provider_get_length, MRB_ARGS_NONE());

    // color adjustment
    mrb_define_method(mrb, video_provider, "get_color_adjustment", video_provider_get_color_adjustment, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "set_color_adjustment", video_provider_set_color_adjustment, MRB_ARGS_REQ(1));

    // interactivity
    mrb_define_method(mrb, video_provider, "send_event", video_provider_send_event, MRB_ARGS_REQ(1));

    // advanced control
    mrb_define_method(mrb, video_provider, "set_playback_flags", video_provider_set_playback_flags, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "set_speed", video_provider_set_speed, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "get_speed", video_provider_get_speed, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "set_volume", video_provider_set_volume, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "get_volume", video_provider_get_volume, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "set_stream_attributes", video_provider_set_stream_attributes, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "set_audio_outputs", video_provider_set_audio_outputs, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "get_audio_outputs", video_provider_get_audio_outputs, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "set_audio_delay", video_provider_set_audio_delay, MRB_ARGS_REQ(1));

    // event buffers
    mrb_define_method(mrb, video_provider, "create_event_buffer", video_provider_create_event_buffer, MRB_ARGS_NONE());
    mrb_define_method(mrb, video_provider, "enable_events", video_provider_enable_events, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "disable_events", video_provider_disable_events, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "attach_event_buffer", video_provider_attach_event_buffer, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, video_provider, "detach_event_buffer", video_provider_detach_event_buffer, MRB_ARGS_REQ(1));
}

