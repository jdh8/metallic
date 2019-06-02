import { buffer } from "../memory.mjs";
import util from "util";

var TextEncoder = TextEncoder || util.TextEncoder;
const [,, ...argv] = process.argv;

export const __argc = () => argv.length;

export const __argv = pointer => new Uint32Array(buffer, pointer)
	.set(argv.map(s => s.length + 1).reduce((x, y, i) => [...x, x[i] + y], [0]));

export const __args = pointer => new Uint8Array(buffer, pointer)
	.set(new TextEncoder().encode(argv.join('\0') + '\0'));
