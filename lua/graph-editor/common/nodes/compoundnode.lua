local CompoundNodeEditor = {}

function CompoundNodeEditor.build(compoundNode, nodeWidget, pinsWidget)
    nodeWidget.nodeNameContainer:rightClick(function()
        local mainWindow = nodeWidget.mainWindow
        local subGraphPath = compoundNode:getGraphPath()
        local graphInfo = mainWindow:getCurrentGraphInfo()
        local nodeType = graphInfo.graph.nodeType
        nodeWidget.mainWindow:openGraph(subGraphPath, nodeType, nil)
        return true
    end)
    return false
end

return CompoundNodeEditor