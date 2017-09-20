local NodeRepository = flat.require 'graph/noderepository'

flat.graph = {}
flat.graph.repositories = {}

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

function flat.graph.addNodeClasses(nodeType, root)
    local repository = getNodeRepository(nodeType)
    repository:load(nodeType, function(path)
        return require(root .. '/' .. path)
    end)
end