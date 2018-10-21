print '0'

local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local ScriptNodeRuntime = flat.require 'graph/script/scriptnoderuntime'
local CommonStateMachineNode = flat.require 'graph/common/nodes/statemachinenode'

print '1'

-- runtime

local StateMachineNodeRuntime = ScriptNodeRuntime:inherit()

function StateMachineNodeRuntime:init(node)
    self.context = nil
    self.stateMachineDescription = nil
    node:getStateMachineDescription()
end

function StateMachineNodeRuntime:update(node)
    if not self.stateMachineDescription then
        self.context = self:readPin(node.contextInPin)
        self.stateMachineDescription = node:getStateMachineDescription()
        self:cacheRuleScripts()
        self:enterState 'init'
    end
    self:evaluateRules()
    self:updateCurrentState()
end

function StateMachineNodeRuntime:cacheRuleScripts()
    -- compile rules
end

function StateMachineNodeRuntime:enterState(stateName)
    local state = assert(self.stateMachineDescription.states[stateName])

end

function StateMachineNodeRuntime:evaluateRules()
    local scriptRuntime = ScriptRuntime:new(script)
    local runner = scriptRuntime:getRunner(...)
    return runner()
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