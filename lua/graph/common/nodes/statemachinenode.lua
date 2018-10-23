local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'
local RuleNode = flat.require 'graph/statemachine/nodes/statemachine/rulenode'
local StateNode = flat.require 'graph/statemachine/nodes/statemachine/statenode'
local TransitionNode =  flat.require 'graph/statemachine/nodes/statemachine/transitionnode'

local StateMachineNode = InnerGraphNode:inherit 'State Machine'

function StateMachineNode:init()
    InnerGraphNode.init(self)
    self.innerGraph.nodeType = 'statemachine'
end

function StateMachineNode:buildPins()
    self.contextInPin = self:addInputPin(PinTypes.ANY, 'Context')

    self.resultOutPin = self:addOutputPin(PinTypes.ANY, 'Result')
end

local function getNodeName(node)
    return node.nameInPin.pluggedOutputPin.node:getValue()
end

function StateMachineNode:getStateMachineDescription()
    local states = {}
    local transitions = {}
    local nodeInstances = self.innerGraph.nodeInstances
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
                outTransitionNodes = outTransitionNodes,
                inTransitions = {},
                outTransitions = {}
            }
        elseif mt == TransitionNode then
            local stateName = getNodeName(nodeInstance)
            transitions[nodeInstance] = {
                name = stateName,
                graph = nodeInstance.innerGraph
            }
        end
    end

    for stateName, state in pairs(states) do
        for i = 1, #state.inTransitionNodes do
            local transitionNode = state.inTransitionNodes[i]
            state.inTransitions[i] = transitions[transitionNode]
        end
        state.inTransitionNodes = nil

        for i = 1, #state.outTransitionNodes do
            local transitionNode = state.outTransitionNodes[i]
            state.outTransitions[i] = transitions[transitionNode]
        end
        state.outTransitionNodes = nil
    end

    local stateMachineDescription = {
        states = states
    }

    --[[
    print 'states'
    for stateName, state in pairs(stateMachineDescription.states) do
        print '---'
        print(stateName, state)
        if #state.outRules > 0 then
            print '\tOut rules:'
            for i = 1, #state.outRules do
                local rule = state.outRules[i]
                print('', '', rule.name)
            end
        end
        if #state.inTransitions > 0 then
            print 'In transitions:'
            for i = 1, #state.inTransitions do
                local transition = state.inTransitions[i]
                print('', '', transition.name)
            end
        end
        if #state.outTransitions > 0 then
            print 'Out transitions:'
            for i = 1, #state.outTransitions do
                local transition = state.outTransitions[i]
                print('', '', transition.name)
            end
        end
    end
    print '==='
    ]]

    return stateMachineDescription
end

return StateMachineNode