#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkSumComponentTreeFilter.h"
#include "itkKeepNLobesComponentTreeFilter.h"
#include "itkComponentTreeLeavesToBinaryImageFilter.h"

int main(int argc, char * argv[])
{
  if( argc != 5 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage connectivity lobes" << std::endl;
    std::cerr << "  inputImage: an input image (up to dim=3)" << std::endl;
    std::cerr << "  outputImage: the output image" << std::endl;
    std::cerr << "  connectivity: 1 for fully connected, or 0" << std::endl;
    std::cerr << "  lobes: nb of lobes" << std::endl;
    exit(1);
    }
    
  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ComponentTree< PType, dim, unsigned long > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[3] ) );

  typedef itk::SumComponentTreeFilter< TreeType > IIType;
  IIType::Pointer ii = IIType::New();
  ii->SetInput( maxtree->GetOutput() );

  typedef itk::KeepNLobesComponentTreeFilter< TreeType > KeepType;
  KeepType::Pointer keep = KeepType::New();
  keep->SetInput( ii->GetOutput() );
  keep->SetNumberOfLobes( atoi( argv[4] ) );
  itk::SimpleFilterWatcher watcher(keep, "keep");
  
  typedef itk::ComponentTreeLeavesToBinaryImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer l2i = T2IType::New();
  l2i->SetInput( keep->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}
