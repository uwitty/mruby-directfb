print DirectFB.to_s + '.DFXL_NONE: ' + DirectFB::DFXL_NONE.to_s + "\n"

ret = DirectFB::init()
DirectFB::error('DirectFB::init()', ret)
ret = DirectFB::set_option('bg-none', '')
ret = DirectFB::set_option('no-cursor', '')
dfb = DirectFB::create()

layer = dfb.get_display_layer(DirectFB::DLID_PRIMARY)
print "get display layer: #{layer.class.to_s}.\n"
layer.set_cooperative_level(DirectFB::DLSCL_ADMINISTRATIVE)

surface = layer.get_surface()
print "get surface: #{surface.class.to_s}.\n"
surface.set_color(0, 0, 255, 255)
surface.fill_rectangle(0, 0, 256, 256)
surface.flip(DirectFB::Region.new(64, 64, 128, 128), 0)

dfb.enum_input_devices() {|id, desc|
    print "ruby callback block: {id:#{id}, desc:\n"
    print "  type:#{desc.type}\n"
    print "  caps:#{desc.caps}\n"
    print "  min_keycode:#{desc.min_keycode}\n"
    print "  max_keycode:#{desc.max_keycode}\n"
    print "  max_axis:#{desc.max_axis}\n"
    print "  max_button:#{desc.max_button}\n"
    print "  name:#{desc.name}\n"
    print "  vendor:#{desc.vendor}\n"
    print "}\n"
}

DirectFB::usleep(2*1000*1000);
dfb.suspend
dfb.resume
dfb.wait_idle
dfb.wait_for_sync
surface.release()
layer.release()
dfb.release()

