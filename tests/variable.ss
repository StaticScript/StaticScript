let ai: int;
let af: number;
let ab: boolean;
let as: string;

ss_assert(ai == 0);
ss_assert(af == 0);
ss_assert(ab == false);
ss_assert(as == "");

// =-----------------------------------------------=

let bi: int = 1;
let bf: number = 1.0;
let bb: boolean = false;
let bs: string = "string content";

ss_assert(bi == 1);
ss_assert(bf == 1.0);
ss_assert(bb == false);
ss_assert(bs == "string content");

// =-----------------------------------------------=

let ci = 1;
let cf = 1.0;
let cb = false;
let cs = "string content";

ss_assert(ci == 1);
ss_assert(cf == 1.0);
ss_assert(cb == false);
ss_assert(cs == "string content");

// =-----------------------------------------------=

const di: int = 1;
const df: number = 1.0;
const db: boolean = false;
const ds: string = "string content";

ss_assert(di == 1);
ss_assert(df == 1.0);
ss_assert(db == false);
ss_assert(ds == "string content");

// =-----------------------------------------------=

const ei = 1;
const ef = 1.0;
const eb = false;
const es = "string content";

ss_assert(ei == 1);
ss_assert(ef == 1.0);
ss_assert(eb == false);
ss_assert(es == "string content");