import cstring from "../internal/cstring.mjs";
import errno from "../internal/errno.json";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

class DIR
{
	constructor(path)
	{
		this.path = path;
		this.list = fs.readdirSync(path, "buffer");
		this.iterator = this.list.values();
	}

	rewind()
	{
		try { this.list = fs.readdirSync(this.path, "buffer"); }
		catch (ignored) {}

		this.iterator = this.list.values();
	}
};

const extract = (buffer, pointer) => new Uint32Array(buffer, pointer, 1)[0];

export const __opendir = wrap((memory, pointer, path) =>
{
	const view = new Uint32Array(memory.buffer, pointer, 1);
	const index = memory.dirs.push(new DIR(cstring(memory.buffer, path))) - 1;

	view[0] = index;
});

export const __closedir = (memory, directory) =>
{
	delete memory.dirs[extract(memory.buffer, directory)];
};

export const __readdir = wrap((memory, directory, entry) =>
{
	const iteration = memory.dirs[extract(memory.buffer, directory)].iterator.next();

	if (!iteration.done) {
		const result = new Uint8Array(memory.buffer, entry);
		result.set(iteration.value);
		result[iteration.value.length] = 0;
		return 1;
	}
});

export const __rewinddir = wrap((memory, directory) => memory.dirs[extract(memory.buffer, directory)].rewind());
