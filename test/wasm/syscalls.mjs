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

const settime = (view, date) =>
{
	const seconds = Math.trunc(date / 1000);

	view.setBigInt64(0, BigInt(seconds), true);
	view.setInt32(8, 1e6 * (date - 1000 * seconds), true);
};

const setstat = (view, big, normal) =>
{
	view.setBigUint64(0, big.dev, true);
	view.setBigUint64(8, big.ino, true);
	view.setUint32(16, normal.mode, true);
	view.setUint32(20, normal.nlink, true);
	view.setUint32(24, normal.uid, true);
	view.setUint32(28, normal.gid, true);
	view.setBigUint64(32, big.rdev, true);
	view.setBigInt64(40, big.size, true);
	view.setInt32(48, normal.blksize, true);
	view.setBigInt64(56, big.blocks, true);

	const buffer = view.buffer;
	const pointer = view.byteOffset;

	settime(new DataView(buffer, pointer + 64, 16), normal.atimeMs);
	settime(new DataView(buffer, pointer + 80, 16), normal.mtimeMs);
	settime(new DataView(buffer, pointer + 96, 16), normal.ctimeMs);
};

let userspace;

const cstring = pointer => userspace.slice(pointer, userspace.indexOf(0, pointer));

export const __setup = buffer =>
{
	const valid = buffer instanceof ArrayBuffer;

	if (valid)
		userspace = new Uint8Array(buffer);

	return valid - 1;
}

export const __read = wrap((fd, pointer, size) => fs.readSync(fd, userspace, pointer, size));

export const __write = wrap((fd, pointer, size) => fs.writeSync(fd, userspace, pointer, size));

export const __open = wrap((path, flags, mode) => fs.openSync(cstring(path), flags, mode));

export const __close = wrap(fd => fs.closeSync(fd));

export const __stat = wrap((path, pointer) =>
{
	const string = cstring(path);
	const view = new DataView(userspace.buffer, pointer, 112);

	setstat(view, fs.statSync(string, { bigint: true }), fs.statSync(string));
});

export const __lseek = () => -38;
