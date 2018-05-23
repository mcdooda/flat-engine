window.onload = function() {
    var pick = document.getElementById('pick');
    var file = document.getElementById('file');
    var content = document.getElementById('content');

    var currentZoomLevel = 1/10000;

    function showSections(depth, events, sectionNames) {
        for (var event of events) {
            var item = document.createElement('li');
            item.classList.add('section');
            item.classList.add('section-' + event.sectionId);
            item.setAttribute('data-start-time', event.startTime);
            item.setAttribute('data-end-time', event.endTime);
            item.setAttribute('data-depth', depth);
            item.style.top = (depth * 20) + 'px';
            var text = document.createElement('span');
            var textContent = sectionNames[event.sectionId] + ': ' + (event.endTime - event.startTime) / 1000000 + 'ms';
            text.textContent = textContent;
            text.setAttribute('title', textContent);
            item.appendChild(text);
            content.appendChild(item);
            showSections(depth + 1, event.events, sectionNames);
        }
    }

    function computeContentHeight() {
        var sections = content.querySelectorAll('.section');
        var maxDepth = 0;
        for (var section of sections) {
            var depth = parseInt(section.getAttribute('data-depth'));
            if (depth > maxDepth) {
                maxDepth = depth;
            }
        }
        content.style.height = ((maxDepth + 2) * 20) + 'px';
    }

    function setZoomLevel(zoomLevel) {
        currentZoomLevel = zoomLevel;
        console.log(zoomLevel);
        var sections = content.querySelectorAll('.section');
        var sessionStartTime = sections[0].getAttribute('data-start-time');
        for (var section of sections) {
            var startTime = section.getAttribute('data-start-time');
            var duration = section.getAttribute('data-end-time') - startTime;
            var width = duration * zoomLevel;
            section.style.width = width + 'px';
            var left = (startTime - sessionStartTime) * zoomLevel;
            section.style.left = left + 'px';
        }
    }

    function initContentElement() {
        computeContentHeight();
        setZoomLevel(currentZoomLevel);
        content.addEventListener('wheel', function(e) {
            setZoomLevel(currentZoomLevel * Math.pow(1.1, -e.deltaY / 3));
        });
        content.classList.remove('hidden');
    }

    function showProfileSession(profileSession) {
        showSections(0, profileSession.profiledEvents, profileSession.sectionNames);
        initContentElement();
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