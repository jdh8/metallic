import * as library from "./library.mjs";
import fs from "fs";

const exec = code =>
{
	const env = Object.create(null);
	let memory;

	for (const key in library)
		env[key] = (...x) => library[key](memory, ...x);

	const { exports } = new WebAssembly.Instance(new WebAssembly.Module(code), { env });
	memory = exports.memory;
	exports._start();
};

exec(fs.readFileSync(process.argv[2]));
