window.onload = function() {
    var pick = document.getElementById('pick');
    var file = document.getElementById('file');
    var content = document.getElementById('content');
    var timelineCursor = document.getElementById('timeline-cursor');

    var binaryTreeView;

    var currentVisibleRange = {
        startTime: Number.MAX_SAFE_INTEGER,
        endTime:   Number.MIN_SAFE_INTEGER,
        duration: 0
    };

    function addSectionsToTree(depth, events, sectionNames) {
        for (var event of events) {
            var section = document.createElement('li');
            section.classList.add('section');
            section.classList.add('section-' + event.sectionId);
            section.setAttribute('data-start-time', event.startTime);
            section.setAttribute('data-end-time', event.endTime);
            section.setAttribute('data-depth', depth);
            section.style.top = (depth * 20) + 'px';
            section.setAttribute('title', sectionNames[event.sectionId] + ': ' + (event.endTime - event.startTime) / 1000000 + 'ms');
            section.addEventListener('dblclick', (function(section) { return function() { zoomToSection(section) }; })(section));

            binaryTreeView.insertSection(section, event.startTime, event.endTime, depth);
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

    function smoothLerp(from, to, t) {
        var f = t<.5 ? 8*t*t*t*t : 1-8*(--t)*t*t*t;
        return from * (1 - f) + to * f;
    }

    var isZooming = false;
    function zoomToVisibleRange(startTime, endTime) {
        if (isZooming) {
            return;
        }

        isZooming = true;
        var initialStartTime = currentVisibleRange.startTime;
        var initialEndTime = currentVisibleRange.endTime;
        var animationDuration = 500;
        var totalDuration = 0;
        var stepDuration = 50;
        var interval = setInterval(function() {
            totalDuration += stepDuration;
            var ratio = totalDuration / animationDuration;
            if (ratio >= 1) {
                ratio = 1;
                clearInterval(interval);
                isZooming = false;
            }
            var newStartTime = smoothLerp(initialStartTime, startTime, ratio);
            var newEndTime = smoothLerp(initialEndTime, endTime, ratio);
            setVisibleRange(newStartTime, newEndTime);
        }, stepDuration);
    }

    function zoomToSection(section) {
        var startTime = parseInt(section.getAttribute('data-start-time'));
        var endTime = parseInt(section.getAttribute('data-end-time'));
        zoomToVisibleRange(startTime, endTime);
    }

    function getCursorTimelinePosition(e) {
        return currentVisibleRange.startTime + currentVisibleRange.duration * ((e.pageX - content.offsetLeft) / content.offsetWidth);
    }

    function initContentElement() {
        content.style.height = ((binaryTreeView.sectionsDepth + 1) * 20) + 'px';
        content.addEventListener('wheel', function(e) {
            var cursorTimelinePosition = getCursorTimelinePosition(e);
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

        var isSelecting = false;
        var selectionStart;
        var selectionStartTimelinePosition;
        var selectionEndTimelinePosition;
        content.addEventListener('mousedown', function(e) {
            isSelecting = true;
            selectionStartTimelinePosition = getCursorTimelinePosition(e);
            selectionStart = parseInt(timelineCursor.style.left);
            timelineCursor.classList.add('selecting');
        });
        content.addEventListener('mouseup', function(e) {
            selectionEndTimelinePosition = getCursorTimelinePosition(e);
            if (selectionStartTimelinePosition > selectionEndTimelinePosition) {
                var tmp = selectionStartTimelinePosition;
                selectionStartTimelinePosition = selectionEndTimelinePosition;
                selectionEndTimelinePosition = tmp;
            }
            if (selectionStartTimelinePosition < selectionEndTimelinePosition) {
                zoomToVisibleRange(selectionStartTimelinePosition, selectionEndTimelinePosition);
            }
            timelineCursor.classList.remove('selecting');
            timelineCursor.style.width = '1px';
            isSelecting = false;
            timelineCursor.style.left = (e.pageX - content.offsetLeft + content.scrollLeft - 2) + 'px';
        });
        content.addEventListener('mouseenter', function(e) {
            timelineCursor.classList.remove('hidden');
        });
        content.addEventListener('mouseleave', function(e) {
            timelineCursor.classList.add('hidden');
        });
        content.addEventListener('mousemove', function(e) {
            if (isSelecting) {
                var position = e.pageX - content.offsetLeft + content.scrollLeft - 2;
                var left = Math.min(position, selectionStart);
                var right = Math.max(position, selectionStart);
                timelineCursor.style.left = left + 'px';
                timelineCursor.style.width = (right - left) + 'px';
            } else {
                timelineCursor.style.left = (e.pageX - content.offsetLeft + content.scrollLeft - 2) + 'px';
            }
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