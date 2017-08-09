flat.node = {}
flat.node.editor = {}

function flat.node.editor.open(editorContainer, graphPath)
    local MainWindow = flat.require 'graph-editor/mainwindow'
    local window = MainWindow:open(editorContainer)
    window:openGraph(graphPath)
end

