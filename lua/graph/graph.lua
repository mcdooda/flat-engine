local PinTypes = flat.require 'graph/pintypes'
local MissingNode = flat.require 'graph/missingnode'

local max = math.max

local Graph = {}
Graph.__index = Graph

function Graph:new()
    local o = {
        nodeInstances = {},
        entryNodes = {},
        contextNodes = {},
        contextType = PinTypes.ANY,
        compounds = {},
        reroutes = {},
        subGraphIds = {},
        graphOrigin = debug.traceback()
    }
    return setmetatable(o, self)
end

function Graph:addNode(nodeClass, buildPins)
    assert(nodeClass)
    local node = nodeClass:new()
    if buildPins == nil then
        node:buildPins()
    end
    flat.arrayAdd(self.nodeInstances, node)
    node:addedToGraph(self)
    return node
end

function Graph:addNodeInstance(node)
    flat.arrayAdd(self.nodeInstances, node)
    node:addedToGraph(self)
end

function Graph:addMissingNode(nodeName)
    -- special node type when a node class cannot be found
    local node = MissingNode:new(nodeName)
    flat.arrayAdd(self.nodeInstances, node)
    node:addedToGraph(self)
    return node
end

function Graph:removeNode(node)
    node:unplugAllPins()
    local index = flat.arrayRemoveValueCyclic(self.nodeInstances, node)
    node:removedFromGraph(self)
    return index
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
    flat.arrayAdd(self.entryNodes, node)
end

function Graph:addContextNode(node)
    flat.arrayAdd(self.contextNodes, node)
end

function Graph:getContextNodes()
    return self.contextNodes
end

function Graph:setContextType(contextType, isLoadingGraph)
    self.contextType = contextType
    local nodeInstances = self.nodeInstances
    for i = 1, #nodeInstances do
        local nodeInstance = nodeInstances[i]
        nodeInstance:setContextType(contextType, isLoadingGraph)
    end
end

function Graph:getContextType()
    return self.contextType
end

function Graph:addCompound(node)
    flat.arrayAdd(self.compounds, node)
end

function Graph:addReroute(node)
    flat.arrayAdd(self.reroutes, node)
end

function Graph:loadGraphFromFile(graphPath)
    local graphFile = io.open(graphPath, 'r')
    if not graphFile then
        return false
    end
    local graphFileAsString = graphFile:read '*all'
    graphFile:close()
    local graphDescription = load(graphFileAsString)()
    local graphErrors = self:load(graphDescription)
    self.graphOrigin = 'File: ' .. graphPath
    return graphErrors
end

function Graph:load(graphDescription)
    assert(graphDescription, 'Trying to load a nil graph description')
    local nodeType = assert(graphDescription.nodeType, 'Graph has no type')
    local nodeClasses = assert(flat.graph.getNodeClasses(nodeType))

    self.nodeType = nodeType

    local errors = {}
    local function addError(message)
        flat.arrayAdd(errors, message)
    end

    -- build nodes
    local nodes = graphDescription.nodes
    for i = 1, #nodes do
        local node = nodes[i]
        local nodeName = node.name
        local nodeInstance
        local nodeClass = nodeClasses[nodeName]
        if nodeClass then
            nodeInstance = self:addNode(nodeClass, false)
        else
            addError('Node ' .. nodeName .. ' does not exist or is not registered')
            nodeInstance = self:addMissingNode(nodeName)
        end
        local loadArguments = node.loadArguments
        if loadArguments then
            nodeInstance:load(table.unpack(loadArguments))
        else
            nodeInstance:load()
        end
        nodeInstance:buildPins()
    end

    -- connect nodes together
    local links = graphDescription.links
    for i = 1, #links do
        local link = links[i]

        local outputNodeIndex = link[1]
        local outputPinIndex = link[2]
        local outputNode = assert(self.nodeInstances[outputNodeIndex], 'No node #' .. tostring(outputNodeIndex) .. ' in graph')
        local outputPin = assert(outputNode:getOutputPin(outputPinIndex), 'No output pin #' .. tostring(outputPinIndex) .. ' in node #' .. outputNodeIndex)

        local inputNodeIndex = link[3]
        local inputPinIndex = link[4]
        local inputNode = assert(self.nodeInstances[inputNodeIndex], 'No node #' .. tostring(inputNodeIndex) .. ' in graph')
        local inputPin = assert(inputNode:getInputPin(inputPinIndex), 'No input pin #' .. tostring(inputPinIndex) .. ' in node #' .. inputNodeIndex .. ' (' .. inputNode:getName() .. ')')

        outputNode:plugPins(outputPin, inputNode, inputPin, nil, true)
    end

    self:postLoad()

    return errors
