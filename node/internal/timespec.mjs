import { buffer } from "../memory.mjs";

export default (pointer, nanos) =>
{
	const view = new DataView(buffer, pointer, 16);

	view.setBigInt64(0, nanos / 1000000000n, true);
	view.setInt32(8, Number(nanos % 1000000000n), true);
};
