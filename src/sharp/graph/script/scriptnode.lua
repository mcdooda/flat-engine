local Node = require 'node'
local ScriptNodeRuntime = require 'script/scriptnoderuntime'

local ScriptNode = Node:inherit()

function ScriptNode:getRuntimeType()
    return ScriptNodeRuntime
end

function ScriptNode:execute(runtime)
    error('pure virtual function')
end

function ScriptNode:prepareReadPin(runtime, inputPin)
    -- nothing to do by default
end

return ScriptNode