# CleanGSUI

\[Avionics\] This repository houses the new-and-improved 'clean' code for UTAT's Ground Station User Interface.

## Usage
To use this software, set up the [build environment](#build) and compile it for the required platform; there are no pre-compiled binaries of CleanGSUI available at the moment. Once the software running, a 'Process' JSON file can be created as described in the [process file documentation](#process-file-format). Load in the file via `Process -> Open Process File` from the menu bar. Then, click `Process -> Start Process` to begin. If a system diagram is required, click `System Diagram -> Open Image` to add it to the right side of the interface. `System Diagram -> Clear Image` can be used to remove this image. Once done with the process, use `Process -> Close Process` to close the current process. A new process can be opened following the same steps as above from this point.

## Process File Format
This section describes the format of the 'Process' JSON file that is consumed by CleanGSUI. Note that while this information may not be up-to-date (there may be experimental features which are not described here), the features described here are guaranteed to work. 

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
`State` objects are the building blocks out of which processes are built. A 'Process' in CleanGSUI is a set of `state` objects which can be progressed through in a logical, controllable manner with automatic assistance of the software. A `state` object has the following fields:
* `"name"`: The name of this state.
* `"safetyRating"`: The safety rating of this state. This is a colour value like `red`, `green`, which highlight the safe limits of approaching equipment in this state.
* `"description"`: A longer description of this state. The sentence(s) here (from all states) will be displayed on the left-side of CleanGSUI's interface.
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
