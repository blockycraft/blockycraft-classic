# Compilation of Blockycraft

**NOTE:** _The project cannot be compiled at the moment, due to issues with dependencies._

Compilation follows the standard process for compilation:

We use **premake4** as our cross-platform build system. First you will need to build all
the static libraries that the projects depend on. To build the libraries, open up a
terminal, and **cd** to the top level of the repository and then run the
following:

```bash
premake4 gmake
make
```

This will build the following static libraries, and place them in the top level **lib** folder:

```bash
> libcs488-framework.a
> libglfw3.a
> libimgui.a
```

Next we can build a specific project.  To do this, **cd** into one of the project folders,
say **src** for example, and run the following terminal commands in order to compile the A3 executable using all .cpp files in the Blockycraft source directory:

```bash
cd src/
premake4 gmake
make
./Blockycraft
```

## Dependencies

The project has dependencies on:

* [GLEW](http://glew.sourceforge.net/)
* [SDL](https://www.libsdl.org/)
* [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/)
