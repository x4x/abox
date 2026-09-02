# abox
A Box of basic tools. Or another probebly more shity implementation of a busybox.
This is just a fun project!!

## build

```bash
make -j4
```

## use

```bash
cd build

./abox echo "Hello World!" > t.txt
./abox ls
./abox cat t.txt
./abox wc t.txt
./abox rm t.txt

# or
ln -s ./busybox cat
./busybox/cat
```