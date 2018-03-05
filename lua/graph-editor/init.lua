flat.graph.editor = {}

function flat.graph.editor.open(editorContainer, graphPath, nodeType, metadata, onSave, getRunnerFileCode)
    local MainWindow = flat.require 'graph-editor/mainwindow'
    local window = MainWindow:new(editorContainer, metadata, onSave, getRunnerFileCode)
    return window:openGraph(graphPath, nodeType)
end
