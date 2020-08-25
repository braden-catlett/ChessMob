# ChessMob Specification #
## Introduction ##
Our project will provide a unique spin on the classic Chess game by only allowing the player to have a king and several of only one other type of piece. This game, called ChessMob, will be on the Android and iOS platforms so that the game will be very portable and be playable at any time or place. The specification techniques that we will be using are UML diagrams, a finite state machine for each part of the project and “screenshots” we have worked up for our client.

The CASE tools we will be using are Git, SourceTree, Eclipse, Visual Studio, and Xcode. Git will be used for source control. SourceTree is a multi-platform repository manager that will make the use of Git simpler and more straightforward. Eclipse will be used for the development and testing of the Android interface. Visual Studio will be used for the development and testing of the C++ game logic code. And Xcode will be used for the development and testing of the iOS interface.


## Constraints ##
Our monetary costs will be minimal because publishing Android applications is free except for a small, one-time developer enrollment cost that will be covered by the client. Publishing iOS applications is a bit more expensive, but we expect to either have the client pay the Apple Developer fee as well or use the existing developer account of one of our group members. We are constrained to a development period that runs through to the middle of May with a beta release which we decided will be on May 7th.

The only possibility for the need to have the app work with other applications or require backwards compatibility will be in future releases in the form as the ability to play online games. Online games will likely require interaction with a server, so in order to maintain backwards compatibility in the future releases the methods of communication between the server and the mobile application will need to be maintained and developed in such a way that past version can still use our servers for their online play..

With the game logic written in C++, the app should be relatively easy to port to other platforms such as Windows or Mac OS. We would, of course, like to make the app as reliable as possible in order to maintain a good user experience, but we also realize that we might have to sacrifice some reliability in order to ship by the end of the semester. Similarly, we would like to make the app as responsive as possible, but the intense calculations needed for the computer player combined with the limited hardware resources of some mobile devices might create unavoidable performance losses at higher difficulty levels. Since the computer intelligence will have to make more calculations to find the best possible move.


## Acceptance Criteria ##
The test scenarios we have to consider vary from the normal game scenario to the unexpected or unpredictable event happening. These scenarios include the phone dying during gameplay, a phone action like a call or text occurring, or the user pressing the home or back button to quickly exit the application. We also have to consider the balancing of the game when we match different piece sets against each other. The different combinations will give the game its uniqueness and will make it more fun to play. Our passing requirements would be that the game correctly sets up the user defined game settings, the application successfully handles all player moves and maintains the board until the game ends for a normal case. For the abuse or exceptional cases we want the game to save the state of the board so that it can be reloaded later.This will be accomplished by saving the state at the beginning of each turn so that there will always be at least a recent version of the game saved when the unexpected happens. In the case where a player attempts to make an illegal move the move will be rejected and the player will get another chance to move.


## Specifications ##
We have worked up a UML diagram for our backend which will contain all of our game logic and object classes so it lends itself to a UML diagram as the best way to specify it. The diagrams are located [here](https://docs.google.com/drawings/d/1hTuwYq8IDiiBO186CGWQo7OmElPAV70bGeBZH2-wnuw/edit?usp=sharing). Basically, each type of piece will have a unique way of checking if a given move is valid and the game will be managed through a single, all-powerful entity that tracks the board and acts as a barrier between the user interface and the pieces and game logic themselves.

We also have worked up “screenshots” of what visually we want ChessMob to look like using Google’s drawing tools and an assortment of images found in various places. Besides providing a template for what the app will look like, these will serve as a tool to get input from our client about the project and his desires on its look and feel. These screenshots are located [here](https://docs.google.com/drawings/d/1OC69UDxWSUKMLhelk5DpuZ_559EO5T8hQqMBBBJD6_U/edit?usp=sharing).

Finally, we have developed a finite state machine for our application starting from when a user launches the application until they have exited the application. The finite state machine along with a flow chart of the various screens that the user will navigate during use of ChessMob is located [here](https://docs.google.com/drawings/d/18FAPrG0t7kOK0pvNByxbe4_erWg9-Up0nKg44B1G1K8/edit?usp=sharing).


## Schedule ##
There is a spreadsheet containing a tentative schedule located [here](https://docs.google.com/spreadsheet/ccc?key=0AmnboCsmtc6UdGEycFlER1FNbXFoYWlnZkNpQThVMEE&usp=sharing). Most of the design work will be over the next two weeks (April 8-19). The backend and the generic UI (that we will be using on both platforms) will be designed in parallel. During the development of the user interface, we will be working closely with the client to make sure that he will like the look and feel and makes the application user friendly.

After designing the application, we will start the implementation phase. We have planned on implementing the backend and both UIs  in parallel and then group together to get them working together. We have done research on the process of connecting the backend into the UIs on both Platforms. Yet this will be the area that could result in the most problems.

The beta dates will on May 7th. At this point, we plan on having both platforms working with the menu structure necessary for a user to select their game settings and play a full game with at least a basic form of artificial intelligence so the computer player can at least perform a move. Along with the application using 2D graphics for game play.

The general aspects of the application and the parts related to connecting the backend and the user interface will be designed by all of us but we have planned on Brad implementing the Android application, Michael implementing the iOS application and Ace and Brayden are implementing the Backend in parallel. Each of us have strengths in the areas we have assigned to each other, so it should increase our efficiency and allow us to efficiently parallelize the development.

Testing will happen in parallel with development with each person being responsible for testing their own code. We will also work with each other to come up with more scenarios to test so that the final product is as solid as it can be without having to push back any milestone dates very far, if at all.


## End Game ##
We have discussed that our client will own the product when the semester ends and we will work out payments to each contributing member if the application makes money while on each platform’s application market. We currently plan on continuing to work on the project until we feel it is a complete, well-developed product with the possibility that some of us will continue to maintain it so long as we have time. We will be able to communicate with the client by email, phone, or (in some cases) in person due to our client being located in Spokane.


## Testing ##
Upon walking through our specifications we needed to update our screenshots to better reflect the options the user will see when they first start up ChessMob. We also discovered that we need to include a pause/options screen that will only be accessible during game play. This will allow the user to either quit the game or change some simple settings. Also, we need to include a way to announce when victory or defeat is reached which will either manifest in another activity that simply states “Victory” or “Defeat” in a large font or more likely have a dialog box popup when a game reaches conclusion that takes the user back to main screen when dismissed from view. All of our specifications have an origin in our requirements document except for the pause screen and the victory/defeat screen the need for which was discovered while developing our specifications.

It is also worth noting that there are some items discussed in the requirements document that are not addressed in this document. These items are generally considered non-essential and will be reviewed for inclusion into the product at a later date. Some of the notable items in this category are: network play, multiple games, moving saved games between devices, a tutorial, and time restrictions on moves (speed Chess).


## Discarded Strategies ##
Since Chess is a fairly linear game, this application will also have a fairly linear feel to it. As such, we have discarded the Petri Net technique. We found that we can easily represent this as a finite state machine.