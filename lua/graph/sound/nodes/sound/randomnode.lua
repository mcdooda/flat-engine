local SoundNode = flat.require 'graph/sound/soundnode'
local PinTypes = flat.require 'graph/pintypes'

local RandomNode = SoundNode:inherit 'Random'

function RandomNode:buildPins()
    if self.inputPinCount then
        for i = 1, self.inputPinCount do
            local pinName = 'Sound ' .. tostring(i)
            self:addInputPin(flat.types['flat.audio.Chunk'], pinName)
        end
        self.inputPinCount = nil
    end
    self:addInputPin(flat.types['flat.audio.Chunk'], 'New Pin', self.addCustomPin)

    self.soundOutPin = self:addOutputPin(flat.types['flat.audio.Chunk'], 'Sound')
end

function RandomNode:addCustomPin(pin, otherPin)
    pin.pinName = otherPin.pinName
    self:addInputPin(flat.types['flat.audio.Chunk'], 'New Pin', self.addCustomPin)
    return true
end

function RandomNode:execute(runtime)
    local validInputPins = {}
    for i = 1, #self.inputPins do
        local inputPin = self.inputPins[i]
        if self:isInputPinPlugged(inputPin) then
            flat.arrayAdd(validInputPins, inputPin)
        end
    end

    if #validInputPins > 0 then
        local randomPin = validInputPins[math.random(1, #validInputPins)]
        local sound = runtime:readPin(randomPin)
        runtime:writePin(self.soundOutPin, sound)
    end
end

function RandomNode:getLoadArguments()
    return #self.inputPins - 1
end

function RandomNode:load(...)
    self.inputPinCount = ...
end

function RandomNode:isInputPinOptional(inputPin)
    return self:isLastInputPin(inputPin)
end

return RandomNode