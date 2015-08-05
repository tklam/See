# See

Sometimes we want to determine the RGB colour values of an object. *See* is an
application for that purpose. Since I am a layman in the field of computer
vision, the identification result is currently not very accurate. 

The code in this repository is the result of my work done during an upsetting
weekend. You may find the code useful, particularly if you are trying to
integrate Qt/QML with OpenCV. Pull requests are very welcome.

If you just want to use it, you can download the APK
[here](https://dl.dropboxusercontent.com/u/9754213/colourIdentifier.apk).

## Depends on

+ Qt 5.5.0
+ OpenCV 2.4.11.0

## Compilation (for Android)
The code is cross-platform. It should be easy to port the application to iOS.

1. compile Qt for Android. The reference PKGBUILD for ArchLinux is available at:
   [Github Gist](https://gist.github.com/tklam/321e7a66cc6cae4681b2)
2. compile OpenCV for Android ([Reference](http://opencv.org/platforms/android.html))
3. modify the path of the OpenCV SDK in the Qt project file: See.pro
4. build the project in QtCreator

## Usage

1. calibration - fill the white rectangle at the centre of the viewfinder with
   a white object (not light source), and click [Calibrate].
2. identification - fill the white rectangle at the centre of the viewfinder with
   the colour to be detected, and click [Detect colour].

## License
+ GPL 3

## TODOs

+ get rid of all hardcoded values :-)
+ test the application on iOS
