#ifndef FLAT_MEMORY_H
#define FLAT_MEMORY_H

#define FLAT_DELETE(obj) delete obj; obj = nullptr;

#define FLAT_DELETE_ARRAY(obj) delete[] obj; obj = nullptr;

#define FLAT_FREE(obj) free(obj); obj = nullptr;

#endif // FLAT_MEMORY_H



