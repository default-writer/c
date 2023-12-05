KernelDefs = postscript.h types.h globals.h errors.h procs.h except.h
KernelObjects = array.o control.o debug.o dict.o error.o exec.o math.o\
		name.o scanner.o stack.o stream.o string.o type.o vm.o
GraphPakDefs = reducer.h graphics.h gray.h graphicspriv.h
GraphPakObjects = graphpak.o reducer.o matrix.o graphics.o gray.o\
		stroke.o path.o image.o
DeviceDefs = framedevice.h
DeviceObjects = device.o nulldevice.o framedevice.o banddevice.o\
		sundev.o versatecdev.o inputdevice.o
FontDefs = fonts.h
FontObjects = font.o fontdisk.o fontcache.o fontshow.o fontbuild.o\
		 xylock.o bezier.o curvefit.o
UserObjects = user.o unix.o

CFLAGS = -O -R -DVAX -DRELEASE

PS: $(KernelObjects) $(GraphPakObjects) $(DeviceObjects) $(FontObjects)\
	$(UserObjects)
	ld -X /mnt/geschke/vaxpslib/crt.o\
		$(KernelObjects) $(GraphPakObjects) $(DeviceObjects)\
		$(FontObjects) $(UserObjects)\
		/mnt/geschke/vaxpslib/pslib.a -o PS
	strip PS
	size PS

$(KernelObjects): $(KernelDefs)
$(GraphPakDefs): $(KernelDefs)
$(DeviceDefs): $(KernelDefs) $(GraphPakDefs)
$(GraphPakObjects): $(KernelDefs) $(GraphPakDefs)
$(DeviceObjects): $(KernelDefs) $(GraphPakDefs) $(DeviceDefs)
$(FontObjects): $(FontDefs) $(GraphPakDefs) $(KernelDefs)
$(UserObjects): $(KernelDefs)
