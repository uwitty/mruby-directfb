#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

#include "directfb_event_buffer.h"
#include "directfb_surface.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"

struct mrb_directfb_event_buffer_data {
    IDirectFBEventBuffer* event_buffer;
};

static void mrb_directfb_event_buffer_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_event_buffer_data* data = (struct mrb_directfb_event_buffer_data*)p;
    if (data != NULL) {
        IDirectFBEventBuffer* event_buffer = (IDirectFBEventBuffer*)data->event_buffer;
        if (event_buffer != NULL) {
            event_buffer->Release(event_buffer);
            data->event_buffer = NULL;
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_event_buffer_type = {"EventBuffer", mrb_directfb_event_buffer_free};

mrb_value mrb_directfb_event_buffer_wrap(mrb_state* mrb, struct RClass* c, IDirectFBEventBuffer* event_buffer)
{
    struct mrb_directfb_event_buffer_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_event_buffer_data));
    data->event_buffer = event_buffer;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_event_buffer_type, data));
}

mrb_value mrb_directfb_event_buffer_value(mrb_state* mrb, IDirectFBEventBuffer* event_buffer)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern_lit(mrb, "EventBuffer")));
    return mrb_directfb_event_buffer_wrap(mrb, c, event_buffer);
}

IDirectFBEventBuffer* mrb_directfb_event_buffer(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_event_buffer_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_event_buffer_type, struct mrb_directfb_event_buffer_data);
    if (data != NULL) {
        return data->event_buffer;
    } else {
        return NULL;
    }
}

// ============================================================================
// DirectFB::EventBuffer methods

static mrb_value event_buffer_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_event_buffer_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_event_buffer_type, struct mrb_directfb_event_buffer_data);
    if ((data != NULL) && (data->event_buffer != NULL)) {
        data->event_buffer->Release(data->event_buffer);
        data->event_buffer = NULL;
    }
    return mrb_nil_value();
}

static mrb_value event_buffer_reset(mrb_state* mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        ret = buffer->Reset(buffer);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value event_buffer_wait_for_event(mrb_state* mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        ret = buffer->WaitForEvent(buffer);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value event_buffer_wait_for_event_with_timeout(mrb_state* mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        mrb_int secs, msecs;
        mrb_get_args(mrb, "ii", &secs, &msecs);
        ret = buffer->WaitForEventWithTimeout(buffer, secs, msecs);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value event_buffer_get_event(mrb_state* mrb, mrb_value self)
{
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        DFBEvent event;
        DFBResult ret = buffer->GetEvent(buffer, &event);
        if (!ret) {
            return mrb_directfb_event_value(mrb, &event);
        }
    }
    return mrb_nil_value();
}

static mrb_value event_buffer_peek_event(mrb_state* mrb, mrb_value self)
{
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        DFBEvent event;
        DFBResult ret = buffer->PeekEvent(buffer, &event);
        if (!ret) {
            return mrb_directfb_event_value(mrb, &event);
        }
    }
    return mrb_nil_value();
}

static mrb_value event_buffer_has_event(mrb_state* mrb, mrb_value self)
{
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        DFBResult ret = buffer->HasEvent(buffer);
        return (ret == DFB_OK)? mrb_true_value() : mrb_false_value();
    }

    return mrb_nil_value();
}

static mrb_value event_buffer_post_event(mrb_state* mrb, mrb_value self)
{
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        DFBEvent event;
        mrb_value event_object;
        mrb_get_args(mrb, "o", &event_object);
        mrb_directfb_event(mrb, event_object, &event);
        return mrb_fixnum_value(buffer->PostEvent(buffer, &event));
    }
    return mrb_nil_value();
}

static mrb_value event_buffer_wake_up(mrb_state* mrb, mrb_value self)
{
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        return mrb_fixnum_value(buffer->WakeUp(buffer));
    }

    return mrb_nil_value();
}

#if 0
static mrb_value event_buffer_create_file_descriptor(mrb_state* mrb, mrb_value self)
{
    return mrb_nil_value();
}
#endif

static mrb_value event_buffer_enable_statistics(mrb_state* mrb, mrb_value self)
{
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        DFBBoolean enable;
        mrb_bool b;
        mrb_get_args(mrb, "b", &b);
        enable = (b != FALSE)? DFB_TRUE : DFB_FALSE;
        return mrb_fixnum_value(buffer->EnableStatistics(buffer, enable));
    }

    return mrb_nil_value();
}

static mrb_value event_buffer_get_statistics(mrb_state* mrb, mrb_value self)
{
    IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, self);
    if (buffer != NULL) {
        DFBEventBufferStats stats;
        DFBResult ret = buffer->GetStatistics(buffer, &stats);
        if (!ret) {
            return mrb_directfb_event_buffer_stats_value(mrb, &stats);
        }
    }

    return mrb_nil_value();
}

void mrb_directfb_define_event_buffer(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::EventBuffer
    struct RClass* event_buffer = mrb_define_class_under(mrb, outer, "EventBuffer", mrb->object_class);
    MRB_SET_INSTANCE_TT(event_buffer, MRB_TT_DATA);

    mrb_define_method(mrb, event_buffer, "release", event_buffer_release, MRB_ARGS_NONE());
    mrb_define_method(mrb, event_buffer, "reset", event_buffer_reset, MRB_ARGS_NONE());
    mrb_define_method(mrb, event_buffer, "wait_for_event", event_buffer_wait_for_event, MRB_ARGS_NONE());
    mrb_define_method(mrb, event_buffer, "wait_for_event_with_timeout", event_buffer_wait_for_event_with_timeout, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, event_buffer, "get_event", event_buffer_get_event, MRB_ARGS_NONE());
    mrb_define_method(mrb, event_buffer, "peek_event", event_buffer_peek_event, MRB_ARGS_NONE());
    mrb_define_method(mrb, event_buffer, "has_event", event_buffer_has_event, MRB_ARGS_NONE());
    mrb_define_method(mrb, event_buffer, "post_event", event_buffer_post_event, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, event_buffer, "wake_up", event_buffer_wake_up, MRB_ARGS_NONE());
    /* mrb_define_method(mrb, event_buffer, "create_file_descriptor", event_buffer_create_file_descriptor, MRB_ARGS_NONE()); */
    mrb_define_method(mrb, event_buffer, "enable_statistics", event_buffer_enable_statistics, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, event_buffer, "get_statistics", event_buffer_get_statistics, MRB_ARGS_NONE());
}

