local uniqueObjectGetters = setmetatable({
    __index = function(t, type)
        error('No flat.getUniqueObject() implementation for type ' .. flat.typetostring(type))
    end
}, {})

local function getUniqueObject(value)
    local type = flat.type(value)
    return uniqueObjectGetters[type](value)
end

local function registerUniqueObjectGetter(typename, uniqueObjectGetter)
    local type = flat.types[typename]
    uniqueObjectGetters[type] = uniqueObjectGetter
end

registerUniqueObjectGetter(
    'TABLE',
    function(object)
        return object
    end
)

flat.getUniqueObject = getUniqueObject
flat.registerUniqueObjectGetter = registerUniqueObjectGetter