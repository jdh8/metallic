import * as env from "./env.mjs";
import * as memory from "./memory.mjs";
import fs from "fs";

const { exports } = new WebAssembly.Instance(new WebAssembly.Module(fs.readFileSync(process.argv[2])), { env });
memory.set(exports.memory.buffer);
exports._start();
