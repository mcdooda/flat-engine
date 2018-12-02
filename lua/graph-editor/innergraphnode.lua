local NodeEditor = flat.require 'graph-editor/nodeeditor'

local InnerGraphNodeEditor = NodeEditor:inherit()

function InnerGraphNodeEditor:build(innerGraphNode, nodeWidget, pinsWidget)
    nodeWidget.nodeNameContainer:rightClick(function()
        local mainWindow = nodeWidget.mainWindow
        local graphInfo = mainWindow:getCurrentGraphInfo()
        nodeWidget.mainWindow:openSubGraph(innerGraphNode.innerGraph, innerGraphNode.subGraphId, graphInfo)
        return true
    end)
    return false
end

return InnerGraphNodeEditor