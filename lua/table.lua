local getmetatable = getmetatable
local tostring = tostring
local type = type
local max = math.max

local function tableLength(t)
    local length = 0
    for _ in pairs(t) do
        length = length + 1
    end
    return length
end

local function tableFindValueKey(t, v)
    for key, value in pairs(t) do
        if value == v then
            return key
        end
    end
end

-- see http://lua-users.org/lists/lua-l/2008-11/msg00106.html
local function isInt(n)
    return type(n) == 'number' and (n + 2 ^ 52) - 2 ^ 52 == n
end

local function tableMaxIntKey(t)
    local maxIntKey = 0
    for key in pairs(t) do
        if isInt(key) then
            maxIntKey = max(key, maxIntKey)
        end
    end
    return maxIntKey
end

local function tableIsArray(t)
    return #t == tableLength(t)
end

local function arrayAdd(t, value)
    assert(tableIsArray(t))
    t[#t + 1] = value
end

local function arrayFindValueIndex(t, value)
    assert(tableIsArray(t))
    for i = 1, #t do
        if t[i] == value then
            return i
        end
    end
end

local function arrayRemoveIndex(t, index)
    assert(tableIsArray(t))
    local length = #t
    for i = index, length - 1 do
        t[i] = t[i + 1]
    end
    t[length] = nil
end

local function arrayRemoveValue(t, value)
    assert(tableIsArray(t))
    local index = assert(arrayFindValueIndex(t, value), 'value not found')
    arrayRemoveIndex(t, index)
    return index
end

local function arrayRemoveIndexCyclic(t, index)
    assert(tableIsArray(t))
    local length = #t
    t[index] = t[length]
    t[length] = nil
end

local function arrayRemoveValueCyclic(t, value)
    assert(tableIsArray(t))
    local index = assert(arrayFindValueIndex(t, value), 'value not found')
    arrayRemoveIndexCyclic(t, index)
    return index
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

flat.tableLength = tableLength
flat.tableFindValueKey = tableFindValueKey
flat.tableMaxIntKey = tableMaxIntKey

flat.tableIsArray = tableIsArray

flat.arrayAdd = arrayAdd
flat.arrayFindValueIndex = arrayFindValueIndex
flat.arrayRemoveIndex = arrayRemoveIndex
flat.arrayRemoveValue = arrayRemoveValue
flat.arrayRemoveIndexCyclic = arrayRemoveIndexCyclic
flat.arrayRemoveValueCyclic = arrayRemoveValueCyclic

flat.sortedPairs = sortedPairs