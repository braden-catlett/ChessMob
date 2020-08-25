# Designs #
As stated on the home page of this wiki, ChessMob is a modified version of Chess "where each player has a king and one other type of piece. The number of pieces that a player starts with is determined by the type that they chose: 6 queens, 7 rooks, 9 bishops, 13 knights, or 31 pawns." Our intent is to develop this game as an application for both the Android and iOS systems.

This project is split into three main modules: the [game logic and AI back end](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Backend), the [Android interface](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/Android), and the [iOS interface](https://bitbucket.org/ataylor13/chess_mob/wiki/Designs/iOS). The back end module is to be written in C++ too allow for use on both the Android and iOS platforms. The Android interface will be written through the use of the Android NDK to allow for interaction between the Android-specific Java code and the C++ back end. Similarly, the iOS interface will be written in the standard iOS fashion with the appropriate ties to allow for interaction with the C++ back end.

As stated in the [specifications](https://bitbucket.org/ataylor13/chess_mob/wiki/Specifications) document, the CASE tools we will be using are Git, SourceTree, Eclipse, Visual Studio, and Xcode. Git will be used for source control. SourceTree is a multi-platform repository manager that will make the use of Git simpler and more straightforward. Eclipse will be used for the development and testing of the Android interface. Visual Studio will be used for the development and testing of the C++ game logic code. And Xcode will be used for the development and testing of the iOS interface.

## Coding Style Guide ##
We have developed a style guide for all code to ensure code consistency and readability throughout the project. This style guide can be found [here](https://bitbucket.org/ataylor13/chess_mob/wiki/Style%20Guide).

## System Architecture ##
In order to run this application, an Android or iOS device will be required. Android users will need to have at least version 3.0. Because we are targeting mobile devices, we will be assuming that they have at most a few GB of storage space, limited RAM, and limited processing power.