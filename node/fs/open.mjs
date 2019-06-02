import { buffer } from "../memory.mjs";
import cstring from "../internal/cstring.mjs";
import wrap from "../internal/wrap.mjs";
import fs from "fs";

export const __open = wrap((path, flags, mode) => fs.openSync(cstring(buffer, path), flags, mode));
export const __close = wrap(fd => fs.closeSync(fd));
