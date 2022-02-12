import { HeapValue, Heap } from './Heap.mjs';

let heap = new Heap((A,B)=>A.v>B.v);

let a = {v:2};
let b = {v:1};
let c = {v:3};
let d = {v:4};
let e = {v:5};

let aH = new HeapValue(a);
let bH = new HeapValue(b);
let cH = new HeapValue(c);
let dH = new HeapValue(d);
let eH = new HeapValue(e);

heap.add(aH);
heap.add(bH);
heap.add(cH);
heap.add(dH);
heap.add(eH);

console.log(heap.get().value);

heap.delete(bH);
console.log(heap.get().value);
c.v = 1;
heap.adjust(cH);
console.log(heap.get().value);
heap.delete(aH);
heap.delete(cH);

console.log(heap.pop().value);
heap.add(aH);
console.log(heap.get().value);

