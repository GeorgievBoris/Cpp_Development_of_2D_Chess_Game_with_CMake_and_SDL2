cmake_minimum_required(VERSION 3.5.1)

message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
# any CMakeLists.txt that is added to this CMakeList.txt somewhere below, it will have access to the functions from helpers.cmake
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_helpers/helpers.cmake)
message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

project(chess LANGUAGES CXX)

set(_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(_INC_DIR ${_BASE_DIR}/include)
set(_SRC_DIR ${_BASE_DIR}/src)

# to siplify the current content, the game can be "placed" in a library...
# ... but in this case we will enforce the strong rule that ...
# ... the game cannot use functionalities related to the Engine class

add_executable(${PROJECT_NAME}
                ${_INC_DIR}/common/CommonDefines.h

                ${_INC_DIR}/engine/config/EngineCfg.h
                ${_INC_DIR}/engine/Engine.h
                ${_INC_DIR}/engine/EngineCfgLoader.h
                ${_INC_DIR}/game/config/GameCfg.h
                ${_INC_DIR}/game/Game.h
                ${_INC_DIR}/game/board/GameBoard.h
                ${_INC_DIR}/game/board/MoveSelector.h
                ${_INC_DIR}/game/board/GameBoardAnimator.h
                ${_INC_DIR}/game/utils/BoardPos.h
                ${_INC_DIR}/game/utils/BoardUtils.h
                ${_INC_DIR}/game/pieces/types/ChessPiece.h
                # ${_INC_DIR}/game/pieces/types/UnfinishedPiece.h
                ${_INC_DIR}/game/pieces/types/Rook.h
                ${_INC_DIR}/game/pieces/types/Pawn.h
                ${_INC_DIR}/game/pieces/PieceHandler.h
                ${_INC_DIR}/game/pieces/PieceHandlerPopulator.h
                ${_INC_DIR}/game/defines/ChessDefines.h
                ${_INC_DIR}/game/defines/ChessStructs.h
                ${_INC_DIR}/game/proxies/GameBoardProxy.h
                ${_INC_DIR}/game/proxies/GameProxy.h
                ${_INC_DIR}/game/logic/GameLogic.h
                ${_INC_DIR}/game/logic/InputInverter.h
                ${_INC_DIR}/game/panels/PiecePromotionPanel.h
                ${_INC_DIR}/game/panels/buttons/PiecePromotionButton.h
                ${_INC_DIR}/game/panels/config/PiecePromotionPanelCfg.h
                ${_INC_DIR}/game/panels/config/PiecePromotionButtonCfg.h
                # headers created by me:
                ${_INC_DIR}/game/pieces/types/Bishop.h
                ${_INC_DIR}/game/pieces/types/Knight.h
                ${_INC_DIR}/game/pieces/types/Queen.h
                ${_INC_DIR}/game/pieces/types/King.h
                ${_INC_DIR}/StartScreen/StartScreen.h
                ${_INC_DIR}/StartScreen/defines/StartScreenDefines.h
                ${_INC_DIR}/StartScreen/config/StartScreenCfg.h
                ${_INC_DIR}/StartScreen/buttons/StartScreenButton.h
                ${_INC_DIR}/StartScreen/buttons/config/StartScreenButtonCfg.h
                ${_INC_DIR}/game/buttons/QuitGameButton.h
                ${_INC_DIR}/game/animations/WinnerAnimator.h
                ${_INC_DIR}/game/proxies/PieceHandlerProxy.h
                ${_INC_DIR}/game/pieces/PieceMoveAnimator.h
                

                ${_SRC_DIR}/engine/Engine.cpp
                ${_SRC_DIR}/engine/EngineCfgLoader.cpp
                ${_SRC_DIR}/game/Game.cpp
                ${_SRC_DIR}/game/board/GameBoard.cpp
                ${_SRC_DIR}/game/board/MoveSelector.cpp
                ${_SRC_DIR}/game/board/GameBoardAnimator.cpp
                ${_SRC_DIR}/game/utils/BoardUtils.cpp
                ${_SRC_DIR}/game/pieces/types/ChessPiece.cpp
                # ${_SRC_DIR}/game/pieces/types/UnfinishedPiece.cpp
                ${_SRC_DIR}/game/pieces/types/Rook.cpp
                ${_SRC_DIR}/game/pieces/types/Pawn.cpp
                ${_SRC_DIR}/game/pieces/PieceHandler.cpp
                ${_SRC_DIR}/game/pieces/PieceHandlerPopulator.cpp
                ${_SRC_DIR}/game/logic/GameLogic.cpp
                ${_SRC_DIR}/game/logic/InputInverter.cpp
                ${_SRC_DIR}/game/panels/PiecePromotionPanel.cpp
                ${_SRC_DIR}/game/panels/buttons/PiecePromotionButton.cpp
                ${_SRC_DIR}/main.cpp
                # source files created by me:
                ${_SRC_DIR}/game/pieces/types/Bishop.cpp
                ${_SRC_DIR}/game/pieces/types/Knight.cpp
                ${_SRC_DIR}/game/pieces/types/Queen.cpp
                ${_SRC_DIR}/game/pieces/types/King.cpp                
                ${_SRC_DIR}/StartScreen/StartScreen.cpp
                ${_SRC_DIR}/StartScreen/buttons/StartScreenButton.cpp
                ${_SRC_DIR}/game/buttons/QuitGameButton.cpp
                ${_SRC_DIR}/game/animations/WinnerAnimator.cpp
                ${_SRC_DIR}/game/pieces/PieceMoveAnimator.cpp
)

set_target_cpp_standard(${PROJECT_NAME} 17)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
        PRIVATE
            ${_INC_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
        PRIVATE
            manager_utils # write this down in the book: the moment we link to the "manager_utils" library, we will take everything that is PUBLIC in the "target_include_directories()" of "manager_utils"
)


if(${CMAKE_BUILD_TYPE} MATCHES Release)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
        -DRELEASE_BUILD=1
)

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD # POST_BUILD or PRE_BUILD are possible entries
        COMMAND ${CMAKE_COMMAND} -E copy_directory

        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
