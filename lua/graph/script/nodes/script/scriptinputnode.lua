local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ScriptInputNode = FunctionalScriptNode:inherit()

function ScriptInputNode:getName()
    return 'Script Input'
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