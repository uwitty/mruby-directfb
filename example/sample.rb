include DirectFB::Constants

FONT_FILENAME = "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf"

class App
  def initialize(dfb, layer, filenames)
    @dfb = dfb
    @layer = layer
    @filenames = filenames
    @idx = 0

    @image = ImageWindow.new(dfb, layer)
    @desc  = DescWindow.new(dfb, layer)

    @dfb.get_input_device(DIDID_KEYBOARD) do |keyboard|
      @event_buffer = keyboard.create_event_buffer
    end

    load_image(@filenames[@idx])

    @image.window.set_opacity(255)
    @desc.window.set_opacity(255)
  end

  def update
    @event_buffer.wait_for_event
    event = @event_buffer.get_event
    return false unless event
    return false if (event.type == DIET_KEYPRESS) && (event.key_id == DIKI_ESCAPE)

    updated = false

    if (event.type == DIET_KEYRELEASE) && (event.key_id == DIKI_RIGHT)
      @idx = (@idx + 1) % @filenames.length
      updated = true
    end
    if (event.type == DIET_KEYRELEASE) && (event.key_id == DIKI_LEFT)
      @idx = (@idx - 1) % @filenames.length
      updated = true
    end

    if updated
      filename = @filenames[@idx]
      load_image(filename)
      @image.surface.flip
    end
    true
  end

  def load_image(filename)
      @image.clear
      @desc.clear

      @dfb.create_image_provider(filename) do |provider|
        desc = provider.get_surface_description
        img = @dfb.create_surface(desc)
        if img
          provider.render_to(img, nil)
          @image.update(img)
          @desc.update(App.basename(filename), img.width, img.height)
        end
        img.release
      end
  end

  def release
    @event_buffer.release if @event_buffer
    @desc.release
    @image.release
  end

  def self.basename(filename)
    index = filename.rindex('/')
    if index
      filename[(index+1)..-1]
    else
      filename
    end
  end

  class Window
    attr_accessor :dfb, :layer, :window, :surface

    def initialize(dfb, layer, width, height)
      desc = {caps:DWCAPS_ALPHACHANNEL, posx:0, posy:0, width:width, height:height}
      window = layer.create_window(desc)
      raise "failed to create window:#{desc}" unless window

      surface = window.get_surface
      unless surface
        window.release
        raise 'failed to get surface from window:#{desc}'
      end

      self.dfb     = dfb
      self.layer   = layer
      self.window  = window
      self.surface = surface

      self
    end

    def release
      self.surface.release if self.surface
      self.surface = nil
      self.window.release if self.window
      self.window = nil
    end
  end

  class DescWindow < Window
    def initialize(dfb, layer)
      font_desc = {height:30}
      @font = dfb.create_font(FONT_FILENAME, font_desc)
      raise "dfb.create_font(#{FONT_FILENAME}, #{desc})" unless @font
      font_desc = {height:15}
      @font2 = dfb.create_font(FONT_FILENAME, font_desc)
      raise "dfb.create_font(#{FONT_FILENAME}, #{desc})" unless @font

      cfg = layer.get_configuration
      width  = (cfg[:width] / 2 - 16).round
      height = (cfg[:height] / 8).round
      super(dfb, layer, width, height)
      self.window.move_to((cfg[:width] / 2).truncate, 16)
      self.surface.clear(16, 16, 16, 128)

      self
    end

    def release
      @font2.release
      @font.release
      super
    end

    def clear
      self.surface.clear(16, 16, 16, 128)

      self.surface.set_color(240, 32, 32, 255)
      line_width = self.surface.width - 20
      self.surface.fill_rectangle(10, 40, line_width, 1)
    end

    def update(basename, width, height)
      @title = basename
      self.surface.set_font(@font)
      self.surface.set_color(240, 240, 240, 255)
      self.surface.draw_string(@title, 12, 36, DSTF_LEFT)

      self.surface.set_font(@font2)
      self.surface.set_color(200, 200, 200, 255)
      self.surface.draw_string("#{width}x#{height}", 12, 56, DSTF_LEFT)
    end
  end

  class ImageWindow < Window
    DEFAULT_RED   = 128
    DEFAULT_GREEN = 128
    DEFAULT_BLUE  = 128
    DEFAULT_ALPHA = 255

    def initialize(dfb, layer)
      cfg = layer.get_configuration
      super(dfb, layer, cfg[:width], cfg[:height])
      self.window.move_to(0, 0)
      self.surface.clear(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE, DEFAULT_ALPHA)
      self
    end

    def clear
      self.surface.clear(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE, DEFAULT_ALPHA)
    end

    def update(image)
      self.surface.stretch_blit(image, nil, calc_dst_rect(image, self.surface))
    end

    def calc_dst_rect(src, dst)
        aspect = dst.width / dst.height
        src_aspect = src.width / src.height

        if src_aspect > aspect
          height = (dst.width / src_aspect).round
          y = ((dst.height - height)/2).round
          DirectFB::Rectangle.new(0, y, dst.width, height)
        else
          width = (dst.height * src_aspect).round
          x = ((dst.width - width)/2).round
          DirectFB::Rectangle.new(x, 0, width, dst.height)
        end
    end
  end
end

DirectFB.run2({width:960, height:540}) do |dfb, layer|
  app = App.new(dfb, layer, ARGV)
  begin
    while app.update
      #DirectFB.usleep(100*1000)
    end
  ensure
    app.release
  end
end

