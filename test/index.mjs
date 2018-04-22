import fs from "fs";

process.argv.slice(2).map(async path => await WebAssembly.instantiate(fs.readFileSync(path)));
