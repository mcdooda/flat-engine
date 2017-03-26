#ifndef FLAT_MEMORY_H
#define FLAT_MEMORY_H

#define FLAT_DELETE(obj) (delete obj, obj = nullptr)

#define FLAT_DELETE_ARRAY(obj) (delete[] obj, obj = nullptr);

#define FLAT_FREE(obj) free(obj); obj = nullptr;

#define FLAT_INIT_VALUE 0x11
#define FLAT_WIPE_VALUE 0xEE

#endif // FLAT_MEMORY_H



