# Maintainer: x4x 1581191+x4x@users.noreply.github.com
pkgname=abox
pkgver=r15.3d1becc
pkgrel=1
pkgdesc="A minimal busybox-like collection of basic Unix tools"
arch=('x86_64')
url="https://github.com/x4x/abox"
license=('unknown')
depends=('glibc')
makedepends=('git' 'make' 'gcc')
provides=('abox')
source=("$pkgname::git+$url.git")
sha256sums=('SKIP')

pkgver() {
  cd "$srcdir/$pkgname"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
  cd "$srcdir/$pkgname"
  make -j$(nproc)
}

package() {
  cd "$srcdir/$pkgname/build"
  install -Dm755 abox "$pkgdir/usr/bin/abox"
  # Recreate applet symlinks, e.g. cat -> abox
  #for applet in "$srcdir/$pkgname"/applets/*; do
  #  name=$(basename "$applet" .c)
  #  ln -sf abox "$pkgdir/usr/bin/$name"
  #done
}
