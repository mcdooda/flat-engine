local nodes = {
    init = 'init',
    scriptInput = 'script/scriptinput',
    bool = 'bool',
    number = 'number',
    string = 'string',
    test = 'test',
    multiply = 'math/multiply'
}

local nodeTypes = {}
for nodeName, nodePath in pairs(nodes) do
    nodeTypes[nodeName] = flat.require('graph/script/nodes/' .. nodePath .. 'node')
end
return nodeTypes