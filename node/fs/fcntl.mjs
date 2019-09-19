import syscalls from "../build/Release/syscalls.node";
import errno from "../internal/errno.json";

export const __fcntl = syscalls.fcntl || (() => -errno.EINVAL);
