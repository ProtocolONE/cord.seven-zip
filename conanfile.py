import os
from conans import ConanFile, MSBuild, VisualStudioBuildEnvironment, tools

componentName = "SevenZip"

class CoreConan(ConanFile):
    name = componentName
#    version = "1.0"
    license = "<Put the package license here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Core here>"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "visual_studio"

    scm = {
      "type": "git",
      "url": "auto",
      "revision": "auto"
    }
    
    def build(self):
        os.chdir("depends/7z1701-src/CPP/7zip/Bundles/StaticLib");

        makeHeader = ""

        if self.options.shared == "False":
          makeHeader += "STATIC_LIB=1\n"
          makeHeader += "NO_DEFAULT_RES=1\n"
        
        if self.settings.compiler.runtime == "MD" or self.settings.compiler.runtime == "MDd":
          makeHeader += "MY_SINGLE_THREAD=1\n"
          
        if self.settings.build_type == "Debug":
          makeHeader += "MY_DEBUG=1\n"
          
        makeHeader += '!include "makefile.base"\n'

        tools.save("makefile.conan", makeHeader)
        
        if self.settings.os == "Windows":
          vcvars = tools.vcvars_command(self.settings)
          build_command = ""
          print("{0} && nmake -f makefile.conan".format(vcvars))
          self.run("{0} && nmake -f makefile.conan".format(vcvars))
      
        os.chdir("../../../../../..")
        os.chdir('SevenZip++')
        msbuild = MSBuild(self)
        msbuild.build(
        "SevenZip++.vcxproj".format(componentName)
         , platforms={ 
            "x86" : "Win32"
            ,'x86_64' : 'x64'
          }
        )

    def package(self):
        self.copy("*.h", dst="include/SevenZip", src="SevenZip++")
        self.copy("SevenZip++*.lib", dst="lib", keep_path=False)
        self.copy("SevenZip++*.dll", dst="bin", keep_path=False)
        self.copy("SevenZip++*.dylib*", dst="lib", keep_path=False)
        self.copy("SevenZip++*.so", dst="lib", keep_path=False)
        self.copy("SevenZip++*.a", dst="lib", keep_path=False)

    def package_info(self):
      name = "SevenZip"
        
      if self.settings.build_type == "Debug":
        name += "d"

      name += ".lib"
      self.cpp_info.libs = [name] # The libs to link against
      
      
      self.cpp_info.includedirs = ['include']  # Ordered list of include paths
      self.cpp_info.libdirs = ['lib']  # Directories where libraries can be found
      self.cpp_info.resdirs = ['res']  # Directories where resources, data, etc can be found
      self.cpp_info.bindirs = ['bin']  # Directories where executables and shared libs can be found
      self.cpp_info.defines = []  # preprocessor definitions
      self.cpp_info.cflags = []  # pure C flags
      self.cpp_info.cppflags = []  # C++ compilation flags
      self.cpp_info.sharedlinkflags = []  # linker flags
      self.cpp_info.exelinkflags = []  # linker flags
      
      # if self.options.shared == "False":
        # self.cpp_info.defines.append("{0}_STATIC_LIB".format(componentName.upper()))

        