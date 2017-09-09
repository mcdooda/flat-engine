local NodeRepository = flat.require 'graph/noderepository'

local Graph = {}
Graph.__index = Graph

function Graph:new()
    local o = {
        nodeInstances = {},
        entryNodes = {}
    }
    return setmetatable(o, self)
end

function Graph:addNode(nodeType)
    local node = nodeType:new()
    local nodeIndex = #self.nodeInstances + 1
    self.nodeInstances[nodeIndex] = node
    node:addedToGraph(self)
    return node
end

function Graph:addEntryNode(node)
    self.entryNodes[#self.entryNodes + 1] = node
end

function Graph:loadGraph(graphPath)
    local PinTypes = flat.require 'graph/pintypes'

    local env = { PinTypes = PinTypes }
    function env.__index(env, nodeType)
        local nodeRegistry = NodeRepository:getNodesForType(nodeType)
        return function(savedGraph)
            self:load(nodeType, savedGraph, nodeRegistry)
        end
    end

    setmetatable(env, env)

    loadfile(graphPath, 'bt', env)()
end

function Graph:load(nodeType, savedGraph, nodeRegistry)
    self.nodeType = nodeType

    -- build nodes
    local nodes = savedGraph.nodes
    for i = 1, #nodes do
        local node = nodes[i]
        if type(node) == 'table' then
            local nodeName = node[1]
            local nodeInstance = self:addNode(nodeRegistry[nodeName])
            nodeInstance:init(select(2, table.unpack(node)))
        else
            local nodeName = node
            local nodeInstance = self:addNode(nodeRegistry[nodeName])
            nodeInstance:init()
        end
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

return Graph