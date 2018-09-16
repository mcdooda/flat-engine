local Graph = flat.require 'graph/graph'
local Node = flat.require 'graph/node'

local InnerGraphNode = Node:inherit()

function InnerGraphNode:init()
    self.innerGraph = Graph:new()
end

function InnerGraphNode:addedToGraph(graph)
    if self.subGraphId then
        graph:addSubGraphId(self.subGraphId)
    else
        self.subGraphId = graph:makeNewSubGraphId()
    end
end

function InnerGraphNode:getLoadArguments()
    local subGraphId = assert(self.subGraphId, 'Inner graph has no sub graph id')
    local graphDescription = self.innerGraph:getDescription()
    return subGraphId, graphDescription
end

function InnerGraphNode:load(subGraphId, savedGraph)
    local innerGraph = self.innerGraph
    local loaded, err = pcall(function()
        self.subGraphId = assert(subGraphId)
        innerGraph:load(savedGraph)
    end)
    if not loaded then
        print('Could not load inner graph in node ' .. self:getName() .. ': ' .. err)
        self.subGraphId = nil
        self.innerGraph = Graph:new() -- clear the graph to avoid unexpected errors in the editor
        self.innerGraph.nodeType = innerGraph.nodeType
        return false
    end
    return true
end

return InnerGraphNode