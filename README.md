# SW Calculator

Program calculates S and W parameters of the 511 annihilation peak of the annihilation radiation. It is possible to estimate the binding electron energies by fitting the CDB spectrum with the contributions from e+ annihilation on electrons with different wave functions. Electrons from conduction band contribute into the spectrum as an inverted parabola. Upon the shape of the parabola we calculate Fermi energy. Check our [research group website SelimLab](http://physics.bgsu.edu/~faselim/).

<img src="http://physics.bgsu.edu/selimlab/wp-content/uploads/2016/12/swcalculator-screenshot.png" alt="SW Calculator desktop application" style="width: 100%;"/>

Application is written in C++ with CERN ROOT libraries and GUI. In order to run the app first we need to install ROOT libraries. Unfortunately it is impossible to do static link your applications object files with the ROOT libraries as well as build their static versions. Moreover, sometimes source headers should be available at runtime.

Currently it is possible to run application either on macOS or Linux. I will try to compile the windows binary whenever the native Windows ROOT 6 build will be released.

## Setup instructions (macOS)

### Installing xCode

Download xCode here [https://developer.apple.com/xcode/downloads/](https://developer.apple.com/xcode/downloads/).Note. I've tested new xCode 9 on macOS Sierra on Nov 4, 2017 and it turned out that it throws some compilation errors. As a temporary workaround I recommend downloading older xCode versions here [https://developer.apple.com/download/more/](https://developer.apple.com/download/more/). I used 8.3.3

Accept xCode license. Open Terminal and enter:

```bash
sudo xcodebuild -license
```

Install xCode command-line tools from Terminal

```bash
xcode-select --install
```

### Installing ROOT

#### Method 1. Binary distribution
Download and install the correct package (.dmg) for your macOS version here [https://root.cern.ch/downloading-root](https://root.cern.ch/downloading-root)

#### Method 2. Compile from source

Sometimes the binary distribution on the ROOT website might not yet be available if you are running a recent release of macOS. In this case you have to build ROOT from source.

You will need to install CMake first. Get it here [https://cmake.org/download/](https://cmake.org/download/). Install a .dmg and enable console use by modifying PATH variable. Do `nano ~/.bash_profile` and add following:

```bash
PATH=$PATH:/Applications/CMake.app/Contents/bin
export PATH
```

Current software requires ROOT to be built with FFTW libraries. First, [http://www.fftw.org/download.html](download) and unpack FFTW sources to your computer. Navigate to the correspondent folder and run:

```bash
./configure
make
sudo make install
```
Next download ROOT sources that can be built on your system. Tip: if you are running macOS High Sierra 10.13 you can go only with ROOT 10.11.02 and newer. I've succeeded building it with xCode 9. Unpack your ROOT sources and do:

```bash
mkdir build
cd build
cmake -Dfftw3=ON -DFFTW_LIBRARY=/usr/local/lib/libfftw3.a -DFFTW_INCLUDE_DIR=/usr/local/include -Droofit=ON /path/to/your/root/source/directory
```
Here we turn on support for `fftw3` and `roofit`. Double check that your paths to `libfftw3.a` and `libfftw3.h` are correct. Next,

```bash
make -j8
```
Here `-j8` specifies number of your CPU cores.

#### Setup ROOT environment variables
After you built the ROOT open Terminal, set **ROOTSYS** variable and add ROOT to **$PATH**. In order to be able to dynamically link your executables with ROOT libraries set **DYLD_LIBRARY_PATH**. Execute `nano ~/.bash_profile` and add following lines:

```bash
ROOTSYS=/Users/petrstepanov/root_v6.11.02
PATH=$PATH:$ROOTSYS/bin
DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$ROOTSYS/lib

export ROOTSYS
export PATH
export DYLD_LIBRARY_PATH
```

Instructions for other platforms can be found here [https://root.cern.ch/root/EnvVars.html](https://root.cern.ch/root/EnvVars.html)

Now you should be able to run ROOT by typing `root` in Terminal.

### Building the ROOT application
Optional. Some GUI ROOT apps require X server. Not sure if this program really needs it but anyway. Download and install xQuartz from [https://www.xquartz.org](https://www.xquartz.org)

Download and unpack source package from this page. Green button `Clone or download` on the top right of the page.

Open Terminal, navigate into the unpacked folder in Terminal and type `make`. Your executable will show up under `dist/` folder.

Cern ROOT apps sometimes require sources at runtime. Declare `ROOT_INCLUDE_PATH` environment variable containing the path to the software's unpacked folder. Again open `nano ~/.bash_profile` and add following line:

```bash
export ROOT_INCLUDE_PATH=/path/to/your/application/folder:$ROOT_INCLUDE_PATH
```

### Creating the launcher
Open `Automator` macOS application. Select `Application` and click `Choose` button on the bottom right. In the list of action locate and drag `Run AppleScript` to the right actions panel. Enter following text into the AppleScript textarea

```bash
on run {input, parameters}
  tell application "Terminal"
    activate
    do script with command "cd /path/to/executable/dist/folder/;./sw-calculator"
  end tell
end run
```

Save the script and give it a desired name `SW Calculator`? Next in order to change the application icon locate your app in Finder under `/Applications/` folder, right click and select `Show Package Contents`. Go to `Contents/Resources`. Replace `AutomatorApplet.icns` with an `.icns` file from the `resources` folder in GitHub repo but keep the name `AutomatorApplet.icns`. Voila now you can see a neat icon in Spotlight. Dang cool.

## Setup instructions (Ubuntu-based linux)

Download CERN root binary distribution that corresponds to the linux version you have [https://root.cern.ch/downloading-root](https://root.cern.ch/downloading-root). Unpack and move the files to the desired location.

Set environment variables. Open terminal and type `nano ~/.bashrc`. In the end of file add following:

```bash
export ROOTSYS=/home/your-user-name/path-to-your-ROOT-folder
export PATH=$PATH:$ROOTSYS/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib
```

Save file. Run `source ~/.bashrc`. Now you should be able to run ROOT by typing `root` in terminal.

Install Git if you don't have it. In terminal run `sudo apt-get install git`. Navigate to the folder where the software will be located and download the sources. Do:

```bash
git clone https://github.com/petrstepanov/sw-calculator.git
cd sw-calculator
make
sudo make install
```

In order to run the application type `sw-calculator` in terminal. Below find instructions on how to create a desktop launcher.

### Creating a launcher

We can create a launcher and have `TLIST Processor` in the Ubuntu application list. One of the ways to do it is following. First create a script to launch the application. It is an ok practice to keep scripts under `~/bin` directory in home folder.
```bash
mkdir ~/bin; cd ~/bin
nano ./sw-calculator.sh
```

The script will navigate to the application folder and run it. Also this is gonna be a separate bash instance so we need to re-declare the `LD_LIBRARY_PATH` environment variable in order for the session to access ROOT shared libraries.
```bash
#!/bin/bash
export ROOTSYS=/home/your-user-name/path-to-your-ROOT-folder
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib

cd /home/your-user-name/path-to-your-application-folder/dist
./sw-calculator
exec bash
```

Next, we save script and make it executable `chmod +x ./sw-calculator.sh`. Add your `~/bin` folder to `PATH`. Also, add `~/bin` to PATH variable in `~/.bashrc`:

```bash
export PATH=$PATH:/home/your-user-name/bin
```

Now we will create an application launcher. Open Terminal and type `nano ~/.local/share/applications/sw-calculator.desktop`. Insert following content:

```bash
[Desktop Entry]
Type=Application
Name=SW Calculator
Comment=Doppler spectra processor
Exec=sw-calculator.sh
Icon=/home/your-user-name/path-to-your-application-folder/resources/sw-calculator.png
Categories=Utility
Keywords=sw;doppler;positron;calculator;
```

Tip. If you want to have the terminal output window as well, set `Exec=gnome-terminal -e "sw-calculator.sh"` on Ubuntu or `Exec=pantheon-terminal -e "sw-calculator.sh"` on systems with Pantheon desktop environment like elementary OS.

Save file and make it executable `chmod +x ~/.local/share/applications/sw-calculator.desktop`.

Now the SW Calculator icon appear under the applications menu on your system. You can run it from there.

---

Shoot me an email with feedback or questions: [petrs@bgsu.edu](mailto:petrs@bgsu.edu)
