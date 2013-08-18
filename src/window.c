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

#include "directfb_window.h"
#include "directfb_surface.h"
#include "directfb_event_buffer.h"
#include "directfb_constants.h"
#include "directfb_descriptions.h"
#include "directfb_misc.h"
#include "directfb_font.h"

struct mrb_directfb_window_data {
    IDirectFBWindow* window;
    int width;
    int height;
};

static void mrb_directfb_window_free(mrb_state* mrb, void* p)
{
    struct mrb_directfb_window_data* data = (struct mrb_directfb_window_data*)p;
    if (data != NULL) {
        IDirectFBWindow* window = (IDirectFBWindow*)data->window;
        if (window != NULL) {
            window->Release(window);
        }
        mrb_free(mrb, data);
    }
}

static struct mrb_data_type mrb_directfb_window_type = {"Window", mrb_directfb_window_free};

mrb_value mrb_directfb_window_value(mrb_state* mrb, IDirectFBWindow* window)
{
    struct RClass* class_directfb = mrb_class_get(mrb, "DirectFB");
    struct RClass* c = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(class_directfb), mrb_intern(mrb, "Window")));
    return mrb_directfb_window_wrap(mrb, c, window);
}

mrb_value mrb_directfb_window_wrap(mrb_state* mrb, struct RClass* c, IDirectFBWindow* window)
{
    int width = 0;
    int height = 0;
    window->GetSize(window, &width, &height);

    struct mrb_directfb_window_data* data = mrb_malloc(mrb, sizeof(struct mrb_directfb_window_data));
    data->window = window;
    data->width  = width;
    data->height = height;

    mrb_value obj = mrb_obj_value(Data_Wrap_Struct(mrb, c, &mrb_directfb_window_type, data));
    mrb_iv_set(mrb, obj, mrb_intern_cstr(mrb, "font"), mrb_nil_value());
    return obj;
}

IDirectFBWindow* mrb_directfb_window(mrb_state *mrb, mrb_value value)
{
    struct mrb_directfb_window_data* data = DATA_CHECK_GET_PTR(mrb, value, &mrb_directfb_window_type, struct mrb_directfb_window_data);
    if (data != NULL) {
        return data->window;
    } else {
        return NULL;
    }
}

// ============================================================================
// DirectFB::Window methods

static mrb_value window_release(mrb_state *mrb, mrb_value self)
{
    struct mrb_directfb_window_data* data = DATA_CHECK_GET_PTR(mrb, self, &mrb_directfb_window_type, struct mrb_directfb_window_data);
    if ((data != NULL) && (data->window != NULL)) {
        data->window->Release(data->window);
        data->window = NULL;
    }
    return mrb_nil_value();
}

static mrb_value window_get_id(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        DFBWindowID win_id;
        DFBResult ret = window->GetID(window, &win_id);
        if (!ret) {
            return mrb_fixnum_value(win_id);
        }
    }
    return mrb_nil_value();
}

static mrb_value window_get_position(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        int x, y;
        DFBResult ret = window->GetPosition(window, &x, &y);
        if (!ret) {
            mrb_value a[2];
            a[0] = mrb_fixnum_value(x);
            a[1] = mrb_fixnum_value(y);
            return mrb_ary_new_from_values(mrb, 2, a);
        }
    }
    return mrb_nil_value();
}

static mrb_value window_get_size(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        int width, height;
        DFBResult ret = window->GetSize(window, &width, &height);
        if (!ret) {
            mrb_value a[2];
            a[0] = mrb_fixnum_value(width);
            a[1] = mrb_fixnum_value(height);
            return mrb_ary_new_from_values(mrb, 2, a);
        }
    }
    return mrb_nil_value();
}

static mrb_value window_create_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        IDirectFBEventBuffer* buffer;
        DFBResult ret = window->CreateEventBuffer(window, &buffer);
        if (!ret) {
            return mrb_directfb_event_buffer_value(mrb, buffer);
        }
    }
    return mrb_nil_value();
}

#if 0
static mrb_value window_attach_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "attached_event_buffer"), mrb_nil_value());

        mrb_value buffer_object;
        mrb_get_args(mrb, "o", &buffer_object);
        IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, buffer_object);
        DFBResult ret = window->AttachEventBuffer(window, buffer);
        if (!ret) {
            mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "attached_event_buffer"), buffer_object);
            return mrb_directfb_event_buffer_value(mrb, buffer);
        }
    }
    return mrb_nil_value();
}

