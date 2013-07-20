include DirectFB::Constants

fontname = "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf"
text = "DirectFB rulez!"

ret = DirectFB::init()
DirectFB::error('DirectFB::init()', ret)
dfb = DirectFB::create()

layer = dfb.get_display_layer(DLID_PRIMARY)
layer.set_cooperative_level(DLSCL_ADMINISTRATIVE)

primary = layer.get_surface
w = primary.width
h = primary.height

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

x = w
while x > -text_width
  primary.set_color(0, 0, 0, 0xff)
  primary.fill_rectangle(0, 0, w, h)
  primary.set_color(0x80, 0x80, 0xff, 0xff)
  ret = primary.draw_string(text, x, h/2, DSTF_LEFT)
  x -= 1
  primary.flip
end

font.release
primary.release
layer.release
dfb.release
