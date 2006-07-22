#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkImageToMaximumTreeFilter.h"
#include "itkImageToMinimumTreeFilter.h"
#include "itkComponentTreeToImageFilter.h"
#include "itkComponentTree.h"
#include "itkSizeComponentTreeFilter.h"
#include "itkAttributeOpeningComponentTreeFilter.h"

int main(int, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ComponentTree< PType, dim, unsigned long > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
//  filter->Update();

  typedef itk::SizeComponentTreeFilter< TreeType > SizeType;
  SizeType::Pointer size = SizeType::New();
  size->SetInput( filter->GetOutput() );
  size->Update();
  std::cout << "InPlace: " << size->GetInPlace() << std::endl;

//  size->GetOutput()->GetRoot()->print();


  typedef itk::AttributeOpeningComponentTreeFilter< TreeType > InPlaceType;
  InPlaceType::Pointer inplace = InPlaceType::New();
  inplace->SetInput( size->GetOutput() );
  inplace->SetInPlace( true );
  inplace->SetThreshold( atoi(argv[3]) );
  
//  (*filter->GetOutput()->GetRoot()->GetChildrenList().begin())->MergeChildren();

 

/*	std::cout << filter << std::endl;
  std::cout << filter->GetOutput() << std::endl;
	std::cout << filter->GetOutput()->GetRoot()->CountChildren() << std::endl;
	std::cout << filter->GetOutput()->GetRoot()->Depth() << std::endl;
*/
	
	typedef itk::ComponentTreeToImageFilter< TreeType, IType > T2IType;
	T2IType::Pointer filter2 = T2IType::New();
	filter2->SetInput( inplace->GetOutput() );
//	filter2->Update();
	
//  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter2->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

