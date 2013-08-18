class DirectFB
  def self.run(layer_conf=nil)
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

      yield dfb, layer, primary
    ensure
      primary.release if primary
      layer.release if layer
      dfb.release if dfb
    end
  end

  def self.run2(layer_conf=nil)
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

      yield dfb, layer
    ensure
      layer.release if layer
      dfb.release if dfb
    end
  end

  def create_image_provider(filename)
    if block_given?
      provider = create_image_provider_impl(filename)
      if provider
        begin
          yield provider
        ensure
          provider.release
        end
      end
    else
      create_image_provider_impl(filename)
    end
  end

  class Surface
    def lock(flags)
      if block_given?
        ptr, pitch = self.lock_impl flags
        begin
          yield ptr, pitch
        ensure
          self.unlock if ptr
        end
      else
        self.lock_impl flags
      end
    end
  end
end

