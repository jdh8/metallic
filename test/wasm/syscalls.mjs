import fs from "fs";

const wrap = f => (...x) =>
{
	try {
		return f(...x);
	}
	catch (e) {
		const errno = e.errno | 0;

		if (errno)
			return errno;

		throw e;
	}
};

let userspace;

export const __setup = buffer =>
{
	const valid = buffer instanceof ArrayBuffer;

	if (valid)
		userspace = new Uint8Array(buffer);

	return valid - 1;
}

export const __read = wrap((fd, pointer, size) => fs.readSync(fd, userspace, pointer, size));

export const __write = wrap((fd, pointer, size) => fs.writeSync(fd, userspace, pointer, size));

export const __lseek = () => -38;