else()
    # add_subdirectory(test) # commented temporarily while finishing the Chess
endif()

add_subdirectory(utils) # here the name of the folder must be added, NOT the name of the binary
add_subdirectory(${_BASE_DIR}/sdl_utils) # here the name of the folder must be added, NOT the name of the binary
add_subdirectory(${_BASE_DIR}/manager_utils) # here the name of the folder must be added, NOT the name of the binary



# The below code and explanations by Zhivko are in response to Martin's question:
# Martin's quesiton: "Idea for the lecture on Thursday: Packaging of the already built (Linux or Windows)...
#... applicaiton with all necessary files and libraries needed for its work on a different machine (preferably without being...
#... necessary to install anything else / whatever else, but not to copy-paste the folder with the files)" 

# install() command of CMake. With it, we give rules where we want to have our application installed...
# ... and for example we do a .deb package
# because we link only to manager_utils in "target_link libraries", here we put only manager_utils

# RUNTIME -> says where the binary / game will go when it is UNPACKED !!!
# LIBRARY -> will put dependencies which we "carry" together with our own application, for instance libraries, if there are any
# DESTINATION -> will put installation files, if there are any

# Zhivko: I have set the "/home/" directory for RUNTIME DESTINATION, LIBRARY DESTINATION and DESTINATION
# ... because I know that this directory will exist for any other Linux OS, BUT ...
# ... if we want to be 100% accurate we have to write a big program called "installer" which is a complex task...
# ... the "installer" permits the user to choose the location where he/she wants to install and do many other things too

install(
    TARGETS ${PROJECT_NAME}
    COMPONENT manager_utils 
    RUNTIME DESTINATION "/home/" #
    LIBRARY DESTINATION "/home/" 
    DESTINATION "/home/" 
)

# then we use the "cpack" tool
# the great thing is that CMake has integration with many, many other tools, including cpack

