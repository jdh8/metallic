import util from "util";

var TextEncoder = TextEncoder || util.TextEncoder;
const argv = process.argv.slice(2);
const args = new TextEncoder().encode(argv.join('\0') + '\0');

export const __argc = () => argv.length;

export const __argv = (memory, pointer, size) =>
{
	new Uint8Array(memory.buffer, pointer).set(args.slice(0, size));
	return args.length;
};
