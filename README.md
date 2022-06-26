# EZmem
V3 of wraloc - a memory debug tool to help understand your program and help find leaks

# Usage
```
curl -O https://raw.githubusercontent.com/lorenuars19/EZmem/main/ezmem.h
```
then
```
#include "ezmem.h"
```

## More info
This repo contains header fragments in the `srcs/` folder which are concatenated to the final ezmem.h
This is done only for ease of developement.
You only need to get the latest `ezmem.h` and include it in the files you want to be tracked.
This project uses `open` and `close` to write and read to files, however it * should * not use more more than one `FD`.
The calls to `open` and I/O manip are all encapsulated inside the wrapped funcs, so it * should * not affect your FDS

# Contribute
This project is still being polished.
Feel free to submit `issues` and `pull requests`, I'll gladly review them.

Thanks for using my tool.
