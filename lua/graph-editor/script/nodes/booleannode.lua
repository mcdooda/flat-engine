local BooleanNodeEditor = {}

local function boolToStringAndColor(bool)
    assert(flat.type(bool) == flat.types.BOOLEAN)
    if bool then
        return 'True', 0x058C40FF
    else
        return 'False', 0xC0392BFF
    end
end

function BooleanNodeEditor.build(boolNode, nodeWidget, pinsWidget)
    local str, color = boolToStringAndColor(boolNode:getValue())
    local textValue = Widget.makeText(str, table.unpack(flat.ui.settings.defaultFont))
    textValue:setMargin(2)
    textValue:setTextColor(color)
    local _, h = textValue:getSize()
    textValue:setSize(30, h)
    textValue:click(function()
        -- toggle on click
        local newValue = not boolNode:getValue()
        boolNode:setValue(newValue)
        local str, color = boolToStringAndColor(newValue)
        textValue:setText(str)
        textValue:setTextColor(color)
        textValue:setSize(30, h)
    end)
    pinsWidget:addChild(textValue)
    return true
end

return BooleanNodeEditor