#configuration of cpack - many, many other OPTIONAL optons exist - the below two are compulsory !!!
SET(CPACK_GENERATOR "DEB") # "DEB" or "RPM" can be set
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Zhivko Petrov") #required
# if we wish, we can give version of our applicaton (i.e. 1.00, 1.01, etc) with other OPTIONAL configuration parameters
# we can explicitly tell it it what are the dependencies - again an OPTIONAL configuration parameter

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # shared Library dependencies (i.e. for sdl)

#this line must be added last, because CPack consumes what configuration we have set in the above lines
INCLUDE (CPack)

# after we type "cmake ..", "make -j" AND "cpack" on separate rows, we get a .deb file "chess-0.1.1-Linux.deb"...
# this ".deb" file can be send to any person and he/she can install it in Linux OS !!!
# can install it: 1) via a visual tool OR 2) via the terminal
# if we do it via the terminal we use the command: "sudo dpkg -i chess-0.1.1-Linux.deb"
# since we already have the exactly the same version of the applicaiton, it will unpack the same version on itself...
# BUT if had a new version in the .deb package, we would have updated the version that is already installed on the PC...
# a HINT: Linux updates work exactly in the same way - we update the things that we already have

# So we have installed in the "/home/" directory. So if we navigate via the terminal to this location and type "ll"...
# ... we will see that the unpacked "chess*" binary is displayed
# if we attempt to run it via "./chess" it will run successfully and will attempt to initialize...
# ... BUT errors will be thrown - these are the if statements with the "return EXIT_FAILURE" checks that we have typed in the initialization parts of our code
# Zhivko did the build in DEBUG mode (i.e. cmake .. -DCMAKE_BUILD_TYPE=Debug)
# He changed to Release mode (i.e. cmake .. -DCMAKE_BUILD_TYPE=Release) and got the same errors...
# ... after again building the applicaiton, running cpack and attempting to run "./chess"
# So again "chess" cannot be run, because we do not have all the needed .png files.
# The applicaiton searches for them where the binary is located at "/home/"
# But to copy .png files is a more dificult task - it can happen with CMake of course, but we may have headaches when we...
# ... attempt to place them in a SYSTEM DIRECTORY - for this reason all games are usually NOT installed in C:/win32/etc...
# .. , but are installed in some folder C:/games/etc.. which is in "our user" i.e. we have permissions here
# So we can either do copy->paste of the .png files and place them where the binary is located ("/home/"), ...
# ... OR we can do a hack to see that things are working:  We are gonna create a shortcut to them ...
# ... (i.e. in Linux the so called symbolic link)... use the following command, in which "f" in "-sf" means force replace the link if it already exists...
# ... "sudo ln -sf ~/workspace/...navigate to the project location and resources folder .../resources/ ." NOTE: the "." is very important it means the current directory
# ... to check if the symbolic link is successfully created, type "ll" and check if "resources -> /home/..../resources/" is exists...
# FINALLY, we type "./chess" and the game is running successfully !!! - mission accomplished !!!

# the installers of all games / applicaitons do the same thing - they have greatly compressed the applicaiton to be extra small...
# ... and they use algorithms for decompression, ask where to install it, do setups of some variables , etc...
# ... and also "hide" the resources (i.e. pictures) so that we cannot steal them easily



# One Final thing:
# if we type "dpkg -I chess-0.1.1-Linux.deb" it will list options (i.e. package, version, maintainer, etc...) related to the package...
# ... but we do not see any dependencies related information -> use " set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) "
# ... so we re-generate the .deb package ( via cmake .. and cpack commands in the terminal)...
# ... so if we now type again "dpkg -I chess-0.1.1-Linux.deb" we see on what its dependency tree depends...
# ... (Depends: libc6 ..., libgcc1 ..., libsdl2 ..., libsdl2-image ..., libsdl2-mixer ..., libsdl2-ttf ..., libstdc++6 ...)


# Zhivko: " For Windows, I have not dealt with it...Some colleague has written on the forum  that...
#... a cross-platform compilation exists, if we use such a tool chain we can generate, we can generate artificats...
#... for several operational systems. Like the .deb package and also for windows and to upload them somewhere, ...
#... from where somebody can download them. Of course writing an installer for all OS(s) is not an easy task...
#... for this reason we see that not every applicaiton/game is supported for all OS(s)"

