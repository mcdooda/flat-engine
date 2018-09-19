local StringNodeEditor = {}

function StringNodeEditor.build(stringNode, nodeWidget, pinsWidget)
    local textInputWidget = Widget.makeTextInput(table.unpack(flat.ui.settings.defaultFont))
    textInputWidget:setText(stringNode:getValue())
    local _, h = textInputWidget:getSize()
    local inputContainer = Widget.makeFixedSize(120, h + 2)
    inputContainer:setMargin(2)
    inputContainer:setBackgroundColor(0xFFFFFFFF)
    textInputWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    textInputWidget:setSize(100, h)
    textInputWidget:setTextColor(0x000000FF)
    textInputWidget:setMargin(1)
    textInputWidget:valueChanged(function()
        local value = textInputWidget:getText()
        stringNode:setValue(value)
    end)
    inputContainer:addChild(textInputWidget)
    pinsWidget:addChild(inputContainer)
    return true, inputContainer
end

return StringNodeEditor