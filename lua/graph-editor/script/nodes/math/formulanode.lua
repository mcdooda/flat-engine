local FormulaNodeEditor = {}

function FormulaNodeEditor.build(formulaNode, nodeWidget, pinsWidget)
    local formulaWidget = Widget.makeTextInput(table.unpack(flat.ui.settings.defaultFont))
    formulaWidget:setText(formulaNode:getFormula())
    local _, h = formulaWidget:getSize()
    local inputContainer = Widget.makeFixedSize(120, h + 2)
    inputContainer:setMargin(2)
    inputContainer:setBackgroundColor(0xFFFFFFFF)
    formulaWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    formulaWidget:setSize(100, h)
    formulaWidget:setTextColor(0x000000FF)
    formulaWidget:setMargin(1)
    formulaWidget:valueChanged(function()
        local formula = formulaWidget:getText()
        formulaNode:setFormula(formula)
    end)
    formulaWidget:submit(function()
        formulaNode:updateInputPins()
        local foldedNodes = nodeWidget.mainWindow:getFoldedNodes()
        Timer.start(0.01, nil, function()
            nodeWidget:rebuild(foldedNodes)
            nodeWidget.mainWindow:updateAllNodesPinSocketWidgets()
            nodeWidget.mainWindow:drawLinks(true)
        end)
    end)
    inputContainer:addChild(formulaWidget)
    pinsWidget:addChild(inputContainer)
    return true, inputContainer
end

return FormulaNodeEditor