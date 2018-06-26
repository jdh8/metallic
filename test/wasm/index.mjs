import fs from "fs";

WebAssembly.instantiate(fs.readFileSync(process.argv[2])).then(module => module.instance.exports._start());
