export default f => (...x) =>
{
	try {
		return f(...x);
	}
	catch (e) {
		const errno = e.errno | 0;

		if (errno < 0)
			return errno;

		throw e;
	}
};
