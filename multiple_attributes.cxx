#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkSumComponentTreeFilter.h"
#include "itkPhysicalSizeComponentTreeFilter.h"
#include "itkLocalIntensityComponentTreeFilter.h"
#include "itkVolumeLevellingComponentTreeFilter.h"
#include "itkComponentTreeLeavesToLabelImageFilter.h"
#include "itkNumberOfPixelsComponentTreeFilter.h"
#include "itkLeafComponentTreeFilter.h"
#include "itkIntensityComponentTreeFilter.h"
#include "itkKeepNLobesComponentTreeFilter.h"
#include "itkAttributeFilteringComponentTreeFilter.h"
#include <iomanip>

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

template< class NodeType, class ImageType > void printNodeAttributes( const NodeType * node, const ImageType * img )
{
  for( int i=0; i<NodeType::AttributeType::Dimension; i++ )
    {
    std::cout << node->GetAttribute()[i] << "\t";
    }
  for( int i=0; i<ImageType::ImageDimension; i++ )
    {
    std::cout << img->ComputeIndex( node->GetFirstIndex() )[i] << "\t";
    }
  std::cout << reinterpret_cast<long>(node) << "\t";
  std::cout << reinterpret_cast<long>(node->GetParent()) << "\t";
  std::cout << node->GetChildren().size() << "\t";
  if( node->GetParent() )
    {
    std::cout << node->GetParent()->GetChildren().size() << "\t";
    }
  else
    {
    std::cout << "-1" << "\t";
    }
    
  std::cout << std::endl;
}


template< class NodeType, class ImageType > void printAttributes( const NodeType * node, const ImageType * img )
{
  printNodeAttributes< NodeType, ImageType >( node, img );
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    printAttributes( *it, img );
    }
}


template< class NodeType, class ImageType > void printBranchAttributes( const NodeType * node, const ImageType * img )
{
  printNodeAttributes< NodeType, ImageType >( node, img );
  if( !node->IsRoot() )
    {
    printBranchAttributes< NodeType, ImageType >( node->GetParent(), img );
    }
}


template< class NodeType, class AccessorType > void setBranchAttribute( NodeType * node )
{
  AccessorType accessor;
  accessor( node, 1 );
  if( !node->IsRoot() )
    {
    setBranchAttribute< NodeType, AccessorType >( node->GetParent() );
    }
}


template< class NodeType, class AccessorType > void initAttribute( NodeType * node )
{
  AccessorType accessor;
  accessor( node, 0 );
  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    initAttribute< NodeType, AccessorType >( *it );
    }
}


template< class NodeType, class SpotAccessorType, class BranchAccessorType, class ImageType > void setSpotAttribute( NodeType * node, const ImageType * img )
{
  SpotAccessorType spotAccessor;
  spotAccessor( node, img->GetPixel( img->ComputeIndex( node->GetFirstIndex() ) ) );
  if( spotAccessor( node ) != 0 && node->IsLeaf() )
    {
    setBranchAttribute< NodeType, BranchAccessorType >( node );
    }
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    setSpotAttribute< NodeType, SpotAccessorType, BranchAccessorType, ImageType >( *it, img );
    }
}


template< class NodeType, class SpotAccessorType, class SpotBaseAccessorType > void setSpotBaseAttribute( NodeType * node )
{
  SpotAccessorType spotAccessor;
  SpotBaseAccessorType spotBaseAccessor;

  if( node->GetParent() )
    {
    if( spotAccessor( node ) != spotAccessor( node->GetParent() ) )
      {
      spotBaseAccessor( node, spotAccessor( node ) );
      }
    else
      {
      spotBaseAccessor( node, 0 );
      }
    }
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    setSpotBaseAttribute< NodeType, SpotAccessorType, SpotBaseAccessorType >( *it );
    }
}


int main(int argc, char * argv[])
{
  if( argc < 5 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage connectivity lobes" << std::endl;
    std::cerr << "  inputImage: an input image (up to dim=3)." << std::endl;
    std::cerr << "  outputImage: the value of the attribute for all the pixels, with unsigned long type." << std::endl;
    std::cerr << "  connectivity: 1 for fully connected, or 0" << std::endl;
    exit(1);
    }
    
  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef unsigned long PType2;
  typedef itk::Image< PType2, dim > IType2;

  typedef itk::ComponentTree< PType, dim, itk::FixedArray< double, 10 > > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[3] ) );
//   itk::SimpleFilterWatcher watcher(maxtree, "max tree");

  typedef TreeType::NodeType NodeType;

  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 0 > IntegratedIntensityAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 1 > PhysicalSizeAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 2 > LocalIntensityAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 3 > VolumeLevellingAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 4 > SizeAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 5 > LeafAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 6 > IntensityAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 7 > SpotAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 8 > BranchAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< NodeType, double, 9 > SpotBaseAccessor;

  typedef itk::SumComponentTreeFilter< TreeType, IntegratedIntensityAccessor > IntegratedIntensityType;
  IntegratedIntensityType::Pointer integrated_intensity = IntegratedIntensityType::New();
  integrated_intensity->SetInput( maxtree->GetOutput() );
