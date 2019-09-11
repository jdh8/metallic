import { buffer } from "../memory.mjs";
import errno from "../internal/errno.json";
import wrap from "../internal/wrap.mjs";
import fs from "fs-ext";

export const __read = wrap((fd, ...rest) => fs.readSync(fd, new Uint8Array(buffer), ...rest));
export const __write = wrap((fd, ...rest) => fs.writeSync(fd, new Uint8Array(buffer), ...rest));
export const __lseek = wrap(fs.seekSync);

export const __llseek = wrap((fd, high, low, result, whence) =>
{
	new BigUint64Array(buffer, result)[0] = fs.seekSync(fd, high * 2**32 + low, whence);
});

export const __pread = __read;
export const __pwrite = __write;
