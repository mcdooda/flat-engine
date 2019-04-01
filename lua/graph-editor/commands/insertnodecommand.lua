local Command = flat.require 'history/command'

local InsertNodeCommand = Command:inherit 'Insert node'

function InsertNodeCommand:new(mainWindow, nodeEntry, x, y)
	local o = setmetatable({ 
		mainWindow = mainWindow,
	 	nodeEntry  = nodeEntry,
	 	node 	   = nil,
	 	x 		   = x,
	 	y 		   = y,
 	}, self)
	return o
end

function InsertNodeCommand:executeDo()
	assert(self.mainWindow:layoutSanityCheck())
    local foldedNodes = self.mainWindow:getFoldedNodes() -- this must be called before adding the new node to the graph!
    local content = self.mainWindow:getContent()
    local graphInfo = self.mainWindow.currentGraphInfo
    local graph = graphInfo.graph
    local nodeClasses = flat.graph.getNodeClasses(graph.nodeType)
    if self.nodeEntry.nodeName then
        self.node = graph:addNode(nodeClasses[self.nodeEntry.nodeName])
    else
        assert(self.nodeEntry.compoundPath)
        self.node = graph:addNode(nodeClasses['compound'], false)
        if self.node:load(self.nodeEntry.compoundPath) then
            self.node:buildPins()
        else
            flat.ui.error('Cound not load compound ' .. compoundPath)
            return
        end
    end
    local nodeIndex = graph:findNodeIndex(self.node)
    local contentSizeX, contentSizeY = content:getComputedSize()
    local scrollX, scrollY = content:getScrollPosition()
    local nodeWidgetX = self.x + scrollX
    local nodeWidgetY = self.y + scrollY - contentSizeY -- move the relative position from bottom left to top left
    assert(not graphInfo.layout[nodeIndex])
    graphInfo.layout[nodeIndex] = {nodeWidgetX, nodeWidgetY}
    local nodeWidget = self.mainWindow:makeNodeWidget(self.node, foldedNodes)
    nodeWidget:setVisiblePosition(nodeWidgetX, nodeWidgetY)
    content:addChild(nodeWidget:getContainer())
    assert(self.mainWindow:layoutSanityCheck())
end
		
function InsertNodeCommand:executeUndo()
	assert(self.mainWindow:layoutSanityCheck())

    local graphInfo = self.mainWindow.currentGraphInfo
    local graph = graphInfo.graph
    local layout = graphInfo.layout
    local nodeWidgets = self.mainWindow.currentGraphInfo.nodeWidgets
    local node = self.node

    -- delete inner graph
    if node.subGraphId then
        layout.subGraphLayouts[node.subGraphId] = nil
        self.mainWindow:closeSubGraphIfOpen(node.subGraphId)
    end

    -- delete connected folded nodes
    for i = 1, #node.inputPins do
        local inputPin = node.inputPins[i]
        if inputPin.pluggedOutputPin then
            local outputNode = inputPin.pluggedOutputPin.node
            if not nodeWidgets[outputNode] then
                assert(outputNode:isConstant())
                local nodeCount = #graph:getNodes()
                local nodeIndex = graph:removeNode(outputNode)
                layout[nodeIndex] = layout[nodeCount]
                layout[nodeCount] = nil
            end
        end
    end

    assert(self.mainWindow:layoutSanityCheck())

    -- we cannot use flat.arrayRemoveIndexCyclic(layout, nodeIndex) as layout is not a valid array (contains holes)
    local nodeIndex = graph:findNodeIndex(node)
    local nodeCount = #graph:getNodes()
    layout[nodeIndex] = layout[nodeCount]
    layout[nodeCount] = nil

    -- delete the node widget
    local nodeWidget = nodeWidgets[node]
    if nodeWidget then
        nodeWidgets[node] = nil
        nodeWidget:delete()
    else
        -- hide the folded node in the pin
        assert(node:isConstant(), 'Node has no widget but is not constant')
        assert(#node.outputPins == 1)
        local outputPin = assert(node.outputPins[1])

        -- the node might have already been unplugged from outside
        assert(#outputPin.pluggedInputPins <= 1, "A folded constant node must be connected to 0 or 1 node, " .. tostring(#outputPin.pluggedInputPins) .. " connected")
        local pluggedInputPin = outputPin.pluggedInputPins[1]
        if pluggedInputPin then
            local inputNode = assert(pluggedInputPin.node)
            local inputPin = assert(pluggedInputPin.inputPin)
            local inputNodeWidget = assert(nodeWidgets[inputNode])
            inputNodeWidget:hideFoldedConstantNode(inputPin)
        end
    end

    -- delete the actual node
    graph:removeNode(node)

    assert(self.mainWindow:layoutSanityCheck())
end

return InsertNodeCommand