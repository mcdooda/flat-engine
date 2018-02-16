local ComparisonOperatorNode = flat.require 'graph/script/nodes/math/comparisonoperatornode'

local EqualNode = ComparisonOperatorNode:inherit 'Equal'

function EqualNode.compute(operand1, operand2)
    return operand1 == operand2
end

return EqualNode