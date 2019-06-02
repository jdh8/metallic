export default (buffer, pointer) => new Uint8Array(buffer, pointer, new Uint8Array(buffer, pointer).indexOf(0));
