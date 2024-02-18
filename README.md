# SW Calculator

Program calculates S and W parameters of the 511 annihilation peak of the annihilation radiation. It is possible to estimate the binding electron energies by fitting the CDB spectrum with the contributions from e+ annihilation on electrons with different wave functions. Electrons from conduction band contribute into the spectrum as an inverted parabola. Upon the shape of the parabola we calculate Fermi energy. Check our [research group website SelimLab](http://physics.bgsu.edu/~faselim/).

<img src="http://petrstepanov.com/static/screenshot-sw-calculator.png" alt="SW Calculator desktop application" style="width: 100%;"/>

Application is written in C++ with CERN ROOT libraries and GUI. In order to run the app first we need to install ROOT libraries. Unfortunately it is impossible to do static link your applications object files with the ROOT libraries as well as build their static versions. Moreover, sometimes source headers should be available at runtime.

Currently it is possible to run application either on macOS or Linux. I will try to compile the windows binary whenever the native Windows ROOT 6 build will be released.

## Installation on Windows

### Prerequisites

1. Download and install [Visual Studio Build Tools](https://aka.ms/vs/17/release/vs_BuildTools.exe).
2. Download and install ROOT release for Windows. Use `Windows Visual Studio 2022 64-bit x64` executable from the [latest release page](https://root.cern/install/all_releases/). During the installation ensure ROOT environment is added to the system PATH.
3. ROOT releases not necessarily includes the Fourier Transform library (FFTW). [Download it here](http://www.fftw.org/install/windows.html). Use x64 version. Unpack and copy the `libfftw3-3.dll` to ROOT's `bin` folder, e.g. `C:\root_v6.##.##\bin`.
4. Above FFTW library is linked to ROOT's `libFFTW.dll` and should be renamed correctly. Rename `libfftw3-3.dll` to `fftw3-3.dll`.
5. **Optional**. To ensure the correct name for the FFTW library, open ROOT's `libFFTW.dll` in [Dependency Walker](https://dependencywalker.com/) program. The correct name for the FFTW library will be listed as a missing dependency in the top-right panel.

### Compilation

1. Download and extract the program source code (green button on the top of this page).
2. Open Developer Command Prompt from the Start menu.
3. In the Command Prompt navigate to extracted program folder, e.g. `cd C:\Users\<your-user>\Downloads\sw-calculator`.
4. Run `.\install.bat`.

### Execution

8. Find and run `swcalculator.exe` in your ROOT bin folder, e.g. `C:\root-v.6.##.##\bin\swcalculator.exe`.

## Installation on Windows 10 with WSL

1. Install Ubuntu linux inside Windows Subsystem for Linux (WSL) as described [here](https://medium.com/@petrstepanov/installing-cern-root-in-windows-subsystem-for-linux-wsl-e83533a10a18).
2. Open _Start → Ubuntu_ to launch linux Terminal. Proceed with Installation on Linux below.

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
