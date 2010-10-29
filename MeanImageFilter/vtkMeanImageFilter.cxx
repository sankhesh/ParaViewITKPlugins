
#include "vtkMeanImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkImageShiftScale.h"

vtkStandardNewMacro(vtkMeanImageFilter);

//----------------------------------------------------------------------------
vtkMeanImageFilter::vtkMeanImageFilter()
{
  this->VTKExporter = vtkImageExport::New();
  this->ITKImporter = ITKImageImportType::New();
  this->MeanFilter = ITKMeanFilterType::New();
  this->ITKExporter = ITKImageExportType::New();
  this->VTKImporter = vtkImageImport::New();

  this->InitializeITKImporter();
  this->InitializeITKExporter();
}

//----------------------------------------------------------------------------
vtkMeanImageFilter::~vtkMeanImageFilter()
{
  if (this->VTKExporter)
    {
      this->VTKExporter->Delete();
    }
  
  if (this->VTKImporter)
    {
      this->VTKImporter->Delete();
    }
}

//----------------------------------------------------------------------------
void vtkMeanImageFilter::InitializeITKImporter()
{
  // This call takes the place of the usual SetInput() method.
  this->ITKImporter->SetCallbackUserData(this->VTKExporter);

  // Set the rest of the callbacks
  this->ITKImporter->SetBufferPointerCallback(this->VTKExporter->GetBufferPointerCallback());
  this->ITKImporter->SetDataExtentCallback(this->VTKExporter->GetDataExtentCallback());
  this->ITKImporter->SetOriginCallback(this->VTKExporter->GetOriginCallback());
  this->ITKImporter->SetSpacingCallback(this->VTKExporter->GetSpacingCallback());
  this->ITKImporter->SetNumberOfComponentsCallback(this->VTKExporter->GetNumberOfComponentsCallback());
  this->ITKImporter->SetPipelineModifiedCallback(this->VTKExporter->GetPipelineModifiedCallback());
  this->ITKImporter->SetPropagateUpdateExtentCallback(this->VTKExporter->GetPropagateUpdateExtentCallback());
  this->ITKImporter->SetScalarTypeCallback(this->VTKExporter->GetScalarTypeCallback());
  this->ITKImporter->SetUpdateDataCallback(this->VTKExporter->GetUpdateDataCallback());
  this->ITKImporter->SetUpdateInformationCallback(this->VTKExporter->GetUpdateInformationCallback());
  this->ITKImporter->SetWholeExtentCallback(this->VTKExporter->GetWholeExtentCallback());
}

//----------------------------------------------------------------------------
void vtkMeanImageFilter::InitializeITKExporter()
{
  // This call takes the place of the usual SetInput() method.
  this->VTKImporter->SetCallbackUserData(ITKExporter->GetCallbackUserData());

  // Set the rest of the callbacks
  this->VTKImporter->SetUpdateInformationCallback(ITKExporter->GetUpdateInformationCallback());
  this->VTKImporter->SetPipelineModifiedCallback(ITKExporter->GetPipelineModifiedCallback());
  this->VTKImporter->SetWholeExtentCallback(ITKExporter->GetWholeExtentCallback());
  this->VTKImporter->SetSpacingCallback(ITKExporter->GetSpacingCallback());
  this->VTKImporter->SetOriginCallback(ITKExporter->GetOriginCallback());
  this->VTKImporter->SetScalarTypeCallback(ITKExporter->GetScalarTypeCallback());
  this->VTKImporter->SetNumberOfComponentsCallback(ITKExporter->GetNumberOfComponentsCallback());
  this->VTKImporter->SetPropagateUpdateExtentCallback(ITKExporter->GetPropagateUpdateExtentCallback());
  this->VTKImporter->SetUpdateDataCallback(ITKExporter->GetUpdateDataCallback());
  this->VTKImporter->SetDataExtentCallback(ITKExporter->GetDataExtentCallback());
  this->VTKImporter->SetBufferPointerCallback(ITKExporter->GetBufferPointerCallback());
}

//----------------------------------------------------------------------------
int vtkMeanImageFilter::RequestData(vtkInformation *request,
					    vtkInformationVector **inputVector,
					    vtkInformationVector *outputVector)
{
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkImageData *input = vtkImageData::SafeDownCast
    (inInfo->Get(vtkDataObject::DATA_OBJECT()));
  if(!input)
    {
    vtkErrorMacro("Output is not of type vtkImageData");
    return 0;
    }

  vtkImageShiftScale* castSource = vtkImageShiftScale::New();
  castSource->SetOutputScalarTypeToFloat();
  castSource->SetInput(input);
  castSource->Update();


  // Hook up to the beginning of the ITK pipeline
  this->VTKExporter->SetInput(castSource->GetOutput());

  // Set up ITK pipeline settings here
  this->MeanFilter->SetInput(this->ITKImporter->GetOutput());

  ITKImageType::SizeType radius;
  for (unsigned int i = 0; i < 3; i++)
    {
    radius[i] = this->NeighborhoodRadius[i];
    }
  this->MeanFilter->SetRadius(radius);
  this->MeanFilter->Update();


  // Now connect the ITK pipeline output to the VTK output
  this->ITKExporter->SetInput(this->MeanFilter->GetOutput());
  this->ITKExporter->Update();

  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkImageData *output = vtkImageData::SafeDownCast
    (outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!output)
    {
    vtkErrorMacro("Output is not of type vtkImageData");
    return 0;
    }

  this->VTKImporter->Update();

  output->DeepCopy(this->VTKImporter->GetOutput());

  return 1;
}

//----------------------------------------------------------------------------
void vtkMeanImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
