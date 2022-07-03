local TextInputTheme = flat.ui.settings.theme.textInput

local function choice(message, choices, defaultValue)
    local content = Widget.makeExpand()
    content:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

    local returnValue
    local choiceIndex
    do
        local choiceInputBorder = Widget.makeExpand()
        choiceInputBorder:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        choiceInputBorder:setPositionPolicy(Widget.PositionPolicy.CENTER)
        choiceInputBorder:setBackgroundColor(TextInputTheme.BORDER_COLOR)
        choiceInputBorder:setMargin(0, 0, 5, 0)

        do
            local choiceInputBackground = Widget.makeColumnFlow()
            choiceInputBackground:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
            choiceInputBackground:setBackgroundColor(TextInputTheme.BACKGROUND_COLOR)
            choiceInputBackground:setMargin(1)

            local choiceFields = {}

            local selectedItemIndex

            local function selectItem(itemIndex)
                if selectedItemIndex then
                    choiceFields[selectedItemIndex]:setBackgroundColor(0)
                end
                returnValue = choices[itemIndex]
                choiceIndex = itemIndex
                choiceFields[itemIndex]:setBackgroundColor(0x3498DBFF)
                selectedItemIndex = itemIndex
            end

            local defaultItemIndex

            for i = 1, #choices do
                local choice = choices[i]

                local choiceField = Widget.makeExpand()
                choiceField:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

                do
                    local textLabelWidget = Widget.makeText(choice, table.unpack(TextInputTheme.FONT))
                    textLabelWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
                    textLabelWidget:setTextColor(TextInputTheme.TEXT_COLOR)
                    choiceField:addChild(textLabelWidget)
                end

                choiceField:click(function()
                    selectItem(i)
                end)

                choiceInputBackground:addChild(choiceField)
                choiceFields[i] = choiceField

                if defaultValue and choice == defaultValue then
                    defaultItemIndex = i
                end
            end

            if defaultItemIndex then
                selectItem(defaultItemIndex)
            end

            choiceInputBorder:addChild(choiceInputBackground)
        end

        content:addChild(choiceInputBorder)
    end

    local okButtonClicked = false
    local okButton = {
        text = 'OK',
        click = function()
            okButtonClicked = true
        end
    }

    local choiceCanceled = false
    local cancelButton = {
        text = 'Cancel',
        click = function()
            choiceCanceled = true
        end
    }

    local messageWindow = flat.ui.message(message, content, { okButton, cancelButton })
    messageWindow:setTitle ''
    messageWindow:setSize(300, 250)
    local windowClosed = false
    messageWindow.onClose:on(function()
        choiceCanceled = true
        windowClosed = true
    end)

    while not okButtonClicked and not choiceCanceled do
        coroutine.yield()
    end

    if okButtonClicked then
        messageWindow:close()
        return returnValue, choiceIndex
    else
        if not windowClosed then
            messageWindow:close()
        end
    end
end

flat.ui.choice = choice