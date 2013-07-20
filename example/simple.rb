include DirectFB::Constants

ret = DirectFB::init()
DirectFB::error('DirectFB::init()', ret)
dfb = DirectFB::create()

layer = dfb.get_display_layer(DLID_PRIMARY)
layer.set_cooperative_level(DLSCL_ADMINISTRATIVE)

primary = layer.get_surface
w = primary.width
h = primary.height
print "w:#{w} h:#{h}\n"

primary.fill_rectangle(0, 0, w, h)

primary.set_color(0x80, 0x80, 0xff, 0xff)
ret = primary.draw_line(0, h/2, w - 1, h/2)
DirectFB::error('draw_line()', ret)

primary.flip

DirectFB::usleep(5*1000*1000)

primary.release
layer.release
dfb.release


