/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $
  Author:    Gavin Baker <gavinb@cs.mu.oz.au>

  Copyright (c) 2004 Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkSaltAndPepperNoiseImageFilter.h"
#include "itkThreadSafeMersenneTwisterRandomVariateGenerator.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
SaltAndPepperNoiseImageFilter<TInputImage, TOutputImage>
::SaltAndPepperNoiseImageFilter()
{
  m_Probability = 0.01;
  m_Minimum = NumericTraits< OutputImagePixelType >::NonpositiveMin();
  m_Maximum = NumericTraits< OutputImagePixelType >::max();
}


template <class TInputImage, class TOutputImage>
void
SaltAndPepperNoiseImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{
  InputImageConstPointer  inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);
  
  // create a random generator per thread
  typename Statistics::ThreadSafeMersenneTwisterRandomVariateGenerator::Pointer rand = 
      Statistics::ThreadSafeMersenneTwisterRandomVariateGenerator::New();
  rand->Initialize();
  
  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  ImageRegionConstIterator<TInputImage>  inputIt(inputPtr, inputRegionForThread);
  ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while( !inputIt.IsAtEnd() ) 
    {
    double r = rand->GetVariate();
    if( r < m_Probability )
      {
      if( r < m_Probability / 2 )
        {
        // salt
        outputIt.Set( m_Maximum );
        }
      else
        {
        // pepper
        outputIt.Set( m_Minimum );
        }
      }
    else
      {
      // keep the data unchanged
      outputIt.Set( (OutputImagePixelType) inputIt.Get() );
      }
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}

template <class TInputImage, class TOutputImage>
void
SaltAndPepperNoiseImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os,
            Indent indent) const
{
    Superclass::PrintSelf(os, indent);
    os << indent << "Probability: " 
       << static_cast<typename NumericTraits<double>::PrintType>(this->GetProbability())
       << std::endl;
    os << indent << "Minimum: " 
       << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(this->GetMinimum())
       << std::endl;
    os << indent << "Maximum: " 
       << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(this->GetMaximum())
       << std::endl;
}

} /* namespace itk */