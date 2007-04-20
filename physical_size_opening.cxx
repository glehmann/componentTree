#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkPhysicalSizeComponentTreeFilter.h"
#include "itkAttributeFilteringComponentTreeFilter.h"
#include "itkComponentTreeToImageFilter.h"

int main(int argc, char * argv[])
{
  if( argc != 5 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage connectivity size" << std::endl;
    std::cerr << "  inputImage: an input image (up to dim=3)." << std::endl;
    std::cerr << "  outputImage: the value of the attribute for all the pixels, with float type." << std::endl;
    std::cerr << "  connectivity: 1 for fully connected, or 0" << std::endl;
    std::cerr << "  size: the size of the components to remove" << std::endl;
    exit(1);
    }
    
  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ComponentTree< PType, dim, double > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[3] ) );

  typedef itk::PhysicalSizeComponentTreeFilter< TreeType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( maxtree->GetOutput() );
  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::AttributeFilteringComponentTreeFilter< TreeType > FilteringType;
  FilteringType::Pointer filter2 = FilteringType::New();
  filter2->SetInput( filter->GetOutput() );
  filter2->SetLambda( atof( argv[4] ) );

  typedef itk::ComponentTreeToImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer filter3 = T2IType::New();
  filter3->SetInput( filter2->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter3->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

