include DirectFB::Constants

print DirectFB.to_s + '.DFXL_NONE: ' + DFXL_NONE.to_s + "\n"

ret = DirectFB::init()
DirectFB::error('init()', ret)
ret = DirectFB::set_option('bg-none', '')
ret = DirectFB::set_option('no-cursor', '')
dfb = DirectFB::create()

layer = dfb.get_display_layer(DLID_PRIMARY)
print "get display layer: #{layer.class.to_s}.\n"
layer.set_cooperative_level(DLSCL_ADMINISTRATIVE)

surface = layer.get_surface()
print "get surface: #{surface.class.to_s}.\n"
surface.set_color(0, 0, 255, 255)
surface.fill_rectangle(0, 0, 256, 256)
surface.flip(DirectFB::Region.new(64, 64, 128, 128), 0)

dfb.enum_input_devices() {|id, desc|
    print "ruby callback block: {id:#{id}, desc:#{desc.to_s}}\n"
}

DirectFB::usleep(2*1000*1000);
dfb.suspend
dfb.resume
dfb.wait_idle
dfb.wait_for_sync
surface.release()
layer.release()
dfb.release()

