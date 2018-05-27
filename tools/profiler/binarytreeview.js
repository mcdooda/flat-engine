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
        this.depth = 0;
        this.maxTreeDepth = 5;
    }

    insertSection(sectionId, sectionName, startTime, endTime, depth) {
        var item = document.createElement('li');
        item.classList.add('section');
        item.classList.add('section-' + sectionId);
        item.setAttribute('data-start-time', startTime);
        item.setAttribute('data-end-time', endTime);
        item.setAttribute('data-depth', depth);
        item.style.top = (depth * 20) + 'px';
        var text = document.createElement('span');
        var textContent = sectionName + ': ' + (endTime - startTime) / 1000000 + 'ms';
        text.textContent = textContent;
        text.setAttribute('title', textContent);
        item.appendChild(text);

        if (depth > this.depth)
            this.depth = depth;

        this.insertItem(this.tree, item, startTime, endTime, 1);
    }

    insertItem(node, item, startTime, endTime, treeDepth) {
        if (treeDepth == this.maxTreeDepth) {
            node.items.push(item);
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
            this.insertItem(node.left, item, startTime, endTime, treeDepth + 1);
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
            this.insertItem(node.right, item, startTime, endTime, treeDepth + 1);
        } else {
            node.items.push(item);
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
            for (var item of node.items) {
                var itemStartTime = parseInt(item.getAttribute('data-start-time'));
                var itemEndTime = parseInt(item.getAttribute('data-end-time'));
                if (this.rangesOverlap(startTime, endTime, itemStartTime, itemEndTime)) {
                    result.add(item);
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