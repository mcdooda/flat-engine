local Slot = {}
Slot.__index = Slot

function Slot:new()
    local o = setmetatable({
        callbacks = {}
    }, self)
    return o
end

function Slot:on(callback)
    self.callbacks[#self.callbacks + 1] = callback
end

function Slot:off(callback)
    local index
    local numCallbacks = #self.callbacks
    for i = 1, numCallbacks do
        if self.callbacks[i] == callback then
            index = i
            break
        end
    end

    if not index then
        return false
    end

    if index < numCallbacks then
        self.callbacks[index] = self.callbacks[numCallbacks]
    end
    self.callbacks[numCallbacks] = nil
    return true
end

function Slot:__call(...)
    for i = 1, #self.callbacks do
        self.callbacks[i](...)
    end
end

flat.Slot = Slot