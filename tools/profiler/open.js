window.onload = function() {
    var pickElement = document.getElementById('pick');
    var fileElement = document.getElementById('file');
    var profileSessionElement = document.getElementById('profile-session');
    var timelineElement = document.getElementById('timeline');
    var timelineCursorElement = document.getElementById('timeline-cursor');
    var sectionsStatsElement = document.querySelector('#sections-stats tbody');
    var sectionsStatsElement = document.querySelector('#sections-stats tbody');

    var profileSession;
    var binaryTreeView;
    var stats;

    var currentVisibleRange = {
        startTime: Number.MAX_SAFE_INTEGER,
        endTime:   Number.MIN_SAFE_INTEGER,
        duration: 0
    };

    function addSectionsToTree(depth, events, sectionNames) {
        for (var event of events) {
            var sectionElement = document.createElement('li');
            sectionElement.classList.add('section');
            sectionElement.classList.add('section-' + (event.sectionId % 20));
            sectionElement.setAttribute('data-start-time', event.startTime);
            sectionElement.setAttribute('data-end-time', event.endTime);
            sectionElement.setAttribute('data-depth', depth);
            sectionElement.style.top = (depth * 20) + 'px';
            sectionElement.setAttribute('title', sectionNames[event.sectionId] + ': ' + (event.endTime - event.startTime) / 1000000 + 'ms');
            sectionElement.addEventListener('dblclick', (function(sectionElement) { return function() { zoomToSection(sectionElement) }; })(sectionElement));

            binaryTreeView.insertSection(sectionElement, event.sectionId, event.startTime, event.endTime, depth);
            stats.insertSection(sectionElement, event.sectionId, event.startTime, event.endTime);
            addSectionsToTree(depth + 1, event.events, sectionNames);
        }
    }

    function setVisibleRange(startTime, endTime) {
        currentVisibleRange.startTime = startTime;
        currentVisibleRange.endTime = endTime;
        var duration = endTime - startTime;
        currentVisibleRange.duration = duration;

        var newVisibleSections = binaryTreeView.getSectionsInRange(startTime, endTime);
        var currentVisibleSections = timelineElement.querySelectorAll('.section');
        for (var section of currentVisibleSections) {
            if (!newVisibleSections.has(section)) {
                section.remove();
            }
        }

        //console.log('newVisibleSections', newVisibleSections);
        for (var section of newVisibleSections) {
            if (!section.parentNode) {
                timelineElement.appendChild(section);
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

    function zoomToSection(sectionElement) {
        var startTime = parseInt(sectionElement.getAttribute('data-start-time'));
        var endTime = parseInt(sectionElement.getAttribute('data-end-time'));
        zoomToVisibleRange(startTime, endTime);
    }

    function getCursorTimelinePosition(e) {
        return currentVisibleRange.startTime + currentVisibleRange.duration * ((e.pageX - timelineElement.offsetLeft) / timelineElement.offsetWidth);
    }

    function initTimeline() {
        timelineElement.style.height = ((binaryTreeView.sectionsDepth + 1) * 20) + 'px';
        timelineElement.addEventListener('wheel', function(e) {
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
        timelineElement.addEventListener('mousedown', function(e) {
            isSelecting = true;
            selectionStartTimelinePosition = getCursorTimelinePosition(e);
            selectionStart = parseInt(timelineCursorElement.style.left);
            timelineCursorElement.classList.add('selecting');
        });
        timelineElement.addEventListener('mouseup', function(e) {
            selectionEndTimelinePosition = getCursorTimelinePosition(e);
            if (selectionStartTimelinePosition > selectionEndTimelinePosition) {
                var tmp = selectionStartTimelinePosition;
                selectionStartTimelinePosition = selectionEndTimelinePosition;
                selectionEndTimelinePosition = tmp;
            }
            if (selectionStartTimelinePosition < selectionEndTimelinePosition) {
                zoomToVisibleRange(selectionStartTimelinePosition, selectionEndTimelinePosition);
            }
            timelineCursorElement.classList.remove('selecting');
            timelineCursorElement.style.width = '1px';
            isSelecting = false;
            timelineCursorElement.style.left = (e.pageX - timelineElement.offsetLeft + timelineElement.scrollLeft - 2) + 'px';
        });
        timelineElement.addEventListener('mouseenter', function(e) {
            timelineCursorElement.classList.remove('hidden');
        });
        timelineElement.addEventListener('mouseleave', function(e) {
            timelineCursorElement.classList.add('hidden');
        });
        timelineElement.addEventListener('mousemove', function(e) {
            if (isSelecting) {
                var position = e.pageX - timelineElement.offsetLeft + timelineElement.scrollLeft - 2;
                var left = Math.min(position, selectionStart);
                var right = Math.max(position, selectionStart);
                timelineCursorElement.style.left = left + 'px';
                timelineCursorElement.style.width = (right - left) + 'px';
            } else {
                timelineCursorElement.style.left = (e.pageX - timelineElement.offsetLeft + timelineElement.scrollLeft - 2) + 'px';
            }
        });
    }

    function roundToMs(ns) {
        return Math.round(ns / 1000) / 1000;
    }

    function initStats() {
        var numSections = Object.keys(stats.sections).length;
        for (var sectionId = 0; sectionId < numSections; ++sectionId) {
            var sectionStats = stats.sections[sectionId];

            var sectionStatElement = document.createElement('tr');
            sectionStatElement.classList.add('section-' + (sectionId % 20));

            var sectionNameElement = document.createElement('td');
            sectionNameElement.textContent = profileSession.sectionNames[sectionId];
            sectionStatElement.appendChild(sectionNameElement);

            var sectionAverageElement = document.createElement('td');
            sectionAverageElement.textContent = roundToMs(sectionStats.averageDuration) + 'ms';
            sectionStatElement.appendChild(sectionAverageElement);

            var sectionWorstElement = document.createElement('td');
            sectionWorstElement.classList.add('worst-section');
            sectionWorstElement.textContent = roundToMs(sectionStats.worstDuration) + 'ms';
            (function(worstTimelineSectionElement) {
                sectionWorstElement.addEventListener('click', function() {
                    zoomToSection(worstTimelineSectionElement);
                });
            })(sectionStats.worstElement);
            sectionStatElement.appendChild(sectionWorstElement);
            
            sectionsStatsElement.appendChild(sectionStatElement);
        }
    }

    function initProfileSession() {
        initTimeline();
        initStats();
        profileSessionElement.classList.remove('hidden');
    }

    function showProfileSession() {
        binaryTreeView = new BinaryTreeView(profileSession.startTime, profileSession.endTime);
        stats = new Stats();
        addSectionsToTree(0, profileSession.profiledEvents, profileSession.sectionNames);
        //console.log('binaryTreeView', binaryTreeView);
        initProfileSession();
        var maxInitVisibleRange = 1000000000;
        var endTime = Math.min(profileSession.endTime, profileSession.startTime + maxInitVisibleRange);
        setVisibleRange(profileSession.startTime, endTime);
    }

    fileElement.addEventListener('change', function(event) {
        var worker = new Worker('sessionloader.js');
        worker.onmessage = function(message) {
            if (message.data.profileSession) {
                pickElement.classList.add('hidden');
                //console.log('loadedProfileSession', message.data.profileSession);
                profileSession = message.data.profileSession;
                showProfileSession();
            } else if (message.data.loading) {
                console.log(message.data.loading);
            }
        };
        worker.postMessage(fileElement.files[0]);
    });
}