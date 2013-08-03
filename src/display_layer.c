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

#include "directfb_surface.h"
#include "directfb_constants.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"

// ============================================================================
// IDirectFB::DisplayLayer object

struct mrb_directfb_display_layer_data {
    IDirectFBDisplayLayer* layer;
};

static void mrb_directfb_display_layer_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)p;
    if (data != NULL) {
        IDirectFBDisplayLayer* layer = (IDirectFBDisplayLayer*)data->layer;
        if (layer != NULL) {
            layer->Release(layer);
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_display_layer_type = {"DisplayLayer", mrb_directfb_display_layer_free};

mrb_value mrb_directfb_display_layer_wrap(mrb_state* mrb, struct RClass* c, IDirectFBDisplayLayer* layer)
{
    struct mrb_directfb_display_layer_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_display_layer_data));
    data->layer = layer;
    return mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_display_layer_type, data));
}

IDirectFBDisplayLayer* mrb_directfb_get_display_layer(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)mrb_data_get_ptr(mrb, value, &mrb_directfb_display_layer_type);
    if (data != NULL) {
        return data->layer;
    } else {
        return NULL;
    }
}

// ============================================================================
// DirectFB::DisplayLayer methods

static mrb_value display_layer_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_display_layer_data* data = (struct mrb_directfb_display_layer_data*)mrb_data_get_ptr(mrb, self, &mrb_directfb_display_layer_type);
    if ((data != NULL) && (data->layer != NULL)) {
        data->layer->Release(data->layer);
        data->layer = NULL;
    }
    return mrb_nil_value();
}

static mrb_value display_layer_get_id(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        DFBDisplayLayerID id;
        DFBResult ret = layer->GetID(layer, &id);
        if (!ret) {
            return mrb_fixnum_value(id);
        }
    }
    return mrb_nil_value();
}

static mrb_value display_layer_get_description(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        DFBDisplayLayerDescription desc;
        DFBResult ret = layer->GetDescription(layer, &desc);
        if (!ret) {
            return mrb_directfb_display_layer_description_new(mrb, &desc);
        }
    }
    return mrb_nil_value();
}

static mrb_value display_layer_get_source_descriptions(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer == NULL) {
        return mrb_nil_value();
    }

    DFBResult ret = -1;
    DFBDisplayLayerDescription desc;
    ret = layer->GetDescription(layer, &desc);
    if (ret || (desc.sources <= 0)) {
        return mrb_nil_value();
    }

    int num = desc.sources;
    DFBDisplayLayerSourceDescription* descs = (DFBDisplayLayerSourceDescription*)mrb_malloc(mrb, num * sizeof(DFBDisplayLayerSourceDescription));
    mrb_value* values = (mrb_value*)mrb_malloc(mrb, num * sizeof(mrb_value));

    ret = layer->GetSourceDescriptions(layer, descs);
    if ((descs == NULL) || (values == NULL) || (!ret)) {
        int i;
        for (i = 0; i < num; i++) {
            values[i] = mrb_directfb_display_layer_source_description_new(mrb, &descs[i]);
        }
        mrb_value a = mrb_ary_new_from_values(mrb, num, values);
        mrb_free(mrb, values);
        mrb_free(mrb, descs);
        return a;
    }

    if (values != NULL) {
        mrb_free(mrb, values);
    }
    if (descs == NULL) {
        mrb_free(mrb, descs);
    }

    return mrb_nil_value();
}

static mrb_value display_layer_get_current_output_field(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        int field;
        DFBResult ret = layer->GetCurrentOutputField(layer, &field);
        if (!ret) {
            return mrb_fixnum_value(field);
        }
    }
    return mrb_nil_value();
}

static mrb_value display_layer_get_surface(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        IDirectFBSurface* surface = NULL;
        DFBResult ret = layer->GetSurface(layer, &surface);
        if (!ret) {
            struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
            struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "Surface")));
            return mrb_directfb_surface_wrap(mrb, c, surface);
        }
    }
    return mrb_nil_value();
}

static mrb_value display_layer_set_cooperative_level(mrb_state *mrb, mrb_value self)
{
    mrb_int cooperative_level;
    mrb_get_args(mrb, "i", &cooperative_level);

    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        layer->SetCooperativeLevel(layer, cooperative_level);
    }

    return mrb_nil_value();
}

static mrb_value display_layer_get_configuration(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        DFBDisplayLayerConfig conf;
        DFBResult ret = layer->GetConfiguration(layer, &conf);
        if (!ret) {
            return mrb_directfb_display_layer_configuration_new(mrb, &conf);
        }
    }
    return mrb_nil_value();
}

static mrb_value display_layer_set_configuration(mrb_state *mrb, mrb_value self)
{
    mrb_value config_object;
    mrb_get_args(mrb, "o", &config_object);

    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        DFBDisplayLayerConfig conf;
        mrb_directfb_display_layer_configuration_get(mrb, config_object, &conf);
        ret = layer->SetConfiguration(layer, &conf);
    }

    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_test_configuration(mrb_state *mrb, mrb_value self)
{
    mrb_value config_object;
    mrb_get_args(mrb, "o", &config_object);

    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        DFBDisplayLayerConfig conf;
        mrb_directfb_display_layer_configuration_get(mrb, config_object, &conf);
        DFBDisplayLayerConfigFlags failed;
        ret = layer->TestConfiguration(layer, &conf, &failed);
        if (!ret) {
            return mrb_fixnum_value(failed);
        } else {
            DirectFBError("TestConfiguration", ret);
        }
    }

    return mrb_nil_value();
}

