# SW Calculator

Program calculates S and W parameters of the 511 annihilation peak of the annihilation radiation. It is possible to estimate the binding electron energies by fitting the CDB spectrum with the contributions from e+ annihilation on electrons with different wave functions. Electrons from conduction band contribute into the spectrum as an inverted parabola. Upon the shape of the parabola we calculate Fermi energy. Check our [research group website SelimLab](http://physics.bgsu.edu/~faselim/).

<img src="http://petrstepanov.com/static/screenshot-sw-calculator.png" alt="SW Calculator desktop application" style="width: 100%;"/>

Application is written in C++ with CERN ROOT libraries and GUI. In order to run the app first we need to install ROOT libraries. Unfortunately it is impossible to do static link your applications object files with the ROOT libraries as well as build their static versions. Moreover, sometimes source headers should be available at runtime.

Currently it is possible to run application either on macOS or Linux. I will try to compile the windows binary whenever the native Windows ROOT 6 build will be released.

## Installation on Linux or macOS

1. Install CERN ROOT on your system. Details can be found [here](https://medium.com/@petrstepanov/install-cern-root-roofit-on-macos-and-linux-for-dummies-df787fd41ef4).

2. Open your Terminal and clone the program folder on your hard drive:

```
mkdir -p ~/Downloads && cd ~/Downloads
git clone https://github.com/petrstepanov/sw-calculator
cd ./sw-calculator
```

3. Compile the source code and link with ROOT libraries into a binary executable:
```
make
make install
```
4. In order to launch the application type `sw-calculator` in Terminal.

5. Optionally add an application launcher to the list of your system apps. On Linux execute `make install-linux-launcher`. On macOS run `resources/sw-calculator.pkg`.

## Installation on Windows

Not yet implemented. Contributors that familiar with CMAKE are welome.

## Users Guide

Not yet implemented.

---

Shoot me an email with feedback or questions: [petrs@bgsu.edu](mailto:petrs@bgsu.edu)
