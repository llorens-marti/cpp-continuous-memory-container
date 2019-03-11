# CPP Contiguous Memory Container

This repo contains the code of an experimental Contiguous Memory Container.

This container aims to maintain these characteristics:

- All the stored objects must be kept in contiguous memory.
- No gaps between objects if a given object is destroyed.
- The container will manage the creation and destruction of the objects.
- Every object is referenced by a minimum of one "special" pointer.
- No "special" pointer invalidation ever.

The container provides its own shared pointers that will help iterate/access the elements stored on it.

Future improvements:

- Make sure that there is only one container per type and per thread.
- Alow re-arrangement of stored elements at runtime.
- Allow container to detect and remove circular references.

Known problems:

- Code in destructors make destruction code slow. Even if we invalidate everything we can not avoid calling the destructor.


# How to compile

Run this command: `clang main.cpp -std=c++11 -lstdc++ -Werror -O2 -o c.out`

Run the executable: `./c.out`


