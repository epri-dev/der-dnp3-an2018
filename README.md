# DER Outstation AN2018

DER Outstation 2018 is a lightweight, portable, fast and tested implementation of the Application Note AN2018-001 for Communications with Distributed Energy Resources (DERs)

## Dependencies

* [OpenDNP3](https://github.com/dnp3/opendnp3/)
* [OpenSSL](https://www.openssl.org/)

## OpenSSL

The easiest way to install openssl on Windows is using the binary distributions [here](https://slproweb.com/products/Win32OpenSSL.html).

## Building the Outstation

The application can be built for any platform supported by OpenDNP3.

The build uses [cmake](https://cmake.org/) to generate a platform-specific build file (e.g. makefile or visual studio solution)

To generate the build files, clone the repository and CD into it, then:

```
mkdir build
cd build
cmake ..
```

On Linux you can just continue with make:

```
make
```

On Windows you can just open the generated VS solution file.

## Running the Outstation 

Navigate to the ~/der-an-2018 directory and run the following command from the terminal:  
```
./der
```

## Documentation

The detailed user manual can be found [here](https://www.epri.com)
