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

const callstat = (stat, file, pointer) =>
{
	const { buffer } = userspace;
	const view = new DataView(buffer, pointer, 64);
	const bigints = stat(file, { bigint: true });
	const numbers = stat(file);

	view.setBigUint64(0, bigints.dev, true);
	view.setBigUint64(8, bigints.ino, true);
	view.setUint32(16, numbers.mode, true);
	view.setUint32(20, numbers.nlink, true);
	view.setUint32(24, numbers.uid, true);
	view.setUint32(28, numbers.gid, true);
	view.setBigUint64(32, bigints.rdev, true);
	view.setBigInt64(40, bigints.size, true);
	view.setInt32(48, numbers.blksize, true);
	view.setBigInt64(56, bigints.blocks, true);

	settime(new DataView(buffer, pointer + 64, 16), numbers.atimeMs);
	settime(new DataView(buffer, pointer + 80, 16), numbers.mtimeMs);
	settime(new DataView(buffer, pointer + 96, 16), numbers.ctimeMs);
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

export const __stat = wrap((path, pointer) => callstat(fs.statSync, cstring(path), pointer));

export const __fstat = wrap((fd, pointer) => callstat(fs.fstatSync, fd, pointer));

export const __lstat = wrap((path, pointer) => callstat(fs.lstatSync, cstring(path), pointer));

export const __lseek = () => -38;
