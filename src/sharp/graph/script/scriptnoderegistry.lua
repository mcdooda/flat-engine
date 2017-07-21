local nodes = {
    init = 'init',
    bool = 'bool',
    number = 'number',
    string = 'string',
    test = 'test',
    multiply = 'math/multiply'
}

local nodeTypes = {}
for nodeName, nodePath in pairs(nodes) do
    nodeTypes[nodeName] = require('script/nodes/' .. nodePath .. 'node')
end
return nodeTypes