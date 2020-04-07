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

function flat.graph.getCompounds(nodeType)
    local repository = getNodeRepository(nodeType)
    return repository:getCompounds()
end

local function requireFromRoot(root)
    return function(path)
        return require(root .. '/' .. path)
    end
end

function flat.graph.loadNodeClasses(nodeType, root)
    local repository = getNodeRepository(nodeType)

    -- loads flat nodes
    repository:loadNodes('common', nodeType, flat.require)
    repository:loadNodes(nodeType, nodeType, flat.require)

    -- loads project specific nodes
    local require = requireFromRoot(root)
    repository:loadNodes('common', nodeType, require)
    repository:loadNodes(nodeType, nodeType, require)
end

function flat.graph.loadCompounds(nodeType, root)
    local repository = getNodeRepository(nodeType)
    repository:loadCompounds(nodeType, root)
end

flat.dofile 'graph/script/init.lua'
flat.dofile 'graph/sound/init.lua'