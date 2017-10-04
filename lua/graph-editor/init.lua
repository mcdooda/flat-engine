flat.graph.editor = {}

function flat.graph.editor.open(editorContainer, graphPath, nodeType, metadata)
    local MainWindow = flat.require 'graph-editor/mainwindow'
    local window = MainWindow:open(editorContainer, metadata)
    return window:openGraph(graphPath, nodeType)
end
