local NumberBinaryOperatorNode = flat.require 'graph/script/nodes/math/numberbinaryoperatornode'

local DivideNode = NumberBinaryOperatorNode:inherit 'Divide'

function DivideNode.compute(operand1, operand2)
    return operand1 / operand2
end

return DivideNode