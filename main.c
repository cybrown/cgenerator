#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct {
    int label;
    int result;
} generator_state;

#if __COUNTER__
#endif

#define GENERATOR_VAR \
typedef struct {\
    generator_state state;

#define GENERATOR_BEGIN_IMPL(NAME, RETURN_TYPE, LABEL) \
} generator_state_##NAME;\
RETURN_TYPE NAME (generator_state_##NAME * s);\
RETURN_TYPE generator_run_##NAME (generator_state_##NAME * s) {\
    RETURN_TYPE res = s->state.result;\
    s->state.result = NAME(s);\
    return res;\
}\
generator_state_##NAME * generator_init_state_##NAME() {\
    generator_state_##NAME * ret = (generator_state_##NAME*)memset(malloc(sizeof(generator_state_##NAME)), 0, sizeof(generator_state_##NAME));\
    generator_run_##NAME(ret);\
    return ret;\
}\
RETURN_TYPE NAME (generator_state_##NAME * s) {\
    switch (s->state.label)\
    {\
        case 0:

#define GENERATOR_BEGIN(NAME, RETURN_TYPE) GENERATOR_BEGIN_IMPL(NAME, RETURN_TYPE, __COUNTER__)

#define GENERATOR_END \
    }\
    s->state.label = -1;\
    return 0;\
}

#define GENERATOR_YIELD_IMPL(VALUE, LABEL) \
    s->state.label=LABEL;\
    return VALUE;\
    case LABEL:

#define GENERATOR_YIELD(VALUE) GENERATOR_YIELD_IMPL(VALUE, __COUNTER__)

#define GENERATOR_CREATE(NAME, VAR) generator_state_##NAME * VAR = generator_init_state_##NAME()

#define GENERATOR_RUN(NAME, HANDLE) generator_run_##NAME (HANDLE)

#define GENERATOR_HAS_NEXT(HANDLE) HANDLE->state.label != -1

GENERATOR_VAR
    int a;
    int i;
GENERATOR_BEGIN(test, int)
    for (s->i = 0; s->i < 10; s->i++) {
        GENERATOR_YIELD(s->i);
    }
GENERATOR_END

GENERATOR_VAR
    int prev1;
    int prev2;
    int prev3;
GENERATOR_BEGIN(fib, int)
    s->prev3 = 1;
    while (s->prev1 < 10000) {
        s->prev1 = s->prev2;
        s->prev2 = s->prev3;
        s->prev3 = s->prev1 + s->prev2;
        GENERATOR_YIELD(s->prev1);
    }
GENERATOR_END

int main(void)
{
    GENERATOR_CREATE(fib, gen_fib);
    while (GENERATOR_HAS_NEXT(gen_fib)) {
        printf("%d\n", GENERATOR_RUN(fib, gen_fib));
    }
    printf("Finished\n");
    return 0;
}
