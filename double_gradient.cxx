#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkNumberOfPixelsComponentTreeFilter.h"
#include "itkAttributeFilteringComponentTreeFilter.h"
#include "itkGradientComponentTreeFilter.h"
#include "itkMonotoneComponentTreeFilter.h"
#include "itkComponentTreeAttributeToImageFilter.h"
#include "itkComponentTreeToImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkKeepNLobesComponentTreeFilter.h"
#include "itkComponentTreeLeavesToLabelImageFilter.h"
#include "itkVolumeLevellingComponentTreeFilter.h"
#include "itkLocalIntensityComponentTreeFilter.h"
#include "itkNumberOfPixelsComponentTreeFilter.h"
#include "itkIntensityComponentTreeFilter.h"
#include "itkRecurssiveMaximumComponentTreeFilter.h"

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
    std::cerr << "usage: " << argv[0] << " inputImage outputImage connectivity nb" << std::endl;
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
  maxtree->SetFullyConnected( atoi( argv[3] ) );
//   itk::SimpleFilterWatcher watcher2(maxtree, "filter");

  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 0 > GradientAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 1 > LocalGradientAccessor;

  typedef itk::GradientComponentTreeFilter< TreeType, GradientAccessor > GradientType;
//  typedef itk::LocalGradientComponentTreeFilter< TreeType > GradientType;
  GradientType::Pointer gradient = GradientType::New();
  gradient->SetInput( maxtree->GetOutput() );
//  gradient->SetUseZeroLeaves( true );

//  typedef itk::LocalGradientComponentTreeFilter< TreeType, LocalGradientAccessor > LocalGradientType;
//  typedef itk::VolumeLevellingComponentTreeFilter< TreeType, LocalGradientAccessor > LocalGradientType;
//  typedef itk::LocalIntensityComponentTreeFilter< TreeType, LocalGradientAccessor > LocalGradientType;
//  typedef itk::NumberOfPixelsComponentTreeFilter< TreeType, LocalGradientAccessor > LocalGradientType;
  typedef itk::IntensityComponentTreeFilter< TreeType, LocalGradientAccessor > IntensityType;
  IntensityType::Pointer intensity = IntensityType::New();
  intensity->SetInput(gradient->GetOutput());
  typedef itk::RecurssiveMaximumComponentTreeFilter< TreeType, LocalGradientAccessor > LocalGradientType;
  LocalGradientType::Pointer lgradient = LocalGradientType::New();
  lgradient->SetInput( intensity->GetOutput() );
//  lgradient->SetUseZeroLeaves( true );

  typedef itk::MonotoneComponentTreeFilter< TreeType, GradientAccessor > MonotoneType;
  MonotoneType::Pointer monotone = MonotoneType::New();
//  monotone->SetInput( lmonotone->GetOutput() );
  monotone->SetInput( lgradient->GetOutput() );
  monotone->SetReverseOrdering( true );
  monotone->SetRemoveNodes( true );
  monotone->SetStrictlyMonotone( true );
//   itk::SimpleFilterWatcher watcher(monotone, "monotone");

  typedef itk::KeepNLobesComponentTreeFilter< TreeType, LocalGradientAccessor > KeepType;
  KeepType::Pointer keep = KeepType::New();
  keep->SetInput( monotone->GetOutput() );
  keep->SetNumberOfLobes( atoi( argv[4] ) );
  keep->SetAddNewLeavesToQueue( false );
//  itk::SimpleFilterWatcher watcher(keep, "keep");
  
  keep->Update();
  typedef std::deque< NodeType * > LeavesContainerType;
  LeavesContainerType leaves;
  findLeaves< LeavesContainerType, NodeType >( leaves, keep->GetOutput()->GetRoot() );
  GradientAccessor gradientAccessor;
  LocalGradientAccessor lgradientAccessor;

  for( LeavesContainerType::iterator it = leaves.begin(); it!=leaves.end(); it++ )
    {
    std::cout << (*it)->GetPixel()+0.0 << " " << gradientAccessor(*it) << " " << lgradientAccessor(*it) << std::endl;
    }

  typedef itk::ComponentTreeLeavesToLabelImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer l2i = T2IType::New();
  l2i->SetInput( keep->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();


  return 0;
}

