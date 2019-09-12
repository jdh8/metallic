import errno from "../internal/errno.json";
import timespec from "../internal/timespec.mjs";
import perf from "perf_hooks";

export const __clock_settime = (memory, id) => id ? -errno.EINVAL : -errno.EPERM;

export const __clock_gettime = (memory, id, pointer) =>
{
	const realtime = () => 1000000n * BigInt(Date.now());
	const monotonic = () => process.hrtime.bigint();
	const cputime = () => BigInt(Math.round(1e6 * perf.performance()));
	const clocks = [realtime, monotonic, cputime];
	const i = id >>> 0;

	if (i >= clocks.length)
		return -errno.EINVAL;

	timespec(memory.buffer, pointer, clocks[i]());
};

export const __clock_getres = (memory, id, pointer) =>
{
	const resolutions = [1000000n, 1n, 1n];
	const i = id >>> 0;

	if (i >= resolutions.length)
		return -errno.EINVAL;

	timespec(memory.buffer, pointer, resolutions[i]);
};
