include DirectFB::Constants

DirectFB.run do |dfb, layer, primary|
  primary.fill_rectangle(0, 0, primary.width, primary.height)
  
  primary.set_color(0x80, 0x80, 0xff, 0xff)
  ret = primary.draw_line(0, primary.height/2, primary.width - 1, primary.height/2)
  DirectFB.error('draw_line()', ret)
  
  primary.flip
  
  DirectFB.usleep(5*1000*1000)
end

