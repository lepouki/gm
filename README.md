# gm - GPU Mandelbrot

Very simple visualization of the Mandelbrot set run on the GPU.

## Example output

The output is relatively simple because this was a school project and I can't be bothered.

![Example image.](example.png "Example image.")

## Cloning
Clone recursively to get the required submodules, for example with git:

```
git clone https://github.com/lepouki/gm --recursive
```

## Building

Using CMake in the root folder:

```sh
# Create a build folder.
mkdir build

# Run CMake inside and compile the executable.
# The debug build has some additional output so you can add the CMake option
# `-DCMAKE_BUILD_TYPE=Release` to build in release mode.
cmake build && make
```

Or using the CMake GUI.
