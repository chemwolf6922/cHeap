// @ts-check

class HeapNode{
    /**
     * @type {HeapNode}
     */
    left = undefined;
    /**
     * @type {HeapNode}
     */
    right = undefined;
    /**
     * @type {HeapNode}
     */
    top = undefined;
    /**
     * 
     * @param {HeapValue} value 
     */
    constructor(value) {
        this.value = value;   
    }

    get dump(){
        return {
            value:this.value,
            left:this.left?.dump,
            right:this.right?.dump
        };
    }
}

export class HeapValue{
    /** @type {HeapNode} */
    node = undefined;
    value = undefined;
    /**
     * @param {any} value 
     */
    constructor(value) {
        this.value = value;
    }
}

export class Heap{
    /**
     * @type {HeapNode}
     */
    #entry = undefined;
    #length = 0;
    /**
     * @param {(A:any,B:any)=>boolean} compare the relation of bottom->top
     */
    constructor(compare) {
        /**
         * 
         * @param {HeapNode} n1 
         * @param {HeapNode} n2 
         * @returns {Boolean}
         */
        this.#compare = (n1,n2)=>{
            return compare(n1.value.value,n2.value.value);
        };
    }

    /**
     * 
     * @param {HeapValue} value 
     */
    add(value){
        let newNode = new HeapNode(value);
        value.node = newNode;
        if(this.#entry === undefined){
            this.#entry = newNode;
            this.#length ++;
            return;
        }
        let {node,branch} = this.#getTop(++this.#length);
        node[branch] = newNode;
        newNode.top = node;
        this.#adjustBackward(newNode);
    }

    /**
     * @param {HeapValue} value 
     */
    delete(value){
        let node = value.node;
        if(node === undefined){
            return false;
        }
        if(this.#length === 1){
            this.#entry = undefined;
            this.#length --;
            return true;
        }
        let tailTop = this.#getTop(this.#length);
        let tailNode = tailTop.branch==='left'?tailTop.node.left:tailTop.node.right;
        node.value = tailNode.value;
        node.value.node = node;
        this.#length --;
        if(tailTop.branch==='left'){
            tailTop.node.left = undefined;
        }else{
            tailTop.node.right = undefined;
        }
        if(node !== tailNode){
            node = this.#adjustBackward(node);
            this.#adjustForeward(node);
        }
        return true;
    }

    /**
     * @param {HeapValue} value 
     */
    adjust(value){
        let node = value.node;
        if(node === undefined){
            return false;
        }
        node = this.#adjustBackward(node);
        this.#adjustForeward(node);
        return true;
    }

    /**
     * @returns {HeapValue}
     */
    get(){
        return this.#entry?.value;
    }

    /**
     * @returns {HeapValue}
     */
    pop(){
        const result = this.#entry?.value;
        // TODO: remove entry
        if(this.#length === 1){
            this.#length --;
            this.#entry = undefined;
        }else if(this.#length > 1){
            let {node,branch} = this.#getTop(this.#length);
            this.#entry.value = node[branch].value;
            this.#entry.value.node = this.#entry;
            node[branch] = undefined;
            this.#length --;
            this.#adjustForeward(this.#entry);
        }
        if(result !== undefined){
            result.node = undefined;
        }
        return result;
    }

    /**
     * @param {HeapNode} A 
     * @param {HeapNode} B
     * @returns {boolean} 
     */
    #compare = (A,B)=>undefined;

    /**
     * 
     * @param {HeapNode} node the top node  
     * @returns {HeapNode}
     */
    #adjustForeward(node){
        let targetNode = undefined;
        if(node.left !== undefined && node.right === undefined){
            if(this.#compare(node,node.left)){
                targetNode = node.left;
            }
        }else if(node.left === undefined && node.right !== undefined){
            if(this.#compare(node,node.right)){
                targetNode = node.right;
            }
        }else if(node.left !== undefined && node.right !== undefined){
            let lessNode = undefined;
            if(this.#compare(node.left,node.right)){
                lessNode = node.right;
            }else{
                lessNode = node.left;
            }
            if(this.#compare(node,lessNode)){
                targetNode = lessNode;
            }
        }
        if(targetNode !== undefined){
            // exchange value instead of adjusting connection
            let tempValue = node.value;
            node.value = targetNode.value;
            node.value.node = node;
            targetNode.value = tempValue;
            targetNode.value.node = targetNode;
            return this.#adjustForeward(targetNode);
        }else{
            return node;
        }
    }

    /**
     * 
     * @param {HeapNode} node the bottom node 
     * @returns {HeapNode}
     */
    #adjustBackward(node){
        if(node.top === undefined){
            return node;
        }
        if(this.#compare(node.top,node)){
            // exchange value instead of adjusting connection
            let tempValue = node.value;
            node.value = node.top.value;
            node.value.node = node;
            node.top.value = tempValue;
            node.top.value.node = node.top;
            return this.#adjustBackward(node.top);
        }else{
            return node;
        }
    }

    /**
     * 
     * @param {Number} n starts at 1
     * @returns {{
     *      node:HeapNode,
     *      branch:String
     * }} 
     */
    #getTop(n){
        if(n===1){
            return undefined;
        }
        let node = this.#entry;
        const layers = 32 - Math.clz32(n);
        for(let i=layers-2;i>0;i--){
            if(((n>>>i)&1)===0){
                node = node?.left;
            }else{
                node = node?.right;
            }
        }
        return {node,branch:n&1?'right':'left'};
    }

    get dump(){
        return this.#entry?.dump;
    }

    get length(){
        return this.#length;
    }
}




