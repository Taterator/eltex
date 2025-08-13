# Part 1

PC1> ip 192.168.0.1/24  
Checking for duplicate address...  
PC1 : 192.168.0.1 255.255.255.0

PC2> ip 192.168.0.2/24  
Checking for duplicate address...  
PC2 : 192.168.0.2 255.255.255.0 

PC1> ping 192.168.0.2

84 bytes from 192.168.0.2 icmp_seq=1 ttl=64 time=2.533 ms  
84 bytes from 192.168.0.2 icmp_seq=2 ttl=64 time=5.157 ms  
84 bytes from 192.168.0.2 icmp_seq=3 ttl=64 time=1.599 ms  
84 bytes from 192.168.0.2 icmp_seq=4 ttl=64 time=5.544 ms  
84 bytes from 192.168.0.2 icmp_seq=5 ttl=64 time=0.525 ms

# Part 2

R1#conf t  
R1#conf terminal  
Enter configuration commands, one per line.  End with CNTL/Z.  
R1(config)#int e0/0  
R1(config-if)#ip address 192.168.1.1 255.255.255.0  
R1(config-if)#no shutdown  
R1(config-if)#  
*Mar  1 00:06:15.603: %LINK-3-UPDOWN: Interface Ethernet0/0, changed state to up  
*Mar  1 00:06:16.605: %LINEPROTO-5-UPDOWN: Line protocol on Interface Ethernet0/0, changed state to up  
R1(config-if)#exit  
R1(config)#int e1/0  
R1(config-if)#ip add  
R1(config-if)#ip address 192.168.2.1 255.255.255.0  
R1(config-if)#no shutdown  
R1(config-if)#  
*Mar  1 00:07:24.785: %LINK-3-UPDOWN: Interface Ethernet1/0, changed state to up  
*Mar  1 00:07:25.786: %LINEPROTO-5-UPDOWN: Line protocol on Interface Ethernet1/0, changed state to up  
R1(config-if)#end  
R1#  
*Mar  1 00:07:40.903: %SYS-5-CONFIG_I: Configured from console by console  

PC3> ip 192.168.1.2/24 192.168.1.1  
Checking for duplicate address...  
PC3 : 192.168.1.2 255.255.255.0 gateway 192.168.1.1  

PC4> ip 192.168.2.2/24 192.168.2.1  
Checking for duplicate address...  
PC4 : 192.168.2.2 255.255.255.0 gateway 192.168.2.1

PC3> ping 192.168.2.2


84 bytes from 192.168.2.2 icmp_seq=1 ttl=63 time=29.436 ms  
84 bytes from 192.168.2.2 icmp_seq=2 ttl=63 time=17.852 ms  
84 bytes from 192.168.2.2 icmp_seq=3 ttl=63 time=17.438 ms  
84 bytes from 192.168.2.2 icmp_seq=4 ttl=63 time=16.391 ms  
84 bytes from 192.168.2.2 icmp_seq=5 ttl=63 time=15.827 ms




