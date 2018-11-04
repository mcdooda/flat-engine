local PinTypes = flat.require 'graph/pintypes'

local ScriptRuntime = {}
ScriptRuntime.__index = ScriptRuntime

function ScriptRuntime:new(graph)
    local outputPinValues = {}
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        outputPinValues[node] = {}
        local outputPins = node.outputPins
        for i = 1, #outputPins do
            local outputPin = outputPins[i]
            if outputPin.pinType ~= PinTypes.IMPULSE and #outputPin.pluggedInputPins > 0 then
                outputPinValues[node][outputPin] = { value = nil }
            end
        end
    end
    
    local inputPinValues = {}
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        inputPinValues[node] = {}
        local inputPins = node.inputPins
        for i = 1, #inputPins do
            local inputPin = inputPins[i]
            if inputPin.pinType ~= PinTypes.IMPULSE then
                local pluggedOutputPin = inputPin.pluggedOutputPin
                if pluggedOutputPin then
                    local outputPin = pluggedOutputPin.outputPin
                    local outputNode = pluggedOutputPin.node
                    inputPinValues[node][inputPin] = assert(
                        outputPinValues[outputNode][outputPin],
                        'No output pin value for ' .. outputNode:getName() .. '->' .. outputPin.pinName
                    )
                end
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
        assert(node.getRuntimeType, 'Node ' .. node:getName() .. ' is not supported in scripts (getRuntimeType method missing)')
        local scriptRuntimeType = node:getRuntimeType()
        nodeRuntimes[node] = scriptRuntimeType:new(o, node, nodeInputPinValues, nodeOutputPinValues)
    end

    return setmetatable(o, self)
end

function ScriptRuntime:setContext(context)
    local contextNodes = self.graph:getContextNodes()
    for i = 1, #contextNodes do
        local contextNode = contextNodes[i]
        local contextNodeRuntime = self.nodeRuntimes[contextNode]
        contextNodeRuntime:writePin(contextNode.contextOutPin, context)
    end
end

function ScriptRuntime:writeInputPins(...)
    local numInputs = select('#', ...)
    if numInputs == 0 then
        return
    end

    local inputNode = self.graph.inputNode
    assert(inputNode, 'script has no input node but has ' .. numInputs .. ' input values')

    assert(numInputs == #inputNode.outputPins)
    local inputNodeRuntime = self.nodeRuntimes[inputNode]
    for i = 1, numInputs do
        local outputPin = inputNode.outputPins[i]
        if outputPin then
            local value = select(i, ...)
            inputNodeRuntime:writePin(outputPin, value)
        end
    end
end

function ScriptRuntime:readOutputPins()
    local outputNode = self.graph.outputNode
    if not outputNode then
        return
    end

    local outputNodeRuntime = self.nodeRuntimes[outputNode]
    local outputValues = {}
    local numOutputs = #outputNode.inputPins - 1 -- do not read the last pin
    for i = 1, numOutputs do
        local inputPin = outputNode.inputPins[i]
        assert(inputPin)
        outputValues[i] = outputNodeRuntime:readPin(inputPin)
    end
    return table.unpack(outputValues)
end

function ScriptRuntime:getRunner(...)
    self:writeInputPins(...)

    local entryNodes = self.graph.entryNodes
    local nodeRuntimes = self.nodeRuntimes
    return function()
        for i = 1, #entryNodes do
            local entryNode = entryNodes[i]
            local entryNodeRuntime = nodeRuntimes[entryNode]
            entryNode:execute(entryNodeRuntime)
        end
        return self:readOutputPins()
    end
end

function ScriptRuntime:getNodeRuntime(node)
    return self.nodeRuntimes[node]
end

return ScriptRuntime