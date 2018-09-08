local Theme = flat.ui.settings.theme.toolbar

local Toolbar = {}
Toolbar.__index = Toolbar

function Toolbar:new(parent)
    local o = setmetatable({
        toolbar = nil
    }, self)
    o:build(parent)
    return o
end

function Toolbar:build(parent)
    local toolbar = Widget.makeLineFlow()
    toolbar:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    toolbar:setBackgroundColor(Theme.BACKGROUND_COLOR)
    parent:addChild(toolbar)
    self.toolbar = toolbar
end

function Toolbar:addButton(text)
    local button = Widget.makeLineFlow()
    button:setPadding(2, 4, 2, 4)
    button:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR)

    button:mouseEnter(function()
        button:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR_OVER)
    end)
    button:mouseLeave(function()
        button:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR)
    end)

    do
        local buttonText = Widget.makeText(text, table.unpack(Theme.BUTTON_FONT))
        buttonText:setTextColor(Theme.BUTTON_TEXT_COLOR)
        buttonText:setMargin(0, 5, 0, 5)
        button:addChild(buttonText)
    end

    self.toolbar:addChild(button)
    return button
end

flat.ui.Toolbar = Toolbar