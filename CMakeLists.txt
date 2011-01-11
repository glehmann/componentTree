#Change PROJECT_NAME to the name of your project
PROJECT(componentTree)

# required to avoid warnings while running cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.2)

# cpack stuff
INCLUDE(CPack)

# set the name of the input image used to run the test
SET(INPUT_IMAGE ${CMAKE_SOURCE_DIR}/images/cthead1.png)

#include some macros from another file...
INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)
FIND_PROGRAM(ITK_TEST_DRIVER itkTestDriver)
SET(TEST_COMMAND ${ITK_TEST_DRIVER} --add-before-env PATH ${CMAKE_BINARY_DIR})

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
  ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
  ITKCommon
  ITKBasicFilters
  ITKIO
  ITKReview
)


# install devel files
OPTION(INSTALL_DEVEL_FILES "Install C++ headers" ON)
IF(INSTALL_DEVEL_FILES)
FILE(GLOB develFiles *.h *.txx) 
FOREACH(f ${develFiles})
  INSTALL_FILES(/include/InsightToolkit/BasicFilters FILES ${f})
ENDFOREACH(f)
ENDIF(INSTALL_DEVEL_FILES)



# option for wrapping
OPTION(BUILD_WRAPPERS "Wrap library" OFF)
IF(BUILD_WRAPPERS)
   SUBDIRS(Wrapping)
ENDIF(BUILD_WRAPPERS)
   
   

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project

IF(BUILD_TESTING)

SET(CurrentExe "maxtree")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "mintree")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "nb_of_pixels")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "physical_size")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "sum")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "volume_levelling")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "inplace")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_leaves")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "label_leaves")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "nb_of_pixels_opening")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "physical_size_opening")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "keep_n_lobes")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "multiple_attributes")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "granulometry")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "gradient")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "local_gradient")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "intensity_variation")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "monotone")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "weighted_compactness")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "compactness")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "double_gradient")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "shape")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "math")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "add")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "subtract")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "multiply")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "divide")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "maximum")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "minimum")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "recurssive_add")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "recurssive_maximum")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "recurssive_minimum")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "recurssive_subtract")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "recurssive_multiply")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "recurssive_divide")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

ENDIF(BUILD_TESTING)

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

ADD_TEST(MaxTreeF=0 ${TEST_COMMAND}
   maxtree ${CMAKE_SOURCE_DIR}/images/cthead1.png maxtreeF=0.png 0
   --compare maxtreeF=0.png ${CMAKE_SOURCE_DIR}/images/cthead1.png
)

ADD_TEST(MaxTreeF=1 ${TEST_COMMAND}
   maxtree ${CMAKE_SOURCE_DIR}/images/cthead1.png maxtreeF=1.png 1
   --compare maxtreeF=1.png ${CMAKE_SOURCE_DIR}/images/cthead1.png
)

ADD_TEST(MinTreeF=0 ${TEST_COMMAND}
   mintree ${CMAKE_SOURCE_DIR}/images/cthead1.png mintreeF=0.png 0
   --compare mintreeF=0.png ${CMAKE_SOURCE_DIR}/images/cthead1.png
)

ADD_TEST(MinTreeF=1 ${TEST_COMMAND}
   mintree ${CMAKE_SOURCE_DIR}/images/cthead1.png mintreeF=1.png 1
   --compare mintreeF=1.png ${CMAKE_SOURCE_DIR}/images/cthead1.png
)

ADD_TEST(SizeF=0 ${TEST_COMMAND}
   size ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd sizeF=0.png 0
   --compare sizeF=0.png ${CMAKE_SOURCE_DIR}/images/sizeF=0.png
)

ADD_TEST(SizeF=1 ${TEST_COMMAND}
   size ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd sizeF=1.png 1
   --compare sizeF=1.png ${CMAKE_SOURCE_DIR}/images/sizeF=1.png
)

ADD_TEST(PhysicalSizeF=0 ${TEST_COMMAND}
   physical_size ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd physical_sizeF=0.nrrd 0
   --compare physical_sizeF=0.nrrd ${CMAKE_SOURCE_DIR}/images/physical_sizeF=0.nrrd
)

