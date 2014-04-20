include DirectFB::Constants

filename = "/usr/share/inkscape/tutorials/tux.png"

DirectFB.run() do |dfb, layer, primary|
  keyboard = dfb.get_input_device(DIDID_KEYBOARD)
  
  # create image surface
  provider = dfb.create_image_provider(filename)
  surface_desc = provider.get_surface_description
  logo = dfb.create_surface(surface_desc)
  provider.render_to(logo, nil)
  #provider.render_to(logo, DirectFB::Rectangle.new(0, 0, 64, 64))
  print "image_description:#{provider.get_image_description.to_s}\n"
  provider.release
  print "surface_desc: #{surface_desc.to_s}\n"
  
  max_x = primary.width - surface_desc[:width]
  max_y = primary.height - surface_desc[:height]
  
  sprite_x = (primary.width - surface_desc[:width]) / 2
  sprite_y = (primary.height - surface_desc[:height]) / 2
  
  while (keyboard.get_key_state(DIKI_ESCAPE) != DIKS_DOWN)
    primary.fill_rectangle(0, 0, primary.width, primary.height)
    primary.blit(logo, nil, sprite_x, sprite_y)
    primary.flip
  
    sprite_x -= 1 if keyboard.get_key_state(DIKI_LEFT) == DIKS_DOWN
    sprite_x += 1 if keyboard.get_key_state(DIKI_RIGHT) == DIKS_DOWN
    sprite_y += 1 if keyboard.get_key_state(DIKI_DOWN) == DIKS_DOWN
    sprite_y -= 1 if keyboard.get_key_state(DIKI_UP) == DIKS_DOWN
  
    #DirectFB.usleep(10*1000)
  end

  logo.release
  keyboard.release
end

