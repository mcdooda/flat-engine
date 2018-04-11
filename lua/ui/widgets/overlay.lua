local function overlay()
    local o = Widget.makeExpand()
    o:setBackgroundColor(0x00000088)
    Widget.getRoot():addChild(o)
    return o
end

flat.ui.overlay = overlay