ADD_TEST(PhysicalSizeF=1 ${TEST_COMMAND}
   physical_size ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd physical_sizeF=1.nrrd 1
   --compare physical_sizeF=1.nrrd ${CMAKE_SOURCE_DIR}/images/physical_sizeF=1.nrrd
)

ADD_TEST(IntegratedIntensityF=0 ${TEST_COMMAND}
   integrated_intensity ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd integrated_intensityF=0.nrrd 0
   --compare integrated_intensityF=0.nrrd ${CMAKE_SOURCE_DIR}/images/integrated_intensityF=0.nrrd
)

ADD_TEST(InPlaceF=0 ${TEST_COMMAND}
   inplace ${CMAKE_SOURCE_DIR}/images/cthead1.png inplaceF=0.png 0
   --compare inplaceF=0.png ${CMAKE_SOURCE_DIR}/images/cthead1.png
)

ADD_TEST(BinaryLeavesF=0 ${TEST_COMMAND}
   binary_leaves ${CMAKE_SOURCE_DIR}/images/cthead1.png binary_leavesF=0.png 0 150 50
   --compare binary_leavesF=0.png ${CMAKE_SOURCE_DIR}/images/binary_leavesF=0.png
)

ADD_TEST(BinaryLeavesF=1 ${TEST_COMMAND}
   binary_leaves ${CMAKE_SOURCE_DIR}/images/cthead1.png binary_leavesF=1.png 1 0 255
   --compare binary_leavesF=1.png ${CMAKE_SOURCE_DIR}/images/binary_leavesF=1.png
)

FOREACH(s 10 100 1000 10000 100000)
  ADD_TEST(SizeOpeningF=0Size=${s} ${TEST_COMMAND}
     size_opening ${CMAKE_SOURCE_DIR}/images/cthead1.png size_openingF=0Size=${s}.png 0 ${s}
     --compare size_openingF=0Size=${s}.png ${CMAKE_SOURCE_DIR}/images/size_openingF=0Size=${s}.png
  )
#  ADD_TEST(SizeOpeningF=1Size=${s} ${TEST_COMMAND}
#     size_opening ${CMAKE_SOURCE_DIR}/images/cthead1.png size_openingF=1Size=${s}.png 1 ${s}
#     --compare size_openingF=1Size=${s}.png ${CMAKE_SOURCE_DIR}/images/size_openingF=1Size=${s}.png
#  )
ENDFOREACH(s)

FOREACH(s 100 1000 10000)
  FOREACH(t Maximum Minimum Direct Subtract)
    FOREACH(o 0 1)
      ADD_TEST(PhysicalSizeOpeningF=0Size=${s}Type=${t}Order=${o} ${TEST_COMMAND}
        physical_size_opening ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd physical_size_openingF=0Size=${s}Type=${t}Order=${o}.png 0 ${s} ${t} ${o}
        --compare physical_size_openingF=0Size=${s}Type=${t}Order=${o}.png ${CMAKE_SOURCE_DIR}/images/physical_size_openingF=0Size=${s}Type=${t}Order=${o}.png
      )
    ENDFOREACH(o)
  ENDFOREACH(t)

#  ADD_TEST(PhysicalSizeOpeningF=1Size=${s} ${TEST_COMMAND}
#     physical_size_opening ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd physical_size_openingF=1Size=${s}.png 1 ${s}
#     --compare physical_size_openingF=1Size=${s}.png ${CMAKE_SOURCE_DIR}/images/physical_size_openingF=1Size=${s}.png
#  )
ENDFOREACH(s)

ADD_TEST(KeepNLobesF=1N=4 ${TEST_COMMAND}
   keep_n_lobes ${CMAKE_SOURCE_DIR}/images/cthead1.png keep_n_lobesF=1N=4.png 1 4
   --compare keep_n_lobesF=1N=4.png ${CMAKE_SOURCE_DIR}/images/keep_n_lobesF=1N=4.png
)

