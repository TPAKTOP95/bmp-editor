#!/bin/bash
set -euo pipefail
[ "$#" -ge 1 ] || (echo "Expected at least one argument: command to run" && exit 1)

echo "Rotating small-one without crop..."
"$@" crop-rotate test-data/small-one.bmp test-data/small-one-out.bmp 0 0 3 2
cmp test-data/small-one-out.bmp test-data/small-one-rotated.bmp
echo "    ok"

echo "Cropping and rotating Lena (1)..."
"$@" crop-rotate test-data/lena-512.bmp test-data/lena-512-out.bmp 300 240 80 60
cmp test-data/lena-512-out.bmp test-data/lena-512-cropped-rotated-1.bmp
echo "    ok"

# Careful with binary mode!
echo "Cropping and rotating Lena (2)..."
"$@" crop-rotate test-data/lena-512.bmp test-data/lena-512-out.bmp 100 400 100 100
cmp test-data/lena-512-out.bmp test-data/lena-512-cropped-rotated-2.bmp
echo "    ok"

echo "Failing at monochrome BMP..."
set +e
"$@" crop-rotate test-data/monochrome.bmp test-data/monochrome-out.bmp 0 0 15 10 >.out.tmp 2>.err.tmp
RET_CODE=$?
set -e
echo "    exited with code $RET_CODE (expected 1)"
echo "    stdout was (expected empty):"
cat .out.tmp
[ $RET_CODE == 1 ]
! [ -s .out.tmp ]
echo "    checking stderr..."
diff -bB .err.tmp <(echo "Invalid BMP: expected 24 bits per pixel, got 1")
echo "    ok"

echo "All ok."
