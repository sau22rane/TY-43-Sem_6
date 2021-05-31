MAKEFLAGS += --silent

gppParam = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
assParam = --32
ldParam = -melf_i386

objects = 	obj/loader.o \
			obj/gdt.o \
			obj/hardwareCommunication/port.o \
			obj/drivers/driver.o \
			obj/hardwareCommunication/interrupts.o \
			obj/hardwareCommunication/interruptstub.o \
			obj/hardwareCommunication/pci.o \
			obj/drivers/keyboard.o \
			obj/drivers/mouse.o \
			obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(gppParam) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(assParam) -o $@ $<

kernel.bin: linker.ld $(objects)
	ld $(ldParam) -T $< -o $@ $(objects)

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub

	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg

	echo 'menuentry "Custom OS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso

	rm -rf iso

run: kernel.iso
	VirtualBox --startvm "Custom OS" &

build: kernel.iso

clean:
	rm -rf obj *.o *.bin
	clear