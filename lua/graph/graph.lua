local PinTypes = flat.require 'graph/pintypes'

local Graph = {}
Graph.__index = Graph

function Graph:new()
    local o = {
        nodeInstances = {},
        entryNodes = {}
    }
    return setmetatable(o, self)
end

function Graph:addNode(nodeClass, buildPins)
    local node = nodeClass:new()
    if buildPins == nil then
        node:buildPins()
    end
    local nodeIndex = #self.nodeInstances + 1
    self.nodeInstances[nodeIndex] = node
    node:addedToGraph(self)
    return node
end

function Graph:removeNode(node)
    node:unplugAllPins()
    local nodeIndex = assert(self:findNodeIndex(node))
    local numNodes = #self.nodeInstances
    self.nodeInstances[nodeIndex] = self.nodeInstances[numNodes]
    self.nodeInstances[numNodes] = nil
end

function Graph:findNodeIndex(node)
    for i = 1, #self.nodeInstances do
        if self.nodeInstances[i] == node then
            return i
        end
    end
end

function Graph:addEntryNode(node)
    self.entryNodes[#self.entryNodes + 1] = node
end

function Graph:loadGraph(graphPath)
    local env = { PinTypes = PinTypes }
    function env.__index(env, nodeType)
        local nodeClasses = flat.graph.getNodeClasses(nodeType)
        return function(savedGraph)
            self:load(nodeType, savedGraph, nodeClasses)
        end
    end

    setmetatable(env, env)

    loadfile(graphPath, 'bt', env)()
end

function Graph:load(nodeType, savedGraph, nodeRepository)
    self.nodeType = nodeType

    -- build nodes
    local nodes = savedGraph.nodes
    for i = 1, #nodes do
        local node = nodes[i]
        local nodeName = node.name
        local nodeInstance = self:addNode(nodeRepository[nodeName], false)
        local loadArguments = node.loadArguments
        if loadArguments then
            nodeInstance:load(table.unpack(loadArguments))
        else
            nodeInstance:load()
        end
        nodeInstance:buildPins()
    end

    -- connect nodes together
    local links = savedGraph.links
    for i = 1, #links do
        local link = links[i]

        local outputNodeIndex = link[1]
        local outputPinIndex = link[2]
        local outputNode = self.nodeInstances[outputNodeIndex]
        if not outputNode then
            error('No node #' .. tostring(outputPinIndex) .. ' in graph')
        end
        local outputPin = outputNode:getOutputPin(outputPinIndex)
        if not outputPin then
            error('No output pin #' .. tostring(outputPinIndex) .. ' in node #' .. outputNodeIndex)
        end

        local inputNodeIndex = link[3]
        local inputPinIndex = link[4]
        local inputNode = self.nodeInstances[inputNodeIndex]
        if not inputNode then
            error('No node #' .. tostring(inputNodeIndex) .. ' in graph')
        end
        local inputPin = assert(inputNode:getInputPin(inputPinIndex), 'no input pin for index ' .. inputPinIndex)
        if not inputPin then
            error('No input pin #' .. tostring(intputPinIndex) .. ' in node #' .. inputNodeIndex)
        end

        outputNode:plugPins(outputPin, inputNode, inputPin)
    end
end

function Graph:saveGraph(graphPath)
    local graphDescription = {
        nodes = {},
        links = {}
    }
    local nodes = self.nodeInstances
    for outputNodeIndex = 1, #nodes do
        local node = nodes[outputNodeIndex]

        -- nodes
        local nodeDescription = {
            name = node.path
        }
        local loadArguments = { node:getLoadArguments() }
        if #loadArguments > 0 then
            nodeDescription.loadArguments = loadArguments
        end
        graphDescription.nodes[outputNodeIndex] = nodeDescription

        -- links
        for outputPinIndex = 1, #node.outputPins do
            local outputPin = node.outputPins[outputPinIndex]
            for i = 1, #outputPin.pluggedInputPins do
                local pluggedInputPin = outputPin.pluggedInputPins[i]
                local inputPin = pluggedInputPin.inputPin
                local inputNode = pluggedInputPin.node
                local inputNodeIndex = self:findNodeIndex(inputNode)
                local inputPinIndex = inputNode:findInputPinIndex(inputPin)
                local linkDescription = {outputNodeIndex, outputPinIndex, inputNodeIndex, inputPinIndex}
                graphDescription.links[#graphDescription.links + 1] = linkDescription
            end
        end
    end

    local f = assert(io.open(graphPath, 'w'))
    f:write(self.nodeType)
    f:write ' '
    flat.dumpToOutput(f, graphDescription)
    f:close()
end

return Graph