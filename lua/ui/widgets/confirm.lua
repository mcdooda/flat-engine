
local function confirm(message)
    local okButtonClicked = false
    local okButton = {
        text = 'OK',
        click = function()
            okButtonClicked = true
        end
    }

    local promptCanceled = false
    local cancelButton = {
        text = 'Cancel',
        click = function()
            promptCanceled = true
        end
    }
    -- local WarningIcon = Icon:new 'warning'
    local messageWindow = flat.ui.message(message, nil, { okButton, cancelButton })
    messageWindow:setTitle 'Confirm'
    messageWindow:setIcon 'warning-sign'
    local windowClosed = false

    messageWindow.onClose:on(function()
        promptCanceled = true
        windowClosed = true
    end)

    while not okButtonClicked and not promptCanceled do
        coroutine.yield()
    end

    if okButtonClicked then
        messageWindow:close()
    else
        if not windowClosed then
            messageWindow:close()
        end
    end
    return okButtonClicked
end

flat.ui.confirm = confirm