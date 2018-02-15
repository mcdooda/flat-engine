local NumberBinaryOperatorNode = flat.require 'graph/script/nodes/math/numberbinaryoperatornode'

local SubstractNode = NumberBinaryOperatorNode:inherit 'Substract'

function SubstractNode.compute(operand1, operand2)
    return operand1 - operand2
end

return SubstractNode