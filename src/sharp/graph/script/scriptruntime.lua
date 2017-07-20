local PinTypes = require 'pintypes'

local ScriptRuntime = {}
ScriptRuntime.__index = ScriptRuntime

function ScriptRuntime:new(graph)
    local outputPinValues = {}
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        outputPinValues[node] = {}
        for i, outputPin in pairs(node.outputPins) do
            if outputPin.pinType ~= PinTypes.IMPULSE then
                outputPinValues[node][outputPin] = { value = nil }
            end
        end
    end
    
    local inputPinValues = {}
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        inputPinValues[node] = {}
        for i, inputPin in pairs(node.inputPins) do
            if inputPin.pinType ~= PinTypes.IMPULSE then
                local pluggedOutputPin = inputPin.pluggedOutputPin
                local outputPin = pluggedOutputPin.outputPin
                local outputNode = pluggedOutputPin.node
                inputPinValues[node][inputPin] = outputPinValues[outputNode][outputPin]
            end
        end
    end

    local nodeRuntimes = {}

    local o = {
        graph = graph,
        nodeRuntimes = nodeRuntimes
    }

    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        local nodeInputPinValues = inputPinValues[node]
        local nodeOutputPinValues = outputPinValues[node]
        local scriptRuntimeType = node:getRuntimeType()
        nodeRuntimes[node] = scriptRuntimeType:new(o, node, nodeInputPinValues, nodeOutputPinValues)
    end

    return setmetatable(o, self)
end

function ScriptRuntime:getRunner()
    local entryNodes = self.graph.entryNodes
    local nodeRuntimes = self.nodeRuntimes
    return function()
        for i = 1, #entryNodes do
            local entryNode = entryNodes[i]
            local entryNodeRuntime = nodeRuntimes[entryNode]
            entryNode:execute(entryNodeRuntime)
        end
    end
end

function ScriptRuntime:getNodeRuntime(node)
    return self.nodeRuntimes[node]
end

return ScriptRuntime