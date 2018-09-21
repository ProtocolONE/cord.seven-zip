import os
from conans import ConanFile, MSBuild, VisualStudioBuildEnvironment, tools
from conans.util.files import tmp_file

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
    generators = "visual_studio_multi"

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
        
        libMachine = {
          "x86" : "MachineX86"
          ,'x86_64' : 'MachineX64'
        }.get(self.settings.get_safe("arch"), "")
        
        libMachine_node = "<Lib>" \
                       "<TargetMachine>{}</TargetMachine>" \
                       "</Lib>".format(libMachine) if libMachine else ""
                       
        runtime_library = {"MT": "MultiThreaded",
                     "MTd": "MultiThreadedDebug",
                     "MD": "MultiThreadedDLL",
                     "MDd": "MultiThreadedDebugDLL"}.get(self.settings.get_safe("compiler.runtime"), "")

        runtime_node = "<RuntimeLibrary>" \
                       "{}" \
                       "</RuntimeLibrary>".format(runtime_library) if runtime_library else ""

        props_file_contents = """<?xml version="1.0" encoding="utf-8"?>
<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemDefinitionGroup>
    {0}
    <ClCompile>
      {1}
    </ClCompile>
    
  </ItemDefinitionGroup>
</Project>""".format(libMachine_node, runtime_node)
        with tmp_file(props_file_contents) as props_file_path:
          msbuild = MSBuild(self)
          msbuild.build(
            "SevenZip++.vcxproj"
           , toolset = self.settings.compiler.toolset
           , platforms={ 
              "x86" : "Win32"
              ,'x86_64' : 'x64'
            }
           , properties = {
              "ForceImportBeforeCppTargets" : props_file_path
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

        