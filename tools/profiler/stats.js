class Stats {

    constructor() {
        this.sections = {};
    }

    insertSection(sectionElement, sectionId, startTime, endTime) {
        var section = this.sections[sectionId];
        if (!section) {
            section = {
                worstDuration: 0,
                worstElement:  null,

                averageDuration: 0,
                numSections:     0
            };
            this.sections[sectionId] = section;
        }
        
        var duration = endTime - startTime;

        if (section.worstDuration < duration) {
            section.worstDuration = duration;
            section.worstElement  = sectionElement;
        }

        section.averageDuration = (section.averageDuration * section.numSections + duration) / (section.numSections + 1);
        ++section.numSections;
    }

}