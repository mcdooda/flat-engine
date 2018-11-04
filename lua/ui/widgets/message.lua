local ToolbarTheme = flat.ui.settings.theme.toolbar -- TODO
local WindowTheme = flat.ui.settings.theme.window -- TODO
local Button = require 'data/scripts/ui/button'

local function message(messageString, messageContent, buttons)
    local overlay = flat.ui.overlay()

    local window = flat.ui.Window:new(Widget.getRoot())
    window.onClose:on(function()
        overlay:removeFromParent()
    end)
    window:setSize(300, 125)
    window:setPositionPolicy(Widget.PositionPolicy.CENTER)
    window:setPosition(0, 0)
    window:setResizable(false)

    local content = Widget.makeExpand()
    content:setMargin(5)
    local contentContainer = Widget.makeColumnFlow()
    contentContainer:setSizePolicy(Widget.SizePolicy.EXPAND)

    do
        local messageText = Widget.makeText(messageString, table.unpack(WindowTheme.CONTENT_FONT))
        messageText:setTextColor(WindowTheme.CONTENT_TEXT_COLOR)
        messageText:setPositionPolicy(Widget.PositionPolicy.CENTER)
        contentContainer:addChild(messageText)
    end

    if messageContent then
        contentContainer:addChild(messageContent)
    end

    do
        local spacer = Widget.makeExpand()
        contentContainer:addChild(spacer)
    end

    if buttons then
        local buttonsContainer = Widget.makeLineFlow()
        buttonsContainer:setPositionPolicy(Widget.PositionPolicy.BOTTOM_RIGHT)
        for i = 1, #buttons do
            local button = buttons[i]

            local buttonText = Widget.makeText(button.text, table.unpack(ToolbarTheme.BUTTON_FONT))
            buttonText:setTextColor(ToolbarTheme.BUTTON_TEXT_COLOR)
            local buttonWidget = Button:new(
                buttonText,
                {
                    color = ToolbarTheme.BUTTON_BACKGROUND_COLOR,
                    hoverColor = ToolbarTheme.BUTTON_BACKGROUND_COLOR_OVER
                }
            )
            buttonWidget:click(button.click)
            buttonsContainer:addChild(buttonWidget.container)
        end
        contentContainer:addChild(buttonsContainer)
    end

    content:addChild(contentContainer)
    window:setContent(content)

    return window
end

flat.ui.message = message