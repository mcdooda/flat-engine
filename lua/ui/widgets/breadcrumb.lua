local Theme = flat.ui.settings.theme.breadcrumb

local Breadcrumb = {}
Breadcrumb.__index = Breadcrumb

function Breadcrumb:new(parent)
    local o = setmetatable({
        breadcrumb = nil,
        buttons = {},
        currentButton = nil
    }, self)
    o:build(parent)
    return o
end

function Breadcrumb:build(parent)
    print('parent', parent)
    local breadcrumb = Widget.makeLineFlow()
    breadcrumb:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.COMPRESS_Y)
    breadcrumb:setBackgroundColor(Theme.BACKGROUND_COLOR)
    breadcrumb:setPadding(2)
    breadcrumb:setMargin(1)
    parent:addChild(breadcrumb)
    self.breadcrumb = breadcrumb
end

function Breadcrumb:addItem(text)
    local button = Widget.makeLineFlow()
    button:setPadding(1, 3, 1, 3)
    button:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR)

    button:mouseEnter(function()
        if button ~= self.currentButton then
            button:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR_OVER)
        end
    end)
    button:mouseLeave(function()
        if button == self.currentButton then
            button:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR_CURRENT)
        else
            button:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR)
        end
    end)

    local buttonIndex = #self.buttons + 1
    button:click(function()
        self:setCurrentItem(buttonIndex)
    end)

    do
        local buttonText = Widget.makeText(text, table.unpack(Theme.BUTTON_FONT))
        buttonText:setTextColor(Theme.BUTTON_TEXT_COLOR)
        button:addChild(buttonText)
    end

    local childrenCount = self.breadcrumb:getChildrenCount()
    if #self.buttons > 0 then
        -- add a separator first
        local separator = Widget.makeText('>', table.unpack(Theme.BUTTON_FONT))
        separator:setMargin(1, 2, 0, 2)
        separator:setTextColor(Theme.BUTTON_TEXT_COLOR)
        self.breadcrumb:addChild(separator)
    end

    self.breadcrumb:addChild(button)

    self.buttons[buttonIndex] = button

    return button
end

function Breadcrumb:setCurrentItem(index)
    local newCurrentButton = self.buttons[index]
    if newCurrentButton ~= self.currentButton then
        if self.currentButton then
            self.currentButton:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR)
        end
        newCurrentButton:setBackgroundColor(Theme.BUTTON_BACKGROUND_COLOR_CURRENT)
        self.currentButton = newCurrentButton
    end
end

flat.ui.Breadcrumb = Breadcrumb