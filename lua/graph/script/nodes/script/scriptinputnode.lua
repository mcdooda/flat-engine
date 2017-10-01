local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ScriptInputNode = FunctionalScriptNode:inherit 'Script Input'

function ScriptInputNode:addedToGraph(graph)
    graph.inputNode = self
end

function ScriptInputNode:buildPins()
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

function ScriptInputNode:addCustomPin(pin)
    pin.pinName = self:makePinNameFromType(pin.pinType)
    self:addOutputPinAny('New Pin', self.addCustomPin)
    return true
end

function ScriptInputNode:execute(runtime)
    -- nothing to do, all the pins have already been written in ScriptRuntime:getRunner()
end

function ScriptInputNode:getLoadArguments()
    local loadArguments = {}
    for i = 1, #self.outputPins - 1 do
        local outputPin = self.outputPins[i]
        loadArguments[i] = {
            outputPin.pinType,
            outputPin.pinName
        }
    end
    return table.unpack(loadArguments)
end

function ScriptInputNode:load(...)
    self.pinsData = {...}
end

return ScriptInputNode