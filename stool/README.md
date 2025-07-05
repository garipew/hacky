# stool
Buffer overflow practice

## Build
There are several ways of building this, each one asks for a different solution.

```
make
make ret2libc
make printable
```

It is also possible to specify the targeted architecture with
```
make ARCH=64
```

Extra flags could also be passed the same way, although, for now, the only
flag possible is
```
make -DFROMMAIN
```
