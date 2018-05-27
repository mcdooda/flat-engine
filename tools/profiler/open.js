window.onload = function() {
    var pick = document.getElementById('pick');
    var file = document.getElementById('file');
    var content = document.getElementById('content');

    var binaryTreeView;

    var currentVisibleRange = {
        startTime: Number.MAX_SAFE_INTEGER,
        endTime:   Number.MIN_SAFE_INTEGER,
        duration: 0
    };

    function addSectionsToTree(depth, events, sectionNames) {
        for (var event of events) {
            binaryTreeView.insertSection(event.sectionId, sectionNames[event.sectionId], event.startTime, event.endTime, depth);
            addSectionsToTree(depth + 1, event.events, sectionNames);
        }
    }

    function setVisibleRange(startTime, endTime) {
        currentVisibleRange.startTime = startTime;
        currentVisibleRange.endTime = endTime;
        var duration = endTime - startTime;
        currentVisibleRange.duration = duration;

        var newVisibleSections = binaryTreeView.getSectionsInRange(startTime, endTime);
        var currentVisibleSections = content.querySelectorAll('.section');
        for (var section of currentVisibleSections) {
            if (!newVisibleSections.has(section)) {
                section.remove();
            }
        }

        //console.log('newVisibleSections', newVisibleSections);
        for (var section of newVisibleSections) {
            if (!section.parentNode) {
                content.appendChild(section);
            }
            var sectionStartTime = parseInt(section.getAttribute('data-start-time'));
            var sectionEndTime = parseInt(section.getAttribute('data-end-time'));
            var sectionDuration = sectionEndTime - sectionStartTime;
            section.style.width = (sectionDuration / duration * 100) + '%';
            section.style.left = ((sectionStartTime - startTime) / duration * 100) + '%';
        }
    }

    function initContentElement() {
        content.style.height = ((binaryTreeView.depth + 2) * 20) + 'px';
        content.addEventListener('wheel', function(e) {
            var cursorTimelinePosition = currentVisibleRange.startTime + currentVisibleRange.duration * ((e.pageX - content.offsetLeft) / content.offsetWidth);
            var deltaY = 0;
            if (e.wheelDelta) {
                deltaY = -e.wheelDeltaY / 120;
            } else {
                deltaY = e.deltaY / 3;
            }
            var zoom = Math.pow(1.1, deltaY);
            var newStartTime = cursorTimelinePosition - (cursorTimelinePosition - currentVisibleRange.startTime) * zoom;
            var newEndTime = cursorTimelinePosition - (cursorTimelinePosition - currentVisibleRange.endTime) * zoom;
            //console.log(currentVisibleRange, newStartTime, newEndTime);
            setVisibleRange(newStartTime, newEndTime);
        });
        content.classList.remove('hidden');
    }

    function showProfileSession(profileSession) {
        binaryTreeView = new BinaryTreeView(profileSession.startTime, profileSession.endTime);
        addSectionsToTree(0, profileSession.profiledEvents, profileSession.sectionNames);
        //console.log('binaryTreeView', binaryTreeView);
        initContentElement();
        setVisibleRange(profileSession.startTime, profileSession.endTime);
    }

    file.addEventListener('change', function(event) {
        var reader = new FileReader();
        reader.onloadend = function(event) {
            if (event.target.readyState == FileReader.DONE) {
                var profileSession = BinaryReader.fromString(event.target.result);
                //console.log('profileSession', profileSession);
                pick.classList.add('hidden');
                showProfileSession(profileSession);
            }
        }
        reader.readAsBinaryString(file.files[0]);
    });
}