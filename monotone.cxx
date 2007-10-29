#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkSizeComponentTreeFilter.h"
#include "itkAttributeFilteringComponentTreeFilter.h"
#include "itkGradientComponentTreeFilter.h"
#include "itkMonotoneComponentTreeFilter.h"
#include "itkComponentTreeAttributeToImageFilter.h"
#include "itkComponentTreeToImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char * argv[])
{
  if( argc != 9 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage attributeImage connectivity minSize maxSize removeNodes strictlyMonotone" << std::endl;
    std::cerr << "  inputImage: an input image." << std::endl;
    std::cerr << "  outputImage: the output image." << std::endl;
    std::cerr << "  attributeImage: the value of the attribute for all the pixels, rescaled to unsigned char type." << std::endl;
    std::cerr << "  connectivity: 1 for fully connected, or 0" << std::endl;
    std::cerr << "  minSize: the minimum size of an object in pixels" << std::endl;
    std::cerr << "  maxSize: the maximum size of an object in pixels" << std::endl;
    std::cerr << "  removeNodes: remove the nodes while making the attribute monotones" << std::endl;
    std::cerr << "  strictlyMonotone: make the attribute strictly monotone" << std::endl;
    exit(1);
    }
    
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef float RType;
  typedef itk::Image< RType, dim > RIType;

  typedef itk::ComponentTree< PType, dim, float > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[4] ) );
  itk::SimpleFilterWatcher watcher2(maxtree, "filter");

  typedef itk::SizeComponentTreeFilter< TreeType > SizeType;
  SizeType::Pointer size = SizeType::New();
  size->SetInput( maxtree->GetOutput() );

  typedef itk::AttributeFilteringComponentTreeFilter< TreeType > FilteringType;
  FilteringType::Pointer minFiltering = FilteringType::New();
  minFiltering->SetInput( size->GetOutput() );
  minFiltering->SetLambda( atoi( argv[5] ) );

  FilteringType::Pointer maxFiltering = FilteringType::New();
  maxFiltering->SetInput( minFiltering->GetOutput() );
  maxFiltering->SetFilteringType( "Subtract" );
  maxFiltering->SetReverseOrdering( true );
  maxFiltering->SetLambda( atoi( argv[6] ) );

  typedef itk::GradientComponentTreeFilter< TreeType > GradientType;
  GradientType::Pointer gradient = GradientType::New();
  gradient->SetInput( maxFiltering->GetOutput() );

  typedef itk::MonotoneComponentTreeFilter< TreeType > MonotoneType;
  MonotoneType::Pointer monotone = MonotoneType::New();
  monotone->SetInput( gradient->GetOutput() );
  monotone->SetReverseOrdering( true );
  monotone->SetRemoveNodes( atoi( argv[7] ) );
  monotone->SetStrictlyMonotone( atoi( argv[8] ) );
  itk::SimpleFilterWatcher watcher(monotone, "monotone");

  typedef itk::ComponentTreeToImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer t2i = T2IType::New();
//   t2i->SetInput( filtering->GetOutput() );
  t2i->SetInput( monotone->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( t2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();


  typedef itk::ComponentTreeAttributeToImageFilter< TreeType, RIType > T2AType;
  T2AType::Pointer t2a = T2AType::New();
//   t2a->SetInput( filtering->GetOutput() );
//   t2a->SetInput( gradient->GetOutput() );
  t2a->SetInput( monotone->GetOutput() );

  typedef itk::RescaleIntensityImageFilter< RIType, IType > RI2IType;
  RI2IType::Pointer rescale = RI2IType::New();
  rescale->SetInput( t2a->GetOutput() );

  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();

  return 0;
}

