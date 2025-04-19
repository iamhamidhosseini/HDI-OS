HDI-OS: A Simple Educational Operating System

This project is an educational operating system designed to provide a basic, hands-on experience in operating system development. The system is built upon the ideas and principles found in the XV6 operating system, a teaching OS developed by MIT. The project is being developed with a focus on helping students from the Tehran Polytechnic University gain a better understanding of operating systems by interacting with and customizing their own kernel.
Project Overview

HDI-OS is designed to be a simple operating system that includes essential features like boot loading, kernel development, and basic shell functionalities. The primary goal is to make the concepts of operating systems accessible and tangible to students by providing them with a working OS structure that they can modify and experiment with.
Features:

    Bootloader: A minimal bootloader that loads the kernel into memory.

    Kernel: A basic kernel that supports process management, file handling, and system calls.

    Shell: A custom shell that supports basic commands like ls, cd, and exit.

    Filesystem: A simplistic file system for managing files and directories.

    Memory Management: Basic paging functionality to manage memory effectively.

Development Environment

    Tools:

        GCC for compiling the kernel.

        NASM for assembling the bootloader.

        GRUB for bootloader configuration.

        QEMU for running and testing the OS in a virtual environment.

    System Requirements:

        Linux or WSL (Windows Subsystem for Linux) environment.

        QEMU for virtual machine emulation.

        GRUB for bootloader configuration.

Getting Started
Prerequisites:

    Install required tools:

sudo apt-get install build-essential nasm qemu grub2 xorriso

Clone the repository:

git clone https://github.com/iamhamidhosseini/HDI-OS.git
cd HDI-OS

Build the OS:

make

Run the OS in QEMU:

    make qemu

Directory Structure:

    bootloader: Contains the bootloader source code.

    kernel: Contains the kernel source code, including the multiboot header.

    iso: The directory for building the ISO image.

    build: Temporary build directory for object files and binaries.

Customization:

    Modify the kernel source in kernel/kernel.c to add new features or system calls.

    Update the shell commands in kernel/shell.c for new functionalities.

    Customize the bootloader or GRUB configurations as needed.

Contributing

If you'd like to contribute to this project, feel free to fork the repository and submit pull requests with improvements or bug fixes. Please ensure that you follow the project's code style and write clear commit messages.
Acknowledgments

This project is based on XV6, a teaching operating system by MIT. Special thanks to the MIT team for providing such a valuable resource for learning OS development.

Feel free to modify and improve this as needed!
