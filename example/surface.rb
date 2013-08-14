include DirectFB::Constants

DirectFB.run({width:960, height:540}) do |dfb, layer, primary|
  primary.set_color(64, 64, 64, 255)
  primary.fill_rectangle(0, 0, primary.width, primary.height)
  
  offscreen = dfb.create_surface({caps:DSCAPS_PREMULTIPLIED, width:1920, height:1080, pixelformat:DSPF_ARGB})
  
  offscreen.set_drawing_flags(DSDRAW_NOFX)
  offscreen.set_color(0, 0, 255, 255);
  offscreen.fill_rectangle(0, 0, 1920, 1080)

  offscreen.set_clip(DirectFB::Region.new(240, 135, 960, 540))
  offscreen.clear(0, 0, 0, 0)
  clip = offscreen.get_clip
  print "clip:{x1:#{clip.x1}, y1:#{clip.y1}, x2:#{clip.x2}, y2:#{clip.y2}}\n"

  offscreen.set_clip(DirectFB::Region.new(0, 0, 1920, 1080))
  offscreen.set_drawing_flags(DSDRAW_BLEND)
  offscreen.set_color(0, 255, 255, 255);
  offscreen.fill_rectangle(1920/4, 1080/4, 1920/2, 1080/2)

  primary.set_blitting_flags(DSBLIT_BLEND_ALPHACHANNEL)
  primary.set_porter_duff(DSPD_SRC_OVER)
  ret = primary.stretch_blit(offscreen);
  DirectFB.error('DirectFB::Surface#stretch_blit()', ret)
  
  primary.flip()
  
  DirectFB.usleep(1*1000*1000);
  
  offscreen.release
end

DirectFB.run({width:960, height:540}) do |dfb, layer, primary|
    primary.set_color(16, 16, 32, 255)
    primary.fill_rectangle(0, 0, primary.width, primary.height)
    
    tile = dfb.create_surface({caps:DSCAPS_PREMULTIPLIED, width:60, height:60, pixelformat:DSPF_ARGB})
    
    tile.clear(0, 0, 0, 0)
    tile.set_color(0, 0, 255, 255);
    tile.draw_line(0, 0, 60, 0)
    tile.draw_line(0, 0, 0, 60)
    tile.draw_line(0, 0, 60, 60)
    tile.draw_line(60, 0, 0, 60)

    primary.set_blitting_flags(DSBLIT_BLEND_ALPHACHANNEL)
    ret = primary.tile_blit(tile, nil, 0, 0);
    DirectFB.error('DirectFB::Surface#tile_blit()', ret)
    
    primary.flip()
    
    DirectFB.usleep(1*1000*1000);
    
    tile.release
end

DirectFB.run({width:960, height:540}) do |dfb, layer, primary|
  primary.clear(0, 0, 0, 0)
  primary.set_color(64, 64, 64, 255)
  primary.fill_rectangle(0, 0, primary.width, primary.height)

  primary.set_color(128, 128, 240, 255)
  primary.fill_triangle(primary.width * 0.5, primary.height * 0.1, 
                        primary.width * 0.1, primary.height * 0.9,
                        primary.width * 0.9, primary.height * 0.9)
  primary.flip

  DirectFB.usleep(1*1000*1000)
end

