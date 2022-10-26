# Screenshot

Take screenshot from a VTK dataset in one file or series of files. This is useful when the data is located on a remote machine (a cluster) and ParaView (or X11 display in general) cannot be used.

## Install

    git clone git@github.com:ameli/screenshot.git
    cd screenshot
    mkdir build bin
    cd build
    cmake ..
    make
    cd ..

## Usage

    /bin/screenshot InputBaseFilename  FileStartIndex  FileEndIndex FileExtension

* ``InputBaseFilename``: is the input filename without the iterator and file extension.
* ``FileStartIndex`` and ``FileEndIndex``: the start and end of iterator on file name.
* ``FileExtension`` is one of ``vtk``, ``vti``, ``vtr``, ``vts``, ``vtp``, ``vtu``.

## Example

    ./screenshot  MyFile-  1  20  vtk

The example reads files ``MyFile-1.vtk`` to ``MyFile-20.vtk`` and the generates screenshot files ``MyFile-1.png`` to ``MyFile-20.png``.
