#define ALLOC(size) global_alloc(size, __FILE__, __LINE__)
#define CALLOC(count, size) global_calloc(count, size)
#define FREE(ptr) global_free(ptr)
#define CREATE_THREAD(func, udata) global_create_thread(func, udata)

global_alloc_t* global_alloc;
global_free_t* global_free;

enum dummy_enum_t { EDUMMY };
inline void* operator new(size_t, dummy_enum_t, void* ptr) { return ptr; }
#define PLACEMENT_NEW(ptr) new(EDUMMY, ptr)
#define NEW(T) new(EDUMMY, ALLOC(sizeof(T))) T

void* global_calloc(size_t count, size_t size)
{
	void* ptr = ALLOC(size * count);
	memset(ptr, 0, count * size);
	return ptr;
}
