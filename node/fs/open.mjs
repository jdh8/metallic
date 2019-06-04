import { buffer } from "../memory.mjs";
import cstring from "../internal/cstring.mjs";
import wrap from "../internal/wrap.mjs";
import fs from "fs";
import util from "util";

var TextDecoder = TextDecoder || util.TextDecoder;

const path = f => (path, ...rest) => f(cstring(buffer, path), ...rest);

export const __open = wrap(path(fs.openSync));
export const __close = wrap(fs.closeSync);
export const __access = wrap(path(fs.accessSync));
export const __truncate = wrap(path(fs.truncateSync));
export const __ftruncate = wrap(fs.ftruncateSync);
export const __chdir = wrap(path => process.chdir(new TextDecoder().decode(cstring(buffer, path))));
export const __fchdir = () => -38;
export const __chmod = wrap(path(fs.chmodSync));
export const __fchmod = wrap(fs.fchmodSync);
export const __chown = wrap(path(fs.chownSync));
export const __fchown = wrap(fs.fchownSync);
export const __lchown = wrap(path(fs.lchownSync));
