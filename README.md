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

# What is this, what does it do
This project wraps calls to `malloc` and `free` using `#define`.<br>
It creates an `.ezmem` folder where the program is executed.<br>

## Content of `.ezmem` folder
- README : Short explanation
- .ids.memid : internal file to track current ID whitout the use of global variable 
- log.memlog : Log of all the calls to `malloc` and `free`
- mem/ : contains the memory blocks
- leaks/ : contains the memory blocks that have never been `free`d; A leaked block also contains a memory dump to help you find the source of your leak

At the end of execution a `report.memreport` is generated to summarize the last recorded memory state.

## More info
This repo contains header fragments in the `srcs/` folder which are concatenated to the final ezmem.h, this is done only for ease of developement.<br>
You only need to get the latest `ezmem.h` and include it in the files you want to be tracked.<br>
This project uses `open` and `close` to write and read to files, however it * should * not use more more than one `FD`.<br>
The calls to `open` and I/O manip are all encapsulated inside the wrapped funcs, so it * should * not affect your FDS.<br>

# Contribute
This project is still being polished.<br>
Feel free to submit `issues` and `pull requests`, I'll gladly review them.<br>
<br>
Thanks for using my tool.
