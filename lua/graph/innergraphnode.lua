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

function InnerGraphNode:removedFromGraph(graph)
    graph:removeSubGraphId(self.subGraphId)
    self.subGraphId = nil
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
        innerGraph.graphOrigin = self:getName() .. ' inner graph #' .. subGraphId
    end)
    if not loaded then
        flat.ui.error('Could not load inner graph in node ' .. self:getName() .. ': ' .. err)
        self.subGraphId = nil
        self.innerGraph = Graph:new() -- clear the graph to avoid unexpected errors in the editor
        self.innerGraph.nodeType = innerGraph.nodeType
        self.innerGraph.parentNode = self
        self.innerGraph.graphOrigin = 'Failed to load inner graph #' .. subGraphId
        return false
    end
    return true
end

function InnerGraphNode:validate()
    assert(self.subGraphId)
    local errors = Node.validate(self)
    local innerGraphErrors = self:validateInnerGraph()
    flat.arrayAppend(errors, innerGraphErrors)
    return errors
end

function InnerGraphNode:validateInnerGraph()
    local errors = self.innerGraph:validate()
    for i = 1, #errors do
        local error = errors[i]
        if error.path[1].graph == self.innerGraph then
            error.path[1].subGraphId = self.subGraphId
        end
    end
    return errors
end

return InnerGraphNode