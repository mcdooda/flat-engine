local Node = flat.require 'graph/node'

local InnerGraphNode = Node:inherit()

function InnerGraphNode:getLoadArguments()
    return InnerGraphNode.getLoadArguments(self)
end

function InnerGraphNode:load(graphPath)
    InnerGraphNode.load(self, graphPath)
end

return InnerGraphNode