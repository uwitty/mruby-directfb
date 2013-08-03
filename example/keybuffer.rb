include DirectFB::Constants

#filename = "/usr/share/inkscape/tutorials/tux.png"
filename = "/usr/share/icons/gnome/48x48/places/gnome-foot.png"

DirectFB.run() do |dfb, layer, primary|
  # fill primary surface (default color)
  primary.fill_rectangle(0, 0, primary.width, primary.height)
  primary.flip
  
  # create event buffer
  keyboard = dfb.get_input_device(DIDID_KEYBOARD)
  event_buffer = keyboard.create_event_buffer
  
  # create image surface
  provider = dfb.create_image_provider(filename)
  surface_desc = provider.get_surface_description
  foot = dfb.create_surface(surface_desc)
  provider.render_to(foot, nil)
  #provider.render_to(foot, DirectFB::Rectangle.new(0, 0, 64, 64))
  print "image_description:#{provider.get_image_description.to_s}\n"
  provider.release
  print "surface_desc: #{surface_desc.to_s}\n"
  
  max_x = primary.width - surface_desc[:width]
  max_y = primary.height - surface_desc[:height]
  
  primary.set_blitting_flags(DSBLIT_BLEND_ALPHACHANNEL)
  event_buffer.enable_statistics(true)
  
  quit = false
  until quit
    event_buffer.wait_for_event
    #print "event_buffer.get_statistics:#{event_buffer.get_statistics.num_events}\n"
  
    while event = event_buffer.get_event
        primary.fill_rectangle(0, 0, primary.width, primary.height) if event.type == DIET_KEYRELEASE
  
        if event.type == DIET_KEYPRESS
            if event.key_id == DIKI_ESCAPE
              quit = true 
            else
              sprite_x = rand() * max_x
              sprite_y = rand() * max_y
              #print "sprite_x:#{sprite_x}, sprite_y:#{sprite_y}\n"
              primary.blit(foot, nil, sprite_x, sprite_y)
            end
        end
  
        primary.flip
    end
  end
  #print "event_buffer.get_statistics:#{event_buffer.get_statistics.num_events}\n"
  
  event_buffer.release
  keyboard.release
  foot.release
end

