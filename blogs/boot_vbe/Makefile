all: boot_vbe.img 

boot_vbe.o : boot_vbe.s  
	    as -o $@ $<  
boot_vbe : boot_vbe.o
	    ld --oformat binary -N -Ttext 0x7c00 -o $@ $<  
boot_vbe.img: boot_vbe
	    dd if=./boot_vbe of=boot_vbe.img bs=512 count=1  

clean:  
	    rm ./boot_vbe ./boot_vbe.img *.o bochsout.txt
