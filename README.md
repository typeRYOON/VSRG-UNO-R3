<a name="readme-top"></a>



<!-- PROJECT SHIELDS -->
<div align="center">
  <a href="https://github.com/typeRYOON/VSRG-UNO-R3/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/typeRYOON/VSRG-UNO-R3.svg?style=for-the-badge">
  </a>
  <a href="https://github.com/typeRYOON/VSRG-UNO-R3/stargazers">
    <img src="https://img.shields.io/github/stars/typeRYOON/VSRG-UNO-R3.svg?style=for-the-badge">
  </a>
  <a href="https://github.com/typeRYOON/VSRG-UNO-R3/issues">
    <img src="https://img.shields.io/github/issues/typeRYOON/VSRG-UNO-R3.svg?style=for-the-badge">
  </a>
  <a href="https://github.com/typeRYOON/VSRG-UNO-R3/blob/master/LICENSE.md">
    <img src="https://img.shields.io/github/license/typeRYOON/VSRG-UNO-R3.svg?style=for-the-badge">
  </a>
</div>


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/typeRYOON/VSRG-UNO-R3">
    <img src="res/github/logo.jpg" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">VSRG-UNO-R3</h3>

  <p align="center">
    Arduino VSRG Auto Player w/ Stats Display!
    <br />
    <a href="https://github.com/typeRYOON/VSRG-UNO-R3/tree/main/docs"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/typeRYOON/VSRG-UNO-R3">View Demo</a>
    ·
    <a href="https://github.com/typeRYOON/VSRG-UNO-R3/issues">Report Bug</a>
    ·
    <a href="https://github.com/typeRYOON/VSRG-UNO-R3/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

Here's a blank template to get started:

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![Qt][Qt]][Qt-url]
* [![C++][C++]][C++-url]
* [![LaTex][LaTeX]][LaTeX-url]
* [![Arduino][Arduino]][Arduino-url]
* [![Windows][Windows]][Windows-url]  
* [![Markdown][Markdown]][Markdown-url]  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these steps.

### Prerequisites

* `Windows` : This project only works on Windows due to the usage of `Windows.h`.  
* `Arduino hardware` : Some hardware listed [below](#arduino-hardware) (only if you want to use Arduino features).
* ``

## Installation
Go over to Releases and download `VSRG-UNO-R3.zip` or `VSRG-UNO-R3.7z`.  
Skip the `Building from source` section if you're using the Release binary.

### Building from source

1. Clone the repo
   ```sh
   git clone https://github.com/typeRYOON/VSRG-UNO-R3.git
   cd computer/GUI
   ```
2. Open `CMakeLists.txt` in Qt Creator to open the project.
   ```
   - Please make sure you're using Qt 6.6.2+.
   - Install additional Qt libraries Qt::WebSockets and Qt::SerialPort in the Qt Maintenance Tool.
   ```
3. Build the Debug Build in Qt Creator.
4. In your base Qt folder run `windeployqt6.exe`  
   ```sh
   cd C:/Qt/6.6.2/mingw_64/bin
   windeployqt6 "%PATH_TO_BUILD_EXE_DIR%"
   ```
<p align="right">(<a href="#readme-top">back to top</a>)</p>  

## Arduino Hardware  
> The projects allows you to use 0, 1, or both Arduino microcontrollers at once.  
> Build the below configurations depending on your materials and wanted Arduino features.  

### Arduino 1: Autoplayer  
test123test123

### Arduino 2: Key-strokes / Keys per second display



<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Your Name - [@twitter_handle](https://twitter.com/twitter_handle) - typeRYOON@pm.me

Project Link: [https://github.com/typeRYOON/VSRG-UNO-R3](https://github.com/typeRYOON/VSRG-UNO-R3)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* []()
* []()
* []()

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[product-screenshot]: images/screenshot.png  
[Qt]: https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white  
[Qt-url]: https://www.qt.io/  
[C++]: https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white  
[C++-url]: https://isocpp.org/  
[LaTeX]: https://img.shields.io/badge/latex-%23008080.svg?style=for-the-badge&logo=latex&logoColor=white  
[LaTeX-url]: https://www.latex-project.org/  
[Markdown]: https://img.shields.io/badge/markdown-%23000000.svg?style=for-the-badge&logo=markdown&logoColor=white  
[Markdown-url]: https://www.markdownguide.org/  
[Windows]: https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white  
[Windows-url]: https://www.microsoft.com/en-us/windows
[Arduino]: https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white  
[Arduino-url]: https://www.arduino.cc/  




## VSRG-UNO-R3 : : VSRG Auto Player with Stats Display  
> Note: This project is still very much in development, member Ryan made a README for one of his classes.
---
CS 362 Final Group Project.  
This is a program that requires two Arduino Unos and some additional hardware listed [below.](#arduino-components)

Team Members:  
> Member 1 : : John Ezra See - jsee4  
> Member 2 : : Ryan Magdaleno - rmagd2

## Prerequisites  
Please make sure you have the following prerequisites:
* A computer with 2 USB ports  
* C++23 Compiler  
* [Arduino Hardware](#arduino-components)

## Arduino Components  
> WIP project, parts may change.
* 2 Arduino Unos
* Breadboard  
* Nokia 5110 LCD  
* 16x2 LCD Display  
* 2 10K Ohm Potentiometers  
* 3 220 Ohm Resistors  
* 4/7 5V solenoids
* 4/7 MOSFETs
* Holder for Solenoids
* Many wires

## Downloading the source code
Clone the repository:
```
git clone https://github.com/typeRYOON/VSRG-UNO-R3
cd VSRG-UNO-R3
```

### Getting Started
I can't give more instructions as the project hasn't gotten to a finished state yet.  
> Current Status: Implemented Software at a rudimentary level, will expand on it later.

## License
Distributed under the MIT License. See `LICENSE` for more information.


