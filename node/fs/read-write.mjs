import { buffer } from "../memory.mjs";
import errno from "../internal/errno.json";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

export const __read = wrap((fd, ...rest) => fs.readSync(fd, new Uint8Array(buffer), ...rest));
export const __write = wrap((fd, ...rest) => fs.writeSync(fd, new Uint8Array(buffer), ...rest));
export const __lseek = () => -errno.ENOSYS;
export const __llseek = () => -errno.ENOSYS;
export const __pread = __read;
export const __pwrite = __write;
