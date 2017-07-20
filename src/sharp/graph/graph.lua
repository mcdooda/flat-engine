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

return Graph