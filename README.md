# ZX-Paint

![Build](https://github.com/twelvechairssoftware/zxpaint/workflows/Build/badge.svg)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=twelvechairssoftware_zxpaint&metric=alert_status)](https://sonarcloud.io/dashboard?id=twelvechairssoftware_zxpaint)
## Intro
An open source project to simulate the Sinclair ZX Spectrum's screen drawing capabilities, in the style of a simple modern-ish image manipulation editor! Includes realistic pixel dimensions, and proper attribute-based colour schemes. Why? Simply because the [ZX Spectrum](https://en.wikipedia.org/wiki/ZX_Spectrum) is a unique monster from the 80s that has had such a massive impact on gaming, and demo scenes with its [quirky screen drawing capabilities](https://en.wikipedia.org/wiki/ZX_Spectrum_graphic_modes). Even more honestly - we just wanted to become more familiar with `SDL2` to enhance our skillset for future cross-platform medical simulation software, and just happen to be huge Speccy fans! This is no DPaint, but we hope that other Speccy enthusiasts will enjoy this, too 😊

![Screenshot](https://github.com/twelvechairssoftware/zxpaint/raw/master/images/zxpaint.png)

## Roadmap
1. Basic pixels ✅
2. Window resizing ✅
3. Basic zoom in/out ✅
4. Menu selector buttons (hover and click) ✅
5. Attributes ✅
6. Color/ink/paper selection ✅
7. View panning (maybe scroll bars) 🌤️
8. Basic shapes (line, circle) 🌤️
9. Shape filling 🌤️
10. Text 🌤️
11. Save to file 🌤️
12. Export to `scr` 🌤

✅ - done; 🚧 - in-progress; 🌤️ -  some day, maybe;

## Open source libraries used:
 - SDL2 (https://www.libsdl.org)
 - spdlog (https://github.com/gabime/spdlog)

## Installation (Ubuntu)
 ```bash
 sudo apt-get update
 sudo apt-get install -y build-essential git cmake curl libsdl2-2.0 libsdl2-dev libsdl2-image-dev
 
 curl -L -O https://dl.bintray.com/conan/installers/conan-ubuntu-64_1_32_1.deb
 sudo dpkg -i conan-ubuntu-64_1_32_1.deb
 
 conan profile new default --detect
 conan profile update settings.compiler.libcxx=libstdc++11 default
 conan install . --build=fmt --build=spdlog
 
 cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G "CodeBlocks - Unix Makefiles" .
 cmake --build . --target zxpaint -- -j 2
 ```


## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The code is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2021 Twelve Chairs Software, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
