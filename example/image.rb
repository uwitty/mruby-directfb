include DirectFB::Constants

filename = "/usr/share/inkscape/tutorials/tux.png"

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
surface_desc = provider.get_surface_description
logo = dfb.create_surface(surface_desc)
provider.render_to(logo, nil)
#provider.render_to(logo, DirectFB::Rectangle.new(0, 0, 64, 64))
print "image_description:#{provider.get_image_description.to_s}\n"
provider.release

print "surface_desc: #{surface_desc.to_s}\n"

x = - logo.width
while x < w
  primary.fill_rectangle(0, 0, w, h)
  primary.blit(logo, nil, x, (h - surface_desc[:height]) / 2)
  primary.flip
  x += 2;
end

logo.release
primary.release
layer.release
dfb.release

