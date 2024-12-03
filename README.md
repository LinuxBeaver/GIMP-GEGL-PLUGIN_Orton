# X Plugin name

### Description of plugin: 

![image](https://github.com/user-attachments/assets/cc260487-9eae-4c94-8737-7ec44c39b79a)

![image](https://github.com/user-attachments/assets/c74d24d5-036a-4a3a-b739-916f2d372a25)

GEGL does an orton effect based on my following of a orton tutorial. I recreated a photography tutorial in GEGL syntax to make this

# download  below

## Directory to put Binaries (They do NOT go in the normal plugins folder)

**Windows**

 C:\Users\(USERNAME)\AppData\Local\gegl-0.4\plug-ins

 **Linux**

~/.local/share/gegl-0.4/plug-ins

 **Linux (Flatpak includes Chromebook)**

~/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

Then Restart Gimp and go to "GEGL Operation" and look for "Ocean's Surface 2". Gimp 2.99.16+ users will find the filter in Filters>Render>Fun , where as 2.10 plugins will only be found in the drop down list.


## Compiling and Installing

Run `build_plugin_os.sh` then put the plugin in the correct directory listed above or...

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

  
  ## More previews of this based plugin

  ![image](https://github.com/user-attachments/assets/ad591124-a1d2-4764-80a8-9121457d62f1)
