LGPL just because im borrowing a bit from QT and they require it.

I don't think any of the math functions match std::cmath one to one. 
I'm aiming to be faster, with a margin for error. All of them are the same up until at least the fourth decimal place, a few are good til the fifth.
If C++26 constexpr math ever releases and it's cross platform compatible, I'll swap to it.

I'd like to improve the math functions more, improving speed and maintaining/improving accuracy. Not really sure where to start with that, just getting the functions working with the first implementation I find. Might even try to homebrew some functions, but we'll see.

I'd like to have a preprocessor branch at some point, so that the user can tweak the speed and accuracy of the functions. Preferably with /fp:precise and such, but I'll get there when I get there.

### TODO
* simd
* speed improvements
* DX, Vulkan, and GLSL customizations for the Camera section (it's currently just using my Vulkan version and I don't even know if it's correct)


### References, in no particular order:

* https://github.com/Rinzii/ccmath
* https://en.wikipedia.org/wiki/Fast_inverse_square_root
* https://codebrowser.dev/qt5/qtbase/src/corelib/kernel/qmath.cpp.html#qt_sine_table
* https://developer.download.nvidia.com/cg/index_stdlib.html
* https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrices-what-you-need-to-know-first.html
