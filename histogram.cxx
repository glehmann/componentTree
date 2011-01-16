#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkGradientComponentTreeFilter.h"
#include "itkComponentTreeToHistogramFilter.h"
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

  typedef double PType2;
  typedef itk::Image< PType2, dim > IType2;

  typedef itk::ComponentTree< PType, dim, PType2 > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[3] ) );

  typedef itk::GradientComponentTreeFilter< TreeType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( maxtree->GetOutput() );
  
  typedef itk::ComponentTreeToHistogramFilter< TreeType > HistogramGeneratorType;
  HistogramGeneratorType::Pointer gen = HistogramGeneratorType::New();
  gen->SetInput( filter->GetOutput() );
  itk::SimpleFilterWatcher watcher(gen, "filter");
  gen->Update();

  return 0;
}

