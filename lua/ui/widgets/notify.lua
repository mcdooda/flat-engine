local Icon = require 'data/scripts/ui/icon'

local Notification = {}
Notification.__index = Notification

local themeNotify = flat.ui.settings.theme.notify
local fontNotify = flat.ui.settings.notifyFont
local notifyCenterWidget = nil

local function getNotifyCenterWidget()
    if not notifyCenterWidget then
        notifyCenterWidget = Widget.makeColumnFlow()
        notifyCenterWidget:setPadding(5, 0, 0, 0)
        local root = Widget.getRoot()
        notifyCenterWidget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
        root:addChild(notifyCenterWidget)
    end
    return notifyCenterWidget
end

local function notify(icon, message, color, duration, buttons)
    local notifyCenterWidet = getNotifyCenterWidget()
    local root = Widget.getRoot()
    notifyCenterWidget:removeFromParent()
    root:addChild(notifyCenterWidget)
    color = color or themeNotify.DEFAULT_BACKGROUND_COLOR
    local notification = Notification:new(icon, message, color, duration, buttons)
    notifyCenterWidget:addChild(notification.container)
end

local function success(message, buttons)
    notify('ok-circle', message, themeNotify.SUCCESS_BACKGROUND_COLOR, 3, buttons)
    print('Success:', message)
end

local function warn(message, buttons)
    notify('warning-sign', message, themeNotify.WARNING_BACKGROUND_COLOR, math.huge, buttons)
    print('Warning:', message)
end

local function error(message, buttons)
    notify('remove-circle', message, themeNotify.ERROR_BACKGROUND_COLOR, math.huge, buttons)
    print('Error:', message)
end

local function info(message, buttons)
    notify('alert-circle', message, themeNotify.INFO_BACKGROUND_COLOR, 3, buttons)
    print('Info:', message)
end


function Notification:new(icon, message, color, duration, buttons)
    local container = Widget.makeLineFlow()
    container:setBackgroundColor(color)
    container:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
    container:setMargin(5, 10, 5, 0)

    do
        local iconContainer = Widget.makeLineFlow()
        iconContainer:setBackgroundColor(themeNotify.BACKGROUND_COLOR)
        iconContainer:setMargin(1)
        iconContainer:setPadding(4, 4, 4, 8)

        do
            local icon = Icon:new(icon, 14)
            icon.container:setPadding(7, 0, 0, 0)
            icon:setColor(color)
            icon.container:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
            iconContainer:addChild(icon.container)
        end

        do
            local messageContainer = Widget.makeColumnFlow()

            do
                local textWidget = Widget.makeText(message, table.unpack(fontNotify))
                textWidget:setMargin(5, 15, 8, 6)
                textWidget:setTextColor(themeNotify.TEXT_COLOR)
                messageContainer:addChild(textWidget)
            end

            if buttons then
                local buttonsContainer = Widget.makeLineFlow()
                buttonsContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
                buttonsContainer:setBackgroundColor(0xFF0000FF)

                for i = 1, #buttons do
                    local button = Widget.makeText(buttons[i][1], table.unpack(fontNotify))
                    button:setTextColor(themeNotify.TEXT_COLOR)
                    button:setMargin(5)
                    button:click(function()
                        buttons[i][2]()
                        return true
                    end)
                    buttonsContainer:addChild(button)
                end

                messageContainer:addChild(buttonsContainer)
            end

            iconContainer:addChild(messageContainer)
        end

        container:addChild(iconContainer)
    end

    local o = setmetatable({
        isRemoved = false,
        container = container
    }, self)

    container:click(function()
        o:remove()
    end)

    if duration < math.huge then
        local timer = flat.Timer()
        timer:onEnd(function()
            o:remove()
        end)
        timer:start(duration)
    end

    return o
end

function Notification:remove(color)
    if not self.isRemoved then
        self.isRemoved = true
        local container = self.container
        local width, height = container:getComputedSize()
        container:setSizePolicy(Widget.SizePolicy.FIXED)
        container:setSize(width, height)
        local effectDuration = 0.3
        local timer = flat.Timer()
        timer:onUpdate(function(timer, elapsedTime)
            local ratio = (effectDuration - elapsedTime) / effectDuration
            local newHeight = height * ratio
            container:setSize(width, newHeight)
            container:setMargin(5 * ratio, 10, 5 * ratio, 0)
        end)
        timer:onEnd(function()
            container:removeFromParent()
        end)
        timer:start(effectDuration)
    end
end

flat.ui.warn = warn
flat.ui.error = error
flat.ui.success = success
flat.ui.info = info