# Screenshot

This program takes a screenshot from a VTK dataset in one file or series of files. This is useful when the data is located on a remote machine (a cluster) and ParaView (or X11 display in general) cannot be used.

**Input:** The input can be one or series of VTK data files with iterable file names. The input file extensions should be ``vtk``, ``vti``, ``vtr``, ``vts``, ``vtp``, ``vtu``.

**Output:** For each input file, a ``*.png`` image file is generated.

## Prerequisites

1. Install *GNU GCC* compiler, *OpenMP*, and *Cmake*:

       sudo apt-get install g++ libomp-dev cmake -y

2. Install *VTK* development:

       sudo apt install libvtk5-dev -y

   Alternatively, [download VTK](https://vtk.org/download/) and compile it.

## Install

Get the source code

    git clone git@github.com:ameli/screenshot.git
    cd screenshot
	
Compile in ``./build`` directory

    mkdir build
    cmake -B build .
    make -C build
	
After the compilation, the executable file is located in ``./bin/screenshot``.

## Usage

    ./bin/screenshot InputBaseFilename  FileStartIndex  FileEndIndex FileExtension

* ``InputBaseFilename``: is the input filename without the iterator and file extension.
* ``FileStartIndex`` and ``FileEndIndex``: the start and end of iterator on file name.
* ``FileExtension`` is one of ``vtk``, ``vti``, ``vtr``, ``vts``, ``vtp``, ``vtu``.

## Example

    ./bin/screenshot  MyFile-  1  20  vtk

This example reads the files ``MyFile-1.vtk`` to ``MyFile-20.vtk`` and the generates screenshot files ``MyFile-1.png`` to ``MyFile-20.png``.

## License

BSD 3-clause.
