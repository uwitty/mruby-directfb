include DirectFB::Constants

def info_dfb(dfb)
  desc = dfb.get_device_description
  if desc
    print "dfb.get_device_description() -> #{desc}\n"
  else
    print "dfb.get_device_description() failed. \n"
  end

  print "dfb.enum_video_modes()\n"
  dfb.enum_video_modes do |width, height, bpp|
    print "  -> width:#{width}, height:#{height}, bpp:#{bpp}\n"
  end

  print "dfb.enum_screens()\n"
  dfb.enum_screens do |id, desc|
    print "  -> id:#{id} desc:#{desc}\n"
  end

  print "dfb.enum_display_layers()\n"
  dfb.enum_display_layers do |id, desc|
    print "  -> id:#{id} desc:#{desc}\n"
  end

  print "dfb.enum_input_devices()\n"
  dfb.enum_input_devices do |id, desc|
    print "  -> id:#{id} desc:#{desc}\n"
  end
end

def info_layer(layer)
  print "layer.get_id() -> #{layer.get_id()}\n"
  print "layer.get_description() -> #{layer.get_description()}\n"
  print "layer.get_current_output_field() -> #{layer.get_current_output_field()}\n"
  print "layer.get_configuration() -> #{layer.get_configuration()}\n"

  #x, y = layer.get_cursor_position
  #print "layer.get_cursor_position() -> x:#{x}, y:#{y}\n"

  #layer.warp_cursor(100, 100)
  #x, y = layer.get_cursor_position
  #print "layer.get_cursor_position() -> x:#{x}, y:#{y}\n"
end

def info_surface(surface)
  print "surface.get_capabilities() -> #{surface.get_capabilities()}\n"
  print "surface.get_position() -> #{surface.get_position()}\n"
  print "surface.get_size() -> #{surface.get_size()}\n"
  print "surface.get_pixel_format() -> #{surface.get_pixel_format()}\n"
  print "surface.get_acceleration_mask() -> #{surface.get_acceleration_mask()}\n"
  print "surface.get_clip() -> #{surface.get_clip()}\n"
end

DirectFB.run() do |dfb, layer, primary|
  info_dfb(dfb)
  info_layer(layer)
  info_surface(primary)
end

