local NumberNodeEditor = {}

function NumberNodeEditor.build(numberNode, nodeWidget, pinsWidget)
    local textInputWidget = Widget.makeTextInput(table.unpack(flat.ui.settings.defaultFont))
    textInputWidget:setText(tostring(numberNode:getValue()))
    local _, h = textInputWidget:getSize()
    local inputContainer = Widget.makeFixedSize(60, h + 2)
    inputContainer:setMargin(2)
    inputContainer:setBackgroundColor(0xFFFFFFFF)
    textInputWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    textInputWidget:setSize(100, h)
    textInputWidget:setTextColor(0x000000FF)
    textInputWidget:setMargin(1)
    textInputWidget:valueChanged(function()
        local value = tonumber(textInputWidget:getText())
        if value then
            numberNode:setValue(value)
        end
    end)
    textInputWidget:submit(function()
        local value = tonumber(textInputWidget:getText())
        if value then
            numberNode:setValue(value)
        else
            textInputWidget:setText('0')
            numberNode:setValue(0)
        end
    end)
    inputContainer:addChild(textInputWidget)
    pinsWidget:addChild(inputContainer)
    return true
end

return NumberNodeEditor