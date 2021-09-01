<h1 align="center">Carina</h1>
<p align="center">
    <img src="https://github.com/UTATRocketry/Carina/workflows/Build%20and%20test/badge.svg">
    <img src="https://github.com/UTATRocketry/Carina/workflows/Make%20release/badge.svg">
</p>

[Avionics] Carina: The new and improved user interface and driver software for the UTAT Ground Station.

![image](https://user-images.githubusercontent.com/25436568/131623179-44d48654-0c7c-4eb8-a3ec-a1b31fdcd67a.png)
<p align="center"><em>Carina running a test process on a Raspberry Pi</em></p>

## Usage
Once the software running, a 'Process' JSON file can be created as described in the [process file documentation](#process-file-format). Load in the file via `Process -> Open Process File` from the menu bar. Then, click `Process -> Start Process` to begin. If a system diagram is required, click `System Diagram -> Open Image` to add it to the right side of the interface. `System Diagram -> Clear Image` can be used to remove this image. Once done with the process, use `Process -> Close Process` to close the current process. A new process can be opened following the same steps as above from this point.

## Process File Format
This section describes the format of the 'Process' JSON file that is consumed by Carina. Note that while this information may not be up-to-date (there may be experimental features which are not described here), the features described here are guaranteed to work. 

### Overall structure
This file consists of three main sections: `sensors`, `actuators`, and `states`
```
{
    "sensors": { ... },
        "id1": { ... },
        "id2": { ... }
    },
    "actuators": {
        "id3": { ... },
        "id4": { ... },
        "id5": { ... }
    },
    "states": {
        "start": { ... },
        "id7": { ... },
        "id1": { ... }
    }
}
```
Each section consists of JSON objects with fields specific to that type (i.e. `sensor` objects will have different fields from `state` objects). These fields will be elaborated upon in their respective sections. However, all such objects contain an **id**, or identification field. These ids **must be unique** and these unique names **must be provided by you**. Also, note that the `states` section **must** have one state with id `"start"`. Failure to do any of these things will currently result in **unrealiable** behaviour.

### Sensors
Only sensors which output a single floating-point value are currently supported. A `sensor` object has the following fields:
* `name`: The name of this sensor.
```
"id1": { 
  "name": "Pressure Sensor 1"
}
```

### Actuators
Only actuators which have two states (open/close, etc.) are currently supported. An `actuator` object has the following fields:
* `name`: The name of this actuator.
```
"id3": { 
  "name": "Vent Valve Actuator"
}
```

### States
`State` objects are the building blocks out of which processes are built. A 'Process' in Carina is a set of `state` objects which can be progressed through in a logical, controllable manner with automatic assistance of the software. A `state` object has the following fields:
* `"name"`: The name of this state.
* `"safetyRating"`: The safety rating of this state. This is a colour value like `red`, `green`, which highlight the safe limits of approaching equipment in this state.
* `"description"`: A longer description of this state. The sentence(s) here (from all states) will be displayed on the left-side of Carina's interface.
* `"actions"`: The list of sensors and actuators that must be visible in this state. More information on this field is provided [here](#actions).
* `"transitions"`: An object of state ids to (currently) 'proceed' to (when the Proceed button is clicked), or 'abort' to (when the Abort button is clicked).
* `"checks"`" An object of checks to perform on the sensors/actuators in `actions` upon transition to another state. More information on this field is provided [here](#checks).
```
"start": {
    "name": "Start state",
    "safetyRating": "green",
    "description": "This is the example start state.",
    "actions": [
        {
            "id": "id3",
            ...
        },
        {
            "id": "id2",
            ...
        },
        {
            "id": "id4",
            ...
        }
    ],
    "transitions": {
        "proceed": "state2",
        "abort": null
    },
    "checks": {
        ...
    }
}
```

#### `actions`
The `actions` array consists of a number of unnamed JSON objects for each actuator or sensor that must be visible in this state. This object must contain the `id` field of the actuator/sensor. This **must** be on of the IDs defined in the `sensors` or `actuators` fields. If the object is referring to a sensor, no extra fields are currently supported. If the object is referring to an actuator, the following fields are currently supported:
* `"timed"`: If set to `true`, a timer will indicate how long it's been since this actuator has been actuated.
* `"automatic"`: If set to `true`, this actuator will be automatically triggered when this state is entered.
```
{
    "id": "id4",
    "timed": true,
    "automatic": false
}
```

#### `checks`
The `checks` field consists of two objects, one for checking actuator/sensors on transition via `proceed`, and one for doing so via `abort`. Separate checks on the same actuators can thus be defined depending on the transition. If no checks are desired, this field can be omitted. If checks are desired, the object must contain the ID of the sensor/actuator to be checked, and
* `"open"` / `"close"` if the ID is of an actuator and must be open/close upon exit
or
* `[a, b]` if the ID is of a sensor, where `a, b` are integers. The state cannot be exited until the sensor value is in between these values.
```
"checks": {
    "proceed": {
        "id4": "close",
        "id2": [50, 100]
    },
    "abort": {

    }
}
```

## Setup and Build
### Setup
Carina is built using C++14 and Qt5. While the majority of the codebase can be compiled on any OS and architecture that has a C++14-compiliant compiler, we have decided to develop on Linux to ensure consistent behaviour and reproducibility. Thus, if your OS isn't Linux-based (i.e. Windows, Mac OS, etc.), you will need to set up a virtual machine to work on Carina. There are two ways to do this:

#### 1. With Docker (recommended)
Docker is a product which allows applications be developed and run in isolated, reproducible Linux environments known as 'containers'. The `Dockerfile` in this repository builds a 'one-click' Docker image which contains all of Carina's dependencies, a C++ compiler, and other tools pre-installed.
1. [Mac, Windows] First, install [Docker Desktop](https://docs.docker.com/desktop/) for your operating system. On Windows, use the WSL 2 backend if you're unsure which to choose. Ensure Docker is installed by running `docker --version` - if you see `Docker version ..., build ...`, you're set.
2. Pull the pre-built image with `docker pull ghcr.io/utatrocketry/carina_dev_img:latest`. This download is around 2.6Gb, so it may take a while.  **Note:** If it's much slower than your average internet speed, GitHub's bandwith limitations may have been exceeded. If so, skip this step.
3. Download and extract the zip of this repository.
4. [If you skipped Step 2] Run `docker -t carina_dev_img .` inside the `Carina` folder to build the image locally. **Note:** This will take a while.
5. Run `docker run -d -t -p 3390:3390 --name carina_dev_img_cont --mount type=bind,source=<absolute path to Carina>,target="/root/Desktop/Carina" carina_dev_img` to start the container. **Note:** The path to Carina must be *absolute*. Otherwise, this may not work.
6. Run `docker exec -it carina_dev_img_cont bash`. This will replace your terminal with `bash` running inside the container.
7. Type `/etc/init.d/xrdp stop` and then `/etc/init.d/xrdp start` to start the remote RDP server.
8. [Mac] Download and install [Microsoft Remote Desktop](https://apps.apple.com/us/app/microsoft-remote-desktop/id1295203466?mt=12). [Windows]  Open the 'Remote Desktop Connection' app.
9. Connect to `localhost:3390`. In the login screen, enter `root` for the username and `password` for the password.

At this point, you should see a standard desktop environment. The Carina repository will be on the Desktop.

#### 2. Installing Linux on VirtualBox
Alternatively, you can set up a normal virtual machine on your system. These instructions detail how to install Ubuntu 20.04 on VirtualBox, although you can install any distro of your choice (Fedora, Debian, etc).
1. Download VirtualBox [here](https://www.virtualbox.org/wiki/Downloads) and install it.
2. Download an Ubuntu image file for version 20.04 or above [here](https://ubuntu.com/download/desktop)
3. Start VirtualBox and click `New` to set up a new virtual machine.
4. Enter a name for the virtual machine (ex. Ubuntu 20.04 LTS), choose a folder that will hold the virtual machine file, and select Linux and Ubuntu (64-bit) in the drop-down menus. Click `Next` once done.
5. Choose how much RAM to allocate to the virtual machine. Ubuntu recommends 4 GB (4096 MB), but the virtual machine will still work fine with say 3 GB (3072 MB). It is recommended to stay within the green side of the slider when allocating the RAM. Click `Next` once done.
6. Choose to create a virtual hard disk and click `Create`. In the following window, make sure VDI (VirtualBox Disk Image) is selected, then click Next.
7. Make sure Dynamically allocated is selected and click `Next`. In the following window, make sure that the virtual hard disk image file is selected, then choose to reserve 30-40 GB to the virtual box. Click `Create` once done. *Note*: If you wish to increase the size in storage that you reserve in the future, you can do so through the VirtualBox settings for this virtual machine, then by increasing the partition size in the virtual machine itself.
8. Start the virtual machine. Load the Ubuntu image file that you downloaded into the virtual CD drive dialog box. After that, the standard Ubunutu installer will open. Choose the appropriate options like keyboard type, etc. through the guided installation process and install Ubuntu. Ubuntu's installer provides an option to install either a minimal install or a full install. You may want to proceed with a minimal install to conserve disk space.

**Note:** At one point in the installation you will be warned that all your existing files on your storage drive will be deleted. However, since the OS is being installed on a virtual machine, your existing files will not be affected, and you can safely proceed.

**Note:** Some images of VirtualBox during the installation process can be found [here](https://docs.google.com/document/d/1vtiW0nMQoRikPAnt-vooWnDsOmRCbjK-_7TtYX9y6Fs/edit?usp=sharing). These images will likely be migrated here sometime in the future.

Now, you will need to set up the dependencies:
1. Install Qt5 and Qt Creator from the [official website](https://www.qt.io/download). Make sure to download Qt v5.xx (this should be the default).
2. Install and set up Git for Linux by following the steps [here](https://www.atlassian.com/git/tutorials/install-git). Note that some distros come with Git pre-installed; you can check this by typing `git --version` into a terminal. If you get a version number, then you can skip this step. You may also have to install `gcc, g++, make` and other tools; the terminal will tell you how to do so.
3. Install MCC Universal Library. This is used to communicate with MCC devices. Install it by following the steps [here](https://github.com/mccdaq/uldaq).
4. Install Googletest. This is the testing framework used by Carina. Clone [this repository](https://github.com/google/googletest) into the same directory `Carina` is in.

### Build
Now that you have a Linux environment and all the dependencies set up, you can build Carina. To do so, do the following in a terminal:
```zsh
cd Carina
mkdir build
cd build
# Omit -DULDAQ_AVAILABLE=True if uldaq.h is not needed or present.
# Omit -DWIRINGPI_AVAILABLE=True if wiringpi is not present or needed.
cmake -S .. -B . -DULDAQ_AVAILABLE=True -DWIRINGPI_AVAILABLE=True
# The previous step created a Makefile for the Linux target. This will start the build process. This may take a few minutes.
make
./Carina
```
If you're using Qt Creator for development, click 'Open Project' and select `CMakeLists.txt`. Qt Creator will configure itself appropriately and the green 'Play' button at the bottom right will automatically run a variant of the steps above. However, running them manually first is recommended to ensure that everything is installed properly.

## Contributing
Avionics uses the ClickUp tool for keeping track of new features to add, progress, and other relevant information. The list can be found [here](https://app.clickup.com/t/1g3n9t). If you cannot access this, please see the [Contact](#contact) section and ask the concerned people to add you to the Clickup.

Carina follows the **Clean Architecture** software design paradigm. A short introduction to this paradigm and how Carina uses it can be found [here](https://docs.google.com/document/d/1nojiR9O3od-X_OAvxXngy7_2Q1aDiMwlL0bXFLyTvfU/edit). We highly recommend you read up about Clean Architecture before contributing; otherwise, most of our decisions and flow of control will seem very mysterious.

To contribute code, we follow the 'forking' method, outlined below:
1. Create a fork of this repository to your own personal account and clone it.
2. Make your changes there.
3. Create a pull request to this repository on GitHub.
4. Your pull request will be reviewed - any modifications needed will be communicated. Once the code is satisfactory, it will be merged into the master branch. 
5. Do a `git pull` of this repository to your local clone to bring it up to date. Do this every time you start working on a new feature, otherwise you risk contributing to an 'out of date' version of the software.
GitHub has some good guides about forking, pull requests, etc. Please review these if this process is unclear.


## Contact
Please contact @Ammol or @Vedang on the UTAT Rocketry Slack for any help, questions, etc. 
