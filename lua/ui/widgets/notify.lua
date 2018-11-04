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

local function notify(icon, message, color)
    local notifyCenterWidet = getNotifyCenterWidget()
    local root = Widget.getRoot()
    notifyCenterWidget:removeFromParent()
    root:addChild(notifyCenterWidget)
    color = color or themeNotify.DEFAULT_BACKGROUND_COLOR
    local notification = Notification:new(icon, message, color)
    notifyCenterWidget:addChild(notification.container)
end

local function success(message)
    notify('ok-circle', message, themeNotify.SUCCESS_BACKGROUND_COLOR)
end

local function warn(message)
    notify('warning-sign', message, themeNotify.WARNING_BACKGROUND_COLOR)
end

local function error(message)
    notify('remove-circle', message, themeNotify.ERROR_BACKGROUND_COLOR)
end

local function info(message)
    notify('alert-circle', message, themeNotify.INFO_BACKGROUND_COLOR)
end


function Notification:new(icon, message, color)
    local container = Widget.makeLineFlow()
    container:setBackgroundColor(color)
    container:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
    container:setMargin(5, 10, 5, 0)
    local iconContainer = Widget.makeLineFlow()
    iconContainer:setBackgroundColor(themeNotify.BACKGROUND_COLOR)
    iconContainer:setMargin(1)
    iconContainer:setPadding(4, 0, 4, 8)
    local icon = Icon:new(icon, 14)
    icon.container:setPadding(0, 0, 7, 0)
    icon:setColor(color)
    iconContainer:addChild(icon.container)

    container:addChild(iconContainer)
    local textWidget = Widget.makeText(message, table.unpack(fontNotify))
    textWidget:setMargin(5, 15, 8, 6)
    textWidget:setTextColor(themeNotify.TEXT_COLOR)
    iconContainer:addChild(textWidget)

    local o = setmetatable({
        isRemoved = false,
        container = container
    }, self)

    container:click(function()
        o:remove()
    end)

    local timer = flat.Timer()
    timer:onEnd(function()
        o:remove()
    end)
    timer:start(3)

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