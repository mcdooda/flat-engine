local scriptNodeRegistry = require 'script/scriptnoderegistry'

for nodeName, node in pairs(scriptNodeRegistry) do
    print(nodeName, node)
end