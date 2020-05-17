
### `Intro`
This is a project that uses openvr to take two screen shots one of the left eye and one for the right eye. This is an example of how to get that data. I don't know if this is remotely a good way of going about it this is the first time in ages that've touched c++. 

#### `Usage`
This needs libs found at [here](https://github.com/ValveSoftware/openvr) it needs
- [openVr/lib/win64/openvr_api.lib](https://github.com/ValveSoftware/openvr/blob/master/lib/win64/openvr_api.lib)
- [x64/Release/openvr_api.dll](https://github.com/ValveSoftware/openvr/blob/master/bin/win64/openvr_api.dll)
- [openVr/openvr.h](https://github.com/ValveSoftware/openvr/blob/master/headers/openvr.h)

That the source of those files is there a better way of doing this?

Open the project in [visual studio](https://visualstudio.microsoft.com) and from there you should be able to build the project of run in debug.
You'll need steam vr running at the same time.

#### `Images`
Hope this help someone looking into this
<div style="text-align:center"><img style="width:400px" src="https://github.com/anzerr/vr.screenshot/blob/master/assets/left.bmp?raw=true" /></div>
<div style="text-align:center"><img style="width:400px" src="https://github.com/anzerr/vr.screenshot/blob/master/assets/right.bmp?raw=true" /></div>