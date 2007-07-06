#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkComponentTree.h"
#include "itkImageToMaximumTreeFilter.h"
#include "itkIntegratedIntensityComponentTreeFilter.h"
#include "itkPhysicalSizeComponentTreeFilter.h"
#include "itkLocalIntensityComponentTreeFilter.h"
#include "itkVolumeLevellingComponentTreeFilter.h"
#include "itkComponentTreeToImageFilter.h"
#include "itkSizeComponentTreeFilter.h"
#include "itkLeafComponentTreeFilter.h"
#include "itkIntensityComponentTreeFilter.h"

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

  inline AttributeType operator()( ComponentTreeNodeType * node )
    {
    return node->GetAttribute()[ Index ];
    }

  inline void operator()( ComponentTreeNodeType * node, const AttributeType & value )
    {
    node->GetAttribute()[ Index ] = value;
    }
};

}

}

template< class NodeType > void printAttributes( const NodeType * node )
{
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    printAttributes( *it );
    }
  for( int i=0; i<NodeType::AttributeType::Dimension; i++ )
    {
    std::cout << node->GetAttribute()[i] << "\t";
    }
  std::cout << std::endl;
}


template< class NodeType > void printBranchAttributes( const NodeType * node )
{
  for( int i=0; i<NodeType::AttributeType::Dimension; i++ )
    {
    std::cout << node->GetAttribute()[i] << "\t";
    }
  std::cout << std::endl;
  if( !node->IsRoot() )
    {
    printBranchAttributes( node->GetParent() );
    }
}


int main(int argc, char * argv[])
{
  if( argc < 4 )
    {
    std::cerr << "usage: " << argv[0] << " inputImage outputImage connectivity [x y z]" << std::endl;
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

  typedef itk::ComponentTree< PType, dim, itk::FixedArray< double, 7 > > TreeType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ImageToMaximumTreeFilter< IType, TreeType > MaxTreeType;
  MaxTreeType::Pointer maxtree = MaxTreeType::New();
  maxtree->SetInput( reader->GetOutput() );
  maxtree->SetFullyConnected( atoi( argv[3] ) );
//   itk::SimpleFilterWatcher watcher(maxtree, "max tree");

  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< TreeType::NodeType, double, 0 > IntegratedIntensityAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< TreeType::NodeType, double, 1 > PhysicalSizeAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< TreeType::NodeType, double, 2 > LocalIntensityAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< TreeType::NodeType, double, 3 > VolumeLevellingAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< TreeType::NodeType, double, 4 > SizeAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< TreeType::NodeType, double, 5 > LeafAccessor;
  typedef itk::Functor::ArrayAttributeComponentTreeNodeAccessor< TreeType::NodeType, double, 6 > IntensityAccessor;

  typedef itk::IntegratedIntensityComponentTreeFilter< TreeType, IntegratedIntensityAccessor > IntegratedIntensityType;
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

  typedef itk::SizeComponentTreeFilter< TreeType, SizeAccessor > SizeType;
  SizeType::Pointer size = SizeType::New();
  size->SetInput( local_intensity->GetOutput() );
//   itk::SimpleFilterWatcher watcher4(size, "size");

  typedef itk::LeafComponentTreeFilter< TreeType, LeafAccessor > LeafType;
  LeafType::Pointer leaf = LeafType::New();
  leaf->SetInput( size->GetOutput() );
//   itk::SimpleFilterWatcher watcher5(leaf, "size");

  typedef itk::IntensityComponentTreeFilter< TreeType, IntensityAccessor > IntensityType;
  IntensityType::Pointer intensity = IntensityType::New();
  intensity->SetInput( leaf->GetOutput() );
//   itk::SimpleFilterWatcher watcher6(intensity, "intensity");

  typedef itk::ComponentTreeToImageFilter< TreeType, IType > T2IType;
  T2IType::Pointer filter2 = T2IType::New();
  filter2->SetInput( intensity->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter2->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  std::cout << "#integrated_intensity" << "\t"
    << "physical_size" << "\t"
    << "local_intensity" << "\t"
    << "volume_levelling" << "\t"
    << "size" << "\t"
    << "leaf" << "\t"
    << "intensity" << "\t"
    << std::endl;
  if( argc == 4 )
    {
    printAttributes( intensity->GetOutput()->GetRoot() );
    }
  else
    {
    itk::Index< dim > idx;
    for( int i=0; i<dim; i++ )
      {
      idx[i] = atoi( argv[ i+4 ] );
      }
    printBranchAttributes( intensity->GetOutput()->GetNode( idx ) );
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

