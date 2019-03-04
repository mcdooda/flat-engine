local NumberBinaryOperatorNode = flat.require 'graph/script/nodes/math/numberbinaryoperatornode'

local RandomIntNode = NumberBinaryOperatorNode:inherit 'Random Int'

local random = math.random

function RandomIntNode.compute(operand1, operand2)
    return random(operand1, operand2)
end

return RandomIntNode