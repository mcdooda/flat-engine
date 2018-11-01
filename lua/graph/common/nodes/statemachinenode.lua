local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'
local RuleNode = flat.require 'graph/statemachine/nodes/statemachine/rulenode'
local StateNode = flat.require 'graph/statemachine/nodes/statemachine/statenode'
local TransitionNode = flat.require 'graph/statemachine/nodes/statemachine/transitionnode'
local EnterNode = flat.require 'graph/statemachine/nodes/statemachine/enternode'

local function getNodeName(node)
    if node.nameInPin then
        return node.nameInPin.pluggedOutputPin.node:getValue()
    end
end

-- state machine description

local StateMachineDescription = {}
StateMachineDescription.__index = StateMachineDescription

function StateMachineDescription:new(graph)
    local enterNode
    local statesByName = {}
    local statesByNode = {}
    local transitions = {}
    local nodeInstances = graph.nodeInstances
    for i = 1, #nodeInstances do
        local nodeInstance = nodeInstances[i]
        local nodeName = getNodeName(nodeInstance)
        local mt = getmetatable(nodeInstance)
        if mt == StateNode then
            -- plugged rules (there are only output rules on state nodes)
            local outRules = {}
            local rulesPluggedInputPins = nodeInstance.rulesOutPin.pluggedInputPins
            for i = 1, #rulesPluggedInputPins do
                local ruleNodeInstance = rulesPluggedInputPins[i].node
                local ruleName = getNodeName(ruleNodeInstance)
                if #ruleNodeInstance.stateOutPin.pluggedInputPins == 0 then
                    print('No state following rule \'' .. ruleName .. '\', discarding it')
                else
                    local outStateNodeInstance = #ruleNodeInstance.stateOutPin.pluggedInputPins[1].node
                    if #ruleNodeInstance.stateOutPin.pluggedInputPins > 1 then
                        print('Several states following rule \'' .. ruleName .. '\', keeping \'' .. getNodeName(outStateNodeInstance) .. '\'')
                    end
                    local outStateNodeInstance = ruleNodeInstance.stateOutPin.pluggedInputPins[1].node

                    flat.arrayAdd(outRules, {
                        name = ruleName,
                        graph = ruleNodeInstance.innerGraph,
                        outStateNode = outStateNodeInstance
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

            -- output transitions and state
            local outStateNode
            local outTransitionNodes = {}
            local statePluggedInputPins = nodeInstance.stateOutPin.pluggedInputPins
            for i = 1, #statePluggedInputPins do
                local connectedStateNodeInstance = statePluggedInputPins[i].node
                local mt = getmetatable(connectedStateNodeInstance)
                if mt == StateNode then
                    if not outStateNode then
                        outStateNode = connectedStateNodeInstance
                    else
                        print('Several output states for state \'' .. nodeName .. '\', keeping \'' .. getNodeName(outStateNode) .. '\'')
                    end
                elseif mt == TransitionNode then
                    flat.arrayAdd(outTransitionNodes, connectedStateNodeInstance)
                end
            end

            if not statesByName[nodeName] then
                local state = {
                    name = nodeName,
                    graph = nodeInstance.innerGraph,
                    outRules = outRules,
                    inTransitionNodes = inTransitionNodes,
                    outTransitionNodes = outTransitionNodes,
                    outStateNode = outStateNode
                }
                statesByName[nodeName] = state
                statesByNode[nodeInstance] = state
            else
                print('Several states with the same name: \'' .. nodeName .. '\'')
            end
        elseif mt == TransitionNode then
            transitions[nodeInstance] = {
                name = nodeName,
                graph = nodeInstance.innerGraph,
                inStates = {},
                outStates = {}
            }
        elseif mt == EnterNode then
            if not enterNode then
                enterNode = nodeInstance
            else
                print('Several enter nodes')
            end
        end
    end

    for stateName, state in pairs(statesByName) do
        -- set rules out state from out state node
        local outRules = state.outRules
        for i = 1, #outRules do
            local outRule = outRules[i]
            outRule.outState = statesByNode[outRule.outStateNode]
            outRule.outStateNode = nil
        end

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

        -- set out state from state node
        if state.outStateNode then
            state.outState = statesByNode[state.outStateNode]
            state.outStateNode = nil
        end
    end

    for stateName, state in pairs(statesByName) do
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

    local initState
    if enterNode then
        local pluggedInputPins = enterNode.stateOutPin.pluggedInputPins
        local numPluggedInputPins = #pluggedInputPins
        if numPluggedInputPins > 0 then
            if numPluggedInputPins > 1 then
                print('Several states connected to the Enter node')
            end
            local initNode = pluggedInputPins[1].node
            local initNodeName = getNodeName(initNode)
            initState = statesByName[initNodeName]
        else
            print('No state connected to the Enter node')
        end
    else
        print('No enter node, the state machine will not run properly')
        statesByName = {}
    end

    return setmetatable({
        states = statesByName,
        initState = initState
    }, self)
end

function StateMachineDescription:getStates()
    return self.states
end

function StateMachineDescription:getStateByName(stateName)
    return assert(self.states[stateName])
end

function StateMachineDescription:getTransitionBetweenStates(from, to)
    if not from.outRules then -- from is a transition
        return
    end

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