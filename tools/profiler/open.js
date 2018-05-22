window.onload = function() {
    var pick = document.getElementById('pick');
    var file = document.getElementById('file');
    var content = document.getElementById('content');

    function showSections(li, events, sectionNames) {
        var list = document.createElement('ul');
        for (i in events) {
            var event = events[i];
            var item = document.createElement('li');
            var p = document.createElement('p');
            p.textContent = sectionNames[event.sectionId] + ': ' + (event.endTime - event.startTime) / 1000000 + 'ms';
            item.appendChild(p);
            showSections(item, event.events, sectionNames);
            list.appendChild(item);
        }
        li.appendChild(list);
    }

    function showProfileSession(profileSession) {
        showSections(content, profileSession.profiledEvents, profileSession.sectionNames);
        content.classList.remove('hidden');
    }

    file.addEventListener('change', function(event) {
        var reader = new FileReader();
        reader.onloadend = function(event) {
            if (event.target.readyState == FileReader.DONE) {
                var profileSession = BinaryReader.fromString(event.target.result);
                console.log(profileSession);
                pick.classList.add('hidden');
                showProfileSession(profileSession);
            }
        }
        reader.readAsBinaryString(file.files[0]);
    });
}