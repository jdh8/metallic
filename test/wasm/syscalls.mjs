import fs from "fs";

let userspace;

export const setup = buffer => userspace = new Uint8Array(buffer);

export const read = (fd, pointer, size) => fs.readSync(fd, userspace, pointer, size);

export const write = (fd, pointer, size) => fs.writeSync(fd, userspace, pointer, size);
