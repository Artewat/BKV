
                    Cypress Semiconductor Corporation
                   CyUSB Suite For Linux, version 1.0.5
                       Updated by Ho-Ro to use Qt5
                   ====================================

Pre-requisites:
---------------

 1. libusb-1.0.x is required for compilation and functioning of the API library.

 2. Native gcc/g++ tool-chain and the GNU make utility are required for
    compiling the library and application.

 3. Qt5 development packages are required for building the cyusb GUI application.

 4. If you want to build a Debian package you need also the packages
    'checkinstall' and 'fakeroot'.

 5. The pidof command is used by the cyusb_linux application to handle
    hot-plug of USB devices.


Installation Steps:
-------------------

 1. cd to the main directory where the files were extracted. 
    
 2. 'make lib' compiles the libcyusb library and creates a dynamic library.
    'make gui' compiles the cyusb_linux GUI application.
    'make all' combines both steps above.
    You can test the GUI application before installation: 'bin/cyusb'
    (To use the auto detection of newly installed USB devices you have to install the program.) 
    
 3. 'make install' installs the libcyusb library and the application program 
    in the system directories (/usr/local/lib and /usr/local/bin).
    It also sets up a set of UDEV rules in /etc/udev/rules.d 
    and updates the environment variables under the /etc/profile.d directory.
    It installs the global config file '/etc/cyusb.conf' (origin is directory 'configs').
    For a clean uninstall call 'make uninstall', this command removes the library,
    the application program and the configurations from the system directories.
    
    As these changes require root (super user) permissions,
    'make install' and 'make uninstall' need to be executed from a root login, e.g.:
    'sudo make install'

 4. 'make deb' creates lib and gui and creates a debian package that installs and uninstalls 
    cleanly under debian and ubuntu. The package building can be done as user. 
    You must be root to install the *.deb package, e.g.:
    'sudo apt install ./cyusb_1.0.5-1_amd64.deb'

 5. The GUI application can now be launched using the 'cyusb' command. 
    You should call it always from a command line to make use of the status messages.

 6. Programs using the library libcyusb require a valid config file, this is either
    the global one '/etc/cyusb.conf' or an individual file for each user,
    '~/.config/cyusb/cyusb.conf', which takes precedence over the global config.

EEPROM:
-------

If you only want to store an application file on the *large* EEPROM of the FX2, you can also use
the CLI tool [fx2eeprom](https://github.com/Ho-Ro/fx2eeprom) that requires less preparation effort.
