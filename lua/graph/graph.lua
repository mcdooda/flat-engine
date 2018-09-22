local PinTypes = flat.require 'graph/pintypes'

local Graph = {}
Graph.__index = Graph

function Graph:new()
    local o = {
        nodeInstances = {},
        entryNodes = {},
        compounds = {},
        subGraphIds = {}
    }
    return setmetatable(o, self)
end

function Graph:addNode(nodeClass, buildPins)
    assert(nodeClass)
    local node = nodeClass:new()
    if buildPins == nil then
        node:buildPins()
    end
    local nodeIndex = #self.nodeInstances + 1
    self.nodeInstances[nodeIndex] = node
    node:addedToGraph(self)
    return node
end

function Graph:addNodeInstance(node)
    local nodeIndex = #self.nodeInstances + 1
    self.nodeInstances[nodeIndex] = node
    node:addedToGraph(self)
end

function Graph:removeNode(node)
    node:unplugAllPins()
    local nodeIndex = assert(self:findNodeIndex(node))
    local numNodes = #self.nodeInstances
    self.nodeInstances[nodeIndex] = self.nodeInstances[numNodes]
    self.nodeInstances[numNodes] = nil
    node:removedFromGraph(self)
end

function Graph:getNodes()
    return self.nodeInstances
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

function Graph:addCompound(node)
    self.compounds[#self.compounds + 1] = node
end

function Graph:loadGraphFromFile(graphPath)
    local ok, err = pcall(function()
        local savedGraph = dofile(graphPath)
        assert(savedGraph, 'no return value!')
        self:load(savedGraph)
    end)
    if not ok then
        ok, err = pcall(function()
            -- fallback to the old format
            local env = { PinTypes = PinTypes }
            function env.__index(env, nodeType)
                return function(savedGraph)
                    savedGraph.nodeType = nodeType
                    self:load(savedGraph)
                end
            end
        
            setmetatable(env, env)
        
            assert(loadfile(graphPath, 'bt', env))()
        end)
        if not ok then
            error('Could not load graph ' .. graphPath)
        end
    end
end

function Graph:load(savedGraph)
    assert(savedGraph, 'Trying to load a nil graph')
    local nodeType = savedGraph.nodeType
    local nodeClasses = assert(flat.graph.getNodeClasses(nodeType))

    self.nodeType = nodeType

    -- build nodes
    local nodes = savedGraph.nodes
    for i = 1, #nodes do
        local node = nodes[i]
        local nodeName = node.name
        local nodeClass = assert(nodeClasses[nodeName], 'Node ' .. nodeName .. ' does not exist or is not registered')
        local nodeInstance = self:addNode(nodeClass, false)
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
        local inputPin = assert(inputNode:getInputPin(inputPinIndex), 'no input pin for index ' .. tostring(inputPinIndex))
        if not inputPin then
            error('No input pin #' .. tostring(intputPinIndex) .. ' in node #' .. inputNodeIndex)
        end

        outputNode:plugPins(outputPin, inputNode, inputPin, nil, true)
    end
end

function Graph:resolveCompounds()
    assert(self.compounds) -- can be empty for now (it will do nothing), but not nil
    local compounds = self.compounds
    for i = 1, #compounds do
        local compound = compounds[i]
        compound:resolveSubCompounds()
        compound:resolve(self)
    end
    self.compounds = nil -- avoid calling resolveCompounds again
end

function Graph:getDescription()
    local graphDescription = {
        nodeType = self.nodeType,
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
    return graphDescription
end

function Graph:toString()
    local graphDescription = self:getDescription()
    return 'return ' .. flat.dumpToString(graphDescription)
end

function Graph:saveGraph(graphPath)
    local graphDescription = self:getDescription()
    local f = assert(io.open(graphPath, 'w'))
    f:write 'return '
    flat.dumpToOutput(f, graphDescription)
    f:close()
end

function Graph:makeNewSubGraphId()
    local newSubGraphId = #self.subGraphIds + 1
    self.subGraphIds[newSubGraphId] = true
    return newSubGraphId
end

function Graph:addSubGraphId(subGraphId)
    assert(not self.subGraphIds[subGraphId])
    self.subGraphIds[subGraphId] = true
end

function Graph:removeSubGraphId(subGraphId)
    assert(self.subGraphIds[subGraphId])
    self.subGraphIds[subGraphId] = nil
end

return Graph