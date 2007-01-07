#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkInPlaceComponentTreeFilter.h"
#include "itkComponentTreeToImageFilter.h"

int main(int argc, char * argv[])
{
  if( argc != 4 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage" << std::endl;
    std::cerr << "  inputImage: an input image (up to dim=3)" << std::endl;
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

  typedef itk::InPlaceComponentTreeFilter< TreeType > InPlaceType;
  
  InPlaceType::Pointer inplace = InPlaceType::New();
  inplace->SetInput( filter->GetOutput() );
  inplace->Update();

  InPlaceType::Pointer notinplace = InPlaceType::New();
  notinplace->SetInput( filter->GetOutput() );
  notinplace->SetInPlace( false );

  // the tests
  
  // verify that the cloned tree is the same than the original one
  typedef itk::ComponentTreeToImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer filter2 = T2IType::New();
  filter2->SetInput( notinplace->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter2->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
  
  // check the default in place value
  if( !inplace->GetInPlace() )
    {
    std::cerr << "wrong default value" << std::endl;
    return 1;
    }

  // modify the cloned tree. The original one shouldn't be modified
  unsigned long nbOfChildren = filter->GetOutput()->GetRoot()->CountChildren();
  notinplace->GetOutput()->NodeFlatten( notinplace->GetOutput()->GetRoot() );
  unsigned long nbOfChildren_modif = notinplace->GetOutput()->GetRoot()->CountChildren();
  unsigned long nbOfChildren_non_modif = inplace->GetOutput()->GetRoot()->CountChildren();
  
  if( nbOfChildren_non_modif != nbOfChildren || nbOfChildren_modif != 1 )
    {
    std::cerr << "wrong number of children" << std::endl;
    return 1;
    }
  
  return 0;
}

