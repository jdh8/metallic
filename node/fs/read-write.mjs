import syscalls from "../build/Release/syscalls.node";
import errno from "../internal/errno.json";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

export const __read = wrap((memory, fd, ...rest) => fs.readSync(fd, new Uint8Array(memory.buffer), ...rest));
export const __write = wrap((memory, fd, ...rest) => fs.writeSync(fd, new Uint8Array(memory.buffer), ...rest));
export const __lseek = syscalls.lseek;
export const __llseek = syscalls.llseek;
export const __pread = __read;
export const __pwrite = __write;
