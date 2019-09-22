import cstring from "../internal/cstring.mjs";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

const DIR = path =>
{
	const list = fs.readdirSync(path, "buffer");
	const iterator = list.values();
	return { path, list, iterator };
};

export const __opendir = wrap((memory, pointer, path) =>
{
	const view = new Uint32Array(memory.buffer, pointer);

	view[0] = memory.dictionary.push(DIR(cstring(memory.buffer, path))) - 1;
});

export const __closedir = (memory, directory) =>
{
	delete memory.dictionary[new Uint32Array(memory.buffer, directory)[0]];
};

export const __readdir = (memory, directory, entry) =>
{
	const iteration = memory.dictionary[new Uint32Array(memory.buffer, directory)[0]].iterator.next();

	if (!iteration.done) {
		const result = new Uint8Array(memory.buffer, entry);
		result.set(iteration.value);
		result[iteration.value.length] = 0;
		return 1;
	}
};

export const __rewinddir = (memory, directory) =>
{
	const index = new Uint32Array(memory.buffer, directory)[0];
	const alias = memory.dictionary[index];

	try { memory.dictionary[index] = DIR(alias.path) }
	catch (e) { alias.iterator = alias.list.values() }
};
