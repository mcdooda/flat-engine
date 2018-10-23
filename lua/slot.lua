local Slot = {}
Slot.__index = Slot

function Slot:new()
    local o = setmetatable({
        callbacks = {}
    }, self)
    return o
end

function Slot:on(callback)
    flat.arrayAdd(self.callbacks, callback)
end

function Slot:off(callback)
    local index = flat.findArrayValue(self.callbacks, callback)
    if not index then
        return false
    end
    flat.arrayRemoveIndexCyclic(self.callbacks, index)
    return true
end

function Slot:__call(...)
    for i = 1, #self.callbacks do
        self.callbacks[i](...)
    end
end

flat.Slot = Slot