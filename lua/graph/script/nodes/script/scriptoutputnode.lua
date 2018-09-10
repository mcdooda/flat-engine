local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ScriptOutputNode = FunctionalScriptNode:inherit 'Script Output'

function ScriptOutputNode:addedToGraph(graph)
    graph.outputNode = self
end

function ScriptOutputNode:buildPins()
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
end

function ScriptOutputNode:addCustomPin(pin, otherPin)
    pin.pinName = otherPin.pinName
    self:addInputPin(PinTypes.ANY, 'New Pin', self.addCustomPin)
    return true
end

function ScriptOutputNode:execute(runtime)
    -- nothing to do, all the pins will be read from the connected pins' nodes
    assert(false)
end

function ScriptOutputNode:getLoadArguments()
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

function ScriptOutputNode:load(...)
    self.pinsData = {...}
end

return ScriptOutputNode