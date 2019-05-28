import fs from "fs";

let memory;

const write = stream => (pointer, size) => size * stream.write(new Uint8Array(memory, pointer, size));

const env =
{
	__stdin: (pointer, size) => fs.readSync(0, memory, pointer, size),
	__stdout: write(process.stdout),
	__stderr: write(process.stderr),
};

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