end

function Graph:postLoad()
    for i = 1, #self.nodeInstances do
        self.nodeInstances[i]:postLoad()
    end
end

function Graph:resolveCompoundsAndReroutes()
    self:resolveCompounds()
    self:resolveReroutes()
end

function Graph:resolveCompounds()
    -- see CompoundNode:resolve() for the graph resolution logic

    -- can be empty is there is no compound in the graph (it will do nothing), but not nil
    assert(self.compounds, 'Compounds already resolved!')

    local compounds = self.compounds
    for i = 1, #compounds do
        local compound = compounds[i]
        compound:resolveSubCompounds()
        compound:resolve(self)
    end

    -- avoid calling resolveCompounds again
    self.compounds = nil
end

function Graph:resolveReroutes()
    -- can be empty is there is no reroute in the graph (it will do nothing), but not nil
    assert(self.reroutes, 'Reroutes already resolved!')

    -- 1. unplug output links and plug them into the input pin's output pin instead
    for i = 1, #self.reroutes do
        local reroute = self.reroutes[i]
        local outputPin = reroute.inPin.pluggedOutputPin
        if outputPin then
            reroute:unplugInputPin(reroute.inPin)
            local pluggedInputPins = reroute.outPin.pluggedInputPins
            local inputPinsToReplug = {}
            -- copy the input pins array to iterate over it without worrying about the indices when they are unplugged
            for j = 1, #pluggedInputPins do
                local inputPin = pluggedInputPins[j]
                inputPinsToReplug[j] = {
                    inputPin = inputPin.inputPin,
                    node = inputPin.node
                }
            end
            for j = 1, #inputPinsToReplug do
                local inputPin = inputPinsToReplug[j]
                assert(inputPin.node:findInputPinIndex(inputPin.inputPin))
                inputPin.node:unplugInputPin(inputPin.inputPin, true)
                assert(inputPin.node:findInputPinIndex(inputPin.inputPin))
                outputPin.node:plugPins(outputPin.outputPin, inputPin.node, inputPin.inputPin, true, true)
            end
        else
            reroute:unplugAllOutputPins()
        end
    end

    -- 2. remove the reroute nodes from the graph
    for i = 1, #self.reroutes do
        local reroute = self.reroutes[i]
        self:removeNode(reroute)
    end

    -- avoid calling resolveReroutes again
    self.reroutes = nil
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
                flat.arrayAdd(graphDescription.links, linkDescription)
            end
        end
    end
    return graphDescription
end

function Graph:clone()
    local graphDescription = self:getDescription()
    local clone = getmetatable(self):new()
    clone:load(graphDescription)
    clone.graphOrigin = self.graphOrigin .. ' (clone)'
    assert(flat.dumpToString(graphDescription) == flat.dumpToString(clone:getDescription()))
    return clone
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

function Graph:validate()
    local graphErrors = {}
    for i = 1, #self.nodeInstances do
        local nodeInstance = self.nodeInstances[i]
        local nodeErrors = nodeInstance:validate()
        for j = 1, #nodeErrors do
            local nodeError = nodeErrors[j]
            
            if not nodeError.path then
                nodeError.path = {
                    {
                        graph = self,
                        nodeIndex = i
                    }
                }
            else
                table.insert(nodeError.path, 1, {
                    graph = self,
                    nodeIndex = i
                })
            end

            graphErrors[#graphErrors + 1] = nodeError
        end
    end
    return graphErrors
end

function Graph:makeNewSubGraphId()
    local highestSubGraphId = 0
    for subGraphId in pairs(self.subGraphIds) do
        highestSubGraphId = max(subGraphId, highestSubGraphId)
    end
    local newSubGraphId = highestSubGraphId + 1
    self:addSubGraphId(newSubGraphId)
    return highestSubGraphId + 1
end

function Graph:addSubGraphId(subGraphId)
    assert(not self.subGraphIds[subGraphId])
    self.subGraphIds[subGraphId] = true
end

function Graph:removeSubGraphId(subGraphId)
    assert(self.subGraphIds[subGraphId], 'Sub graph id ' .. tostring(subGraphId) .. ' is not part of the graph')
    self.subGraphIds[subGraphId] = nil
end

return Graph