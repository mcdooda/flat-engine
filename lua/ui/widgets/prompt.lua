local TextInputTheme = flat.ui.settings.theme.textInput

local function prompt(message, defaultValue)
    local content = Widget.makeExpand()
    content:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    content:setBackgroundColor(0xFF0000FF)

    local textInputWidget
    do
        local textInputBorder = Widget.makeExpand()
        textInputBorder:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        textInputBorder:setPositionPolicy(Widget.PositionPolicy.CENTER)
        textInputBorder:setBackgroundColor(TextInputTheme.BORDER_COLOR)

        do
            local textInputBackground = Widget.makeExpand()
            textInputBackground:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
            textInputBackground:setBackgroundColor(TextInputTheme.BACKGROUND_COLOR)
            textInputBackground:setMargin(1)

            do
                textInputWidget = Widget.makeTextInput(table.unpack(TextInputTheme.FONT))
                textInputWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
                textInputWidget:setTextColor(TextInputTheme.TEXT_COLOR)
                if defaultValue then
                    textInputWidget:setText(defaultValue)
                end
                textInputBackground:addChild(textInputWidget)
            end

            textInputBorder:addChild(textInputBackground)
        end

        content:addChild(textInputBorder)
    end

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

    local messageWindow = flat.ui.message(message, content, { okButton, cancelButton })
    messageWindow:setTitle ''
    local windowClosed = false
    messageWindow.onClose:on(function()
        promptCanceled = true
        windowClosed = true
    end)

    while not okButtonClicked and not promptCanceled do
        coroutine.yield()
    end

    if okButtonClicked then
        local returnValue = textInputWidget:getText()
        messageWindow:close()
        return returnValue
    else
        if not windowClosed then
            messageWindow:close()
        end
    end
end

flat.ui.prompt = prompt