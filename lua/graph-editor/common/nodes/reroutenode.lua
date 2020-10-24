local NodeEditor = flat.require 'graph-editor/nodeeditor'

local RerouteNodeEditor = NodeEditor:inherit()

function RerouteNodeEditor:buildNodeName(rerouteNode)
    local handle = Widget.makeFixedSize(1, 10)
    handle:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    return handle
end

function RerouteNodeEditor:build(rerouteNode, nodeWidget, pinsWidget)
    return true
end

function RerouteNodeEditor:update(rerouteNode, nodeWidget, pinsWidget)
    local handle = nodeWidget.nodeNameContainer:getChildAtIndex(1)
    local pinColor = nodeWidget:getPinColorByType(rerouteNode.inPin.pinType)
    handle:setBackgroundColor(pinColor)
    return true
end

return RerouteNodeEditor