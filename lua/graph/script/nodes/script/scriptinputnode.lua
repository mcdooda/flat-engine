local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ScriptInputNode = FunctionalScriptNode:inherit()

function ScriptInputNode:addedToGraph(graph)
    graph.inputNode = self
end

function ScriptInputNode:buildPins()
    -- nothing to do, pins from this node are created manually
end

function ScriptInputNode:execute(runtime)
    -- nothing to do, all the pins have already been written in ScriptRuntime:getRunner()
end

function ScriptInputNode:init(...)
    for i = 1, select('#', ...) do
        local pinType = select(i, ...)
        self:addOutputPin(pinType)
    end
end

return ScriptInputNode