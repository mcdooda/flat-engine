local scriptNodeRegistry = require 'graph/script/scriptnoderegistry'

for nodeName, node in pairs(scriptNodeRegistry) do
    print(nodeName, node)
end