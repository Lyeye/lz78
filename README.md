# lz78

A naive implementation of Lempel-Ziv-78 compression and decompression to help me understand the algorithm. Works well for small files. Performs badly for larger files in most cases because the dictionary size is not limited and therefore several bytes may be used for a dictionary entry reference.

## Compilation
```
make
```

## Usage
compression:
```
./lz78 c input compressed
```

decompression:
```
./lz78 d compressed output
```

The ```test.sh``` script compresses the source file ```lz78.cpp``` and then decompresses it again. It then verifies that the two files match using ```diff```.

## TODO
* limit dictionary size
* output with bit granularity