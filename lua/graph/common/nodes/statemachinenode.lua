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
    return node.inputPins[1].pluggedOutputPin.node:getValue()
end

function StateMachineNode:getStateMachineDescription()
    local states = {}
    local transitions = {}
    local nodeInstances = self.innerGraph.nodeInstances
    for i = 1, #nodeInstances do
        local nodeInstance = nodeInstances[i]
        local mt = getmetatable(nodeInstance)
        if mt == StateNode then
            -- plugged rules
            local outRules = {}
            local rulesPluggedInputPins = nodeInstance.rulesOutPin.pluggedInputPins
            for i = 1, #rulesPluggedInputPins do
                local ruleNodeInstance = rulesPluggedInputPins[i].node
                if #ruleNodeInstance.stateOutPin.pluggedInputPins == 0 then
                    print('No state following rule ' .. getNodeName(ruleNodeInstance) .. ', discarding it')
                else
                    local outStateNodeInstance = #ruleNodeInstance.stateOutPin.pluggedInputPins[1].node
                    if #ruleNodeInstance.stateOutPin.pluggedInputPins > 1 then
                        print('Several states following rule ' .. getNodeName(ruleNodeInstance) .. ', keeping' .. getNodeName(outStateNodeInstance))
                    end
                    local outStateNodeInstance = ruleNodeInstance.stateOutPin.pluggedInputPins[1]
                    outRules[#outRules + 1] = {
                        graph = ruleNodeInstance.innerGraph,
                        outState = outStateNodeInstance
                    }
                end
            end

            -- state name
            local stringConstantNode = nodeInstance.nameInPin.pluggedOutputPin.node
            local stateName = stringConstantNode:getValue()

            states[stateName] = {
                graph = nodeInstance.innerGraph,
                outRules = outRules,
                inTransitions = {}, 
                outTransitions = {},
            }
        end
    end

    local stateMachineDescription = {
        states = states,
        transitions = transitions
    }

    print 'states'
    for k,v in pairs(stateMachineDescription.states) do
        print(k, v, k:getName())
    end
    print '==='
    
    print 'transitions'
    for k,v in pairs(stateMachineDescription.transitions) do
        print(k, v, k:getName())
    end
    print '==='

    return stateMachineDescription
end

return StateMachineNode