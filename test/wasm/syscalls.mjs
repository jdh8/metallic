import fs from "fs";

let userspace;

export const __setup = buffer => userspace = new Uint8Array(buffer);

export const __read = (fd, pointer, size) => fs.readSync(fd, userspace, pointer, size);

export const __write = (fd, pointer, size) => fs.writeSync(fd, userspace, pointer, size);
