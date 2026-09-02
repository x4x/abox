# abox
A Box of basic tools. Or another probebly more shity implementation of a busybox.
This is just a fun project!!

## build

```bash
make -j4
```

### build packages

#### arch AUR

```bash
makepkg --printsrcinfo > .SRCINFO

# build and install package
makepkg -si
```

## use

```bash
cd build

./abox cat 

# or
ln -s ./busybox cat
./busybox/cat
```

