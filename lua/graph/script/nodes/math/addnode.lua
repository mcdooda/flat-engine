local NumberBinaryOperatorNode = flat.require 'graph/script/nodes/math/numberbinaryoperatornode'

local AddNode = NumberBinaryOperatorNode:inherit 'Add'

function AddNode.compute(operand1, operand2)
    return operand1 + operand2
end

return AddNode