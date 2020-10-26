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
    self.perContextData = setmetatable({}, { __mode = 'k' })
    self.stateMachineDescription = node:getStateMachineDescription()
    self.hasError = false
end

function StateMachineNodeRuntime:execute(node)
    if self.hasError then
        return
    end
    local context = self:readPin(node.contextInPin)
    local contextId = flat.getUniqueObject(context)
    local contextData = self.perContextData[contextId]
    xpcall(function()
        if not contextData then
            contextData = { context = context }
            self:cacheScriptRuntimes(contextData)
            self:enterState(contextData, self.stateMachineDescription.initState)
            self.perContextData[contextId] = contextData
        end
    end, function()
        self.hasError = true
        flat.ui.error('Could not load state machine')
        if flat.debug then
            flat.debug.printstack()
        end
    end)
    xpcall(function()
        if contextData.currentStateThread then
            self:updateCurrentState(contextData)
            local newState = self:evaluateRules(contextData)
            if newState then
                self:enterState(contextData, newState)
            end
        end
    end, function()
        self.hasError = true
        flat.ui.error('Error while updating state machine')
        if flat.debug then
            flat.debug.printstack()
        end
    end)
end

function StateMachineNodeRuntime:cacheScriptRuntimes(contextData)
    -- compile rules
    local runtimesByState = {}
    for stateName, state in pairs(self.stateMachineDescription:getStates()) do
        local stateGraph = state.graph:clone()
        stateGraph:resolveCompoundsAndReroutes()
        local scriptRuntime = ScriptRuntime:new(stateGraph)
        scriptRuntime:setContext(contextData.context)

        local ruleScriptRuntimes = {}
        for i = 1, #state.outRules do
            local rule = state.outRules[i]
            local ruleGraph = rule.graph:clone()
            ruleGraph:resolveCompoundsAndReroutes()
            local ruleScriptRuntime = ScriptRuntime:new(ruleGraph)
            ruleScriptRuntime:setContext(contextData.context)
            ruleScriptRuntimes[rule] = ruleScriptRuntime
        end

        runtimesByState[state] = {
            scriptRuntime = scriptRuntime,
            ruleScriptRuntimes = ruleScriptRuntimes
        }
    end
    contextData.runtimesByState = runtimesByState
end

function StateMachineNodeRuntime:enterStateByName(contextData, stateName)
    local state = self.stateMachineDescription:getStateByName(stateName)
    if state then
        self:enterState(contextData, state)
    end
end

function StateMachineNodeRuntime:enterState(contextData, newState)
    assert(newState, 'Cannot enter nil state')
    local currentState = contextData.currentState
    local nextState
    if currentState then
        local transition = self.stateMachineDescription:getTransitionBetweenStates(currentState, newState)
        if transition then
            newState = transition
            nextState = newState
        end
    end

    contextData.currentState = newState
    contextData.currentStateThread = cocreate(function()
        local runner = contextData.runtimesByState[newState].scriptRuntime:getRunner()
        runner()
    end)
    contextData.nextState = nextState
end

function StateMachineNodeRuntime:evaluateRules(contextData)
    local currentState = contextData.currentState
    if currentState then
        local ruleScriptRuntimes = contextData.runtimesByState[currentState].ruleScriptRuntimes
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

function StateMachineNodeRuntime:updateCurrentState(contextData)
    local currentStateThread = assert(contextData.currentStateThread)
    coresume(currentStateThread)
    if contextData.nextState then
        local nextState = contextData.nextState
        contextData.currentState = nextState
        contextData.currentStateThread = cocreate(function()
            local runner = contextData.runtimesByState[nextState].scriptRuntime:getRunner()
            runner()
        end)
    end
    contextData.nextState = nil
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