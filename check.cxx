#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkImageToMaximumTreeFilter.h"
#include "itkImageToMinimumTreeFilter.h"
#include "itkComponentTreeAttributeToImageFilter.h"
#include "itkComponentTree.h"
#include "itkIntegratedIntensityComponentTreeFilter.h"
#include "itkMeanImageFilter.h"
#include "itkAttributeFilteringComponentTreeFilter.h"
#include <list>

int main(int, char * argv[])
{
  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;
  typedef itk::Image< unsigned short, dim > IType2;

  typedef itk::ComponentTree< PType, dim, unsigned long > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

/*  typedef itk::MeanImageFilter< IType, IType > MeanType;
  MeanType::Pointer mean = MeanType::New();
  mean->SetInput( reader->GetOutput() );
  itk::Size<3> r;
  r.Fill(2);
  mean->SetRadius( r );
  itk::SimpleFilterWatcher mWatcher(mean, "mean");*/

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  itk::SimpleFilterWatcher watcher(filter, "max-tree");

// IType::Pointer img = IType::New();
// IType::RegionType region;
// IType::RegionType::SizeType s;
// s[0]=1024;
// s[1]=1024;
// s[2]=70;
// region.SetSize(s);
// img->SetRegions(region);
// img->Allocate();
// img->FillBuffer(128);
// filter->SetInput( img );

/*filter->Update();
sleep(15);
std::cout << "children: " << filter->GetOutput()->GetRoot()->CountChildren() << std::endl;
std::cout << "indexes: " << filter->GetOutput()->GetRoot()->CountIndexes( filter->GetOutput()->GetLinkedListArray() ) << std::endl;
*/

  typedef itk::IntegratedIntensityComponentTreeFilter< TreeType > SizeType;
  SizeType::Pointer size = SizeType::New();
  size->SetInput( filter->GetOutput() );
  itk::SimpleFilterWatcher sWatcher(size, "size");

  typedef itk::AttributeFilteringComponentTreeFilter< TreeType > InPlaceType;
  InPlaceType::Pointer inplace = InPlaceType::New();
  inplace->SetInput( size->GetOutput() );
  inplace->SetInPlace( true );
  inplace->SetThreshold( atoi(argv[3]) );
  itk::SimpleFilterWatcher fWatcher(inplace, "attribFilter");

  typedef itk::ComponentTreeAttributeToImageFilter< TreeType, IType2 > T2IType;
  T2IType::Pointer filter2 = T2IType::New();
  filter2->SetInput( inplace->GetOutput() );
  itk::SimpleFilterWatcher cWatcher(filter2, "tree to img");

  typedef itk::ImageFileWriter< IType2 > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter2->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

