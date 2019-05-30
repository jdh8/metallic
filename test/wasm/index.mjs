import fs from "fs";
import * as syscalls from "./syscalls.mjs";

WebAssembly.instantiate(fs.readFileSync(process.argv[2]), { env: syscalls }).then(module =>
{
	const { exports } = module.instance;
	syscalls.setup(exports.memory.buffer);
	exports._start();
})
.catch(error =>
{
	console.error(error);
	process.exit(1);
});
