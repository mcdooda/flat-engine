local NodeRepository = {}

function NodeRepository:load(nodeType)
    local nodePaths = flat.require('graph/' .. nodeType .. '/' .. nodeType .. 'noderegistry')
    local registry = {}
    for i = 1, #nodePaths do
        local nodePath = nodePaths[i]
        local nodeClass = flat.require('graph/' .. nodeType .. '/nodes/' .. nodePath .. 'node')
        nodeClass.path = nodePath
        registry[nodePath] = nodeClass
    end
    return registry
end

function NodeRepository:getNodesForType(nodeType)
    local registry = self[nodeType]
    if not registry then
        registry = self:load(nodeType)
        self[nodeType] = registry
    end
    return registry
end

return NodeRepository