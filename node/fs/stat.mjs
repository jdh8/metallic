import { buffer } from "../memory.mjs";
import cstring from "../internal/cstring.mjs";
import timespec from "../internal/timespec.mjs";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

const call = (stat, file, pointer) =>
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

	timespec(pointer + 64, BigInt(Math.round(1e6 * numbers.atimeMs)));
	timespec(pointer + 80, BigInt(Math.round(1e6 * numbers.mtimeMs)));
	timespec(pointer + 96, BigInt(Math.round(1e6 * numbers.ctimeMs)));
};

export const __stat = wrap((path, pointer) => call(fs.statSync, cstring(buffer, path), pointer));
export const __fstat = wrap((fd, pointer) => call(fs.fstatSync, fd, pointer));
export const __lstat = wrap((path, pointer) => call(fs.lstatSync, cstring(buffer, path), pointer));
