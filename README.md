# LKM-mkdir-SysCallHook
LKM (linux kernel module) to hook syscall functions


Build the .ko (kernel object):
```
git clone https://github.com/Sargastico/LKM-mkdir-SysCallHook.git && cd LKM-mkdir-SysCallHook && make
```

Load the kernel module:
```
sudo insmod main.ko
```

Check the kernel log for lkm output (create a folder anywhere to see the hook working):
```
sudo tail -f /var/log/kern.log
```

