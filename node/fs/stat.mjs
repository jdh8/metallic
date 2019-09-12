import cstring from "../internal/cstring.mjs";
import timespec from "../internal/timespec.mjs";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

const call = (stat, file, buffer, pointer) =>
{
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

	timespec(buffer, pointer + 64, BigInt(Math.round(1e6 * numbers.atimeMs)));
	timespec(buffer, pointer + 80, BigInt(Math.round(1e6 * numbers.mtimeMs)));
	timespec(buffer, pointer + 96, BigInt(Math.round(1e6 * numbers.ctimeMs)));
};

export const __stat = wrap((memory, path, pointer) => call(fs.statSync, cstring(memory.buffer, path), memory.buffer, pointer));
export const __fstat = wrap((memory, fd, pointer) => call(fs.fstatSync, fd, memory.buffer, pointer));
export const __lstat = wrap((memory, path, pointer) => call(fs.lstatSync, cstring(memory.buffer, path), memory.buffer, pointer));

export const __readlink = wrap((memory, path, pointer, size) =>
{
	const value = fs.readlinkSync(cstring(memory.buffer, path), "buffer");
	new Uint8Array(memory.buffer, pointer).set(value.slice(0, size));
	return value.length;
});
