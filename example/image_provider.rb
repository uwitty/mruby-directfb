include DirectFB::Constants

filename = "./earth_1.jpg"

ret = DirectFB::init()
DirectFB::error('DirectFB::init()', ret)
dfb = DirectFB::create()

layer = dfb.get_display_layer(DLID_PRIMARY)
layer.set_cooperative_level(DLSCL_ADMINISTRATIVE)

primary = layer.get_surface
w = primary.width
h = primary.height
print "w:#{w} h:#{h}\n"

provider = dfb.create_image_provider(filename)
provider.set_render_callback() {|rect|
  print "mrb callback rect:{x:#{rect.x}, y:#{rect.y}, w:#{rect.w}, h:#{rect.h}}\n"
}
surface_desc = provider.get_surface_description
logo = dfb.create_surface(surface_desc)
provider.render_to(logo, nil)
#provider.render_to(logo, DirectFB::Rectangle.new(0, 0, 64, 64))
print "image_description:#{provider.get_image_description.to_s}\n"

ret = provider.write_back(logo, DirectFB::Rectangle.new(0, 0, 64, 64), "./test.jpg")
DirectFB::error('provider.write_back', ret)

print "surface_desc: #{surface_desc.to_s}\n"

provider.release
logo.release
primary.release
layer.release
dfb.release


