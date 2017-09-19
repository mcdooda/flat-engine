flat.node = flat.node or {}
flat.node.editor = {}

function flat.node.editor.open(editorContainer, graphPath, nodeType)
    local MainWindow = flat.require 'graph-editor/mainwindow'
    local window = MainWindow:open(editorContainer)
    return window:openGraph(graphPath, nodeType)
end

