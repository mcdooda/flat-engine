local function sortedpairs(t)
    local keys = {}
    for k in pairs(t) do
        keys[#keys + 1] = k
    end
    table.sort(keys, function(a, b)
        local aType = type(a)
        local bType = type(b)
        if aType == bType then
            return a < b
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