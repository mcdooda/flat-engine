local NodeRepository = flat.require 'graph/noderepository'

flat.graph = {}

local repositories = {}
local function getNodeRepository(nodeType)
    local repository = repositories[nodeType]
    if not repository then
        repository = NodeRepository:new(nodeType)
        repositories[nodeType] = repository
    end
    return repository
end

function flat.graph.getNodeClasses(nodeType)
    local repository = getNodeRepository(nodeType)
    return repository:getNodeClasses()
end

function flat.graph.loadNodeClasses(nodeType, root)
    -- loads flat nodes
    local repository = getNodeRepository(nodeType)

    -- loads project specific nodes
    repository:load('common', nodeType, function(path)
        return require(root .. '/' .. path)
    end)
    repository:load(nodeType, nodeType, function(path)
        return require(root .. '/' .. path)
    end)
end

flat.dofile 'graph/script/init.lua'