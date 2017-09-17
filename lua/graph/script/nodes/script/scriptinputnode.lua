local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ScriptInputNode = FunctionalScriptNode:inherit 'Script Input'

function ScriptInputNode:getInitArguments()
    local initArguments = {}
    for i = 1, #self.outputPins do
        local outputPin = self.outputPins[i]
        initArguments[i] = {
            outputPin.pinType,
            outputPin.pinName
        }
    end
    return initArguments
end

function ScriptInputNode:addedToGraph(graph)
    graph.inputNode = self
end

function ScriptInputNode:buildPins()
    -- nothing to do, pins from this node are created manually from the outside
end

function ScriptInputNode:execute(runtime)
    -- nothing to do, all the pins have already been written in ScriptRuntime:getRunner()
end

function ScriptInputNode:init(...)
    for i = 1, select('#', ...) do
        local pin = select(i, ...)
        local pinType = pin[1]
        local pinName = pin[2]
        self:addOutputPin(pinType, pinName)
    end
end

return ScriptInputNode