# SW Calculator

Program calculates S and W parameters of the 511 annihilation peak of the annihilation radiation. It is possible to estimate the binding electron energies by fitting the CDB spectrum with the contributions from e+ annihilation on electrons with different wave functions. Electrons from conduction band contribute into the spectrum as an inverted parabola. Upon the shape of the parabola we calculate Fermi energy. Check our [research group website SelimLab](http://physics.bgsu.edu/~faselim/).

<img src="http://physics.bgsu.edu/selimlab/wp-content/uploads/2016/12/tlist-processor-screenshot.png" alt="SW Calculator desktop application" style="width: 200px;"/>

# Setup instructions

Application is written in C++ with CERN ROOT libraries and GUI. In order to run the app first we need to install ROOT libraries. Unfortunately it is impossible to do static link your applications object files with the ROOT libraries as well as build their static versions. Moreover, sometimes source headers should be available at runtime.

Currently it is possible to run application either on macOS or Linux. I will try to compile the windows binary whenever the native Windows ROOT 6 build will be released.

Below find the instructions for macOS setup. Linux users should be able to

## Installing ROOT

1. Download and install the correct package (.dmg) with respect to your OSX version here [https://root.cern.ch/downloading-root](https://root.cern.ch/downloading-root)
2. Setup ROOT environment variables. Open Terminal and edit .bash_profile in your home folder:

```bash
nano ~/.bash_profile
```

3. Specify **ROOTSYS** variable and add it to **$PATH**. In order to be able to dynamically link your executables with ROOT libraries set **DYLD_LIBRARY_PATH**:

```bash
export ROOTSYS=/Applications/root_v6.10.06/
export PATH=$PATH:$ROOTSYS/bin
export DYLD_LIBRARY_PATH=$ROOTSYS/lib:$DYLD_LIBRARY_PATH
```

Instructions for other platforms can be found here [https://root.cern.ch/root/EnvVars.html](https://root.cern.ch/root/EnvVars.html)

4. Update your environment variables

```bash
source ~/.bash_profile
```

5. Now you should be able to run ROOT by typing `root` in Terminal.

## Installing xCode

6. Download xCode here [https://developer.apple.com/xcode/downloads/](https://developer.apple.com/xcode/downloads/). Important – I've tested new xCode 9 today (Nov 4, 2017) and it turned out that it throws some compilation errors. As a temporary workaround I recommend downloading version 8.3.3 here [https://developer.apple.com/download/more/](https://developer.apple.com/download/more/).

7. Accept xCode license. Open Terminal and enter:

```bash
sudo xcodebuild -license
```

8. Install xCode command-line tools from Terminal

```bash
xcode-select --install
```

## Running the ROOT application

9. Download and unpack source package from this page. Green button `Clone or download` on the top right of the page.

10. Open Terminal, navigate into the unpacked folder in Terminal and type `make`.

9. Some GUI ROOT apps require X server. Not sure if this program really needs it but anyway. Download and install xQuartz from [https://www.xquartz.org](https://www.xquartz.org)

12. Cern ROOT apps sometimes require sources at runtime. Declare `ROOT_INCLUDE_PATH` environment variable containing the path to the software's unpacked folder. Again open `.bash_profile` and add following line:

```bash
export ROOT_INCLUDE_PATH=/path/to/your/application/folder:$ROOT_INCLUDE_PATH
```
