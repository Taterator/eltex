obj-m += net.o
 
all: net.ko net_client
	
net.ko:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
net_client:
	gcc net_client.c -o net_client

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm net_client
