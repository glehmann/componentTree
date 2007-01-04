#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkComponentTreeLeavesToBinaryImageFilter.h"

int main(int argc, char * argv[])
{
  if( argc != 6 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage connectivity foreground background" << std::endl;
    std::cerr << "  inputImage: an input image (up to dim=3)" << std::endl;
    std::cerr << "  outputImage: the output image" << std::endl;
    std::cerr << "  connectivity: 1 for fully connected, or 0" << std::endl;
    std::cerr << "  foreground: foreground value" << std::endl;
    std::cerr << "  background: background value" << std::endl;
    exit(1);
    }
    
  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ComponentTree< PType, dim, unsigned long > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetFullyConnected( atoi( argv[3] ) );

  typedef itk::ComponentTreeLeavesToBinaryImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer filter2 = T2IType::New();
  filter2->SetInput( filter->GetOutput() );
  filter2->SetBackgroundValue( atoi( argv[5] ) );
  filter2->SetForegroundValue( atoi( argv[4] ) );
  itk::SimpleFilterWatcher cWatcher(filter2, "tree to img");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter2->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