# Zhivko attempted to run the chess via the file explorer by DOUBLE CLICKING on the chess file in the build folder...
#... BUT the applicaiton did not start. There is one additional thing that must be done in order to be able to run
#... applications in such a way ( not via the terminal but with the mouse and file exporer). Zhivko did not know what is...
#... the problem.
# There is a way: Put it on the desktop, but not just by copy-pasting the binary on the desktop. A special entries must be written...
#... through which we say what is the type of the applicaiton, what are its permissions, etc... so you can run it...
#... if you do such an entry for your game, of course you will have shortcut for your applicaiton...
#... Unfortunatelly, for Linux it is a bit tricky to do it.
# Answer by TDS: " Must create the so called "desktop file". It acts as a launcher of the programme...
#... Can look up for it in google. It is very easy to do it."

# Zhivko's reply to Martin's question ends here !!!


# Martin's second question: "Idea for the lecture on Thursday: ....
# ... Unit Testing(setup for some of the popular frameworks, 1-2 example tests)"

# Zhivko: "I choose to use the Google Framework - G-Test. G-test is used for unit tests and functional tests.
# G-Mock is the second option, but it is more advanced, but is usage is more specific. Unit tests means to be able to test...
#... certain functionalities, classes, functions, etc. Check whether they work correctly. Such tests greatly help to capture...
#.... any bugs while/after writing the code."

# Zhivko: "To start the testing framework, as a starter we must decide if we want to test our applicaiton, ...
# ... or test a certain library. A test for the "utils" library must be inside the utils library...
#... Same goes for the sdl_utils library. Each library has tests for itself. In this case this means that each library...
#... will setup this framework separately for itslef, which is a time-consuming task. If we are going to do a smaller project,...
#... can setup the framework only at one place - outside at our binary. After that we "stretch a hand"...
#... downstream and pick any required library. 

#Zhivko: "It is good that we made libraries for the chess. If utils, sdl_utils and manager_utils were not libraries...
#... all files would have been listed in the add_executable() here in this file...
#... When we start writing tests, we will create new binaries. Each new test will be...
#... a new binary. A binary may contain 10 tests. And now, one file must be included and compiled for each test...
#... separately. Image if we have 20 files for each test, to compile the same files what will happen - a lot of time-consuming...
#... compilation and a lot of hamalogia. But the good thing is that we have libraries. We build them once, and after that we...
#... only link to them. Our chess binary and our tests binaries (separate binaries) will directly link to any the ready library.
# NOTE: if we want to write tests for "chess", each single file or a sequence of files that we want to test, we must compile...
#... them here and there (at 2 places). To escape from this, we must turn the whole chess into a library too. For instance all...
#... files in add_executable - leave only the main.cpp. Main.cpp will only link to this new chess library. In this way...
#... in theory we will reuse all files, if we want to test them. We are just going to link to this library. This is left...
#... as a task for the more avanturistic of us."

# Zhivko: "Lets setup a basic working example. We start by creating a test folder."

# Zhivko: "Kolkoto po-rano se nauchim da polzvame unit testings - tolkova po-dobre. Ima celi firmi, celi branshove, koito...
# ne polzvat testove, no ima i takiva koito sa vmaniacheni da gi polzvat. Mozhe da ima otdel ot testeri - QA koito testvat...
# vsqko edno prilozhenie. No po-dobria variant e developers da imat pone nqkakuv basic suit ot testove koito da runnvat...
# za da xvashtat greshki, predi oshte da sa dostignali do QA ekip. Ili ako nqmate QA ekip, ti kato developer si i QA i ti...
# shte si spestish bezsunni noshti i debugvame v production ako hvanesh bugovete oshte predi da sa se sluchili. No spokoino...
# tova e mnogo, mnogo advance information - s vremeto i praktika se nauchava. Fakta che go znaem sega e bonus tochki! "