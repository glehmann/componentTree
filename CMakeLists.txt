#Change PROJECT_NAME to the name of your project
PROJECT(componentTree)

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

SET(CurrentExe "size")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "physical_size")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "integrated_intensity")
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
   physical_size ${CMAKE_SOURCE_DIR}/images/cthead1_small.nrrd integrated_intensityF=0.nrrd 0
   --compare integrated_intensityF=0.nrrd ${CMAKE_SOURCE_DIR}/images/integrated_intensityF=0.nrrd
)

