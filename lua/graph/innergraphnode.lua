local Graph = flat.require 'graph/graph'
local Node = flat.require 'graph/node'

local InnerGraphNode = Node:inherit()

function InnerGraphNode:init()
    self.innerGraph = Graph:new()
end

function InnerGraphNode:getLoadArguments()
    return self.innerGraph:getDescription()
end

function InnerGraphNode:load(savedGraph)
    local innerGraph = self.innerGraph
    local loaded, err = pcall(function()
        innerGraph:load(savedGraph)
    end)
    if not loaded then
        print('Could not load inner graph in node ' .. self:getName() .. ': ' .. err)
        self.innerGraph = Graph:new() -- clear the graph to avoid unexpected errors in the editor
        return false
    end
    return true
end

return InnerGraphNode