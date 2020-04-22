<p align="center">
	<img width="200px" src="https://i.imgur.com/t1hhMP5.png"/>
</p>
<p align="center">
	UVIDEO is a C++ automated program that edits the visual aspects of your clips using some basic color grading and correction. 
</p>
	
---------

<p align="center">
	<a href="">   <img src="https://img.shields.io/badge/Liscence-MIT-green" alt="Liscence"></a>   <img src="https://img.shields.io/badge/Platform-Windows-lightgray" alt="Platform"></a><a href="">   <img src="https://img.shields.io/badge/Version-1.013-blue"alt="Version"></a>
</p>

The main idea of this program is to deprive you of all the **repetitive labor** while editing your videos.
The program will take care of the color balencing and polishing based on the settings that you chose.

You can either choose the **custom mode** and choose all your settings manually. 
Or you can choose the **automatic mode** to analyze some of your previous videos and calculate the settings.

Once everything is setup you can start automatically editing your videos  and stop losing your time doing labor :wink:

<br></br>
**Table of contents**
* [Features](#Features)
* [Usage](#Usage)
* [How-it-works](#How-it-works)
* [How-to-Contribute](#How-to-Contribute)
* [Acknowledgements](#Acknowledgements)
<br></br>

**Features**
---
- Supports color grading/correction 
- Contains optimized image analyzation/edition functions
- It has precalculated default settings based on popular youtube videos
- And offers multiple approches to setup your settings

**Usage**
---
```
-Download the project
-Compile the code
-Run uvideo.exe

Commands :
	-settings		 //Show the current settings
	-settings -s	  	 //Set custom settings
	-settings -a <dir>	 //Calculate settings, by analyzing videos
	-edit <dir>		 //Edit your videos
```
**How it works**
---
The program is based on four fundamental parts for color correction.


**RGB Parade**

We use the rgb parade to change the color balance, by using temperature and tint.
<p align="center"><img width="300px" src="https://i.imgur.com/FDINr3T.png"/></p>
So basically if a color is too present we arrage the temperature and tint so the colors are balanced and natural

<br></br>
**Waveform**

The second part is using a waveform representation of the image to adjust the whites/blacks/shadows
<p align="center"><img width="600px" src="https://i.imgur.com/ATBXpax.png"/></p>

<br></br>
**Vector scope**

We then use a vector scope to adjust the vividity of the image by changing the saturation.

<p align="center"><img width="350px" src="https://i.imgur.com/TaTRGA5.png"/></p>

<br></br>
**Polishing**

And finally we check brigthness and the drakness of image. We then use the exposure and shadows to match the desired look
and apply a mask to improve even more.

**How to Contribute**
---

1. Clone repo and create a new branch: `$ git checkout https://github.com/aflprojects/uvideo -b name_for_new_branch`.
2. Make changes and test
3. Submit Pull Request with comprehensive description of changes

**Acknowledgements**
---
+ Base logo vector made by [Freepik](https://www.freepik.com/) from [Flaticon](www.flaticon.com).

