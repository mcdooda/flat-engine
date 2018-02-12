local NumberBinaryOperatorNode = flat.require 'graph/script/nodes/math/numberbinaryoperatornode'

local MultiplyNode = NumberBinaryOperatorNode:inherit 'Multiply'

function MultiplyNode.compute(operand1, operand2)
    return operand1 * operand2
end

return MultiplyNode