var BinaryReader = {

    Codes: {
        PUSH_SECTION:  '\x00',
        POP_SECTION:   '\x01',
        SECTION_NAMES: '\x02',
    },

    fromString: function(string) {
        var byteIndex = 0;

        function readNBytes(n) {
            var c = string.slice(byteIndex, byteIndex + n);
            byteIndex += n;
            return c;
        }

        function pushBackNBytes(n) {
            byteIndex -= n;
        }

        function binaryStringToNumber(c) {
            var result = 0;
            for (var i = 0; i < c.length; ++i) {
                result += c.charCodeAt(i) * Math.pow(256, i);
            }
            return result;
        }

        function readEvents(parent) {
            while (true) {
                var code = readNBytes(1);
                if (code == BinaryReader.Codes.PUSH_SECTION) {
                    var event = { events: [] };
                    parent.events.push(event);
                    var sectionId = readNBytes(2);
                    var startTime = readNBytes(8);
                    readEvents(event);
                    var popCode = readNBytes(1);
                    console.assert(popCode == BinaryReader.Codes.POP_SECTION);
                    var endTime = readNBytes(8);
                    event.sectionId = binaryStringToNumber(sectionId);
                    event.startTime = binaryStringToNumber(startTime);
                    event.endTime   = binaryStringToNumber(endTime);
                } else if (code == BinaryReader.Codes.POP_SECTION) {
                    pushBackNBytes(1);
                    break;
                } else {
                    console.assert(code == BinaryReader.Codes.SECTION_NAMES);
                    break;
                }
            }
        };

        function readSectionNames(sectionNames) {
            while (true) {
                var length = readNBytes(1);
                if (length === '') {
                    break;
                } else {
                    var sectionName = readNBytes(length.charCodeAt(0));
                    sectionNames.push(sectionName);
                }
            }
        };

        var eventsRoot = { events: [] };
        readEvents(eventsRoot);

        var sectionNames = [];
        readSectionNames(sectionNames);

        var profiledEvents = eventsRoot.events;
        var startTime = profiledEvents[0].startTime;
        var endTime = profiledEvents[profiledEvents.length - 1].endTime;

        return {
            profiledEvents: profiledEvents,
            sectionNames:   sectionNames,
            startTime:      startTime,
            endTime:        endTime
        };
    }
};