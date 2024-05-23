# NIDAQmx addon for C++ console applications
This repository contains my customized addons for DAQ C++ console applications.

As a student, I faced difficulties in developing practical applications for my research, projects, and experiments. Managing multiple APIs or libraries within a single main() function was especially challenging.

To address this issue, I decided to develop addons for simpler and easier integration.

These addons aim to facilitate simple, easy, and high-level implementation, prioritizing usability over deep and low-level utilization. While the current functions may be limited, I intend to continuously add and upgrade them.

This addon is developed based on Windows Microsoft Visual Studio 2022.

I hope these addons prove helpful to you.


# How to use (For beginners)
You have to manually download API and library to use this addons. 

1. Project configuration  
     a) Create a new project or open your project where you want to add this addon. (Check "Place solution and project in the same directory.")  
     b) Copy and paste "daq" folder into the solution directory.  

2. Project Properties setting  
   a) Project -> Properties -> C/C++ -> General -> Additional Include Directories -> Edit -> Add a new line ".\daq\include"  
   b) Project -> Properties -> C/C++ -> General -> Additional Include Directories -> Edit -> Add a new line ".\daq\src"  
   c) Project -> Properties -> Linker -> General -> Additional Library Directories -> Edit -> Add a new line ".\daq\lib"  
   d) Project -> Properties -> Linker -> Input -> Additional Dependencies -> Edit -> Add a new line "NIDAQmx.lib"  

3. NIDAQmx driver Download  
   https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html#532710 -> (tested with Windows 2024 Q2, but other versions may work)

4. Library location  
   a) Find "NIDAQmx.h" in your_download_path\National Instruments\Shared\ExternalCompilerSupport\C\include (Other files not required)  
   b) Copy and paste these files into ".\daq\include".  
   c) Find "NIDAQmx.lib" in your_download_path\National Instruments\Shared\ExternalCompilerSupport\C\lib32\msvc (32-bit) or your_download_path\National Instruments\Shared\ExternalCompilerSupport\C\lib64\msvc (64-bit).  
   d) Copy and paste these files into ".\daq\lib\x86" (32-bit) or ".\daq\libx64" (64-bit).  

5. Add source codes into the solution, and use them for implementations.  



























