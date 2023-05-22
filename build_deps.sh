#!/usr/bin/env sh
base=$(dirname $(readlink -f "$0"))
deps=$base/deps
mkdir -p $deps/{lib,include}

required=("git" "make" "wget" "cmake" "g++")
for cmd in "${required[@]}"; do
	if !command -v "$cmd" &> /dev/null; then
		echo "$cmd not found. Please install."
		exit 1
	fi
done

download() {
	set -e
	dir=$(mktemp -d) && cd "$dir"
	git init
	git remote add origin "$1" 2>/dev/null
	git fetch origin
	git reset --hard "origin/$2"

	cp -r include/* "$deps/include"
	mkdir -p build && cd build && cmake ..
	make -j8
	cp -r lib/* "$deps/lib"

	rm -rf "$dir"
}

download "https://github.com/SFML/SFML" "2.5.x"&
download "https://github.com/MiguelMJ/Candle" "master"&
wget -O "$deps/include/doctest.h" "https://github.com/doctest/doctest/releases/download/v2.4.11/doctest.h"&
wait