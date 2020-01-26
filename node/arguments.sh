#!/bin/sh

if node --es-module-specifier-resolution=node -e ''; then
	echo --es-module-specifier-resolution=node
else
	echo --experimental-modules
fi
