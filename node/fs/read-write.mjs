import { buffer } from "../memory.mjs";
import fs from "fs";

export const __read = (fd, pointer, size) => fs.readSync(fd, new Uint8Array(buffer, pointer, size));
export const __write = (fd, pointer, size) => fs.writeSync(fd, new Uint8Array(buffer, pointer, size));
export const __lseek = () => -38;
export const __llseek = () => -38;
