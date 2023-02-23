# dlopen (dlfcn.h) trouble
## fatal error: RUNTIME_CHECK or Segmentation fault

I want to know if a certain Linux library (i.e. shared object, .so) contains a symbol (specifically, a function) with a specified name.

In order to do that the library must first be opened by <b><u>dlopen</u></b>:

> https://man7.org/linux/man-pages/man3/dlopen.3.html

Then, using the handle returned by <b><u>dlopen</u></b>, a symbol address must be obtained by <b><u>dlsym</u></b>:

> https://man7.org/linux/man-pages/man3/dlsym.3.html

If that succeeds, it is almost safe to say that the library contains that symbol. In a rare case when <b><u>dlsym</u></b> returns NULL, one has to make sure by subsequently calling <b><u>dlerror</u></b>. If that doesn't return a description (t.i. it returns NULL) the dlsym finding is confirmed: the symbol is found.

That's what is implemented in <b><u>lib_info.cpp</b></u>:

> <b>bool LibInfo::libraryContainsSymbol(string libraryPath, string symbolName)</b>

The <b><span style="color:red">problem</span></b> is, however, that sometimes <b>it doesn't even come to calling <u>dlsym</u></b>, because either

> <i>fatal error: RUNTIME_CHECK(OPENSSL_init_ssl((0x00000200L | 0x00000400L | 0x00001000L | 0x00002000L | 0x00004000L) | 0x00000040L, ((void *)0)) == 1) failed</i>

> <i>Aborted (core dumped)</i>

or
> <i>Segmentation fault (core dumped)</i>

is caused by <b><u>dlopen</u></b> first.

Strangelly, <b>dlopen</b> <u><i>doesn't cause the errors</i> specified above even when given a <b>nonexistent path</b> or a path to an ordinary <b>text file</b></u> as an input.

So it must be something in the .so. Maybe not even the regular .so. Maybe it's somehow gotten corrupt only on my system. Or something else. Even if that's the case, one should nevertheless be able to identify such irregularities before even attempting a dlopen in order to avoid the problems specified above. All I want is to know how.

I am prepared to settle with false negatives (saying that the symbol is not found in .so even if it is, in fact, there). What I'm not prepared to settle with are false positives (saying that the symbol is found when it's not there) nor with Segmentation faults.

Anyone?

