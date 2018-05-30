class BinaryTreeView {

    constructor(startTime, endTime) {
        this.tree = {
            startTime: startTime,
            endTime: endTime,
            items: [],
            left: null,
            right: null
        }
        this.startTime = startTime;
        this.endTime = endTime;
        this.duration = endTime - startTime;
        this.sectionsDepth = 0;
        this.maxTreeDepth = 10;
    }

    insertSection(sectionElement, sectionId, startTime, endTime, sectionDepth) {
        if (sectionDepth > this.sectionsDepth) {
            this.sectionsDepth = sectionDepth;
        }

        this.insertItem(this.tree, sectionElement, startTime, endTime, 1);
    }

    insertItem(node, sectionElement, startTime, endTime, treeDepth) {
        if (treeDepth == this.maxTreeDepth) {
            node.items.push(sectionElement);
            return;
        }

        var middle = (node.startTime + node.endTime) / 2;
        if (node.startTime <= startTime && endTime <= middle) {
            if (!node.left) {
                node.left = {
                    startTime: node.startTime,
                    endTime: middle,
                    items: [],
                    left: null,
                    right: null
                }
            }
            this.insertItem(node.left, sectionElement, startTime, endTime, treeDepth + 1);
        } else if (middle <= startTime && endTime <= node.endTime) {
            if (!node.right) {
                node.right = {
                    startTime: middle,
                    endTime: node.endTime,
                    items: [],
                    left: null,
                    right: null
                }
            }
            this.insertItem(node.right, sectionElement, startTime, endTime, treeDepth + 1);
        } else {
            node.items.push(sectionElement);
        }
    }

    getSectionsInRange(startTime, endTime) {
        var result = new Set();
        this.findSectionsInRange(result, this.tree, startTime, endTime);
        return result;
    }

    rangesOverlap(s1, e1, s2, e2) {
        return e1 > s2 && s1 < e2;
    }

    findSectionsInRange(result, node, startTime, endTime) {
        if (this.rangesOverlap(node.startTime, node.endTime, startTime, endTime)) {
            for (var sectionElement of node.items) {
                var itemStartTime = parseInt(sectionElement.getAttribute('data-start-time'));
                var itemEndTime = parseInt(sectionElement.getAttribute('data-end-time'));
                if (this.rangesOverlap(startTime, endTime, itemStartTime, itemEndTime)) {
                    result.add(sectionElement);
                }
            }

            if (node.left) {
                this.findSectionsInRange(result, node.left, startTime, endTime);
            }
            if (node.right) {
                this.findSectionsInRange(result, node.right, startTime, endTime);
            }
        }
    }

}