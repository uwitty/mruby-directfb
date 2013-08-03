include DirectFB::Constants

filename = "./earth_1.jpg"
#filename = "/usr/share/inkscape/tutorials/tux.png"

DirectFB.run() do |dfb, layer, primary|
  provider = dfb.create_image_provider(filename)
  provider.set_render_callback() {|rect|
    print "mrb callback rect:{x:#{rect.x}, y:#{rect.y}, w:#{rect.w}, h:#{rect.h}}\n"
  }

  surface_desc = provider.get_surface_description
  print "surface_desc: #{surface_desc.to_s}\n"

  logo = dfb.create_surface(surface_desc)
  provider.render_to(logo, nil)
  print "image_description:#{provider.get_image_description.to_s}\n"

  provider.release
  logo.release
end

