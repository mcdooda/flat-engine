local ScriptNode = flat.require 'graph/script/scriptnode'
local ScriptNodeRuntime = flat.require 'graph/script/scriptnoderuntime'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'
local CommonStateMachineNode = flat.require 'graph/common/nodes/statemachinenode'
local PinTypes = flat.require 'graph/pintypes'

local cocreate = coroutine.create
local coresume = coroutine.resume

-- runtime

local StateMachineNodeRuntime = ScriptNodeRuntime:inherit()

function StateMachineNodeRuntime:init(node)
    self.context = nil
    self.stateMachineDescription = node:getStateMachineDescription()
    self.isRunning = false
end

function StateMachineNodeRuntime:execute(node)
    if not self.isRunning then
        self.context = self:readPin(node.contextInPin)
        self.isRunning = true
        self:cacheScriptRuntimes()
        self:enterState(self.stateMachineDescription.initState)
    end
    if self.currentStateThread then
        self:updateCurrentState()
        local newState = self:evaluateRules()
        if newState then
            self:enterState(newState)
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
    self.currentStateThread = cocreate(function()
        local runner = self.runtimesByState[newState].scriptRuntime:getRunner()
        runner()
    end)
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
    local currentStateThread = assert(self.currentStateThread)
    coresume(currentStateThread)
    if self.nextState then
        local nextState = self.nextState
        self.currentState = nextState
        self.currentStateThread = cocreate(function()
            local runner = self.runtimesByState[nextState].scriptRuntime:getRunner()
            runner()
        end)
    end
    self.nextState = nil
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