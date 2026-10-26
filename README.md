These are source codes for my Bachelor's thesis: Visualization of tactile and aural sensations (2D game in C++/OpenGL)

README in czech follows:

Zdrojové kódy jsou ve složce /src

Projekt vyžaduje následující (nachází se ve složce /src/libs):
libpng (include, lib)
glm (testováno s verzí 0.9.5.1) (include)
Otevřete solution v MS Visual Studiu, přidejte výše uvedné dle Include a Library Directories v nabídce VC++ Directories

V Linker - Input přidejte soubory s příponou lib:
..\lib\glew32s.lib
..\lib\glew32.lib
OpenGL32.lib
libpng.lib
zlib.lib

Přeložený spustitelný soubor je ve složce /bin

K běhu je vyžadována dynamická knihovna Msvcp110.dll, nejsnažší řešení je instalace
Visual C++ Redistributable for Visual Studio 2012 Update 4 x86
ke stažení zde http://www.microsoft.com/en-us/download/details.aspx?id=30679

More: [BP Slepec](https://romanluks.eu/blog/bp_slepec/)
