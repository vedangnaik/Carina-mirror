# Clean GroundStationUI

\[Avionics\] This repository houses the new-and-improved code for UTAT's Ground Station User Interface.

## Usage
If you want to use this software, you'll have to set up the [build environment](#build) and compile it yourself for the platform of your choice; there are no pre-compiled binaries of CleanGSUI available at the moment. Once you have the software running, you can create a 'Process' JSON file as described in the [process file documentation]() as required. Load in your file via `Process -> Open Process File` from the menu bar.

## Build
The GroundStationUI software is built using C++ and Qt5. It must be compiled in a Linux environment since the DAQ libraries we use only run on Linux. If you currently run Windows, you must first install Linux on a [virtual machine](#installing-linux-on-virtualbox) or a dual-boot, then [set up the project on Linux](#setting-up-on-linux). Users who already have a Linux environment can immediately start [setting up the project on Linux](#setting-up-on-linux).

### Installing Linux on VirtualBox
This instructions detail how to install Ubuntu 20.04 on VirtualBox, although you can install any distro of your choice (Fedora, Debian, etc). VirtualBox is a program which allows 'guest' operating systems to run inside the 'host' operating system in a sandboxed environment.
1. Download VirtualBox [here](https://www.virtualbox.org/wiki/Downloads) and install it.
1. Download an Ubuntu image file for version 20.04 or above [here](https://ubuntu.com/download/desktop)
1. Start VirtualBox and click ‘New’ to set up a new virtual machine.
1. Enter a name for the virtual machine (ex. Ubuntu 19.10), choose a folder that will hold the virtual machine file, and select Linux and Ubuntu (or whichever Linux distribution you are using) in the drop-down menus. Click Next once done.
1. Choose how much RAM to allocate to the virtual machine. Ubuntu recommends 4 GB (4096 MB), but the virtual machine will still work fine with say 3 GB (3072 MB). It is recommended to stay within the green side of the slider when allocating the RAM. Click Next once done.
1. Choose to create a virtual hard disk and click Create. In the following window, make sure VDI (VirtualBox Disk Image) is selected, then click Next.
1. Make sure Dynamically allocated is selected and click Next. In the following window, make sure that the virtual hard disk image file is selected, then choose to reserve 30-40 GB to the virtual box. Click Create once done. Note: If you wish to increase the size in storage that you reserve in the future, you can do so through the VirtualBox settings for this virtual machine, then by increasing the partition size in the virtual machine itself.
1. Start the virtual machine and select the Ubuntu (or whichever Linux distribution you are using) image file that you downloaded and proceed to install the operating system (OS) through the guided installation process provided by the OS.

Ubuntu's installer provides an option to install either a minimal install or a full install. You may want to proceed with a minimal install to conserve disk space. 

**Note:** At one point in the installation you will be warned that all your existing files on your storage drive will be deleted. However, since the OS is being installed on a virtual machine, your existing files will not be affected, and you can safely proceed.

**Note:** Some images of VirtualBox during the installation process can be found [here](https://docs.google.com/document/d/1vtiW0nMQoRikPAnt-vooWnDsOmRCbjK-_7TtYX9y6Fs/edit?usp=sharing). These images will likely be migrated here sometime in the future.

### Setting up on Linux
1. Install Qt5 and Qt Creator. This can be done in a number of ways. You can either download it from the [official website](https://www.qt.io/download), or from your distro's package manager (for Ubunutu, see [this link](https://www.osradar.com/install-qtcreator-on-ubuntu-20-04-18-04/)). Note that if you go with the former option, you may be required to create an account. Whatever way you choose, make sure to download Qt version 5 or above.
1. Install the MCC Universal Library for Linux by following the steps [here](https://github.com/mccdaq/uldaq)
1. Install and set up Git for Linux by following the steps [here](https://www.atlassian.com/git/tutorials/install-git). Note that some distros come with Git pre-installed; you can check this by typing `git --version` into a terminal. If you get a version number, then you can skip this step.
1. Clone this repository and open the `GroundStationUI.pro` file in Qt Creator.

## Contributing
Avionics uses the ClickUp tool for keeping track of new features to add, progress, and other relevant information. The list can be found [here](https://app.clickup.com/t/1g3n9t). CleanGSUI, as the name suggests, follows the **Clean Architecture** software design paradigm. A short introduction to this paradigm and how CleanGSUI uses it can be found [here](https://docs.google.com/document/d/1nojiR9O3od-X_OAvxXngy7_2Q1aDiMwlL0bXFLyTvfU/edit). We highly recommend you read up about Clean Architecture before contributing; otherwise, most of our decisions and flow of control will seem very mysterious.

## Contact
Please contact @Ammol or @Vedang on the UTAT Rocketry Slack for any help, questions, etc. 
