
 # Rocketroll
This is our first game in C++ with SFML. We are still learning!

![Main Menu Screenshot](https://github.com/knocek/Rocketroll/blob/main/screenshots/main-menu.png) 

# Rocketroll

Rocketroll is a game developed in C++ using the [SFML 2.4.0](https://www.sfml-dev.org/) library. This project is designed to be easy to set up and run on any computer with the proper configuration.

---

## 🚀 Prerequisites

Before you can run the project, ensure your system meets the following requirements:

1. **Operating System**: Windows, macOS, or Linux.
2. **Compatible IDE**: Visual Studio 2022/VS Code (or newer) with C++ development tools installed.
3. **SFML Library**: Download [SFML 2.4.0](https://www.sfml-dev.org/download/sfml/2.4.0/) (version for Visual Studio).
4. **MinGW**: Download [MYSYS2 installer](https://github.com/msys2/msys2-installer/releases/download/2024-12-08/msys2-x86_64-20241208.exe)

---

## 🛠️ Setting Up the Project in Visual Studio

1. **Clone the repository**:
   Open your terminal and run:
   ~~~bash  
   git clone https://github.com/knocek/students-mechazilla.git
   ~~~

2. **Set Up SFML**
   1. Navigate to the folder where you installed SFML.
   2. Copy folder SFML to "extern" folder in your project directory.
   3. Copy .dll files to Debug folder created in build folder.

3. **Set Up MinGW**
   1. Follow the instructions given here: [using GCC with MinGW tutorial](https://code.visualstudio.com/docs/cpp/config-mingw)


## 🔧 Building and Running the Game
In Visual Studio Code, click Build.
After the build completes, find the executable file in:

- Rocketroll\build\Debug\

Run the .exe file to start the game!


## 🔧 Troubleshooting
Problem: Missing .dll files
Ensure you copied the .dll files from SFML/bin to the appropriate output folder.

Problem: Executable file not generated
Ensure all SFML paths (include and lib) are properly set up in the project settings.


## 📷 Screenshots
### Main menu
![Main Menu Screenshot](https://github.com/knocek/Rocketroll/blob/main/screenshots/main-menu.png) 

### Options Menu
![Options Menu](https://github.com/knocek/Rocketroll/blob/main/screenshots/options-menu.png) 

### Music Option
![Music Options](https://github.com/knocek/Rocketroll/blob/main/screenshots/music-options.png) 

### Set Rocket Menu
![Set Rocket Menu](https://github.com/knocek/Rocketroll/blob/main/screenshots/setting-rocket.png) 

### Game
![Game](https://github.com/knocek/Rocketroll/blob/main/screenshots/game.png) 


## 💡 Useful Links
- [SFML Official Documentation](https://www.sfml-dev.org/documentation/2.6.2/)
- [Download SFML](https://www.sfml-dev.org/download.php)


## ✌️ Authors
- [Karolina](https://github.com/knocek)
- [Nikodem5](https://github.com/Nikodem5)
- [Mateusz](https://github.com/mateuszmzzz)