if not flat.debug then
    return
end

setmetatable(
    _ENV,
    {
        __index = function(t, name)
            error(debug.traceback('Trying to access a global variable \'' .. tostring(name) .. '\' that does not exist', 2))
        end,
        __newindex = function(t, name, value)
            error(debug.traceback('Trying to declare a global variable \'' .. tostring(name) .. '\' to \'' .. tostring(value) .. '\' ', 2))
        end
    }
)

function flat.debug.monitorAccesses(t, read, write)
    local mt = {}
    local values = {}
    if read then
        mt.__index = function(t, k)
            print '=============='
            print('Reading ' .. tostring(t) .. ': ' ..tostring(k) .. ' (' .. values[k] .. ')')
            flat.debug.printstack()
            print '=============='
            return values[k]
        end
    end
    if write then
        mt.__nweindex = function(t, k, v)
            print '=============='
            print('Writing ' .. tostring(t) .. ': ' ..tostring(k) .. ' = ' .. tostring(v) .. ')')
            flat.debug.printstack()
            print '=============='
            values[k] = v
        end
    end
    setmetatable(t, mt)
end