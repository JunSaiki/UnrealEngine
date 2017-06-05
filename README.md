Getting Started with UE4's AR Sample
====================================

About This Sample
-----------------

This code drop provides an experimental early preview of ARKit support in UE4 as shown at Apple's Worldwide Developers Conference. This code is provided as-is and unsupported. The APIs exposing ARKit are subject to change. We'd recommend shipping titles wait for full support to be released in 4.17.

Prerequisites
-------------

To support iOS 11 and have access to the ARKit framework, you will need to have the latest version of Xcode installed (version 9.0 or later). You can download this directly from Apple at 
 
[https://developer.apple.com/xcode/downloads/](https://developer.apple.com/xcode/downloads/)
 
If you are using a beta Xcode, download and copy Xcode-beta.app into your Applications directory next to your Xcode.app. In a Terminal window, enter this command (you will need an administrator password for this):
 
    sudo xcode-select -s /Applications/Xcode-beta.app/Contents/Developer
 
This will tell our tools to use the beta version of Xcode which has the ARKit and IOS 11 support.
 
And of course, to deploy and run the demo, you will need an ARKit-supported iOS device (starting at the iPhone 6S, iPhone SE, and iPad Pro), running iOS 11 or later. This device has to be provisioned for development use. For more information about setting up signing certificates and provisions for iOS development devices see 
 
[https://docs.unrealengine.com/latest/INT/Platforms/iOS/QuickStart/index.html](https://docs.unrealengine.com/latest/INT/Platforms/iOS/QuickStart/index.html)
 
Setting Up UE4
--------------

After cloning the repository and checking out the dev-ar-kit branch, run Setup.command in the repository root directory to download binary dependencies and content required to run UE4. 

Following that, run the GenerateProjectFiles.sh script in the root folder of your installation folder. This will generate UE4.xcworkspace in the same folder. Open this project in Xcode.
 
Once Xcode is open, select the ShaderCompileWorker scheme and build it.
 
When this is finished, select the UnrealLightmass scheme and build it.
 
When this is finished, select the ARSample scheme. Edit it to be sure the build configuration is Development Editor, then build and run. When the build is finished, it will launch the ARSample project in the UE4 Editor.
 
Installing Device Provisioning Data
-----------------------------------

Once the UE4 editor is running, you'll need to install your signing certificate and provisioning data.
 
Near the top of the window, select Settings and choose Project Settings. In the settings window, scroll down the left pane and click the 'IOS' category, in the Platforms section. In the right pane, use the Import Certificate and Import Provision buttons to add your provision files.
 
Running ARSample on Device
--------------------------

Plug your provisioned iOS 11 device into your Mac, and run the editor. Open the ARSample project as normal. Then, click on the drop down to the right of the Launch button. Select your device from the list. 
 
This will now prepare the project for iOS. The first time will be slower as it caches some changes locally, including compiling the iOS code.
 
Finally it will install the project on your device and run it.
 
When the ARSample app starts on your device, it will ask you to configure your ground alignment. This will determine where your scene will appear to be located in the world. Point your device at a flat surface, then tap a few times until you get a blue rectangle at a comfortable position and size. Tap “Finished”, and the scene will appear where the blue alignment rectangle was. 
 
Choosing Which Renderer to Use
------------------------------

The default renderer for this sample is the desktop forward renderer. It enables a number of higher quality rendering features but support for this renderer on iOS is very experimental! It is going to cost more performance than the mobile renderer and you should expect to find bugs with it.
 
If you like, you can use the mobile renderer instead which also works with ARKit. The feature set is more limited but you can achieve higher framerates. In the Editor, select Settings from the toolbar and choose Project Settings. In the settings window, scroll down the left pane and click the “IOS” category, in the Platforms section. In the right pane, enable “Metal Mobile Renderer” and disable “Metal Desktop-Forward Renderer”.
 
Be Creative!
------------

What can you create with the power of UE4 and Apple's ARKit? Build on top of this sample and share your work with the UE4 community!
 
Any details around your work are welcome in the [Unreal Engine WIP Forums](https://forums.unrealengine.com/forumdisplay.php?11-Work-in-Progress).

Tweet to us! You can tag [@UnrealEngine](https://twitter.com/unrealengine) and use the [#UE4](https://twitter.com/hashtag/UE4?src=hash) hashtag.
