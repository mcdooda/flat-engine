local getmetatable = getmetatable
local tostring = tostring

local function arrayAdd(t, value)
    t[#t + 1] = value
end

local function arrayFindValueIndex(t, value)
    for i = 1, #t do
        if t[i] == value then
            return i
        end
    end
end

local function arrayRemoveIndex(t, index)
    local length = #t
    for i = index, length - 1 do
        t[i] = t[i + 1]
    end
    t[length] = nil
end

local function arrayRemoveValue(t, value)
    local index = assert(arrayFindValueIndex(t, value), 'value not found')
    arrayRemoveIndex(t, index)
end

local function arrayRemoveIndexCyclic(t, index)
    local length = #t
    t[index] = t[length]
    t[length] = nil
end

local function arrayRemoveValueCyclic(t, value)
    local index = assert(arrayFindValueIndex(t, value), 'value not found')
    arrayRemoveIndexCyclic(t, index)
end

local function sortedPairs(t)
    local keys = {}
    for k in pairs(t) do
        arrayAdd(keys, k)
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

flat.arrayAdd = arrayAdd
flat.arrayFindValueIndex = arrayFindValueIndex
flat.arrayRemoveIndex = arrayRemoveIndex
flat.arrayRemoveValue = arrayRemoveValue
flat.arrayRemoveIndexCyclic = arrayRemoveIndexCyclic
flat.arrayRemoveValueCyclic = arrayRemoveValueCyclic

flat.sortedPairs = sortedPairs