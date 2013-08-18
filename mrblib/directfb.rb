class DirectFB
  include DirectFB::Constants

  def self.run(layer_conf=nil, &block)
    unless @initialized
      ret = DirectFB.init()
      DirectFB.error('DirectFB.init()', ret)
      @initialized = true
    end

    begin
      dfb = DirectFB.create()
      layer = dfb.get_display_layer(DLID_PRIMARY)
      layer.set_cooperative_level(DLSCL_ADMINISTRATIVE)
      layer.set_configuration(layer_conf) if layer_conf
      primary = layer.get_surface()

      block.call(dfb, layer, primary)
    ensure
      primary.release if primary
      layer.release if layer
      dfb.release if dfb
    end
  end

  def self.run2(layer_conf=nil, &block)
    unless @initialized
      ret = DirectFB.init()
      DirectFB.error('DirectFB.init()', ret)
      @initialized = true
    end

    begin
      dfb = DirectFB.create()
      layer = dfb.get_display_layer(DLID_PRIMARY)
      layer.set_cooperative_level(DLSCL_ADMINISTRATIVE)
      layer.set_configuration(layer_conf) if layer_conf

      block.call(dfb, layer)
    ensure
      layer.release if layer
      dfb.release if dfb
    end
  end

  def create_image_provider(filename, &block)
    if block
      provider = create_image_provider_impl(filename)
      if provider
        begin
          block.call(provider)
        ensure
          provider.release
        end
      end
    else
      create_image_provider_impl(filename)
    end
  end

  class Surface
    def lock(flags, &block)
      if block
        ptr, pitch = self.lock_impl flags
        begin
          block.call(ptr, pitch)
        ensure
          self.unlock if ptr
        end
      else
        self.lock_impl(flags)
      end
    end
  end

  class WindowEvent
    def to_s
      s  = super()
      s += "{window_id:#{window_id}, type:#{type.to_s(16)}"

      s += ", x:#{x}, y:#{y}" if (type & DWET_POSITION) != 0
      s += ", w:#{w}, h:#{h}" if (type & DWET_SIZE) != 0
      s += ", cx:#{cx}, cy:#{cy}" if (type & DWET_MOTION) != 0

      s += '}'
      s
    end
  end
end

