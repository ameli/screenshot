/*
 * ============================================================================
 *
 *       Filename:  main.h
 *
 *    Description:  Takes screenshots of a vtk file
 *
 *        Version:  1.0
 *        Created:  08/13/2014 04:45:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Siavash Ameli
 *   Organization:  University Of California, Berkeley
 *
 * ============================================================================
 */

#ifndef __TakeScreen_h
#define __TakeScreen_h

// ====================
// Forward Declarations
// ====================

#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkRenderWindow.h>

// ====
// Type
// ====

typedef enum FileType
{
    VTK = 0,
    XML,
} FileType;

// ==========
// Prototypes
// ==========

void ParseArguments(
        int &argc,
        char *argv[],
        char *InputBaseFilename,        // Output
        unsigned int &FileStartIndex,   // Output
        unsigned int &FileEndIndex,     // Output
        char *InputFileExtension);      // Output

void PrintUsage(const char *ExecutableName);

FileType DetectFileType(const char *InputFileExtension);

vtkSmartPointer<vtkDataSet> Read(
        const char *InputFilename,
        FileType InputFileType);

vtkSmartPointer<vtkRenderWindow> Render(vtkDataSet *InputDataSet);

void Write(
        const char *OutputFilename,
        vtkRenderWindow *RenderWindow);

void CallbackFunction(
        vtkObject *Caller,
        unsigned long int EventId,
        void *ClientData,
        void *CallData);

void GenerateInputFilename(
        const char *InputBaseFilename,
        unsigned int FileIterator,
        char *InputFilename);   // Output

void GenerateOutputFilename(
        const char *InputFilename,
        char *OutputFilename);   // Output

#endif