//   itk::SimpleFilterWatcher watcher0(integrated_intensity, "integrated_intensity");

  typedef itk::PhysicalSizeComponentTreeFilter< TreeType, PhysicalSizeAccessor > PhysicalSizeType;
  PhysicalSizeType::Pointer physical_size = PhysicalSizeType::New();
  physical_size->SetInput( integrated_intensity->GetOutput() );
//   itk::SimpleFilterWatcher watcher1(physical_size, "physical_size");

  typedef itk::LocalIntensityComponentTreeFilter< TreeType, LocalIntensityAccessor > LocalIntensityType;
  LocalIntensityType::Pointer local_intensity = LocalIntensityType::New();
  local_intensity->SetInput( physical_size->GetOutput() );
//   itk::SimpleFilterWatcher watcher2(local_intensity, "local_intensity");

  typedef itk::VolumeLevellingComponentTreeFilter< TreeType, VolumeLevellingAccessor > VolumeLevellingType;
  VolumeLevellingType::Pointer volume_levelling = VolumeLevellingType::New();
  volume_levelling->SetInput( local_intensity->GetOutput() );
//   itk::SimpleFilterWatcher watcher3(volume_levelling, "volume_levelling");

  typedef itk::NumberOfPixelsComponentTreeFilter< TreeType, SizeAccessor > SizeType;
  SizeType::Pointer size = SizeType::New();
  size->SetInput( volume_levelling->GetOutput() );
//   itk::SimpleFilterWatcher watcher4(size, "size");

  typedef itk::LeafComponentTreeFilter< TreeType, LeafAccessor > LeafType;
  LeafType::Pointer leaf = LeafType::New();
  leaf->SetInput( size->GetOutput() );
//   itk::SimpleFilterWatcher watcher5(leaf, "size");

  typedef itk::IntensityComponentTreeFilter< TreeType, IntensityAccessor > IntensityType;
  IntensityType::Pointer intensity = IntensityType::New();
  intensity->SetInput( leaf->GetOutput() );
//   itk::SimpleFilterWatcher watcher6(intensity, "intensity");

  typedef itk::AttributeFilteringComponentTreeFilter< TreeType, PhysicalSizeAccessor > FilteringType;
  FilteringType::Pointer filtering = FilteringType::New();
  filtering->SetInput( intensity->GetOutput() );
  filtering->SetLambda( 0.7 );
  filtering->SetFilteringType( "Direct" );
  filtering->SetReverseOrdering( true );
  filtering->SetInPlace( false );

  typedef itk::KeepNLobesComponentTreeFilter< TreeType, VolumeLevellingAccessor > KeepType;
  KeepType::Pointer keep = KeepType::New();
  keep->SetInput( filtering->GetOutput() );
  keep->SetNumberOfLobes( atoi( argv[4] ) );
  keep->SetInPlace( false );

  typedef itk::ComponentTreeLeavesToLabelImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer filter2 = T2IType::New();
  filter2->SetInput( keep->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter2->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  std::cout // << std::fixed
    << "\"integrated_intensity\"" << "\t"
    << "\"physical_size\"" << "\t"
    << "\"local_intensity\"" << "\t"
    << "\"volume_levelling\"" << "\t"
    << "\"size\"" << "\t"
    << "\"leaf\"" << "\t"
    << "\"intensity\"" << "\t"
    << "\"spot\"" << "\t"
    << "\"branch\"" << "\t"
    << "\"spot_base\"" << "\t"
    << "\"x\"" << "\t"
    << "\"y\"" << "\t"
    << "\"z\"" << "\t"
    << "\"node\"" << "\t"
    << "\"parent\"" << "\t"
    << "\"node_children\"" << "\t"
    << "\"parent_children\"" << "\t"
    << std::endl;

  initAttribute< NodeType, SpotAccessor >( intensity->GetOutput()->GetRoot() );
  initAttribute< NodeType, BranchAccessor >( intensity->GetOutput()->GetRoot() );
  setSpotAttribute< NodeType, SpotAccessor, BranchAccessor, IType >( intensity->GetOutput()->GetRoot(), filter2->GetOutput() );
  setSpotBaseAttribute< NodeType, SpotAccessor, SpotBaseAccessor >( intensity->GetOutput()->GetRoot() );

  if( argc == 5 )
    {
  printAttributes( intensity->GetOutput()->GetRoot(), intensity->GetOutput() );
    }
  else
    {
    itk::Index< dim > idx;
    for( int i=0; i<dim; i++ )
      {
      idx[i] = atoi( argv[ i+5 ] );
      }
    printBranchAttributes( intensity->GetOutput()->GetNode( idx ), intensity->GetOutput() );
    }

//   itk::Index<3> idx;
//   idx[0] = 340;
//   idx[1] = 350;
//   idx[2] = 19;
//   std::cout << "region: " << intensity->GetOutput()->GetRequestedRegion() << std::endl;
//   std::cout << "idx: " << idx << "  node: " << intensity->GetOutput()->GetNode( idx ) << std::endl;
//   idx.Fill(-30);
//   std::cout << "idx: " << idx << "  node: " << intensity->GetOutput()->GetNode( intensity->GetOutput()->GetRoot(), idx ) << std::endl;
//   std::cout << "idx: " << idx << "  pixel: " << (double) intensity->GetOutput()->GetPixel( idx ) << std::endl;

  return 0;
}

