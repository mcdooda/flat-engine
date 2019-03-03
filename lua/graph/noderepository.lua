local NodeRepository = {}
NodeRepository.__index = NodeRepository

function NodeRepository:new(nodeType)
    local o = setmetatable({
        nodeType = nodeType,
        nodeClasses = {},
        compounds = {}
    }, self)
    -- all repositories include the common nodes
    o:loadNodes('common', nodeType, flat.require)
    o:loadNodes(nodeType, nodeType, flat.require)
    return o
end

function NodeRepository:loadNodes(nodeTypeRepository, nodeType, require)
    local nodePaths = flat.safeRequire(require, 'graph/' .. nodeTypeRepository .. '/' .. nodeTypeRepository .. 'noderepository')
    if nodePaths then
        for i = 1, #nodePaths do
            local nodePath = nodePaths[i]
            local nodeClass = flat.safeRequire(require, 'graph/' .. nodeType .. '/nodes/' .. nodePath .. 'node')
            if not nodeClass then
                nodeClass = flat.safeRequire(require, 'graph/common/nodes/' .. nodePath .. 'node')
            end
            if nodeClass then
                nodeClass.path = nodePath
                nodeClass.require = require
                self.nodeClasses[nodePath] = nodeClass
            else
                flat.ui.error('Node ' .. nodePath .. ' from ' .. nodeTypeRepository .. ' does not exist')
            end
        end
    end
end

function NodeRepository:loadCompounds(nodeType, root)
    local compoundDirectory = flat.Directory(root)
    compoundDirectory:eachSubFileRecursive(function(file)
        if file:isRegularFile() and file:getFullExtension() == '.graph.lua' then
            local compoundName = file:getShortStem()
            local compoundPath = file:getParentPath() .. '/' .. compoundName
            self.compounds[#self.compounds + 1] = {
                name = compoundName,
                path = compoundPath
            }
        end
    end)
end

function NodeRepository:getNodeClasses()
    return self.nodeClasses
end

function NodeRepository:getCompounds()
    return self.compounds
end

function NodeRepository:importRepository(other)
    for nodePath, nodeClass in pairs(other.nodeClasses) do
        self.nodeClasses[nodePath] = nodeClass
    end
end

return NodeRepository