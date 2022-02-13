local function safeRequire(require, file)
    local callstack
    local function errorHandler(...)
        callstack = debug.traceback()
        return ...
    end
    local results = { xpcall(require, errorHandler, file) }
    if not results[1] then
        local message = results[2]
        -- TODO: fix that, this is bad for performance and inaccurate (the module could be found but require another module that cannot be found)
        if message:match('module \'.+\' not found') then
            return nil
        else
            message = message .. '\n' .. callstack
            error(message)
        end
    end
    return select(2, table.unpack(results))
end

flat.safeRequire = safeRequire