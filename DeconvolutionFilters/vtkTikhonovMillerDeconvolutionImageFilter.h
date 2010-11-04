/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTikhonovMillerDeconvolutionImageFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkTikhonovMillerDeconvolutionImageFilter - peforms Gaussian blurring of an input image
// .SECTION Description
// vtkMyImageShiftFilter is a filter to generate scalar values from a
// dataset.  The scalar values lie within a user specified range, and
// are generated by computing a projection of each dataset point onto
// a line. The line can be oriented arbitrarily. A typical example is
// to generate scalars based on elevation or height above a plane.

#ifndef __vtkTikhonovMillerDeconvolutionImageFilter_h
#define __vtkTikhonovMillerDeconvolutionImageFilter_h

#include <vtkDeconvolutionImageFilter.h>

#include <itkTikhonovMillerDeconvolutionImageFilter.h>

class VTK_EXPORT vtkTikhonovMillerDeconvolutionImageFilter : public vtkDeconvolutionImageFilter
{
public:

  static vtkTikhonovMillerDeconvolutionImageFilter* New();
  vtkTypeMacro(vtkTikhonovMillerDeconvolutionImageFilter, vtkDeconvolutionImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::TikhonovMillerDeconvolutionImageFilter< ITKImageType, ITKImageType, ITKImageType >
    ITKDeconvolutionFilterType;
  //ETX

  // Set/get regularizing parameter Gamma
  vtkSetMacro(Gamma, double);
  vtkGetMacro(Gamma, double);

protected:
  vtkTikhonovMillerDeconvolutionImageFilter();
  ~vtkTikhonovMillerDeconvolutionImageFilter();

  double Gamma;

  //BTX
  ITKDeconvolutionFilterType::Pointer ITKDeconvolutionFilter;
  //ETX

protected:
  int UpdateInternalFilters();

private:
  vtkTikhonovMillerDeconvolutionImageFilter(const vtkTikhonovMillerDeconvolutionImageFilter&);  // Not implemented.
  void operator=(const vtkTikhonovMillerDeconvolutionImageFilter&);  // Not implemented.
};

#endif
