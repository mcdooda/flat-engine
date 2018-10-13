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
    local rules = {}
    local states = {}
    local transitions = {}
    for i = 1, #self.innerGraph.nodeInstances do
        local nodeInstance = self.innerGraph.nodeInstances[i]
        local mt = getmetatable(nodeInstance)
        if mt == StateNode then
            local outRules = {}
            for i = 1, #nodeInstance.rulesOutPin.pluggedInputPins do
                local ruleNodeInstance = nodeInstance.rulesOutPin.pluggedInputPins[i].node
                if #rule.stateOutPin.pluggedInputPins == 0 then
                    print('No state following rule ' .. getNodeName(ruleNodeInstance) .. ', discarding it')
                else
                    local outStateNodeInstance = #rule.stateOutPin.pluggedInputPins[1].node
                    if #rule.stateOutPin.pluggedInputPins > 1 then
                        print('Several states following rule ' .. getNodeName(ruleNodeInstance) .. ', keeping' .. getNodeName(outStateNodeInstance))
                    end
                    local outStateNodeInstance = rule.stateOutPin.pluggedInputPins[1]
                    outRules[ruleNodeInstance] = {
                        graph = ruleNodeInstance.innerGraph,
                        outState = outStateNodeInstance
                    }
                end
            end
            states[nodeInstance] = {
                graph = nodeInstance.innerGraph,
                outRules = outRules,
                inTransitions = {}, 
                outTransitions = {},
            }
        elseif mt == TransitionNode then
            transitions[nodeInstance] = {
                graph = nodeInstance.innerGraph,
                outStates = {},
            }
        end
    end

    local stateMachineDescription = {}

    return stateMachineDescription
end

return StateMachineNode