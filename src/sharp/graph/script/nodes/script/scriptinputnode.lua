local FunctionalScriptNode = require 'script/functionalscriptnode'
local PinTypes = require 'pintypes'

local ScriptInputNode = FunctionalScriptNode:inherit()

function ScriptInputNode:addedToGraph(graph)
    graph.inputNode = self
end

function ScriptInputNode:buildPins()
    -- nothing to do, pins from this node are created manually
end

function ScriptInputNode:execute(runtime)
    -- nothing to do, all the pins have already been written
end

return ScriptInputNode