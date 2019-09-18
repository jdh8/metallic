import syscalls from "../build/Release/syscalls.node";
import cstring from "../internal/cstring.mjs";
import errno from "../internal/errno.json";
import wrap from "../internal/wrap.mjs";
import fs from "fs";
import util from "util";

var TextDecoder = TextDecoder || util.TextDecoder;

const prepare = f => (memory, path, ...rest) => f(cstring(memory.buffer, path), ...rest);
const uncurry = f => (memory, ...x) => f(...x);

export const __open = wrap(prepare(fs.openSync));
export const __close = wrap(uncurry(fs.closeSync));
export const __access = wrap(prepare(fs.accessSync));
export const __truncate = wrap(prepare(fs.truncateSync));
export const __ftruncate = wrap(uncurry(fs.ftruncateSync));
export const __chdir = wrap((memory, path) => process.chdir(new TextDecoder().decode(cstring(memory.buffer, path))));
export const __fchdir = syscalls.fchdir;
export const __chmod = wrap(prepare(fs.chmodSync));
export const __fchmod = wrap(uncurry(fs.fchmodSync));
export const __chown = wrap(prepare(fs.chownSync));
export const __fchown = wrap(uncurry(fs.fchownSync));
export const __lchown = wrap(prepare(fs.lchownSync));

export const __fallocate32 = syscalls.fallocate32
|| wrap((memory, fd, offset_high, offset_low, length_high, length_low) =>
{
	const offset = offset_high * 2**32 + offset_low;
	const length = length_high * 2**32 + length_low;
	const size = offset + length;

	if (fs.fstatSync(fd).size < size)
		fs.ftruncateSync(fd, size);
});
