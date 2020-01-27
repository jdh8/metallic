#!/bin/sh

if node --experimental-modules --es-module-specifier-resolution=node -e ''; then
	echo --experimental-modules --es-module-specifier-resolution=node
else
	echo --experimental-modules
fi
