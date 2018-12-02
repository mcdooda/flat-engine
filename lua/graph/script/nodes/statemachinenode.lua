local ScriptNode = flat.require 'graph/script/scriptnode'
local ScriptNodeRuntime = flat.require 'graph/script/scriptnoderuntime'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'
local CommonStateMachineNode = flat.require 'graph/common/nodes/statemachinenode'
local PinTypes = flat.require 'graph/pintypes'

local coyield = coroutine.yield

-- runtime

local StateMachineNodeRuntime = ScriptNodeRuntime:inherit()

function StateMachineNodeRuntime:init(node)
    self.context = nil
    self.stateMachineDescription = node:getStateMachineDescription()
    self.isRunning = false
end

function StateMachineNodeRuntime:execute(node)
    self.context = self:readPin(node.contextInPin)
    self.isRunning = true
    self:cacheScriptRuntimes()
    self:enterState(self.stateMachineDescription.initState)
    while self.isRunning do
        if self.currentStateRunner then
            self:updateCurrentState()
            coyield()
            local newState = self:evaluateRules()
            if newState then
                self:enterState(newState)
            end
        else
            self.isRunning = false
        end
    end
end

function StateMachineNodeRuntime:cacheScriptRuntimes()
    -- compile rules
    local runtimesByState = {}
    for stateName, state in pairs(self.stateMachineDescription:getStates()) do
        local scriptRuntime = ScriptRuntime:new(state.graph)
        scriptRuntime:setContext(self.context)

        local ruleScriptRuntimes = {}
        for i = 1, #state.outRules do
            local rule = state.outRules[i]
            local ruleScriptRuntime = ScriptRuntime:new(rule.graph)
            ruleScriptRuntime:setContext(self.context)
            ruleScriptRuntimes[rule] = ruleScriptRuntime
        end

        runtimesByState[state] = {
            scriptRuntime = scriptRuntime,
            ruleScriptRuntimes = ruleScriptRuntimes
        }
    end
    self.runtimesByState = runtimesByState
end

function StateMachineNodeRuntime:enterStateByName(stateName)
    local state = self.stateMachineDescription:getStateByName(stateName)
    if state then
        self:enterState(state)
    end
end

function StateMachineNodeRuntime:enterState(newState)
    assert(newState, 'Cannot enter nil state')
    local currentState = self.currentState
    local nextState
    if currentState then
        local transition = self.stateMachineDescription:getTransitionBetweenStates(currentState, newState)
        if transition then
            newState = transition
            nextState = newState
        end
    end

    self.currentState = newState
    self.currentStateRunner = self.runtimesByState[newState].scriptRuntime:getRunner()
    self.nextState = nextState
end

function StateMachineNodeRuntime:evaluateRules()
    local currentState = self.currentState
    if currentState then
        local ruleScriptRuntimes = self.runtimesByState[currentState].ruleScriptRuntimes
        for rule, ruleScriptRuntime in pairs(ruleScriptRuntimes) do
            local runner = ruleScriptRuntime:getRunner()
            local ruleResult = runner()
            if ruleResult then
                return rule.outState
            end
        end
        return currentState.outState
    end
end

function StateMachineNodeRuntime:updateCurrentState()
    local currentStateRunner = self.currentStateRunner
    if currentStateRunner then
        currentStateRunner()
        if self.nextState then
            local nextState = self.nextState
            self.currentState = nextState
            self.currentStateRunner = self.runtimesByState[nextState].scriptRuntime:getRunner()
        else
            -- keep self.currentState for evaluting the rules
            self.currentStateRunner = nil
        end
        self.nextState = nil
    end
end

-- node

local StateMachineNode = (CommonStateMachineNode + ScriptNode):inherit()

function StateMachineNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.updateOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Update')

    CommonStateMachineNode.buildPins(self)
end

function StateMachineNode:getRuntimeType()
    return StateMachineNodeRuntime
end

function StateMachineNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    runtime:execute(self)
    runtime:impulse(self.impulseOutPin)
end

return StateMachineNode