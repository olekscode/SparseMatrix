# GMRES
This is my implementation of GMRES () algorithm.
Sparse matrix formats CSR (Compressed Sparse Row) and CSIR (Compressed Sparse (lower triangle) Row), a.k.a. "Skyline format", are also self-implemented.

## Catalog structure
```
.
+-- doc/
|   +-- gmres.pdf
|   +-- sparse.pdf
+-- src/
|   +-- sparse/
|       +-- csr.h
|       +-- csir.h
|       +-- exception.h
|   +-- gmres.h
|   +-- main.cpp
+-- README.md
```

## References
1) М. Ю. Баландин, Э. П. Шурина "Методы решения СЛАУ большой размерности"