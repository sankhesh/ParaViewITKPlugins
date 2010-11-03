/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkWienerDeconvolutionImageFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkWienerDeconvolutionImageFilter - peforms Gaussian blurring of an input image
// .SECTION Description
// vtkMyImageShiftFilter is a filter to generate scalar values from a
// dataset.  The scalar values lie within a user specified range, and
// are generated by computing a projection of each dataset point onto
// a line. The line can be oriented arbitrarily. A typical example is
// to generate scalars based on elevation or height above a plane.

#ifndef __vtkWienerDeconvolutionImageFilter_h
#define __vtkWienerDeconvolutionImageFilter_h

#include <vtkImageAlgorithm.h>

#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>
#include <itkWienerDeconvolutionImageFilter.h>

class vtkImageExport;
class vtkImageImport;

class VTK_EXPORT vtkWienerDeconvolutionImageFilter : public vtkImageAlgorithm
{
public:

  //BTX
  typedef float
    PixelType;
  typedef itk::Image< PixelType, 3 >
    ITKImageType;
  typedef itk::VTKImageImport< ITKImageType >
    ITKImageImportType;
  typedef itk::VTKImageExport< ITKImageType >
    ITKImageExportType;
  typedef itk::WienerDeconvolutionImageFilter< ITKImageType, ITKImageType, ITKImageType >
    ITKDeconvolutionFilterType;
  //ETX

  static vtkWienerDeconvolutionImageFilter* New();
  vtkTypeMacro(vtkWienerDeconvolutionImageFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Set the convolution kernel image
  void SetKernelImage(vtkAlgorithmOutput* image);

  // Set/get the image padding type
  vtkSetMacro(PaddingMethod, int);
  vtkGetMacro(PaddingMethod, int);

protected:
  vtkWienerDeconvolutionImageFilter();
  ~vtkWienerDeconvolutionImageFilter();

  int PaddingMethod;

  vtkImageExport*                VTKExporter;
  vtkImageExport*                VTKKernelExporter;
  //BTX
  ITKImageImportType::Pointer         ITKImporter;
  ITKImageImportType::Pointer         ITKKernelImporter;
  ITKDeconvolutionFilterType::Pointer ITKDeconvolutionFilter;
  ITKImageExportType::Pointer         ITKExporter;
  //ETX
  vtkImageImport*                VTKImporter;

protected:
  void InitializeITKImporters();
  void InitializeITKExporter();

  virtual int RequestData(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector);
  virtual int RequestUpdateExtent (vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  virtual int FillInputPortInformation(int port, vtkInformation *info);

  //BTX
  template <class T>
  void RunITKPipeline(const T *input);
  //ETX

private:
  vtkWienerDeconvolutionImageFilter(const vtkWienerDeconvolutionImageFilter&);  // Not implemented.
  void operator=(const vtkWienerDeconvolutionImageFilter&);  // Not implemented.
};

#endif