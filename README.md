## A few demo programs to illustrate crypto pitfalls

### encrypt-image

Encrypt an uncompressed image in RGB and RGBA format, using ECB and CBC modes.
With the sample image file `secret.gif`, it is trivial to demonstrate that the
ECB mode encrypts the colors but not the shapes.

Disclaimer: For the purpose of the demo, we used "adequate" values and formats.
- The RGBA format uses 4 bytes per pixels, meaning 4 pixels per AES block.
- The image width is a multiple of 4 to ensure an integral number of AES blocks per line.

Example:
~~~
$ ./encrypt-image secret.gif 
Creating secret.gif.rgb.aes.ecb.jpg using AES-128-ECB, key size: 16, block size: 16, IV size: 0, image size: 900x120 (RGB)
Creating secret.gif.rgb.aes.cbc.jpg using AES-128-CBC, key size: 16, block size: 16, IV size: 16, image size: 900x120 (RGB)
Creating secret.gif.rgba.aes.ecb.jpg using AES-128-ECB, key size: 16, block size: 16, IV size: 0, image size: 900x120 (RGBA)
Creating secret.gif.rgba.aes.cbc.jpg using AES-128-CBC, key size: 16, block size: 16, IV size: 16, image size: 900x120 (RGBA)
~~~

### prime-count

Evaluate the number of primes with a given number of bits.
This program illustrates the entropy of RSA keys of a given size

Example: A 4096-bit RSA key is made of two 2048-bit primes.
~~~
$ ./prime-count 2048
Number of primes < 2^2047: 1.1e+613
Number of primes < 2^2048: 2.3e+613
Number of values < 2^2048: 3.2e+616
Number of 2048-bit primes: 1.1e+613
Number of 2048-bit values: 1.6e+616
~~~
Considering that a "N-bit value" is a value with MSB set (Nth bit),
there are approximately 10^616 2048-bit values and 10^613 2048-bit primes.
