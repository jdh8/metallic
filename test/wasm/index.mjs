import fs from "fs";

let memory;

const write = stream => (pointer, size) => size * stream.write(new Uint8Array(memory, pointer, size));
const putc = stream => c => stream.write(new Uint8Array([c])) ? c & 255 : -1;

const env =
{
	__stdin: (pointer, size) => fs.readSync(0, memory, pointer, size),

	__getchar()
	{
		const buffer = new Uint8Array(1);
		return fs.readSync(0, buffer, 0, 1) ? buffer[0] : -1;
	},

	__stdout: write(process.stdout),
	__stderr: write(process.stderr),
	__putchar: putc(process.stdout),
	__putcerr: putc(process.stderr),
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
