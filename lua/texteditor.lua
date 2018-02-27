
local textEditors = {
    'code',
    'sublime_text'
}
local function open(file)
    local i = 1
    repeat
        local textEditor = textEditors[i]
        local ok = os.execute(textEditor .. ' ' .. file)
        i = i + 1
    until ok or not textEditors[i]
end

flat.textEditor = {
    open = open
}
