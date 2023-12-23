Rustmas Plugin Template (C++)
=============================

A template for implementing (https://github.com/mrozycki/rustmas)[Rustmas]
animation plugins in C++.

In order to create your own animation, make a copy of this repository.
You will need to make appropriate changes in the `manifest.json` file,
as well as implement any required methods inside the `src/animation.cpp` file
(likely at least `update` and `render`). If you create any additional source 
files, remember to update `CMakeLists.txt`.

After everything is done, simply run `cmake --build build --target package` 
to build a plugin package. Remember that this is a native binary file, so you 
need to build on a machine compatible with the one you are running Rustmas on 
(e.g. on a Raspberry Pi if you're running Rustmas on a Raspberry Pi).

Working with the template
-------------------------

Before you start working on your animation, set up a cmake project. You need
to only do this once. You can do it by running:

```
cmake -S . -B build
```

Then you can build your package with:

```
cmake --build build
```

Although in order to run your animation, you may want to create a package
and unpack it in your plugins folder:

```
cmake --build build --target package
cp animation.tgz $RUSTMAS_PLUGIN_DIRECTORY
cd $RUSTMAS_PLUGIN_DIRECTORY
tar xzf animation.tgz
```

Note: the name of the animation package file is based on your project directory
name, so it likely will not be `animation`.
