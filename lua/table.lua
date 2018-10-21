local getmetatable = getmetatable
local tostring = tostring

local function sortedpairs(t)
    local keys = {}
    for k in pairs(t) do
        keys[#keys + 1] = k
    end
    table.sort(keys, function(a, b)
        local aType = type(a)
        local bType = type(b)
        if aType == bType then
            local mt = getmetatable(a)
            if mt and mt.__lt then
                return a < b
            else
                return tostring(a) < tostring(b)
            end
        else
            return aType < bType
        end
    end)
    local index = 1
    local tableSize = #keys
    return function()
        if index > tableSize then
            return
        end
        local key = keys[index]
        local value = t[key]
        index = index + 1
        return key, value
    end
end

flat.sortedpairs = sortedpairs