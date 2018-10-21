local NodeEditor = flat.require 'graph-editor/nodeeditor'

local NumberNodeEditor = NodeEditor:inherit()

function NumberNodeEditor:build(numberNode, nodeWidget, pinsWidget)
    local numberInputWidget = Widget.makeNumberInput(table.unpack(flat.ui.settings.defaultFont))
    numberInputWidget:setValue(numberNode:getValue())
    local _, h = numberInputWidget:getSize()
    local inputContainer = Widget.makeFixedSize(60, h + 2)
    inputContainer:setMargin(2)
    inputContainer:setBackgroundColor(0xFFFFFFFF)
    numberInputWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    numberInputWidget:setSize(100, h)
    numberInputWidget:setTextColor(0x000000FF)
    numberInputWidget:setMargin(1)
    numberInputWidget:setPrecision(4)
    numberInputWidget:setMin(-1000000);
    numberInputWidget:mouseWheel(function(w, x, y)
        return true
    end)
    local function submitValue()
        local value = numberInputWidget:getValue()
        numberNode:setValue(value)
    end

    numberInputWidget:valueChanged(submitValue)
    numberInputWidget:submit(submitValue)
    numberInputWidget:setValue(numberNode:getValue())
    
    inputContainer:addChild(numberInputWidget)
    pinsWidget:addChild(inputContainer)
    return true, inputContainer
end

return NumberNodeEditor