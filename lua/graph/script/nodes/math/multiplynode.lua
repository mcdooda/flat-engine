local BinaryOperatorNode = flat.require 'graph/script/nodes/math/binaryoperatornode'

local MultiplyNode = BinaryOperatorNode:inherit()

function MultiplyNode.compute(operand1, operand2)
    return operand1 * operand2
end

return MultiplyNode