#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkSizeComponentTreeFilter.h"
#include "itkAttributeFilteringComponentTreeFilter.h"
#include "itkGradientComponentTreeFilter.h"
#include "itkLocalGradientComponentTreeFilter.h"
#include "itkMonotoneComponentTreeFilter.h"
#include "itkComponentTreeAttributeToImageFilter.h"
#include "itkComponentTreeToImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"


namespace itk
{
namespace Functor {

/**
 * The default functor used to access a single attribute
 */
template< class TComponentTreeNode, class TAttribute, int VIndex >
class ITK_EXPORT ArrayAttributeComponentTreeNodeAccessor
{
public:
  typedef TComponentTreeNode ComponentTreeNodeType;
  typedef TAttribute AttributeType;
  itkStaticConstMacro(Index, unsigned int, VIndex);

  inline const AttributeType operator()( const ComponentTreeNodeType * node )
    {
    return node->GetAttribute()[ Index ];
    }

//   inline AttributeType operator()( ComponentTreeNodeType * node )
//     {
//     return node->GetAttribute()[ Index ];
//     }

  inline void operator()( ComponentTreeNodeType * node, const AttributeType & value )
    {
    node->GetAttribute()[ Index ] = value;
    }
};

}

}

template<class ContainerType, class NodeType>
void
findLeaves( ContainerType & container, NodeType * node )
{
  
  assert(node != NULL);

  if( node->IsLeaf() )
    {
    container.push_back( node );
    }
  else
    {
    const typename NodeType::ChildrenListType & childrenList = node->GetChildren();
    for( typename NodeType::ChildrenListType::const_iterator it=childrenList.begin(); it!=childrenList.end(); it++ )
      {
      findLeaves( container, *it );
      }
    }
}


int main(int argc, char * argv[])
{
  if( argc != 5 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage attributeImage connectivity" << std::endl;
    std::cerr << "  inputImage: an input image." << std::endl;
    std::cerr << "  outputImage: the output image." << std::endl;
    std::cerr << "  attributeImage: the value of the attribute for all the pixels, rescaled to unsigned char type." << std::endl;
    std::cerr << "  connectivity: 1 for fully connected, or 0" << std::endl;
    exit(1);
    }
    
  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef float RType;
  typedef itk::Image< RType, dim > RIType;

  typedef itk::ComponentTree< PType, dim, itk::FixedArray< double, 2 > > TreeType;
  typedef TreeType::NodeType NodeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[4] ) );
//   itk::SimpleFilterWatcher watcher2(maxtree, "filter");

  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 0 > GradientAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 1 > LocalGradientAccessor;

  typedef itk::GradientComponentTreeFilter< TreeType, GradientAccessor > GradientType;
//  typedef itk::LocalGradientComponentTreeFilter< TreeType > GradientType;
  GradientType::Pointer gradient = GradientType::New();
  gradient->SetInput( maxtree->GetOutput() );
//  gradient->SetUseZeroLeaves( true );

  typedef itk::LocalGradientComponentTreeFilter< TreeType, LocalGradientAccessor > LocalGradientType;
  LocalGradientType::Pointer lgradient = LocalGradientType::New();
  lgradient->SetInput( gradient->GetOutput() );
  lgradient->SetUseZeroLeaves( true );

  typedef itk::MonotoneComponentTreeFilter< TreeType, LocalGradientAccessor > LocalMonotoneType;
  LocalMonotoneType::Pointer lmonotone = LocalMonotoneType::New();
  lmonotone->SetInput( lgradient->GetOutput() );
  lmonotone->SetReverseOrdering( true );
  lmonotone->SetRemoveNodes( false );
  lmonotone->SetStrictlyMonotone( false );

  typedef itk::MonotoneComponentTreeFilter< TreeType, GradientAccessor > MonotoneType;
  MonotoneType::Pointer monotone = MonotoneType::New();
  monotone->SetInput( lmonotone->GetOutput() );
  monotone->SetReverseOrdering( true );
  monotone->SetRemoveNodes( true );
  monotone->SetStrictlyMonotone( false );
//   itk::SimpleFilterWatcher watcher(monotone, "monotone");
  monotone->Update();

  typedef std::deque< NodeType * > LeavesContainerType;
  LeavesContainerType leaves;
  findLeaves< LeavesContainerType, NodeType >( leaves, monotone->GetOutput()->GetRoot() );
  GradientAccessor gradientAccessor;
  LocalGradientAccessor lgradientAccessor;

  for( LeavesContainerType::iterator it = leaves.begin(); it!=leaves.end(); it++ )
    {
    std::cout << (*it)->GetPixel()+0.0 << " " << gradientAccessor(*it) << " " << lgradientAccessor(*it) << std::endl;
    }

//   typedef itk::ComponentTreeToImageFilter< TreeType, IType > T2IType;
//   T2IType::Pointer t2i = T2IType::New();
// //   t2i->SetInput( filtering->GetOutput() );
//   t2i->SetInput( monotone->GetOutput() );
// 
//   typedef itk::ImageFileWriter< IType > WriterType;
//   WriterType::Pointer writer = WriterType::New();
//   writer->SetInput( t2i->GetOutput() );
//   writer->SetFileName( argv[2] );
//   writer->Update();
// 
// 
//   typedef itk::ComponentTreeAttributeToImageFilter< TreeType, RIType > T2AType;
//   T2AType::Pointer t2a = T2AType::New();
// //   t2a->SetInput( filtering->GetOutput() );
// //   t2a->SetInput( gradient->GetOutput() );
//   t2a->SetInput( monotone->GetOutput() );
// 
//   typedef itk::RescaleIntensityImageFilter< RIType, IType > RI2IType;
//   RI2IType::Pointer rescale = RI2IType::New();
//   rescale->SetInput( t2a->GetOutput() );
// 
//   writer->SetInput( rescale->GetOutput() );
//   writer->SetFileName( argv[3] );
//   writer->Update();

  return 0;
}

