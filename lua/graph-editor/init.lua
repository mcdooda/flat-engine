flat.graph.editor = {}

function flat.graph.editor.open(editorContainer, graphPath, nodeType)
    local MainWindow = flat.require 'graph-editor/mainwindow'
    local window = MainWindow:open(editorContainer)
    return window:openGraph(graphPath, nodeType)
end
