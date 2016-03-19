
Nate Bosscher
March 19, 2016

================================================================

README:


Build requires libjpeg and libpng
On ubuntu execute:

	apt-get install libjpeg-dev libpng-dev

Build requires ImageMagick
On ubuntu execute:

apt-get install graphicsmagick-libmagick-dev-compat or similar
apt-get install imagemagick

Comment out debug function in Magick++/image.h to remove conflict with Xenomai
