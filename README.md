# CPP Contiguous Memory Container

This repo contains the code of an experimental Contiguous Memory Container.

This container aims to maintain these characteristics:

- All the stored objects must be kept in contiguous memory.
- No gaps between objects if a given object is destroyed.
- The container will manage the creation and destruction of the objects.
- Every object is referenced by a minimum of one "special" pointer.
- No "special" pointer invalidation ever.

The container provides its own shared pointers that will help iterate/access the elements stored on it.


# How to compile

Run this command: `clang main.cpp -std=c++11 -lstdc++ -Werror -O2 -o c.out`

Run the executable: `./c.out`


