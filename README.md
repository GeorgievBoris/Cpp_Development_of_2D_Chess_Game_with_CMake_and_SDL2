Developing a C++ Application with CMake

First, before providing some details for this repository, some important aspects will be quickly listed.

----------------------------------------- ACKNOWLEDGMENT : START -----------------------------------------

I would like to pay big respect to  Mr. Zhivko Petrov - a very knowledgeable and experienced C/C++ software engineer!

The C++ application presented in this repository was part of a masterclass course that Mr. Zhivko Petrov led as a lecturer.
During the lectures, Mr. Zhivko Petrov attempted to teach his students in professional programming, by applying a few advanced C++ coding practices and
building a certain software architecture concept.

The development of the fundamental logical blocks of this C++ application was demonstrated by him in class.
Each student, who took part in this course, was tasked to finish the remaining detailed parts of the application.
No teamwork between students was expected.

Following the above description, Mr. Zhivko Petrov must be regarded as the father of this C++ application!
My personal contribution to this application involves the completion of its final parts, as required by the course assessmet rules. 

Finally, without Mr. Zhivko Petrov's dedication to teach, share and explain, my personal skills as a C++ developer would have been far less!

Always a big thank you!

----------------------------------------- ACKNOWLEDGMENT : END -----------------------------------------



----------------------------------------- IMPORTANT DETAILS : START -----------------------------------------

Important information about working with the SDL2 library header files (.h)

Make sure that your Integrated Development Environment (IDE) software is instructed to
include the system path of the SDL2 library header files which are located on your machine.
By doing this, you will enable your IDE to index the SDL2 header files and access their contents.
In this way, you can view in your IDE all functions that are part of the SDL2 library and read any useful information
that is related to them (function description, function return type, input/output parameters, etc...).

For developers using Visual Studio Code (VSCode) as an IDE, this can be achieved in the following way:

Step 1: open the file "c_cpp_properties.json" that is located in ".vscode" directory which is generated by VSCode.
        This .JSON file is usually generated by the C/C++ extensions of VSCode

Step 2: add the system path "/usr/include/SDL2" to the "includePath" property.
        The "includePath" property is part of the "configurations" property.

Note: The system path for the SDL2 library header files that is given in Step 2) is valid only for Linux OS.


IMPORTANT:
If you fail to do what is recommended above, you may find yourself in a situation that your project is successfully built and compiled, BUT regardless of that, your IDE keeps marking / highlighting any SDL2 header files in the code as unknown / not recognized / not found / etc. 

----------------------------------------- IMPORTANT DETAILS : END -----------------------------------------


----------------------------------------- DESCRIPTION: START -----------------------------------------

Point 1: 

The purpose of this repository and its commits is to present the step by step evolution of an application (i.e. a game) that is developed in the C++ programming language.

The application is a standard Game of Chess, played by two players.

Point 2:

The applicaiton features only 2D (two-dimensional) graphics.
The SDL2 graphical library is utilised to build this application. 
In very brief, SDL2 works only in 2D, but it functions as a wrapper of the powerful, low-level graphical systems (i.e. Vulkan, Metal, OpenGL, DirextX, etc.)

Point 3:

Last but not least, the CMake (cross-platform) tool is used as a primary means of automating the building process of this application.
Therefore, the "build" directory, which is part of this project, is the dedicated place where the entire "out of source build" process is done by CMake.
Access this directory via your terminal shell and run any necessary build commands there.
This is the directory where the executable / binary file is generated after successful compilation at the end.

If using Linix OS or Mac OS, use CMake in conjunction with the native build system called "Unix Makefiles" to build and compile the project.
If using Windows OS, one option is to use CMake together with "MinGW Makefiles" to do the build and compilation of the project.

Regardless of the OS type that is used, the first step always is to run CMake to produce the necessary build files (usually a "Makefile").
To do this, once you enter into the "build" directory, the command for running CMake is usually "cmake ..".
Then, the second step is to run one of the aforementioned build tools (again, depending on your OS type), in order to commence the actual build / compilation process. 
An alternative to the second step, is to directly run the cross-platform command "cmake --build ." . It works on any type of OS and is an integral feature of CMake that can always be used as a last measure to start building / compiling the applicaiton. However, in brief, this cross-platform command is not considered as a wise option to use and should be avoided.

Point 4:

It must be emphasized that, as a top priority, the final version of this application aims to present a product that offers functionality. Facilitating, the user/player experience, as much as possible, from a practical point of view, is pursued.
Aesthetics / beauty related to graphics is NOT considered as the core goal to be achieved, since this applicaiton is part of a learning course.

Point 5:

The first several commits in this repository aim to demonstrate the capabilities of the SDL2 library and do some random graphical work.
However, parallel with that, these commits also help to build the core logical system of the chess game.
The official chess game development starts at a later stage - will be marked with a git tag.
Please, also view the commit messages for further information.

----------------------------------------- DESCRIPTION: END -----------------------------------------

Any information that is related to the applicaiton development process (i.e. architecture blocks, coding practices, applied programming concepts, etc...) should be searched directly in the content of the Git commits or in the respective commit messages. Adding such information in this README file is possible at a later stage, but not guaranteed.

Information concerning the standard users / players (i.e. User Handbook, etc...), will be added to this README file in the next GitHub commits, as the application development process evolves.


END OF README FILE
