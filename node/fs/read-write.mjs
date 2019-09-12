import errno from "../internal/errno.json";
import wrap from "../internal/wrap.mjs";
import fs from "fs-ext";

export const __read = wrap((memory, fd, ...rest) => fs.readSync(fd, new Uint8Array(memory.buffer), ...rest));
export const __write = wrap((memory, fd, ...rest) => fs.writeSync(fd, new Uint8Array(memory.buffer), ...rest));
export const __lseek = wrap((memory, ...x) => fs.seekSync(...x));

export const __llseek = wrap((memory, fd, high, low, result, whence) =>
{
	new BigUint64Array(memory.buffer, result)[0] = BigInt(fs.seekSync(fd, high * 2**32 + low, whence));
});

export const __pread = __read;
export const __pwrite = __write;
