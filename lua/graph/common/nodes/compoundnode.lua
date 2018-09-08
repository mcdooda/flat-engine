local Graph = flat.require 'graph/graph'
local Node = flat.require 'graph/node'
local PinTypes = flat.require 'graph/pintypes'

local CompoundNode = Node:inherit()

function CompoundNode:addedToGraph(graph)
    graph:addCompound(self)
end

function CompoundNode:isBrowsable()
    return false
end

function CompoundNode:getLastInputPinIndex()
    local graph = self.graph
    local inputPins = graph.inputNode.outputPins
    for i = #inputPins, 1, -1 do
        local inputPin = inputPins[i]
        if inputPin.pinType ~= PinTypes.ANY then
            return i
        end
    end
    return 0
end

function CompoundNode:getLastOutputPinIndex()
    local graph = self.graph
    local outputPins = graph.outputNode.inputPins
    for i = #outputPins, 1, -1 do
        local outputPin = outputPins[i]
        if outputPin.pinType ~= PinTypes.ANY then
            return i
        end
    end
    return 0
end

function CompoundNode:buildPins()
    local graph = self.graph

    if graph.inputNode then
        local lastInputPinIndex = self:getLastInputPinIndex()
        local inputPins = graph.inputNode.outputPins
        for i = 1, lastInputPinIndex do
            local inputPin = inputPins[i]
            self:addInputPin(inputPin.pinType, inputPin.pinName)
        end
    end

    if graph.outputNode then
        local outputPins = graph.outputNode.inputPins
        local lastOutputPinIndex = self:getLastOutputPinIndex()
        for i = 1, lastOutputPinIndex do
            local outputPin = outputPins[i]
            self:addOutputPin(outputPin.pinType, outputPin.pinName)
        end
    end
end

function CompoundNode:getLoadArguments()
    return self.graphPath
end

function CompoundNode:load(graphPath)
    local graph = Graph:new()
    local loaded, err = pcall(function()
        graph:loadGraph(graphPath .. '.graph.lua')
    end)
    if not loaded then
        print('Could not load compound \'' .. graphPath .. '\': ' .. err)
        self.name = '[C] (Loading error)'
        return false
    end
    self.graph = graph
    self.graphPath = graphPath
    self.name = self:getCompoundName(graphPath)
    return true
end

function CompoundNode:getGraphPath()
    return assert(self.graphPath)
end

function CompoundNode:getCompoundName(graphPath)
    return '[C] ' .. string.gsub(graphPath, '.+/([^/]-)$', '%1')
end

function CompoundNode:resolveSubCompounds()
    self.graph:resolveCompounds()
end

