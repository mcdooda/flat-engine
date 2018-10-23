local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'
local RuleNode = flat.require 'graph/statemachine/nodes/statemachine/rulenode'
local StateNode = flat.require 'graph/statemachine/nodes/statemachine/statenode'
local TransitionNode = flat.require 'graph/statemachine/nodes/statemachine/transitionnode'

local function getNodeName(node)
    return node.nameInPin.pluggedOutputPin.node:getValue()
end

-- state machine description

local StateMachineDescription = {}
StateMachineDescription.__index = StateMachineDescription

function StateMachineDescription:new(graph)
    local states = {}
    local transitions = {}
    local nodeInstances = graph.nodeInstances
    for i = 1, #nodeInstances do
        local nodeInstance = nodeInstances[i]
        local mt = getmetatable(nodeInstance)
        if mt == StateNode then
            -- plugged rules (there are only output rules on state nodes)
            local outRules = {}
            local rulesPluggedInputPins = nodeInstance.rulesOutPin.pluggedInputPins
            for i = 1, #rulesPluggedInputPins do
                local ruleNodeInstance = rulesPluggedInputPins[i].node
                local ruleName = getNodeName(ruleNodeInstance)
                if #ruleNodeInstance.stateOutPin.pluggedInputPins == 0 then
                    print('No state following rule ' .. ruleName .. ', discarding it')
                else
                    local outStateNodeInstance = #ruleNodeInstance.stateOutPin.pluggedInputPins[1].node
                    if #ruleNodeInstance.stateOutPin.pluggedInputPins > 1 then
                        print('Several states following rule ' .. ruleName .. ', keeping' .. getNodeName(outStateNodeInstance))
                    end
                    local outStateNodeInstance = ruleNodeInstance.stateOutPin.pluggedInputPins[1]

                    flat.arrayAdd(outRules, {
                        name = ruleName,
                        graph = ruleNodeInstance.innerGraph,
                        outState = outStateNodeInstance
                    })
                end
            end

            -- plugged transitions
            -- input transitions
            local inTransitionNodes = {}
            local stateInPins = nodeInstance.stateInPins
            for i = 1, #stateInPins - 1 do -- the last one is never connected
                local stateInPin = stateInPins[i]
                local connectedStateNodeInstance = stateInPin.pluggedOutputPin.node
                local mt = getmetatable(connectedStateNodeInstance)
                if mt == TransitionNode then
                    flat.arrayAdd(inTransitionNodes, connectedStateNodeInstance)
                end
            end

            -- output transitions
            local outTransitionNodes = {}
            local statePluggedInputPins = nodeInstance.stateOutPin.pluggedInputPins
            for i = 1, #statePluggedInputPins do
                local connectedStateNodeInstance = statePluggedInputPins[i].node
                local mt = getmetatable(connectedStateNodeInstance)
                if mt == TransitionNode then
                    flat.arrayAdd(outTransitionNodes, connectedStateNodeInstance)
                end
            end

            local stateName = getNodeName(nodeInstance)
            states[stateName] = {
                name = stateName,
                graph = nodeInstance.innerGraph,
                outRules = outRules,
                inTransitionNodes = inTransitionNodes,
                outTransitionNodes = outTransitionNodes
            }
        elseif mt == TransitionNode then
            local stateName = getNodeName(nodeInstance)
            transitions[nodeInstance] = {
                name = stateName,
                graph = nodeInstance.innerGraph,
                inStates = {},
                outStates = {}
            }
        end
    end

    for stateName, state in pairs(states) do
        -- set in transitions from transition nodes
        local inTransitions = {}
        local inTransitionNodes = state.inTransitionNodes
        state.inTransitionNodes = nil
        for i = 1, #inTransitionNodes do
            local transitionNode = inTransitionNodes[i]
            local transition = transitions[transitionNode]
            flat.arrayAdd(transition.outStates, state)
            inTransitions[transition] = true
        end
        state.inTransitions = inTransitions

        -- set out transitions from transition nodes
        local outTransitions = {}
        local outTransitionNodes = state.outTransitionNodes
        state.outTransitionNodes = nil
        for i = 1, #outTransitionNodes do
            local transitionNode = outTransitionNodes[i]
            local transition = transitions[transitionNode]
            flat.arrayAdd(transition.inStates, state)
            outTransitions[transition] = true
        end
        state.outTransitions = outTransitions
    end

    for stateName, state in pairs(states) do
        -- find default in transition
        for transition, _ in pairs(state.inTransitions) do
            if #transition.inStates == 0 then
                if not state.defaultInTransition then
                    state.defaultInTransition = transition
                else
                    print('State \'' .. stateName .. '\' has several global in transitions, \'' .. transition.name .. '\' discarded')
                end
            end
        end

        -- find default out transition
        for transition, _ in pairs(state.outTransitions) do
            if #transition.outStates == 0 then
                if not state.defaultOutTransition then
                    state.defaultOutTransition = transition
                else
                    print('State \'' .. stateName .. '\' has several global out transitions, \'' .. transition.name .. '\' discarded')
                end
            end
        end
    end

    return setmetatable({
        states = states
    }, self)
end

function StateMachineDescription:getTransitionBetweenStates(from, to)
    for transition, _ in pairs(from.outTransitions) do
        if to.inTransitions[transition] then
            return transition
        end
    end

    if from.defaultOutTransition then
        return from.defaultOutTransition
    end

    return to.defaultInTransition
end

function StateMachineDescription:debugPrint()
    print '===================================='
    for stateName, state in pairs(self.states) do
        print '-----------------'
        print(stateName, state)
        if #state.outRules > 0 then
            print '\tOut rules:'
            for i = 1, #state.outRules do
                local rule = state.outRules[i]
                print('', '', rule.name)
            end
        end
        print '\tIn transitions:'
        for transition, _ in pairs(state.inTransitions) do
            print('', '', transition.name)
        end
        print('\tdefault in transition: ' .. (state.defaultInTransition and state.defaultInTransition.name or '<none>'))
        print '\tOut transitions:'
        for transition, _ in pairs(state.outTransitions) do
            print('', '', transition.name)
        end
        print('\tdefault out transition: ' .. (state.defaultOutTransition and state.defaultOutTransition.name or '<none>'))
    end
    print '===================================='
end

-- state machine node

local StateMachineNode = InnerGraphNode:inherit 'State Machine'

function StateMachineNode:init()
    InnerGraphNode.init(self)
    self.innerGraph.nodeType = 'statemachine'
end

function StateMachineNode:buildPins()
    self.contextInPin = self:addInputPin(PinTypes.ANY, 'Context')

    self.resultOutPin = self:addOutputPin(PinTypes.ANY, 'Result')
end

function StateMachineNode:getStateMachineDescription()
    local stateMachineDescription = StateMachineDescription:new(self.innerGraph)

    --stateMachineDescription:debugPrint()

    return stateMachineDescription
end

return StateMachineNode