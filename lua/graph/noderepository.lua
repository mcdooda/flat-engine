local NodeRepository = {}
NodeRepository.__index = NodeRepository

function NodeRepository:new(nodeType)
    local o = setmetatable({
        nodeType = nodeType,
        nodeClasses = {}
    }, self)
    -- all repositories include the common nodes
    o:load('common', nodeType, flat.require)
    o:load(nodeType, nodeType, flat.require)
    return o
end

function NodeRepository:load(nodeTypeRepository, nodeType, require)
    local repositoryFound, nodePaths = pcall(function()
        return require('graph/' .. nodeTypeRepository .. '/' .. nodeTypeRepository .. 'noderepository')
    end)
    if repositoryFound then
        for i = 1, #nodePaths do
            local nodePath = nodePaths[i]
            local nodeClass
            pcall(function()
                nodeClass = require('graph/' .. nodeType .. '/nodes/' .. nodePath .. 'node')
            end)
            if not nodeClass then
                pcall(function()
                    nodeClass = require('graph/common/nodes/' .. nodePath .. 'node')
                end)
            end
            if nodeClass then
                nodeClass.path = nodePath
                nodeClass.require = require
                self.nodeClasses[nodePath] = nodeClass
            else
                print('Node ' .. nodePath .. ' from ' .. nodeTypeRepository .. ' does not exist')
            end
        end
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