static mrb_value window_detach_event_buffer(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);

    DFBResult ret = -1;
    if (window != NULL) {
        mrb_value buffer_object;
        mrb_get_args(mrb, "o", &buffer_object);
        IDirectFBEventBuffer* buffer = mrb_directfb_event_buffer(mrb, buffer_object);
        ret = window->DetachEventBuffer(window, buffer);
        if (!ret) {
            mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "attached_event_buffer"), mrb_nil_value());
            return mrb_directfb_event_buffer_value(mrb, buffer);
        }
    }
    return mrb_fixnum_value(ret);
}
#endif

static mrb_value window_enable_events(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);

    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int mask;
        mrb_get_args(mrb, "i", &mask);
        ret = window->EnableEvents(window, mask);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_disable_events(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);

    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int mask;
        mrb_get_args(mrb, "i", &mask);
        ret = window->DisableEvents(window, mask);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_get_surface(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        IDirectFBSurface* surface = NULL;
        DFBResult ret = window->GetSurface(window, &surface);
        if (!ret) {
            return mrb_directfb_surface_value(mrb, surface);
        }
    }
    return mrb_nil_value();
}

static mrb_value window_set_options(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int options;
        mrb_get_args(mrb, "i", &options);
        ret = window->SetOptions(window, options);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_get_options(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        DFBWindowOptions options;
        DFBResult ret = window->GetOptions(window, &options);
        if (!ret) {
            return mrb_fixnum_value(options);
        }
    }
    return mrb_nil_value();
}

