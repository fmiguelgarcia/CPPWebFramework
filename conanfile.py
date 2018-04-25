from conans import ConanFile, tools
import shutil
import os 

class CWFConan(ConanFile):
    # Conan 
    name = "CWF"
    version = "3.0"
    description = """The C++ Web Framework (CWF) is a MVC web framework, Open
    Source, under MIT License, created by Herik Lima and Marcelo Eler, using C++
    with Qt to be used in the development of web applications. The CWF was
    designed to consume few computational resources, such as memory and
    processing, and have a low response time for requests. With MVC
    (Model-View-Controller) architecture, where you can create classes to take
    care of the business layer (Model), use CSTL (C++ Server Pages Standard Tag
    Library) within the Web Pages to take care of data presentation (View) and
    use the controllers as a between the two layers (Controller)."""

    license = "MIT"
    url = "https://www.cppwebframework.com"
    no_copy_source = True
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CPPWebFramework*"
    options = { 
        "qt_version": "ANY"
    }

    def configure(self):
        self.options.qt_version = os.popen("qmake -query QT_VERSION").read().strip()
        self.output.info("Configure Qt Version: %s" % self.options.qt_version)

    #def source(self):
        # Get source code
    #    self.run( "git clone https://github.com/HerikLyma/CPPWebFramework.git") 

    def build(self):
        qmake_options = "CONFIG+=debug " if self.settings.build_type == "Debug" else "CONFIG+=release "
        qmake_options += "CONFIG+=warn_on " 
        qmake_options += "CONFIG+=c++11 " if self.settings.compiler.libcxx == 'libstdc++11' else ''

        self.output.info( 'Qmake options: %s' % qmake_options)
       
        srcDir = os.path.join( self.source_folder, "CPPWebFramework")
        self.run( "qmake {} {}".format( qmake_options, srcDir))

        if self.settings.compiler == "Visual Studio":
            with tools.environment_append( {"CL": "/MP"}):
                self.run( "nmake")
        else:
            self.run( "make -j %d" % tools.cpu_count())

    def package(self):
        self.copy( pattern="*.h", src="CPPWebFramework", dst="include/", keep_path=True)
        self.copy( pattern="*", src="CPPWebFramework/server", dst="share/", keep_path=True)
        self.copy( pattern="libCPPWebFramework.so*", dst="lib/", keep_path=False, symlinks=True)

    def package_info(self):
        self.cpp_info.libs.extend(["CPPWebFramework"])