function CompoundNode:resolve(parentGraph)
    local graph = assert(self.graph)

    -- will resolve the PARENT graph, the current graph is already resolved
    assert(graph.compounds == nil, 'The compound\'s graph should already be resolved')

    local lastInputPinIndex = self:getLastInputPinIndex()
    local lastOutputPinIndex = self:getLastOutputPinIndex()

    local inputNode = graph.inputNode
    local outputNode = graph.outputNode

    -- 1. save links plugged to the compound in the parent graph
    local compoundInputPins = {}
    for i = 1, lastInputPinIndex do
        local compoundInputPin = self.inputPins[i]
        compoundInputPins[i] = {
            pinType = compoundInputPin.pinType,
            pinName = compoundInputPin.pinName,
            pluggedOutputPin = compoundInputPin.pluggedOutputPin,
            -- onPlugged nil in compounds
        }
    end

    local compoundOutputPins = {}
    for i = 1, lastOutputPinIndex do
        local compoundOutputPin = self.outputPins[i]
        -- we have to copy pluggedInputPins as the table will be cleared when removing the compound from the parent graph
        local pluggedInputPins = {}
        for j = 1, #compoundOutputPin.pluggedInputPins do
            pluggedInputPins[j] = compoundOutputPin.pluggedInputPins[j]
        end
        compoundOutputPins[i] = {
            pinType = compoundOutputPin.pinType,
            pinName = compoundOutputPin.pinName,
            pluggedInputPins = pluggedInputPins,
            -- onPlugged nil in compounds
        }
    end

    -- 2. remove compound from parent graph
    parentGraph:removeNode(self)

    -- 3. save links plugged to the input and output nodes inside the compound graph
    local subGraphInputNodeOutputPins = {}
    if inputNode then
        for i = 1, lastInputPinIndex do
            local subGraphInputNodeOutputPin = inputNode.outputPins[i]
            subGraphInputNodeOutputPins[i] = {
                pinType = subGraphInputNodeOutputPin.pinType,
                pinName = subGraphInputNodeOutputPin.pinName,
                pluggedInputPins = subGraphInputNodeOutputPin.pluggedInputPins,
                -- onPlugged nil in compounds
            }
        end
    end

    local subGraphOutputNodeInputPins = {}
    if outputNode then
        for i = 1, lastOutputPinIndex do
            local subGraphOutputNodeInputPin = outputNode.inputPins[i]
            subGraphOutputNodeInputPins[i] = {
                pinType = subGraphOutputNodeInputPin.pinType,
                pinName = subGraphOutputNodeInputPin.pinName,
                pluggedOutputPin = subGraphOutputNodeInputPin.pluggedOutputPin,
                -- onPlugged nil in compounds
            }
        end
    end

    -- 4. clone sub graph nodes into the parent graph (except the input and output nodes)
    local clones = {}
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        if node ~= inputNode and node ~= outputNode then
            local clone = node:clone()
            clones[node] = clone
            parentGraph:addNodeInstance(clone)
        end
    end

    -- 5. restore links between clones
    for node, clone in pairs(clones) do
        for i = 1, #node.outputPins do
            local outputPin = node.outputPins[i]
            local cloneOutputPin = assert(clone.outputPins[i])

            local pluggedInputPins = outputPin.pluggedInputPins
            for j = 1, #pluggedInputPins do
                local pluggedInputPin = pluggedInputPins[j]

                if pluggedInputPin.node ~= outputNode then
                    local inputNode = assert(clones[pluggedInputPin.node])
                    local inputPinIndex = assert(pluggedInputPin.node:findInputPinIndex(pluggedInputPin.inputPin))
                    local inputPin = assert(inputNode.inputPins[inputPinIndex])
                    clone:plugPins(cloneOutputPin, inputNode, inputPin)
                end
            end
        end
    end

    -- 6. plug input and output links
    if inputNode then
        for i = 1, lastInputPinIndex do
            local compoundInputPin = compoundInputPins[i]
            
            local pluggedOutputPin = compoundInputPin.pluggedOutputPin
            if pluggedOutputPin then
                -- node from the parent graph
                local outputNode = pluggedOutputPin.node
                local outputPin = pluggedOutputPin.outputPin
                
                local subGraphInputNodeOutputPin = subGraphInputNodeOutputPins[i]
                local pluggedInputPins = subGraphInputNodeOutputPin.pluggedInputPins
                for j = 1, #pluggedInputPins do
                    local pluggedInputPin = pluggedInputPins[j]

                    -- clone from the compound
                    local inputNode = assert(clones[pluggedInputPin.node])
                    local inputPinIndex = assert(pluggedInputPin.node:findInputPinIndex(pluggedInputPin.inputPin))
                    local inputPin = assert(inputNode.inputPins[inputPinIndex])

                    -- plug the original parent graph node to the clone
                    outputNode:plugPins(outputPin, inputNode, inputPin)
                end
            end
        end
    end

    if outputNode then
        for i = 1, lastOutputPinIndex do
            local subGraphOutputNodeInputPin = subGraphOutputNodeInputPins[i]

            -- clone from the compound
            local outputNode = clones[subGraphOutputNodeInputPin.pluggedOutputPin.node]
            local outputPinIndex = subGraphOutputNodeInputPin.pluggedOutputPin.node:findOutputPinIndex(subGraphOutputNodeInputPin.pluggedOutputPin.outputPin)
            local outputPin = outputNode.outputPins[outputPinIndex]

            local compoundOutputPin = compoundOutputPins[i]
            local pluggedInputPins = compoundOutputPin.pluggedInputPins
            for j = 1, #pluggedInputPins do
                local pluggedInputPin = pluggedInputPins[j]

                -- node from the parent graph
                local inputNode = pluggedInputPin.node
                local inputPin = pluggedInputPin.inputPin

                -- plug the clone to the original parent graph node
                outputNode:plugPins(outputPin, inputNode, inputPin)
            end
        end
    end

    -- will break if resolve is called again
    self.graph = nil
end

return CompoundNode