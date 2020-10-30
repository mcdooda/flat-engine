flat.ui.settings = {
    defaultFont = { flat.assetPath 'fonts/Lato-Medium.ttf', 12 },
    notifyFont = { flat.assetPath 'fonts/Lato-Medium.ttf', 14 },
}

flat.ui.settings.theme = {
    window = {
        BORDER_COLOR             = 0x888888FF,

        -- title
        TITLE_BACKGROUND_COLOR   = 0x444444FF,
        TITLE_TEXT_COLOR         = 0xECF0F1FF,
        TITLE_FONT               = flat.ui.settings.defaultFont,

        -- content
        CONTENT_BACKGROUND_COLOR = 0xEEEEEEFF,
        CONTENT_TEXT_COLOR       = 0x111111FF,
        CONTENT_FONT             = flat.ui.settings.defaultFont
    },
    notify = {
        -- TEXT_COLOR = 0xFFFFFFFF,
        TEXT_COLOR = 0x111111FF,
        ICON_COLOR = 0x00000080,
        SUCCESS_BACKGROUND_COLOR = 0x27AE60FF,
        ERROR_BACKGROUND_COLOR = 0xc0392bFF,
        WARNING_BACKGROUND_COLOR = 0xD35400FF,
        INFO_BACKGROUND_COLOR = 0x2980B9FF,
        BACKGROUND_COLOR = 0xFFFFFFCC
    },
    toolbar = {
        BACKGROUND_COLOR = 0xCCCCCCFF,

        -- button
        BUTTON_TEXT_COLOR            = 0x2C3E50FF,
        BUTTON_BACKGROUND_COLOR      = 0xCCCCCCFF,
        BUTTON_BACKGROUND_COLOR_OVER = 0xDDDDDDFF,
        BUTTON_FONT                  = flat.ui.settings.defaultFont
    },
    breadcrumb = {
        BACKGROUND_COLOR                = 0xBBBBBBFF,

        -- button
        BUTTON_TEXT_COLOR               = 0x2C3E50FF,
        BUTTON_BACKGROUND_COLOR         = 0xBBBBBBFF,
        BUTTON_BACKGROUND_COLOR_OVER    = 0xDDDDDDFF,
        BUTTON_BACKGROUND_COLOR_CURRENT = 0xCCCCCCFF,
        BUTTON_FONT                     = flat.ui.settings.defaultFont
    },
    textInput = {
        BORDER_COLOR     = 0x888888FF,
        BACKGROUND_COLOR = 0xFFFFFFFF,
        TEXT_COLOR       = 0x111111FF,
        FONT             = flat.ui.settings.defaultFont
    },
    graphEditor = {
        node = {
            BACKGROUND_COLOR             = 0xBDC3C7AA,
            SELECTED_BACKGROUND_COLOR    = 0x9BA1A5AA,
            HIGHLIGHTED_BACKGROUND_COLOR = 0xE74C3CAA,
            TITLE_BACKGROUND_COLOR       = 0x2C3E50EE,
            TITLE_TEXT_COLOR             = 0xECF0F1FF,
            PIN_NAME_TEXT_COLOR          = 0x000000FF
        },
        grid = {
            BACKGROUND_COLOR = 0xECF0F1FF,
            BIG_STEP         = 100,
            BIG_LINE_COLOR   = 0xD4D8D9FF,
            BIG_LINE_WIDTH   = 2,
            SMALL_STEP       = 20,
            SMALL_LINE_COLOR = 0xE1E5E6FF,
            SMALL_LINE_WIDTH = 1
        }
    },
    tooltip = {
        BACKGROUND_COLOR = 0xBBBBBBFF,
        TEXT_COLOR       = 0x111111FF,
        FONT             = flat.ui.settings.defaultFont
    }
}