local NodeRepository = {}

function NodeRepository:load(nodeType)
    local nodes = flat.require('graph/' .. nodeType .. '/' .. nodeType .. 'noderegistry')
    local registry = {}
    for nodeName, nodePath in pairs(nodes) do
        registry[nodeName] = flat.require('graph/' .. nodeType .. '/nodes/' .. nodePath .. 'node')
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