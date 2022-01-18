# SW Calculator

Program calculates S and W parameters of the 511 annihilation peak of the annihilation radiation. It is possible to estimate the binding electron energies by fitting the CDB spectrum with the contributions from e+ annihilation on electrons with different wave functions. Electrons from conduction band contribute into the spectrum as an inverted parabola. Upon the shape of the parabola we calculate Fermi energy. Check our [research group website SelimLab](http://physics.bgsu.edu/~faselim/).

<img src="http://petrstepanov.com/static/screenshot-sw-calculator.png" alt="SW Calculator desktop application" style="width: 100%;"/>

Application is written in C++ with CERN ROOT libraries and GUI. In order to run the app first we need to install ROOT libraries. Unfortunately it is impossible to do static link your applications object files with the ROOT libraries as well as build their static versions. Moreover, sometimes source headers should be available at runtime.

Currently it is possible to run application either on macOS or Linux. I will try to compile the windows binary whenever the native Windows ROOT 6 build will be released.

## Installation on Windows 10

1. Install Ubuntu linux inside Windows Subsystem for Linux (WSL) as described [here](https://medium.com/@petrstepanov/installing-cern-root-in-windows-subsystem-for-linux-wsl-e83533a10a18).

2. Open _Start → Ubuntu_ to launch linux Terminal. Proceed with Installation on Linux below.

P.S. Running the program on older versions of Windows is currently not possible.

## Installation on Linux or macOS

Install CERN ROOT on your system. Refer to [official install page](https://root.cern/install/#linux-package-managers) for more details.

Open Terminal and clone program source files in your home folder:

```
cd
git clone https://github.com/petrstepanov/sw-calculator
cd sw-calculator
```
Check program dependencies and create native makefile with CMake. Finally build and install. System will ask for your password to install the app.

```
cmake ./
make
sudo make install
```
On linux you can install application launcher:

```
make install-launcher
```
To install app launcher on macOS run package `resources/sw-calculator.pkg`.

Program source files are not needed any more and can be removed from your computer:

```
rm -rf ~/sw-calculator
```

## Running program

Find application in the system menu. Alternatively, open Termnal and type `sw-calculator` to launch the program. 


---

Shoot me an email with feedback or questions: [stepanovps@gmail.com](mailto:stepanovps@gmail.com)