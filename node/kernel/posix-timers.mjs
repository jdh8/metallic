import timespec from "../internal/timespec.mjs";
import perf from "perf_hooks";

export const __clock_settime = id => id ? -22 : -1;

export const __clock_gettime = (id, pointer) =>
{
	const realtime = () => 1000000n * BigInt(Date.now());
	const monotonic = () => process.hrtime.bigint();
	const cputime = () => BigInt(Math.round(1e6 * perf.performance()));
	const clocks = [realtime, monotonic, cputime];

	if (id >= clocks.length)
		return -22;

	timespec(pointer, clocks[id]());
};

export const __clock_getres = (id, pointer) =>
{
	const resolutions = [1000000n, 1n, 1n];

	if (id >= resolutions.length)
		return -22;

	timespec(pointer, resolutions[id]);
};
