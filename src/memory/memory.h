#ifndef FLAT_MEMORY_MEMORY_H
#define FLAT_MEMORY_MEMORY_H

#ifdef FLAT_DEBUG

#define FLAT_INIT_VALUE 0x11
#define FLAT_WIPE_VALUE 0xEE

#define FLAT_INIT_MEMORY(address, size) std::memset(address, FLAT_INIT_VALUE, size)
#define FLAT_WIPE_MEMORY(address, size) std::memset(address, FLAT_WIPE_VALUE, size)

#else

#define FLAT_INIT_MEMORY(address, size)
#define FLAT_WIPE_MEMORY(address, size)

#endif

#define FLAT_DELETE(object)       (delete object, object = nullptr)
#define FLAT_DELETE_ARRAY(object) (delete[] object, object = nullptr)
#define FLAT_FREE(object)         free(object); object = nullptr;

#define FLAT_STACK_ALLOCATE(Type)        new (alloca(sizeof(Type))) Type
#define FLAT_STACK_DESTROY(object, Type) { object->~Type(); FLAT_WIPE_MEMORY(object, sizeof(*object)); }

#endif // FLAT_MEMORY_MEMORY_H
