include DirectFB::Constants

filename = "/usr/share/inkscape/tutorials/tux.png"

DirectFB.run do |dfb, layer, primary|
  provider = dfb.create_image_provider(filename)
  surface_desc = provider.get_surface_description
  logo = dfb.create_surface(surface_desc)
  provider.render_to(logo, nil)
  #provider.render_to(logo, DirectFB::Rectangle.new(0, 0, 64, 64))
  print "image_description:#{provider.get_image_description.to_s}\n"
  provider.release

  print "surface_desc: #{surface_desc.to_s}\n"

  width = primary.width
  height = primary.height
  x = - logo.width
  while x < width
    primary.fill_rectangle(0, 0, width, height)
    primary.blit(logo, nil, x, (height - surface_desc[:height]) / 2)
    primary.flip
    x += 2;
  end

  logo.release
end
