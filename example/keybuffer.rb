include DirectFB::Constants

#filename = "/usr/share/inkscape/tutorials/tux.png"
filename = "/usr/share/icons/gnome/48x48/places/gnome-foot.png"

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
primary.flip

keyboard = dfb.get_input_device(DIDID_KEYBOARD)
event_buffer = keyboard.create_event_buffer

provider = dfb.create_image_provider(filename)
surface_desc = provider.get_surface_description
foot = dfb.create_surface(surface_desc)
provider.render_to(foot, nil)
#provider.render_to(foot, DirectFB::Rectangle.new(0, 0, 64, 64))
print "image_description:#{provider.get_image_description.to_s}\n"
provider.release

print "surface_desc: #{surface_desc.to_s}\n"

primary.set_blitting_flags(DSBLIT_BLEND_ALPHACHANNEL)

max_x = w - surface_desc[:width]
max_y = h - surface_desc[:height]


quit = false
until quit

  event_buffer.wait_for_event

  while event = event_buffer.get_event
      primary.fill_rectangle(0, 0, w, h) if event.type == DIET_KEYRELEASE

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

  #DirectFB::usleep(10*1000)
end

event_buffer.release
keyboard.release
foot.release
primary.release
layer.release
dfb.release

