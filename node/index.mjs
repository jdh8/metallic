import * as env from "./env.mjs";
import load from "./memory.mjs";
import fs from "fs";

const { exports } = new WebAssembly.Instance(new WebAssembly.Module(fs.readFileSync(process.argv[2])), { env });
load(exports.memory.buffer);
exports._start();
