local BinaryOperatorTemplateNode = flat.require 'graph/script/nodes/math/binaryoperatortemplatenode'

local ComparisonOperatorNode = BinaryOperatorTemplateNode(
    nil,
    flat.types.NUMBER, 'Operand 1',
    flat.types.NUMBER, 'Operand 2',
    flat.types.BOOLEAN, 'Result'
)

return ComparisonOperatorNode