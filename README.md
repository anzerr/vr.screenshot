
### `Intro`
This is a project that uses OpenVR to take two screen shots one of the left eye and one for the right eye. 

This is an example of how to get the eye data. I don't know if this is remotely a good way of going about it this is the first time in ages that have touched c++.

I needed this as a first step in a future project I'm working on and thought it be usefull to have a simple/short example for anyone looking for something similar.

#### `Usage`
This needs libs found at [here](https://github.com/ValveSoftware/openvr) it needs
- [openVr/lib/win64/openvr_api.lib](https://github.com/ValveSoftware/openvr/blob/master/lib/win64/openvr_api.lib)
- [x64/Release/openvr_api.dll](https://github.com/ValveSoftware/openvr/blob/master/bin/win64/openvr_api.dll)
- [openVr/openvr.h](https://github.com/ValveSoftware/openvr/blob/master/headers/openvr.h)
Those are the files the project needs. Is these a better way of handling this?

Open the project in [visual studio](https://visualstudio.microsoft.com) and from there you should be able to build the project or run it in debug.
You'll need steam vr to be running at the same time.

#### `Images`
<p align="center">
    <img width="400" src="https://github.com/anzerr/vr.screenshot/blob/master/assets/left.bmp?raw=true" />
    <img width="400" src="https://github.com/anzerr/vr.screenshot/blob/master/assets/right.bmp?raw=true" />
</p>