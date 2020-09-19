flat.graph.editor = {}

function flat.graph.editor.open(editorContainer, graphPath, nodeType, metadata, onSave, getRunnerCode)
    local MainWindow = flat.require 'graph-editor/mainwindow'
    local window = MainWindow:new(editorContainer, metadata, onSave, getRunnerCode)
    return window:openGraphFromFile(graphPath, nodeType)
end
