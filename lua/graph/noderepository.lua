local NodeRepository = {}
NodeRepository.__index = NodeRepository

function NodeRepository:new(nodeType)
    local o = setmetatable({
        nodeType = nodeType,
        nodeClasses = {}
    }, self)
    -- all repositories include the common nodes
    o:load('common', flat.require)
    o:load(nodeType, flat.require)
    return o
end

function NodeRepository:load(nodeType, require)
    local nodePaths = require('graph/' .. nodeType .. '/' .. nodeType .. 'noderepository')
    for i = 1, #nodePaths do
        local nodePath = nodePaths[i]
        local nodeClass = require('graph/' .. nodeType .. '/nodes/' .. nodePath .. 'node')
        nodeClass.path = nodePath
        nodeClass.require = require
        self.nodeClasses[nodePath] = nodeClass
    end
end

function NodeRepository:getNodeClasses()
    return self.nodeClasses
end

function NodeRepository:importRepository(other)
    for nodePath, nodeClass in pairs(other.nodeClasses) do
        self.nodeClasses[nodePath] = nodeClass
    end
end

return NodeRepository