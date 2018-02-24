flat.ui.settings = {
    defaultFont = { flat.assetPath 'fonts/Lato-Medium.ttf', 12 }
}

flat.ui.settings.theme = {
    window = {
        BORDER_COLOR           = 0x888888FF,

        -- title
        TITLE_BACKGROUND_COLOR = 0x444444FF,
        TITLE_TEXT_COLOR       = 0xECF0F1FF,
        TITLE_FONT             = flat.ui.settings.defaultFont,
    },
    toolbar = {
        BACKGROUND_COLOR = 0xCCCCCCFF,

        -- button
        BUTTON_TEXT_COLOR            = 0x2C3E50FF,
        BUTTON_BACKGROUND_COLOR      = 0xCCCCCCFF,
        BUTTON_BACKGROUND_COLOR_OVER = 0xDDDDDDFF,
        BUTTON_FONT                  = flat.ui.settings.defaultFont
    },
    graphEditor = {
        grid = {
            BIG_STEP         = 100,
            BIG_LINE_COLOR   = 0xD4D8D9FF,
            BIG_LINE_WIDTH   = 2,
            SMALL_STEP       = 20,
            SMALL_LINE_COLOR = 0xE1E5E6FF,
            SMALL_LINE_WIDTH = 1
        }
    }
}