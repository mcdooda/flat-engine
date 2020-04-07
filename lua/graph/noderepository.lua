local NodeRepository = {}
NodeRepository.__index = NodeRepository

function NodeRepository:new(nodeType)
    local o = setmetatable({
        nodeType = nodeType,
        nodeClasses = {},
        compounds = {}
    }, self)
    return o
end

function NodeRepository:loadNodes(nodeTypeRepository, nodeType, require)
    local nodePaths = flat.safeRequire(require, 'graph/' .. nodeTypeRepository .. '/' .. nodeTypeRepository .. 'noderepository')
    if nodePaths then
        for i = 1, #nodePaths do
            local nodePath = nodePaths[i]
            local graphSpecificNodePath = 'graph/' .. nodeType .. '/nodes/' .. nodePath .. 'node'
            local commonNodePath = 'graph/common/nodes/' .. nodePath .. 'node'
            local nodeClass = flat.safeRequire(require, graphSpecificNodePath)

            if not nodeClass then
                nodeClass = flat.safeRequire(require, commonNodePath)
            end
            if nodeClass then
                nodeClass.path = nodePath
                nodeClass.require = require
                self.nodeClasses[nodePath] = nodeClass
            else
                flat.ui.error('Node ' .. nodePath .. ' from ' .. nodeTypeRepository .. ' does not exist (tried ' .. graphSpecificNodePath .. ' and ' .. commonNodePath .. ')')
            end
        end
    end
end

function NodeRepository:loadCompounds(nodeType, root)
    local compoundDirectory = flat.Directory(root)
    if compoundDirectory then
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
end

function NodeRepository:getNodeClasses()
    assert(next(self.nodeClasses), 'No node loaded for \'' .. self.nodeType .. '\' graphs')
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