local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ArrayNode = FunctionalScriptNode:inherit 'Array'

function ArrayNode:buildPins()
    if self.pinsData then
        for i = 1, #self.pinsData do
            local pin = self.pinsData[i]
            local pinType = pin[1]
            local pinName = pin[2]
            self:addInputPin(pinType, pinName)
        end
        self.pinsData = nil
    end
    self:addInputPin(PinTypes.ANY, 'New Pin', self.addCustomPin)

    self.arrayOutPin = self:addOutputPin(flat.types.TABLE, 'Array')
end

function ArrayNode:addCustomPin(pin)
    pin.pinName = self:makePinNameFromType(pin.pinType)
    self:addInputPin(PinTypes.ANY, 'New Pin', self.addCustomPin)
    return true
end

function ArrayNode:execute(runtime)
    local array = {}
    for i = 1, #self.inputPins - 1 do
        local inputPin = self.inputPins[i]
        array[i] = runtime:readPin(inputPin)
    end
    runtime:writePin(self.arrayOutPin, array)
end

function ArrayNode:getLoadArguments()
    local loadArguments = {}
    for i = 1, #self.inputPins - 1 do
        local inputPin = self.inputPins[i]
        loadArguments[i] = {
            inputPin.pinType,
            inputPin.pinName
        }
    end
    return table.unpack(loadArguments)
end

function ArrayNode:load(...)
    self.pinsData = {...}
end

return ArrayNode