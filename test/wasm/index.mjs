import { env, memory } from "../../node/index.mjs";
import fs from "fs";

WebAssembly.instantiate(fs.readFileSync(process.argv[2]), { env }).then(module =>
{
	const { exports } = module.instance;
	memory.set(exports.memory.buffer);
	exports._start();
})
.catch(error =>
{
	console.error(error);
	process.exit(1);
});
