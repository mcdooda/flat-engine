local NumberBinaryOperatorNode = flat.require 'graph/script/nodes/math/numberbinaryoperatornode'

local RandomFloatNode = NumberBinaryOperatorNode:inherit 'Random Float'

local random = math.random

function RandomFloatNode.compute(operand1, operand2)
    return random() * (operand2 - operand1) + operand1
end

return RandomFloatNode