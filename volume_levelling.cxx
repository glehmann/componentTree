#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkVolumeLevellingComponentTreeFilter.h"
#include "itkComponentTreeAttributeToImageFilter.h"

int main(int argc, char * argv[])
{
  if( argc != 4 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage connectivity" << std::endl;
    std::cerr << "  inputImage: an input image (up to dim=3)." << std::endl;
    std::cerr << "  outputImage: the value of the attribute for all the pixels, with unsigned long type." << std::endl;
    std::cerr << "  connectivity: 1 for fully connected, or 0" << std::endl;
    exit(1);
    }
    
  const int dim = 3;
  
  typedef unsigned short PType;
  typedef itk::Image< PType, dim > IType;

  typedef unsigned short PType2;
  typedef itk::Image< PType2, dim > IType2;

  typedef itk::ComponentTree< PType, dim, PType2 > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[3] ) );

  typedef itk::VolumeLevellingComponentTreeFilter< TreeType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( maxtree->GetOutput() );
  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ComponentTreeAttributeToImageFilter< TreeType, IType2 > T2IType;
  T2IType::Pointer filter2 = T2IType::New();
  filter2->SetInput( filter->GetOutput() );

  typedef itk::ImageFileWriter< IType2 > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter2->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

