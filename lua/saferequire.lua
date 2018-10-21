local function safeRequire(require, file)
    local results = { pcall(require, file) }
    if not results[1] then
        local message = results[2]
        if message:match('module \'.+\' not found') then
            return nil
        else
            error(message)
        end
    end
    return select(2, table.unpack(results))
end

flat.safeRequire = safeRequire