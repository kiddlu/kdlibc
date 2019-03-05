#define INT_VALUE 7

enum{
    cpp = 1,
    java,
    python,
};

#define language cpp

#ifndef language
#error language must be defined
#endif

#define CPP      1
#define JAVA     2
#define PYTHON   3

#define LANGUAGE CPP
#ifndef LANGUAGE
#error LANGUAGE must be defined
#endif

extern int printf(const char *format, ...);

int main()
{
    printf("INT_VALUE is %d\n",INT_VALUE);

    printf("java is %d\n" ,java);
    printf("language is %d\n" ,language);

    printf("JAVA is %d\n" ,JAVA);
    printf("LANGUAGE is %d\n" ,LANGUAGE);

#if (language == java)
    printf("java\n");
#elif (language == cpp)
    printf("cpp\n");
#else
    printf("unknown\n");
#endif

#if (LANGUAGE == JAVA)
    printf("JAVA\n");
#elif (LANGUAGE == CPP)
    printf("CPP\n");
#else
    printf("unknown\n");
#endif
}
