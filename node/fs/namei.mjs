import cstring from "../internal/cstring.mjs";
import errno from "../internal/errno.json";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

const unary = f => (memory, path, ...rest) => f(cstring(memory.buffer, path), ...rest);
const binary = f => (memory, from, to, ...rest) => f(cstring(memory.buffer, from), cstring(memory.buffer, to), ...rest);

export const __rename = wrap(binary(fs.renameSync));
export const __mkdir = wrap(unary(fs.mkdirSync));
export const __rmdir = wrap(unary(fs.rmdirSync));
export const __link = wrap(binary(fs.linkSync));
export const __unlink = wrap(unary(fs.unlinkSync));
export const __symlink = wrap(binary(fs.symlinkSync));
export const __mknod = () => -errno.ENOSYS;
