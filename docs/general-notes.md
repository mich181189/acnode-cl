General Notes
=============

The code is a subset of C++. There are a few things to avoid in an embedded environment though:

Use
====
* structs/classes

Be Careful With
===========
* STL Containers - can cause large code size

Avoid Using
=======
* std::stringstream - very slow and causes huge code bloat

