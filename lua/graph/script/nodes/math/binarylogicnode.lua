local BinaryOperatorTemplateNode = flat.require 'graph/script/nodes/math/binaryoperatortemplatenode'

local BinaryLogicNode = BinaryOperatorTemplateNode(
    nil,
    flat.types.BOOLEAN, 'A',
    flat.types.BOOLEAN, 'B',
    flat.types.BOOLEAN, 'Result'
)

return BinaryLogicNode