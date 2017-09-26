local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local FunctionNode = FunctionalScriptNode:inherit 'Function'

function FunctionNode:buildPins()
    self.functionOutPin = self:addOutputPin(PinTypes.FUNCTION, 'Function')
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')

    if self.pinsData then
        for i = 1, #self.pinsData do
            local pin = self.pinsData[i]
            local pinType = pin[1]
            local pinName = pin[2]
            self:addOutputPin(pinType, pinName)
        end
        self.pinsData = nil
    end
    self:addOutputPinAny('New Pin', self.addCustomPin)
end

function FunctionNode:addCustomPin(pin)
    local function capitalize(name)
        return name:sub(1, 1):upper() .. name:sub(2):lower()
    end
    pin.pinName = capitalize(PinTypes:toString(pin.pinType))
    self:addOutputPinAny('New Pin', self.addCustomPin)
    return true
end

function FunctionNode:execute(runtime)
    local functionValue = function(...)
        local numArguments = select('#', ...)
        for i = 1, numArguments do
            local outputPin = self.outputPins[i + 2]
            if outputPin and outputPin.pinType ~= PinTypes.ANY then
                local argument = select(i, ...)
                runtime:writeOptionalPin(outputPin, argument)
            end
        end
        runtime:impulse(self.impulseOutPin)
    end
    runtime:writePin(self.functionOutPin, functionValue)
end

function FunctionNode:getLoadArguments()
    local loadArguments = {}
    for i = 3, #self.outputPins - 1 do
        local outputPin = self.outputPins[i]
        loadArguments[i] = {
            outputPin.pinType,
            outputPin.pinName
        }
    end
    return table.unpack(loadArguments)
end

function FunctionNode:load(...)
    self.pinsData = {...}
end

return FunctionNode