static mrb_value display_layer_enable_cursor(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        mrb_int enable;
        mrb_get_args(mrb, "i", &enable);
        ret = layer->EnableCursor(layer, enable);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_get_cursor_position(mrb_state *mrb, mrb_value self)
{
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        int x, y;
        DFBResult ret = layer->GetCursorPosition(layer, &x, &y);
        if (!ret) {
            mrb_value values[2];
            values[0] = mrb_fixnum_value(x);
            values[1] = mrb_fixnum_value(y);
            return mrb_ary_new_from_values(mrb, 2, values);
        }
    }
    return mrb_nil_value();
}

static mrb_value display_layer_warp_cursor(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        mrb_int x, y;
        mrb_get_args(mrb, "ii", &x, &y);
        ret = layer->WarpCursor(layer, x, y);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_set_cursor_acceleration(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        mrb_int numerator, denominator, threshold;
        mrb_get_args(mrb, "iii", &numerator, &denominator, &threshold);
        ret = layer->SetCursorAcceleration(layer, numerator, denominator, threshold);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_set_cursor_shape(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        mrb_value shape_object;
        mrb_int hot_x, hot_y;
        mrb_get_args(mrb, "oii", &shape_object, &hot_x, &hot_y);
        IDirectFBSurface* shape = mrb_directfb_get_surface(mrb, shape_object);
        ret = layer->SetCursorShape(layer, shape, hot_x, hot_y);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_set_cursor_opacity(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        mrb_int opacity;
        mrb_get_args(mrb, "i", &opacity);
        ret = layer->SetCursorOpacity(layer, opacity);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_wait_for_sync(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        ret = layer->WaitForSync(layer);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_switch_context(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        mrb_int b;
        mrb_get_args(mrb, "b", &b);
        DFBBoolean exclusive = (b != 0)? DFB_TRUE : DFB_FALSE;
        ret = layer->SwitchContext(layer, exclusive);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value display_layer_set_rotation(mrb_state *mrb, mrb_value self)
{
    DFBResult ret = -1;
    IDirectFBDisplayLayer* layer = mrb_directfb_get_display_layer(mrb, self);
    if (layer != NULL) {
        mrb_int rotation;
        mrb_get_args(mrb, "i", &rotation);
        ret = layer->SetRotation(layer, rotation);
    }
    return mrb_fixnum_value(ret);
}

void mrb_directfb_define_display_layer(mrb_state* mrb, struct RClass* outer)
{
    struct RClass* display_layer = mrb_define_class_under(mrb, outer, "DisplayLayer", mrb->object_class);

    mrb_define_method(mrb, display_layer, "release", display_layer_release, MRB_ARGS_NONE());

    // information
    mrb_define_method(mrb, display_layer, "get_id", display_layer_get_id, MRB_ARGS_NONE());
    mrb_define_method(mrb, display_layer, "get_description", display_layer_get_description, MRB_ARGS_NONE());
    mrb_define_method(mrb, display_layer, "get_source_descriptions", display_layer_get_source_descriptions, MRB_ARGS_NONE());
    mrb_define_method(mrb, display_layer, "get_current_output_field", display_layer_get_current_output_field, MRB_ARGS_NONE());

    // interfaces
    mrb_define_method(mrb, display_layer, "get_surface", display_layer_get_surface, MRB_ARGS_NONE());

    // configuration
    mrb_define_method(mrb, display_layer, "set_cooperative_level", display_layer_set_cooperative_level, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, display_layer, "get_configuration", display_layer_get_configuration, MRB_ARGS_NONE());
    mrb_define_method(mrb, display_layer, "test_configuration", display_layer_test_configuration, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, display_layer, "set_configuration", display_layer_set_configuration, MRB_ARGS_REQ(1));

    // cursor handling
    mrb_define_method(mrb, display_layer, "enable_cursor", display_layer_enable_cursor, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, display_layer, "get_cursor_position", display_layer_get_cursor_position, MRB_ARGS_NONE());
    mrb_define_method(mrb, display_layer, "warp_cursor", display_layer_warp_cursor, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, display_layer, "set_cursor_acceleration", display_layer_set_cursor_acceleration, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, display_layer, "set_cursor_shape", display_layer_set_cursor_shape, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, display_layer, "set_cursor_opacity", display_layer_set_cursor_opacity, MRB_ARGS_REQ(3));

    // synchronization
    mrb_define_method(mrb, display_layer, "wait_for_sync", display_layer_wait_for_sync, MRB_ARGS_NONE());

    // contexts
    mrb_define_method(mrb, display_layer, "switch_context", display_layer_switch_context, MRB_ARGS_REQ(1));

    // rotation
    mrb_define_method(mrb, display_layer, "set_rotation", display_layer_set_rotation, MRB_ARGS_REQ(1));
}

