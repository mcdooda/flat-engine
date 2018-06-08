importScripts('binaryreader.js');

onmessage = function(fileName) {
    //console.log(fileName, fileName.data);
    var reader = new FileReader();
    reader.onloadend = function(event) {
        if (event.target.readyState == FileReader.DONE) {
            var profileSession = BinaryReader.fromString(event.target.result);
            postMessage({ profileSession: profileSession });
        }
    }
    reader.readAsBinaryString(fileName.data);
};

