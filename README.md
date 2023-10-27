# GHInterfacesCSGO

Simple base hook for CS:GO on linux


# Running 

![csgo](https://labs.moblog.in/style/image/csgo/run_cs.png)
![csgo](https://labs.moblog.in/style/image/csgo/hook_done.png)


# Compile 

Adjust the settings ![config.hpp](config/config.hpp) !! to point to the correct path relative to the folder.

```
cd build
cmake ..
make
```

# Installation

> `mv launcher_client.so bin/linux64/launcher_client.so`

# Help ? 

How does the code work? take a look at the article I made to explain how it develops

> [Reconstructing the vtable of CS:GO on Linux. ](https://www.moblog.in/2023-06-28-reconstruct-csgo-vtable)