static mrb_value window_set_color(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int r, g, b, a;
        mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
        ret = window->SetColor(window, r, g, b, a);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_set_color_key(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int r, g, b;
        mrb_get_args(mrb, "iii", &r, &g, &b);
        ret = window->SetColorKey(window, r, g, b);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_set_opacity(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int opacity;
        mrb_get_args(mrb, "i", &opacity);
        ret = window->SetOpacity(window, opacity);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_set_opaque_region(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int x1, y1, x2, y2;
        mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
        ret = window->SetOpaqueRegion(window, x1, y1, x2, y2);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_get_opacity(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    if (window != NULL) {
        u8 opacity;
        DFBResult ret = window->GetOpacity(window, &opacity);
        if (!ret) {
            return mrb_fixnum_value(opacity);
        }
    }
    return mrb_nil_value();
}

static mrb_value window_set_cursor_shape(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_value surface_object;
        mrb_int hot_x, hot_y;
        mrb_get_args(mrb, "oii", &surface_object, &hot_x, &hot_y);

        IDirectFBSurface* surface = mrb_directfb_surface(mrb, surface_object);
        ret = window->SetCursorShape(window, surface, hot_x, hot_y);
        if (!ret) {
            mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "cursor_shape"), surface_object);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_request_focus(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->RequestFocus(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_grab_keyboard(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->GrabKeyboard(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_ungrab_keyboard(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->UngrabKeyboard(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_grab_pointer(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->GrabPointer(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_ungrab_pointer(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->UngrabPointer(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_grab_key(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int symbol;
        mrb_int modifiers;
        mrb_get_args(mrb, "ii", &symbol, &modifiers);
        ret = window->GrabKey(window, symbol, modifiers);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_ungrab_key(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int symbol;
        mrb_int modifiers;
        mrb_get_args(mrb, "ii", &symbol, &modifiers);
        ret = window->UngrabKey(window, symbol, modifiers);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_move(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int dx;
        mrb_int dy;
        mrb_get_args(mrb, "ii", &dx, &dy);
        ret = window->Move(window, dx, dy);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_move_to(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int x;
        mrb_int y;
        mrb_get_args(mrb, "ii", &x, &y);
        ret = window->MoveTo(window, x, y);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_resize(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int width;
        mrb_int height;
        mrb_get_args(mrb, "ii", &width, &height);
        ret = window->Resize(window, width, height);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_set_stacking_class(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int stacking_class;
        mrb_get_args(mrb, "i", &stacking_class);
        ret = window->SetStackingClass(window, stacking_class);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_raise(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->Raise(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_lower(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->Lower(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_raise_to_top(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->RaiseToTop(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_lower_to_bottom(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->LowerToBottom(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_put_atop(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_value lower_object;
        mrb_get_args(mrb, "o", &lower_object);
        IDirectFBWindow* lower = mrb_directfb_window(mrb, lower_object);
        ret = window->PutAtop(window, lower);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_put_below(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_value below_object;
        mrb_get_args(mrb, "o", &below_object);
        IDirectFBWindow* below = mrb_directfb_window(mrb, below_object);
        ret = window->PutBelow(window, below);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_close(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->Close(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_destroy(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        ret = window->Destroy(window);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_set_bounds(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int x, y, width, height;
        mrb_get_args(mrb, "iiii", &x, &y, &width, &height);
        ret = window->SetBounds(window, x, y, width, height);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_resize_surface(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_int width, height;
        mrb_get_args(mrb, "ii", &width, &height);
        ret = window->ResizeSurface(window, width, height);
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_bind(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_value window_object;
        mrb_int x, y;
        mrb_get_args(mrb, "oii", &window_object, &x, &y);
        IDirectFBWindow* w = mrb_directfb_window(mrb, window_object);
        ret = window->Bind(window, w, x, y);
        if (!ret) {
            mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "bound_window"), window_object);
        }
    }
    return mrb_fixnum_value(ret);
}

static mrb_value window_unbind(mrb_state *mrb, mrb_value self)
{
    IDirectFBWindow* window = mrb_directfb_window(mrb, self);
    DFBResult ret = -1;
    if (window != NULL) {
        mrb_value window_object;
        mrb_get_args(mrb, "o", &window_object);
        IDirectFBWindow* w = mrb_directfb_window(mrb, window_object);
        ret = window->Unbind(window, w);
        if (!ret) {
            mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "bound_window"), mrb_nil_value());
        }
    }
    return mrb_fixnum_value(ret);
}

void mrb_directfb_define_window(mrb_state* mrb, struct RClass* outer)
{
    // def DirectFB::Window
    struct RClass* window = NULL;
    window = mrb_define_class_under(mrb, outer, "Window", mrb->object_class);

    mrb_define_method(mrb, window, "release", window_release, MRB_ARGS_NONE());

    // retrieving information
    mrb_define_method(mrb, window, "get_id", window_get_id, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "get_position", window_get_position, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "get_size", window_get_size, MRB_ARGS_NONE());

    // event handling
    mrb_define_method(mrb, window, "create_event_buffer", window_create_event_buffer, MRB_ARGS_NONE());
    //mrb_define_method(mrb, window, "attach_event_buffer", window_attach_event_buffer, MRB_ARGS_REQ(1));
    //mrb_define_method(mrb, window, "detach_event_buffer", window_detach_event_buffer, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, window, "enable_events", window_enable_events, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, window, "disable_events", window_disable_events, MRB_ARGS_REQ(1));

    // surface handling
    mrb_define_method(mrb, window, "get_surface", window_get_surface, MRB_ARGS_NONE());

    // appearance and behaviour
#if 0
    mrb_define_method(mrb, window, "set_property", window_set_property, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, window, "get_property", window_get_property, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, window, "remove_property", window_remove_property, MRB_ARGS_REQ(1));
#endif
    mrb_define_method(mrb, window, "set_options", window_set_options, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, window, "get_options", window_get_options, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "set_color", window_set_color, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, window, "set_color_key", window_set_color_key, MRB_ARGS_REQ(3));
    //mrb_define_method(mrb, window, "set_color_key_index", window_set_color_key_index, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, window, "set_opacity", window_set_opacity, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, window, "set_opaque_region", window_set_opaque_region, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, window, "get_opacity", window_get_opacity, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "set_cursor_shape", window_set_cursor_shape, MRB_ARGS_REQ(3));

    // focus handling
    mrb_define_method(mrb, window, "request_focus", window_request_focus, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "grab_keyboard", window_grab_keyboard, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "ungrab_keyboard", window_ungrab_keyboard, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "grab_pointer", window_grab_pointer, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "ungrab_pointer", window_ungrab_pointer, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "grab_key", window_grab_key, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, window, "ungrab_key", window_ungrab_key, MRB_ARGS_REQ(2));

    // position and size
    mrb_define_method(mrb, window, "move", window_move, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, window, "move_to", window_move_to, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, window, "resize", window_resize, MRB_ARGS_REQ(2));

    // stacking
    mrb_define_method(mrb, window, "set_stacking_class", window_set_stacking_class, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, window, "raise", window_raise, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "lower", window_lower, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "raise_to_top", window_raise_to_top, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "lower_to_bottom", window_lower_to_bottom, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "put_atop", window_put_atop, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, window, "put_below", window_put_below, MRB_ARGS_REQ(1));

    // closing
    mrb_define_method(mrb, window, "close", window_close, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "destroy", window_destroy, MRB_ARGS_NONE());

    // geometry
    mrb_define_method(mrb, window, "set_bounds", window_set_bounds, MRB_ARGS_REQ(4));

    // scaling
    mrb_define_method(mrb, window, "resize_surface", window_resize_surface, MRB_ARGS_REQ(2));

    // binding
    mrb_define_method(mrb, window, "bind", window_bind, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, window, "unbind", window_unbind, MRB_ARGS_REQ(1));
}

