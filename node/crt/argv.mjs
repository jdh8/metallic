import { buffer } from "../memory.mjs";
import util from "util";

var TextEncoder = TextEncoder || util.TextEncoder;
const argv = process.argv.slice(2);
const args = new TextEncoder().encode(argv.join('\0') + '\0');

export const __argc = () => argv.length;

export const __argv = pointer =>
{
	if (pointer)
		new Uint8Array(buffer, pointer).set(args);

	return args.length;
};
