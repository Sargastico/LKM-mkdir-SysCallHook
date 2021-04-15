# LKM-SyscallTableHook
LKM (linux kernel module) to hook syscall function calls


Build the .ko (kernel object):
```
git clone https://github.com/Sargastico/LKM-SyscallTableHook/edit/main/README.md && cd LKM-SyscallTableHook && make
```

Load the kernel module:
```
sudo insmod main.ko
```

Check the kernel log for lkm output (create a folder anywhere to see the hook working):
```
sudo tail -f /var/log/kern.log
```

