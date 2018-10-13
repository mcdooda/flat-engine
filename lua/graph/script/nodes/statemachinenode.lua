local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local ScriptNodeRuntime = flat.require 'graph/script/scriptnoderuntime'
local CommonStateMachineNode = flat.require 'graph/common/nodes/statemachinenode'

-- runtime

local StateMachineNodeRuntime = ScriptNodeRuntime:inherit()

function StateMachineNodeRuntime:init(node)
    self.initialized = false
    self.context = nil
end

function StateMachineNodeRuntime:update(node)
    if not self.stateMachineDescription then
        self.context = self:readPin(node.contextInPin)
        self.stateMachineDescription = node:getStateMachineDescription()
        self:enterState 'init'
    end
    self:evaluateRules()
    self:updateCurrentState()
end

function StateMachineNodeRuntime:enterState(stateName)
    local state = assert(self.stateMachineDescription.states[stateName])
end

function StateMachineNodeRuntime:evaluateRules()

end

function StateMachineNodeRuntime:updateCurrentState()

end

-- node

local StateMachineNode = (CommonStateMachineNode + FunctionalScriptNode):inherit()

function StateMachineNode:getRuntimeType()
    return StateMachineNodeRuntime
end

function StateMachineNode:execute(runtime)
    runtime:update(node)
end

return StateMachineNode