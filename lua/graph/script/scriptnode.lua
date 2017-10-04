local Node = flat.require 'graph/node'
local ScriptNodeRuntime = flat.require 'graph/script/scriptnoderuntime'

local ScriptNode = Node:inherit()

function ScriptNode:getRuntimeType()
    return ScriptNodeRuntime
end

function ScriptNode:execute(runtime)
    error('pure virtual function')
end

function ScriptNode:prepareReadPin(runtime, outputPin)
    -- nothing to do by default
end

return ScriptNode