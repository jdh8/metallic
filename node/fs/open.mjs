import { buffer } from "../memory.mjs";
import cstring from "../internal/cstring.mjs";
import wrap from "../internal/wrap.mjs";
import fs from "fs";
import util from "util";

var TextDecoder = TextDecoder || util.TextDecoder;

export const __open = wrap((path, flags, mode) => fs.openSync(cstring(buffer, path), flags, mode));
export const __close = wrap(fs.closeSync);
export const __access = wrap((path, mode) => fs.accessSync(cstring(buffer, path), mode));
export const __truncate = wrap((path, length) => fs.truncateSync(cstring(buffer, path), length));
export const __ftruncate = wrap(fs.ftruncateSync);
export const __chdir = wrap(path => process.chdir(new TextDecoder().decode(cstring(buffer, path))));
export const __fchdir = () => -38;
