import fs from "fs";
import perf from "perf_hooks";

const wrap = f => (...x) =>
{
	try {
		return f(...x);
	}
	catch (e) {
		const errno = e.errno | 0;
		const EFAULT = -14;

		if (errno < 0)
			return errno;

		if (e instanceof RangeError)
			return EFAULT;

		throw e;
	}
};

const timespec = (view, nanoseconds) =>
{
	const seconds = nanoseconds / 1000000000n;
	const nanos = nanoseconds % 1000000000n;

	view.setBigInt64(0, seconds, true);
	view.setInt32(8, Number(nanos), true);
};

let userspace;

const struct = (pointer, size) => new DataView(userspace.buffer, pointer, size);

const callstat = (stat, file, pointer) =>
{
	const view = struct(pointer, 64);
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

	timespec(struct(pointer + 64, 16), BigInt(Math.round(1e6 * numbers.atimeMs)));
	timespec(struct(pointer + 80, 16), BigInt(Math.round(1e6 * numbers.mtimeMs)));
	timespec(struct(pointer + 96, 16), BigInt(Math.round(1e6 * numbers.ctimeMs)));
};

const cstring = pointer => userspace.slice(pointer, userspace.indexOf(0, pointer));

const enosys = () => -38;

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

export const __poll = enosys;

export const __lseek = enosys;

export const __clock_gettime = wrap((id, pointer) =>
{
	const realtime = () => 1000000n * BigInt(Date.now());
	const monotonic = () => process.hrtime.bigint();
	const cputime = () => BigInt(Math.round(1e6 * perf.performance()));
	const clocks = [realtime, monotonic, cputime];
	const EINVAL = -22;

	if (id >= clocks.length)
		return EINVAL;

	timespec(struct(pointer, 16), clocks[id]());
});

export const __clock_getres = wrap((id, pointer) =>
{
	const resolutions = [1000000n, 1n, 1n];
	const EINVAL = -22;

	if (id >= resolutions.length)
		return EINVAL;

	timespec(struct(pointer, 16), resolutions[id]);
});
