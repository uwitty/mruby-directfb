include DirectFB::Constants

fontname = "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf"
text = "DirectFB rulez!"

DirectFB.run do |dfb, layer, primary|
  font = dfb.create_font(fontname, {height:48})
  if font != nil
    print "font created\n"
    primary.set_font(font)
  else
    print "font not created\n"
    exit
  end
  
  text_width = font.get_string_width(text)
  print "text_width: #{text_width}\n"
  
  width = primary.width
  height = primary.height
  x = width
  while x > -text_width
    primary.set_color(0, 0, 0, 0xff)
    primary.fill_rectangle(0, 0, width, height)
    primary.set_color(0x80, 0x80, 0xff, 0xff)
    ret = primary.draw_string(text, x, height/2, DSTF_LEFT)
    x -= 1
    primary.flip
  end
  
  font.release
end

