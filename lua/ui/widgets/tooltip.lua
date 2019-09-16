local Theme = flat.ui.settings.theme.tooltip

local function addTooltip(widget, tooltip)
    local tooltipWidget = Widget.makeColumnFlow()
    tooltipWidget:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
    tooltipWidget:setBackgroundColor(Theme.BACKGROUND_COLOR)
    tooltipWidget:setPadding(5)

    do
        local tooltipLabel = Widget.makeText(tooltip, table.unpack(Theme.FONT))
        tooltipLabel:setTextColor(Theme.TEXT_COLOR)
        tooltipWidget:addChild(tooltipLabel)
    end

    widget:mouseEnter(function()
        Widget.getRoot():addChild(tooltipWidget)
    end)

    widget:mouseLeave(function()
        tooltipWidget:removeFromParent()
    end)

    widget:mouseMove(function()
        local x, y = Mouse.getPosition()
        tooltipWidget:setPosition(x, y - 60)
    end)
end

flat.ui.addTooltip = addTooltip