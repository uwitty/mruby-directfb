class DirectFB
  def self.run(layer_conf=nil)
    unless @initialized
      ret = DirectFB.init()
      DirectFB.error('DirectFB.init()', ret)
      @initialized = true
    end

    dfb = DirectFB.create()
    layer = dfb.get_display_layer(DLID_PRIMARY)
    layer.set_cooperative_level(DLSCL_ADMINISTRATIVE)
    layer.set_configuration(layer_conf) if layer_conf
    primary = layer.get_surface()

    yield dfb, layer, primary

    primary.release if primary
    layer.release if layer
    dfb.release if dfb
  end
end

