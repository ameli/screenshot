/*
 * =====================================================================================
 *
 *       Filename:  main.cxx
 *
 *    Description:  Takes screenshots of a vtk file
 *
 *        Version:  1.0
 *        Created:  08/13/2014 03:00:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Siavash Ameli
 *   Organization:  University Of California, Berkeley
 *
 * =====================================================================================
 */

// =======
// Headers
// =======

#include "main.h"

// VTK
#include <vtkSmartPointer.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkXMLGenericDataObjectReader.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

// STL
#include <cstring>
#include <sstream>
#include <iostream>

// ====
// main
// ====

int main(int argc, char *argv[])
{
    // Parse arguments
    char InputBaseFilename[256];
    unsigned int FileStartIndex;
    unsigned int FileEndIndex;
    char InputFileExtension[256];
    ParseArguments(
            argc, argv,
            InputBaseFilename,     // Output
            FileStartIndex,        // Output
            FileEndIndex,          // Output
            InputFileExtension);   // Output

    // Determine file type
    FileType InputFileType = DetectFileType(InputFileExtension);

    // Iterate over files
    for(unsigned int FileIterator = FileStartIndex;
        FileIterator <= FileEndIndex;
        FileIterator++)
    {
        // Generate Input filename
        char InputFilename[256];
        GenerateInputFilename(
                InputBaseFilename,
                FileIterator,
                InputFilename);

        // Read File
        vtkSmartPointer<vtkDataSet> InputDataSet = Read(InputFilename,InputFileType);
       
        // Render
        vtkSmartPointer<vtkRenderWindow> RenderWindow = Render(InputDataSet);

        // Generate Output Filename
        char OutputFilename[256];
        GenerateOutputFilename(InputFilename,OutputFilename);

        // Write to PNG
        Write(OutputFilename,RenderWindow);

        std::cout << InputFilename << " converted to " << OutputFilename << "." << std::endl;
    }
    
    return EXIT_SUCCESS;
}

// ===============
// Parse Arguments
// ===============

void ParseArguments(
        int &argc,
        char *argv[],
        char *InputBaseFilename,         // Output
        unsigned int &FileStartIndex,    // Output
        unsigned int &FileEndIndex,      // Output
        char *InputFileExtension)        // Output
{
    if(argc < 5)
    {
        PrintUsage(argv[0]);
        exit(0);
    }
    else
    {
        strcpy(InputBaseFilename,argv[1]);
        FileStartIndex = atoi(argv[2]);
        FileEndIndex = atoi(argv[3]);
        strcpy(InputFileExtension,argv[4]);
    }

    // Check Inputs
    if(strlen(InputBaseFilename) < 1)
    {
        std::cerr << "InputBaseFilename is empty." << std::endl;
        exit(0);
    }
    else if(FileStartIndex > FileEndIndex)
    {
        std::cerr << "FileStartIndex should be less than or equal to FileEndIndex." << std::endl;
        exit(0);
    }
}

// ===========
// Print Usage
// ===========

void PrintUsage(const char *ExecutableName)
{
    std::cerr << "Usage: " << ExecutableName << "  InputBaseFilename  FileStartIndex  FileEndIndex FileExtension" << std::endl;

    std::cerr << "\nInputBaseFilename: is the input filename without the iterator and file extension." << std::endl;
    std::cerr << "FileStartIndex and FileEndIndex: the start and end of iterator on file name." << std::endl;
    std::cerr << "FileExtension is one of vtk, vti, vtr, vts, vtp, vtu." << std::endl;

    std::cerr << "\nExample: " << ExecutableName << "  MyFile-  1  20  vtk" << std::endl;
    std::cerr << "The example reads files MyFile-1.vtk to MyFile-20.vtk and " << std::endl;
    std::cerr << "generates screenshot files MyFile-1.png to MyFile-20.png." << std::endl;
}

// ================
// Detect File Type
// ================

// Description:
// This function determines if the input file is legacy VTK file format or XML file.

FileType DetectFileType(const char *InputFileExtension)
{
    FileType InputFileType;

    if(strcmp(InputFileExtension,"vtk") == 0)
    {
        InputFileType = VTK;
    }
    else if(strcmp(InputFileExtension,"vti") == 0 ||
            strcmp(InputFileExtension,"vtp") == 0 ||
            strcmp(InputFileExtension,"vtu") == 0 ||
            strcmp(InputFileExtension,"vtr") == 0 ||
            strcmp(InputFileExtension,"vts") == 0)
    {
        InputFileType = XML;
    }
    else
    {
        std::cerr << "File type: " << InputFileType << " is not supported. Program terminated." << std::endl;
        exit(0);
    }

    return InputFileType;
}

// ====
// Read
// ====

