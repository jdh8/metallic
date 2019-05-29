import fs from "fs";

let memory;

const env =
{
	read: (fd, pointer, size) => fs.readSync(fd, new Uint8Array(memory, pointer, size)),
	write: (fd, pointer, size) => fs.writeSync(fd, new Uint8Array(memory, pointer, size)),
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
