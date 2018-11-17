import fs from "fs";

let memory;

const env =
{
	__stderr: (pointer, size) => size * process.stderr.write(Buffer.from(memory, pointer, size)),
}

WebAssembly.instantiate(fs.readFileSync(process.argv[2]), { env }).then(module =>
{
	const { exports } = module.instance;
	memory = exports.memory.buffer;
	exports._start();
})
.catch(error =>
{
	console.error(error);
	process.exit(1);
});