vtkSmartPointer<vtkDataSet> Read(
        const char *InputFilename,
        FileType InputDataType)   // Output
{
    vtkSmartPointer<vtkDataSet> InputDataSet;

    switch(InputDataType)
    {
        // VTK
        case VTK:
        {
            // Read file
            vtkSmartPointer<vtkGenericDataObjectReader> Reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
            Reader->SetFileName(InputFilename);
            Reader->Update();

            // Get DataSet
            InputDataSet = vtkDataSet::SafeDownCast(Reader->GetOutput());

            break;
        }

        // XML
        case XML:
        {
            // Read file
            vtkSmartPointer<vtkXMLGenericDataObjectReader> Reader = vtkSmartPointer<vtkXMLGenericDataObjectReader>::New();
            Reader->SetFileName(InputFilename);
            Reader->Update();

            // Get DataSet
            InputDataSet = vtkDataSet::SafeDownCast(Reader->GetOutput());

            break;
        }

        // Invalid file type
        default:
        {
            std::cerr << "Invalid file type. Program terminated." << std::endl;
            exit(0);
        }
    }

    // Check output
    if(InputDataSet == NULL)
    {
        std::cerr << "Input DataSet is NULL. Program terminated." << std::endl;
        exit(0);
    }

    return InputDataSet;
}

// ======
// Render
// ======

vtkSmartPointer<vtkRenderWindow> Render(vtkDataSet *InputDataSet)
{
    // Mapper
    vtkSmartPointer<vtkDataSetMapper> Mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    #if VTK_MAJOR_VERSION <= 5
        Mapper->SetInput(InputDataSet);
    #else
        Mapper->SetInputData(InputDataSet);
    #endif

    // Actor
    vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
    Actor->SetMapper(Mapper);

    // Renderer
    vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();
    Renderer->AddActor(Actor);
    Renderer->SetBackground(1,1,1);

    // Render Window
    vtkSmartPointer<vtkRenderWindow> RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    RenderWindow->AddRenderer(Renderer);
    RenderWindow->SetAlphaBitPlanes(1);
    RenderWindow->SetWindowName("Screenshot");
    RenderWindow->BordersOn();
    RenderWindow->Render();

    return RenderWindow;

    // Callback
    // vtkSmartPointer<vtkCallbackCommand> Callback = vtkSmartPointer<vtkCallbackCommand>::New();
    // Callback->SetCallback(CallbackFunction);

    // Render Window Interactor
    // vtkSmartPointer<vtkRenderWindowInteractor> Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    // Interactor->SetRenderWindow(RenderWindow);
    // Interactor->AddObserver(vtkCommand::KeyPressEvent,Callback);

    // Start Visualization
    // Interactor->Initialize();
    // RenderWindow->Render();
    // Interactor->Start();
}

// =====
// Write
// =====

void Write(
        const char *OutputFilename,
        vtkRenderWindow *RenderWindow)
{
    // Window To Image
    vtkSmartPointer<vtkWindowToImageFilter> WindowToImage = vtkSmartPointer<vtkWindowToImageFilter>::New();
    WindowToImage->SetInput(RenderWindow);
    WindowToImage->SetScale(3);
    WindowToImage->SetInputBufferTypeToRGBA();
    WindowToImage->Update();

    // PNG Writer
    vtkSmartPointer<vtkPNGWriter> PNGWriter = vtkSmartPointer<vtkPNGWriter>::New();
    PNGWriter->SetFileName(OutputFilename);
    PNGWriter->SetInputConnection(WindowToImage->GetOutputPort());
    PNGWriter->Write();
}

// =================
// Callback Function
// =================

void CallbackFunction(
        vtkObject *Caller,
        unsigned long int EventId,
        void *ClientData,
        void *CallData)
{
    vtkRenderWindowInteractor *Interactor = static_cast<vtkRenderWindowInteractor*>(Caller);
    Interactor->GetRenderWindow()->Finalize();
    Interactor->TerminateApp();

}

// =======================
// Generate Input Filename
// =======================

void GenerateInputFilename(
        const char *InputBaseFilename,
        unsigned int FileIterator,
        char *InputFilename)   // Output
{
    std::ostringstream InputFilenameStream;
    std::string InputBaseFilenameString(InputBaseFilename);
    InputFilenameStream << InputBaseFilename <<  FileIterator << ".vtk";
    strcpy(InputFilename,InputFilenameStream.str().c_str());
}

// ========================
// Generate Output Filename
// ========================

void GenerateOutputFilename(
        const char *InputFilename,
        char *OutputFilename)   // Output
{
    std::string InputFilenameString(InputFilename);
    std::string InputBaseFilenameString;

    // Find the last dot position
    size_t FoundDot = InputFilenameString.find_last_of(".");
    if(FoundDot != std::string::npos)
    {
        InputBaseFilenameString = InputFilenameString.substr(0,FoundDot);
    }
    else
    {
        std::cerr << "Not dot can be find in the input file name." << std::endl;
        exit(0);
    }

    std::string OutputFilenameString = InputBaseFilenameString + ".png";
    strcpy(OutputFilename,OutputFilenameString.c_str());
}
